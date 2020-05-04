#include "player.h"

player* player::currentOwner = NULL;
bool player::firstMouseAction = true;

player::player() {
	lastXpos = 0.0f;
	plat = platform(0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	if (currentOwner == NULL) {
		currentOwner = this;
	}
}

player::~player() {
	if (currentOwner == this) {
		currentOwner = NULL;
	}
}

void player::draw(const Shader &SO) {
	plat.prepare_to_draw(SO);
	plat.draw(SO);
}

void player::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (currentOwner != NULL) {
		if (firstMouseAction) {
			currentOwner->lastXpos = xpos;
			firstMouseAction = false;
		}
		float offsetX = xpos - currentOwner->lastXpos;
		currentOwner->lastXpos = xpos;

		const float sensitivity = 0.05f;
		offsetX *= sensitivity;

		currentOwner->plat.change_position(offsetX);
	}
}