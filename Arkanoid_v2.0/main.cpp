#include <iostream>

#ifndef _STB_IMAGE_JP_
#define _STB_IMAGE_JP_
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef COMM_BUFLEN
#define COMM_BUFLEN 512
#endif
char sendbuffer[COMM_BUFLEN];
char recvbuffer[COMM_BUFLEN];
bool ReadyToSend = true;		// true if thread ready to send, false if in the middle of communication
bool ReadyToUpdate = false;		// true if data are updated, false if there is no or only old data in the buffer
bool CommunicationControl = false;
bool ShouldEnd = false;
int bricks_count_old = 0;
int bonuses_count_old = 0;
int bricks_count_new = 0;
int bonuses_count_new = 0;
int package_size = 0;

#include "singleplayer/singleplayer.h"
#include "multiplayer_server/multiplayer_server.h"
#include "multiplayer_client/multiplayer_client.h"
#include "logger/logger.h"

using namespace std;


int main() {

	logger::open_log();

	bool endstat = false;
	int resp = 0;
	int exitstat = 0;
	cout << "Welcome to the Arkanoid v2.0 game.\n";

	while (!endstat) {
		logger::log("In main - choice.\n");
		cout << "\nPlease specify which version would you like to run:\n";
		cout << " 0 - Exit\n";
		cout << " 1 - Single Player\n";
		cout << " 2 - Host a multiplayer game\n";
		cout << " 3 - Join a multiplayer game\n";

		cin >> resp;
		switch (resp) {
		case 0:
			logger::log("Main: exit choice.\n");
			cout << "Good bye then!\n";
			endstat = true;
			break;
		case 1:
			logger::log("Main: Singleplayer.\n");
			exitstat = singleplayer();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: Singleplayer.\n");
				endstat = true;
			}
			break;
		case 2:
			logger::log("Main: Multiplayer server.\n");
			exitstat = mutliplayer_server();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: M: Server.\n");
				endstat = true;
			}
			break;
		case 3:
			logger::log("Main: Multiplayer client.\n");
			exitstat = mutliplayer_client();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: M: Client.\n");
				endstat = true;
			}
			break;
		default:
			logger::log("Main: Bad choice.\n");
			cout << "No such option.\n";
		}
	}
	logger::log("Exiting main.\n");
	return 0;
}

