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
#include "../logger/logger.h"

#ifndef _COMMUNICATION_GLOBALS_
#define _COMMUNICATION_GLOBALS_
// here are global variables and macros for network and thread communication
#define DEFAULT_BUFLEN 1024
#define COMM_BUFLEN 512
#define DEFAULT_PORT "8888"
char sendbufferS[COMM_BUFLEN];
char recvbufferS[COMM_BUFLEN];
bool ReadyToSendS = true;		// true if thread ready to send, false if in the middle of communication
bool ReadyToUpdateS = false;		// true if data are updated, false if there is no or only old data in the buffer
bool CommunicationControlS = false;
bool ShouldEndS = false;
int bricks_count_oldS = 0;
int bonuses_count_oldS = 0;
int package_sizeS = 0;
#endif

#include "multiplayer_server.h"

using namespace std;

void framebuffer_size_callback_server(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

void play_level_server(GLFWwindow*, Shader&, level&, player&, player&);
void communicate_server(SOCKET&);


int mutliplayer_server() {
	logger::log("    Multiplayer server: initializing listening socket.\n");
	// Network sockets initialization
	WSADATA _wsaData;
	int _iRes = 0;
	_iRes = WSAStartup(MAKEWORD(2,2), &_wsaData);

	if (_iRes) {
		std::stringstream ss;
		ss << "WSAStartup failed! " << _iRes << "\n" << endl;
		logger::log(ss);
		return 1;
	}

	struct addrinfo* result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints)); // fill memory at &hints with 0
	// Fill hints with hints about socket type 
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	_iRes = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (_iRes) {
		std::stringstream ss;
		ss << "getaddrinfo failed! " << _iRes << endl;
		logger::log(ss);
		WSACleanup();
		return 1;
	}
	SOCKET ListenSocket = INVALID_SOCKET;
	// now create a socket
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	// Make sure that the socket was properly initialized
	if (ListenSocket == INVALID_SOCKET) {
		std::stringstream ss;
		ss << "socket() failed! " << WSAGetLastError() << endl;
		logger::log(ss);
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	_iRes = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	// Make sure that the binding was successfull
	if (_iRes) {
		std::stringstream ss;
		ss << "binding failed! " << WSAGetLastError() << endl;
		logger::log(ss);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);

	logger::log("    Multiplayer server: listening on the socket.\n");
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::stringstream ss;
		ss << "listening failed! " << WSAGetLastError() << endl;
		logger::log(ss);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// To accept the connection from a client we need a client socket
	// Here we accept only a single client
	SOCKET ClientSocket = INVALID_SOCKET;

	logger::log("    Multiplayer server: accepting connection.\n");
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::stringstream ss;
		ss << "accepting failed! " << WSAGetLastError() << endl;
		logger::log(ss);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//ListenSocket is no longer needed
	closesocket(ListenSocket);

	logger::log("    Multiplayer server: greeting.\n");
	string WelcomeMessage = "Hello, this is server, welcome to Arkanoid.";
	char recvbuf[DEFAULT_BUFLEN] = {}; // network communication buffer
	int recvbuflen = DEFAULT_BUFLEN;
	strcpy_s(recvbuf, DEFAULT_BUFLEN, WelcomeMessage.c_str());

	_iRes = send(ClientSocket, recvbuf, WelcomeMessage.length(), 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Send failed! " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	memset(recvbuf, 0, DEFAULT_BUFLEN);
	_iRes = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Recive failed! " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	cout << recvbuf << endl;


	logger::log("    Multiplayer server: initializing window and graphics.\n");
	//	--------------------------------------------------------------------------------------------------------------------
// Here starts window and context initialization
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid_v2.0_Server", NULL, NULL);
	if (window == NULL) {
		logger::log("Failed to open window.\n");
		return 1;
	}
	// Finally we make the context of the window the main context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_server);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// now try to initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		logger::log("Failed to initialize GLAD.\n");
		return 1;
	}

	logger::log("    Multiplayer server: initializing shaders.\n");
	//	--------------------------------------------------------------------------------------------------------------------
	// data communication channel is set now take care of shaders with a special object
	Shader SO("./shader/vertexShader.vert", "./shader/fragmentShader.frag");
	SO.use();

	glEnable(GL_DEPTH_TEST);

	logger::log("    Multiplayer server: loading level.\n");
	bonus dummy_bonus(glm::vec2(0.0f, -20.0f), 0);	// this is a dummy invisible bonus to keep graphics resources populated
	level _level;
	player _player_host;	// is the host of the game, has acces to mouse input which controls his platform
	try {
		_level.load_level(1);
	}
	catch (const char* msg) {
		std::stringstream ss;
		ss << "loading level failed! " << msg << endl;
		logger::log(ss);
		return 1;
	}
	bricks_count_oldS = _level.bricks.size();
	bonuses_count_oldS = _level.bonuses.size();
	player _player_client(glm::vec3(0.0f,0.0f,1.0f));	// is a client, has no access to input, platform position controlled remotely
	//std::thread gameplay(play_level_server, window, SOptr, levelptr, hostptr, clientptr);
	logger::log("    Multiplayer server: initializing gameplay and communication.\n");
	std::thread comms(communicate_server, std::ref(ClientSocket));
	play_level_server(window, SO, _level, _player_host, _player_client);

	//gameplay.join();
	comms.join();

	logger::log("    Multiplayer server: resetting globals and closing window.\n");
	//reset globals
	ReadyToSendS = true;
	ReadyToUpdateS = false;
	CommunicationControlS = false;
	ShouldEndS = false;

	glfwTerminate();

	logger::log("    Multiplayer server: closing connection and shutting down sockets.\n");
	_iRes = shutdown(ClientSocket, SD_SEND); // SD_SEND specifies that the sending side is to be closed
	if (_iRes == SOCKET_ERROR) {
		std::stringstream ss;
		ss << "Shutdown failed! " << WSAGetLastError() << endl;
		logger::log(ss);
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();

	logger::log("    Multiplayer server: exiting.\n");

	return 0;
}

// This function handles resizing of window
void framebuffer_size_callback_server(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}

// This function is responsible for handling gamplay
void play_level_server(GLFWwindow *window, Shader &_SO, level &_level, player& _player1, player &_player2) {
	logger::log("        Multiplayer server play: initializing callbacks and perspective.\n");
	glfwSetCursorPosCallback(window, player::mouse_callback);
	if (_level.bricks.size() > 0) _level.end_level = false;
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
	logger::log("        Multiplayer server play: start the rendering loop.\n");
	while (!_level.end_level) {
		_level.level_process_input(window);

		float timeVal = glfwGetTime();
		deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen

		_level.message.draw(_SO);
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

		//now we copy data to and from buffers if communication thread is ready
		if (!CommunicationControlS) {
			if (ReadyToSendS) {
				//prepare data to send
				float* _lptr = (float*)sendbufferS;	// beggining of memory for communication
				float* _rptr = (float*)(sendbufferS + COMM_BUFLEN);	// end of memory for communication
				// ball and brick counts go first
				*((int*)_lptr) = _level.balls.size();
				_lptr += 1;
				*((int*)_lptr) = _level.bricks.size();
				_lptr += 1;
				*((int*)_lptr) = _level.bonuses.size();
				_lptr += 1;
				// copy platform data
				_lptr = _player1.plat.comm_props(_lptr, _rptr);
				// copy balls data
				for (int i = 0; i < _level.balls.size(); ++i) {
					_lptr = _level.balls[i].comm_props(_lptr, _rptr);
				}
				// conditionally copy bricks data
				if (bricks_count_oldS != _level.bricks.size()) {
					for (int i = 0; i < _level.bricks.size(); ++i) {
						_lptr = _level.bricks[i].comm_props(_lptr, _rptr);
					}
					bricks_count_oldS = _level.bricks.size();
				}
				// conditionally copy bonuses data
				if (bonuses_count_oldS != _level.bonuses.size()) {
					for (int i = 0; i < _level.bonuses.size(); ++i) {
						_lptr = _level.bonuses[i].comm_props(_lptr, _rptr);
					}
					bonuses_count_oldS = _level.bonuses.size();
				}
				package_sizeS = ((char*)_lptr - sendbufferS);
				ReadyToSendS = false;	// tell the communication thread that it needs to send data
			}
			//update recieved data
			if (ReadyToUpdateS) {
				//std::cout << "Update clause server\n";
				float* _lptr = (float*)recvbufferS;	// beggining of memory for communication
				float* _rptr = (float*)(recvbufferS + COMM_BUFLEN);	// end of memory for communication
				// update player 2 info
				_lptr = _player2.plat.read_props(_lptr, _rptr);
				ReadyToUpdateS = false;	// data are updated and recvbuffer has old data now
			}
			CommunicationControlS = true;
		}

	}
	logger::log("        Multiplayer server play: reset globals.\n");
	ShouldEndS = true;
	ReadyToSendS = true;
	ReadyToUpdateS = false;
	CommunicationControlS = false;
	logger::log("        Multiplayer server play: exit.\n");
}

// This is a function run in thread comms that is responsible for server-client communication
void communicate_server(SOCKET& ClientSocket) {
	int _iRes;
	while (!ShouldEndS) {
		//std::cout << glfwGetTime() << "\r";
		if (CommunicationControlS)
		{
			if (!ReadyToSendS) {	// if ReadyToSend = true there's no need to communicate, if false there are new data in sendbuffer
				_iRes = send(ClientSocket, (char*)sendbufferS, package_sizeS, 0);	// send data to client
				if (_iRes == SOCKET_ERROR) {
					cout << "Send failed! " << WSAGetLastError() << endl;
				}
				_iRes = recv(ClientSocket, (char*)recvbufferS, COMM_BUFLEN, 0);	// get data from client
				if (_iRes == SOCKET_ERROR) {
					cout << "Recive failed! " << WSAGetLastError() << endl;
				}
				else {
					ReadyToUpdateS = true;
				}
				ReadyToSendS = true;
			}
			CommunicationControlS = false;
		}
	}
	ShouldEndS = false;
}