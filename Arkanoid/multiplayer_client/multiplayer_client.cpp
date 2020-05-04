#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8888"

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
#include "stb_image.h"
#endif

#include "shader.h"

#include "brick.h"
#include "background.h"
#include "platform.h"
#include "player.h"
#include "ball.h"
#include "level.h"

#include "multiplayer_client.h"

using namespace std;

void framebuffer_size_callback_client(GLFWwindow*, int, int); // a function that will resize the viewport when window size changes

void play_level_client(GLFWwindow*, Shader&, level&, SOCKET&, player&);

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
	char recvbuf[DEFAULT_BUFLEN]; // network communication buffer
	int recvbuflen = DEFAULT_BUFLEN;
	strcpy_s(recvbuf, DEFAULT_BUFLEN, WelcomeMessage.c_str());

	_iRes = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Recive failed! " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	cout << recvbuf << endl;
	_iRes = send(ConnectSocket, recvbuf, (int)strlen(recvbuf), 0);
	if (_iRes == SOCKET_ERROR) {
		cout << "Send failed! " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	//	--------------------------------------------------------------------------------------------------------------------
// Here starts window and context initialization
	glfwInit(); // Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// OpenGL version 3.*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// OpenGL version *.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL use core profile
	// Now we create a window (800x600 pixels)
	GLFWwindow* window = glfwCreateWindow(800, 600, "Arkanoid_v0.1", NULL, NULL);
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
	Shader SO("./vertexShader.vert", "./fragmentShader.frag");
	SO.use();

	glEnable(GL_DEPTH_TEST);


	cout << "Starting game.\n";
	level _level;
	_level.load_level(1);
	player ClientPlayer;
	play_level_client(window, SO, _level, ConnectSocket, ClientPlayer);

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

void framebuffer_size_callback_client(GLFWwindow*, int width, int height) { // a function that will resize the
	glViewport(0, 0, width, height);
}


void play_level_client(GLFWwindow* window, Shader& _SO, level& _level, SOCKET& ConnectSocket, player& _player2) {
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

	float databuffer[32];
	int i = 0;
	int _iRes = 0;
	while (!_level.end_level) {
		_level.level_process_input(window);

		float timeVal = glfwGetTime();
		deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;
		std::cout << "Your score is: " << _level.score << "\r";

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen

		_level._background.draw(_SO);

		i = 0;
		_level.bricks[0].prepare_to_draw(_SO);
		while (i < _level.bricks.size()) {
			if (_level.check_bounce_brick(_level.bricks[i], _level._ball)) {
				_level.bricks.erase(_level.bricks.begin() + i);
				_level.score += 10 * _level.grade;
				//std::cout << "Score: " << score << std::endl;
				//std::cout << "No. of bricks left: " << bricks.size() << std::endl;
				continue;
			}
			_level.bricks[i].draw(_SO);
			++i;
		}
		if (_level.bricks.size() == 0) {
			_level.end_level = true;
			_level.win_cond = true;
		}

		_level._player.draw(_SO);
		_player2.draw(_SO);

		_level._ball.prepare_to_draw(_SO);
		_level._ball.draw(_SO);
		_level._ball.propagate(deltaTime);
		_level.check_bounce_platform(_level._player.plat, _level._ball);
		_level.check_bounce_platform(_player2.plat, _level._ball);
		_level.check_lose(_level._ball);

		//now we perform communication

		_iRes = recv(ConnectSocket, (char*)databuffer, 32 * sizeof(float), 0);
		if (_iRes == SOCKET_ERROR) {
			cout << "Recive failed! " << WSAGetLastError() << endl;
		}
		//ball data
		_level._ball.velocity.x = databuffer[0]; _level._ball.velocity.y = databuffer[1]; _level._ball.speed = databuffer[2]; _level._ball.position.x = databuffer[3];
		_level._ball.position.y = databuffer[4]; _level._ball.futurePosition.x = databuffer[5]; _level._ball.futurePosition.y = databuffer[6];
		// platform player 1 data
		_player2.plat.set_position(databuffer[7]); _player2.plat.lwh.x = databuffer[8]; _player2.plat.xscale = databuffer[9];
		 _player2.plat.TimeModif = databuffer[10];  _player2.plat.DeltaModif = databuffer[11];  _player2.plat.direction = (int)databuffer[12];

		 databuffer[0] = _level._player.plat.position.x;
		 databuffer[1] = _level._player.plat.lwh.x;
		 databuffer[2] = _level._player.plat.xscale;
		 databuffer[3] = _level._player.plat.TimeModif;
		 databuffer[4] = _level._player.plat.DeltaModif;
		 databuffer[5] = (float)_level._player.plat.direction;
		_iRes = send(ConnectSocket, (char*)databuffer, 6 * sizeof(float), 0);
		if (_iRes == SOCKET_ERROR) {
			cout << "Send failed! " << WSAGetLastError() << endl;
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}