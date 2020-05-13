#include "player.h"

player* player::currentOwner = NULL;
bool player::firstMouseAction = true;

// Default constructor - creates platform with default color
player::player() {
	lastXpos = 0.0f;
	plat = platform(0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	if (currentOwner == NULL) {
		currentOwner = this;
	}
}

// Color constructor - creates platform with custom color
player::player(const glm::vec3 &_color) {
	lastXpos = 0.0f;
	plat = platform(0.0f, _color);
	if (currentOwner == NULL) {
		currentOwner = this;
	}
}

// Destructor - if this was the owner of the controls then release the pointer
player::~player() {
	if (currentOwner == this) {
		currentOwner = NULL;
	}
}

// Other methods

void player::resetMouseAction() {
	firstMouseAction = true;
}

// Mouse control
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