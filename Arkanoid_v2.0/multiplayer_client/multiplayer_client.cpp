#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#include <thread>

#pragma comment(lib,"Ws2_32.lib")

#ifndef _GLAD_
#define _GLAD_
#include <glad/glad.h>	// API for pointers in OpenGL
#endif
#ifndef _GLFW3_
#define _GLFW3_
#include <glfw3.h>		// API for context creation
#endif

#ifndef _GLM_STUFF_
#define _GLM_STUFF_
#include <glm/glm/glm.hpp>	// API for linear algebra operations compatible with OpenGL
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#endif

#ifndef _STB_IMAGE_JP_
#define _STB_IMAGE_JP_
#include "../stb_image.h"
#endif

#include "../shader/shader.h"

#include "../brick/brick.h"
#include "../background/background.h"
#include "../platform/platform.h"
#include "../player/player.h"
#include "../ball/ball.h"
#include "../bonus/bonus.h"
#include "../level/level.h"

#ifndef _COMMUNICATION_GLOBALS_
#define _COMMUNICATION_GLOBALS_
// here are global variables and macros for network and thread communication
#define DEFAULT_BUFLEN 1024
#define COMM_BUFLEN 512
#define DEFAULT_PORT "8888"
char sendbufferC[COMM_BUFLEN];
char recvbufferC[COMM_BUFLEN];
bool ReadyToSendC = true;		// true if thread ready to send, false if in the middle of communication
bool ReadyToUpdateC = false;		// true if data are updated, false if there is no or only old data in the buffer
bool ShouldEndC = false;
bool CommunicationControlC = false;
int bricks_count_newC = 0;
int bonuses_count_newC = 0;
#endif

#include "multiplayer_client.h"

using namespace std;

void framebuffer_size_callback_client(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

void play_level_client(GLFWwindow*, Shader&, level&, player&, player&);
void communicate_client(SOCKET&);

int mutliplayer_client() {

	char _IPaddress[32];
	cout << "Please give the IP address of the server.\n";
	cin >> _IPaddress;

	// Network sockets initialization
	WSADATA _wsaData;
	int _iRes = 0;
	_iRes = WSAStartup(MAKEWORD(2, 2), &_wsaData);

	if (_iRes) {
		cout << "WSAStartup failed! " << _iRes << "\n" << endl;
		return 1;
	}

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints)); // fill memory at &hints with 0
	// Fill hints with hints about socket type 
	hints.ai_family = AF_UNSPEC; // IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	_iRes = getaddrinfo(_IPaddress, DEFAULT_PORT, &hints, &result);
	if (_iRes) {
		cout << "getaddrinfo failed! " << _iRes << endl;
		WSACleanup();
		return 1;
	}
	SOCKET ConnectSocket = INVALID_SOCKET;
	// attempt to connect hosts in the linked list result
	for (ptr = result;ptr != NULL;ptr = ptr->ai_next) {

		// first get the socket
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			cout << "socket() failed! " << WSAGetLastError() << endl;
			ptr = NULL;
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// now try to connect
		_iRes = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
		// if failed try next host
		if (_iRes == SOCKET_ERROR) {
			cout << "Connection failed! " << endl;
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	// address info no longer needed
	ptr = NULL;
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		cout << "connection failed! " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	string WelcomeMessage = "Hello, this is client, I want to play Arkanoid.";
	char recvbuf[DEFAULT_BUFLEN] = {}; // network communication buffer
	int recvbuflen = DEFAULT_BUFLEN;

	_iRes = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Recive failed! " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	cout << recvbuf << endl;
	strcpy_s(recvbuf, DEFAULT_BUFLEN, WelcomeMessage.c_str());
	_iRes = send(ConnectSocket, recvbuf, WelcomeMessage.length(), 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Send failed! " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	Sleep(1000);
	cout << "Now I start to load\n";

	//	--------------------------------------------------------------------------------------------------------------------
// Here starts window and context initialization
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid_v2.0_Client", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to open window.\n";
		return 1;
	}
	// Finally we make the context of the window the main context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_client);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// now try to initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD.\n";
		return 1;
	}

	//	--------------------------------------------------------------------------------------------------------------------
	// data communication channel is set now take care of shaders with a special object
	Shader SO("./shader/vertexShader.vert", "./shader/fragmentShader.frag");
	SO.use();

	glEnable(GL_DEPTH_TEST);


	cout << "Press space to begin.\n";
	bonus dummy_bonus(glm::vec2(0.0f, -20.0f), 0);	// this is a dummy invisible bonus to keep graphics resources populated
	level _level;
	player _player_client;	// has mouse control over platform but is not host of the game
	_level.load_level(1);
	bricks_count_newC = _level.bricks.size();
	bonuses_count_newC = _level.bonuses.size();
	player _player_host;	// is host of the game and remotely has control over platform, but has no access to mouse input
	//std::thread gameplay(play_level_client, window, std::ref(SO), std::ref(_level), std::ref(_player_host), std::ref(_player_client));
	std::thread comms(communicate_client, std::ref(ConnectSocket));
	play_level_client(window, SO, _level, _player_host, _player_client);

	//gameplay.join();
	comms.join();

	//reset globals
	ReadyToSendC = true;
	ReadyToUpdateC = false;
	ShouldEndC = false;
	CommunicationControlC = false;

	glfwTerminate();

	_iRes = shutdown(ConnectSocket, SD_SEND);
	if (_iRes == SOCKET_ERROR) {
		cout << "Shutdown failed! " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();


	return 0;
}

// This function handles resizing of window
void framebuffer_size_callback_client(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}

// This function is responsible for handling gamplay
	// player1 is host
	// player2 is here mouse controlled
void play_level_client(GLFWwindow* window, Shader& _SO, level& _level, player& _player1, player& _player2) {
	if (_level.bricks.size() > 0) _level.end_level = false;
	glfwSetCursorPosCallback(window, player::mouse_callback);
	float deltaTime = 0.0f;
	float lastFrame = glfwGetTime();

	_SO.use();
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int tView = glGetUniformLocation(_SO.ID, "view");
	glUniformMatrix4fv(tView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	unsigned int tProj = glGetUniformLocation(_SO.ID, "proj");
	glUniformMatrix4fv(tProj, 1, GL_FALSE, glm::value_ptr(proj));

	// Here starts gamplay/rendering loop
	while (!_level.end_level) {
		_level.level_process_input(window);

		float timeVal = glfwGetTime();
		deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen

		_level._background.draw(_SO);

		_level.handle_bricks(_SO);
		_level.handle_bonuses(_SO, _player1.plat, deltaTime);
		_level.handle_bonuses(_SO, _player2.plat, deltaTime);
		_level.handle_balls(_SO, deltaTime);
		_level.check_bounce_platform(_player1.plat);
		_level.check_bounce_platform(_player2.plat);

		_player1.plat.prepare_to_draw(_SO);
		_player1.plat.draw(_SO);
		_player2.plat.draw(_SO);

		_level.check_lose();

		glfwSwapBuffers(window);
		glfwPollEvents();

		//now we perform communication
		if (!CommunicationControlC) {
			if (ReadyToUpdateC) {
				// update based on recieved data
				float* _lptr = (float*)recvbufferC;	// beggining of memory for communication
				float* _rptr = (float*)(recvbufferC + COMM_BUFLEN);	// end of memory for communication
				// get balls count
				int host_ball_count = *((int*)_lptr);
				_lptr += 1;
				bricks_count_newC = *((int*)_lptr);
				_lptr += 1;
				bonuses_count_newC = *((int*)_lptr);
				_lptr += 1;
				// copy platform data
				_lptr = _player1.plat.read_props(_lptr, _rptr);
				// copy balls data
				for (int i = 0; i < host_ball_count; ++i) {
					if (i >= _level.balls.size()) { // add neew balls if not enough
						ball new_ball;
						_level.balls.push_back(new_ball);
						std::cout << "Adding a ball\t" << _level.balls.size() << std::endl;
					}
					_lptr = _level.balls[i].read_props(_lptr, _rptr);
				}
				while (host_ball_count < _level.balls.size()) { // erase balls over the limit
					_level.balls.erase(_level.balls.begin() + (_level.balls.size() - 1));
					std::cout << "Removing a ball\t" << _level.balls.size() << std::endl;
				}
				if (bricks_count_newC != _level.bricks.size()) {
					std::cout << "Here I update count of bricks to " << bricks_count_newC << "\n";
					for (int i = 0; i < bricks_count_newC; ++i) {
						if (i >= _level.bricks.size()) {
							brick new_brick;
							_level.bricks.push_back(new_brick);
							std::cout << "Adding a brick\t" << _level.bricks.size() << std::endl;
						}
						_lptr = _level.bricks[i].read_props(_lptr, _rptr);
					}
					while (bricks_count_newC < _level.bricks.size()) {
						_level.bricks.erase(_level.bricks.begin() + (_level.bricks.size() - 1));
						std::cout << "Removing a brick\t" << _level.bricks.size() << std::endl;
					}
				}
				if (bonuses_count_newC != _level.bonuses.size()) {
					std::cout << "Here I update count of bonuses to " << bonuses_count_newC << "\n";
					for (int i = 0; i < bonuses_count_newC; ++i) {
						if (i >= _level.bonuses.size()) {
							bonus new_bonus;
							_level.bonuses.push_back(new_bonus);
							std::cout << "Adding a bonus\t" << _level.bonuses.size() << std::endl;
						}
						_lptr = _level.bonuses[i].read_props(_lptr, _rptr);
					}
					while (bonuses_count_newC < _level.bonuses.size()) {
						_level.bonuses.erase(_level.bonuses.begin() + (_level.bonuses.size() - 1));
						std::cout << "Removing a bonus\t" << _level.bonuses.size() << std::endl;
					}
				}
				ReadyToUpdateC = false;	// tell the communication thread that it needs to send data
			}
			//update recieved data
			if (ReadyToSendC) {
				float* _lptr = (float*)sendbufferC;	// beggining of memory for communication
				float* _rptr = (float*)(sendbufferC + COMM_BUFLEN);	// end of memory for communication
				// update player 2 info
				_lptr = _player2.plat.comm_props(_lptr, _rptr);
				ReadyToSendC = false;	// data are updated and recvbuffer has old data now
			}
			CommunicationControlC = true;
		}

	}
	ShouldEndC = true;
	ReadyToSendC = true;
	ReadyToUpdateC = false;
	CommunicationControlC = false;
}

// This is a function run in thread comms that is responsible for server-client communication
void communicate_client(SOCKET& ClientSocket) {
	int _iRes;
	while (!ShouldEndC) {
		//std::cout << glfwGetTime() << "\r";
		if (CommunicationControlC) {
			if (!ReadyToSendC) {	// if ReadyToSend = true there's no need to communicate, if false there are new data in sendbuffer
				_iRes = recv(ClientSocket, (char*)recvbufferC, COMM_BUFLEN, 0);	// send data to client
				if (_iRes == SOCKET_ERROR) {
					cout << "Recive failed! " << WSAGetLastError() << ReadyToUpdateC << endl;
				}
				else {
					ReadyToUpdateC = true;
				}
				//std::cout << _iRes << "\n";
				_iRes = send(ClientSocket, (char*)sendbufferC, 6 * sizeof(float), 0);	// get data from client
				if (_iRes == SOCKET_ERROR) {
					cout << "Send failed! " << WSAGetLastError() << endl;
				}
				ReadyToSendC = true;
			}
			CommunicationControlC = false;
		}
	}
	ShouldEndC = false;
}