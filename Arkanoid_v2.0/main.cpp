#include <iostream>

#ifndef _STB_IMAGE_JP_
#define _STB_IMAGE_JP_
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

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
		cout << "\nPlease specify which version would you like to run:\n";
		cout << " 0 - Exit\n";
		cout << " 1 - Single Player\n";
		cout << " 2 - Host a multiplayer game\n";
		cout << " 3 - Join a multiplayer game\n";

		cin >> resp;
		switch (resp) {
		case 0:
			cout << "Good bye then!\n";
			endstat = true;
			break;
		case 1:
			exitstat = singleplayer();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: Singleplayer.\n");
				endstat = true;
			}
			break;
		case 2:
			exitstat = mutliplayer_server();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: M: Server.\n");
				endstat = true;
			}
			break;
		case 3:
			exitstat = mutliplayer_client();
			if (exitstat != 0) {
				logger::log("Something went wrong with the game: M: Client.\n");
				endstat = true;
			}
			break;
		default:
			cout << "No such option.\n";
		}
	}

	logger::close_log();
	return 0;
}

