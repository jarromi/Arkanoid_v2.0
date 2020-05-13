/*
This file contains 
*/
#include "level.h"

// Default constructor
level::level() {
	end_level = true;
	grade = 1;
	score = 0;
	win_cond = false;
}

// Default destructor
level::~level() {
	clean_level();
}

// Method for loading the level (bricks, ball, background, ...)
void level::load_level(const unsigned int& _grade) {
	grade = _grade;
	if (grade != 1 && grade != 2) {
		logger::log("Only levels 1 and 2 have been implemented. Sorry.\n");
		throw "BAD_LEVEL";
	}
	// try to load background
	try {
		_background.set_state(grade);
	}
	catch (const char *msg) {
		std::stringstream ss;
		ss << "Error: " << msg << "Falied background loading.\n";
		logger::log(ss);
	}

	// try to load positions and states of bricks
	std::fstream inputF;
	glm::vec2 _pos = glm::vec2(0.0, 0.0);
	int state = 0;
	//open, read file with code for vertex shader, create and compile, close file
	try
	{
		inputF.open("./level/level1.dat", std::fstream::in);

		while (inputF >> _pos.x >> _pos.y >> state) {
			brick tempBrick(_pos, state);
			bricks.push_back(tempBrick);
		}
		inputF.close();
		if (brick::get_count() != bricks.size()) {
			logger::log("Something wrong with brick count in level::load_level.\n");
			throw "BAD_BRICK_LOAD";
		}
	}
	catch (const char *msg) {
		std::stringstream ss;
		ss << "ERROR while reading level1.dat file" << msg << "\n";
		logger::log(ss);
		return;
	}

	// try to load a ball - always start with a single one
	try {
		ball tempBall(glm::vec2(0.0f, -8.25f), glm::vec2(0.6f, 0.8f), 0.0f);
		balls.push_back(tempBall);
	}
	catch (const char *msg) {
		std::stringstream ss;
		ss << "ERROR while initializing ball: " << msg << std::endl;
		logger::log(ss);
		return;
	}
}

// Level for handling input form keyboard
void level::level_process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // if the escape key was pressed then
		end_level = true;
		//glfwSetWindowShouldClose(window, true);	// set the variable for window closing to true
		Sleep(10);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // if the escape key was pressed then
		if(balls.size()>0) balls[0].set_speed(10.0f * grade);
	}
}

// Handles brick objects: check for collisions with balls, removes objects, draws, generates bonuses
void level::handle_bricks(const Shader &_SO) {
	int idBR = 0, idBL = 0;	// indices for bricks and balls vectors
	if(idBR <bricks.size()) bricks[0].prepare_to_draw(_SO);
	for (idBL = 0; idBL < balls.size();++idBL) {	// for each ball
		idBR = 0;
		while (idBR < bricks.size()) {				// for each brick
			if (check_bounce_brick(idBR, idBL)) {	// check collisions
				score += 10 * grade;
				if (bricks[idBR].get_state() == 0) {
					if (bricks.size() % 10 == 9) {
						bonus tempBonus(bricks[idBR].get_position(),0);
						bonuses.push_back(tempBonus);
					}
					else if (bricks.size() % 17 == 0) {
						bonus tempBonus(bricks[idBR].get_position(), 1);
						bonuses.push_back(tempBonus);
					}
					else if (bricks.size() % 22 == 5) {
						bonus tempBonus(bricks[idBR].get_position(), 2);
						bonuses.push_back(tempBonus);
					}
					bricks.erase(bricks.begin() + idBR);
					continue;
				}
				else {
					bricks[idBR].decrease_state();
				}
			}
			if( idBL == balls.size()-1) bricks[idBR].draw(_SO);
			++idBR;
		}
	}
	if (bricks.size() == 0) {
		end_level = true;
		win_cond = true;
	}
}

// Handles bonus objects: checks for interaction with platform, draws them, etc.
void level::handle_bonuses(const Shader &_SO, platform &_plat, const float& deltaTime){
	if (bonuses.size() > 0) {
		bonuses[0].prepare_to_draw(_SO);
		int idBON = 0;
		while (idBON < bonuses.size()) {
			if (check_bonus(idBON, _plat)) {
				bonuses.erase(bonuses.begin() + idBON);
				continue;
			}
			if (bonuses[idBON].out_of_bounds()) {
				bonuses.erase(bonuses.begin() + idBON);
				continue;
			}
			bonuses[idBON].propagate(deltaTime);
			bonuses[idBON].draw(_SO);
			++idBON;
		}
	}
}

// Handles ball objects: checks if not out of bounds, propagates, draws
void level::handle_balls(const Shader &_SO, const float &deltaTime) {
	int idBL = 0;
	while(idBL < balls.size()) {
		if (idBL == 0) balls[0].prepare_to_draw(_SO);
		if (balls[idBL].out_of_bounds()) {
			balls.erase(balls.begin() + idBL);
			continue;
		}
		balls[idBL].propagate(deltaTime);
		balls[idBL].draw(_SO);
		++idBL;
	}
	if (balls.size() == 0) {
		end_level = true;
		win_cond = false;
	}
}

// Function that checks two objects for their collision
	// idBR - index of brick
	// idBL - index of ball
bool level::check_bounce_brick(const int &idBR, const int &idBL) {
	glm::vec2 Rvec = balls[idBL].get_future_position() - bricks[idBR].get_position();
	glm::vec2 aRvec = glm::vec3(abs(Rvec.x) - brick::lwh.x, abs(Rvec.y) - brick::lwh.y, 0.0f);
	float dist = glm::distance(aRvec, glm::vec2(0.0f, 0.0f));
	if ((abs(Rvec.x) < brick::lwh.x + ball::rad) && (abs(Rvec.y) < brick::lwh.y)) {	// bounce off sides
		glm::vec2 tempVEL = balls[idBL].get_velocity();
		tempVEL.x *= -1.0f;
		balls[idBL].set_velocity(tempVEL);
		return true;
	}
	else if ((abs(Rvec.x) < brick::lwh.x) && (abs(Rvec.y) < brick::lwh.y + ball::rad)) { // bounce off top/bottom
		glm::vec2 tempVEL = balls[idBL].get_velocity();
		tempVEL.y *= -1.0f;
		balls[idBL].set_velocity(tempVEL);
		return true;
	}
	else if (dist < ball::rad) {	// bounce of corner
		if (Rvec.x < 0) {
			aRvec.x *= -1.;
		}
		if (Rvec.y < 0) {
			aRvec.y *= -1.;
		}
		aRvec = glm::normalize(aRvec);
		glm::vec2 tempVEL = balls[idBL].get_velocity();
		float vdotn = glm::dot(tempVEL, aRvec);
		tempVEL -= 2.0f * vdotn * aRvec;
		if (abs(tempVEL.y) < abs(tempVEL.x) * 0.3f) {	// make sure that ball doesn't move too flat
			float sign = tempVEL.y / abs(tempVEL.y);
			tempVEL.y = abs(tempVEL.x) * 0.3f * sign;
		}
		balls[idBL].set_velocity(tempVEL);
		return true;
	}
	return false;
}

// Function that handles collisions between balls and a given platform
void level::check_bounce_platform(const platform& PLTF) {
	for (int idBL = 0;idBL < balls.size();++idBL) {
		glm::vec2 tempVEL = balls[idBL].get_velocity();
		glm::vec2 pltf_pos = PLTF.get_position();
		glm::vec3 pltf_lwh = PLTF.get_lwh();
		if (tempVEL.y < 0.0f) {
			glm::vec2 Rvec = balls[idBL].get_future_position() - pltf_pos;
			glm::vec2 aRvec = glm::vec2(abs(Rvec.x) - pltf_lwh.x, abs(Rvec.y) - pltf_lwh.y);
			float dist = glm::distance(aRvec, glm::vec2(0.0f, 0.0f));
			if ((abs(Rvec.x) < pltf_lwh.x / 1.5f) && (abs(Rvec.y) < pltf_lwh.y + ball::rad)) { // bounce off top/bottom
				tempVEL.y *= -1.;
				if ((glfwGetTime() - PLTF.get_TimeModif()) * 0.5 + PLTF.get_DeltaModif() * 0.5 < 0.03) {
					tempVEL.x += float(PLTF.get_direction());
				}
				tempVEL = glm::normalize(tempVEL);
				if (tempVEL.y < abs(tempVEL.x) * 0.3f) {
					tempVEL.y = abs(tempVEL.x) * 0.3f;
				}
				balls[idBL].set_velocity(tempVEL);
			}
			else if (((abs(Rvec.x) < pltf_lwh.x) && (abs(Rvec.y) < pltf_lwh.y + ball::rad)) || (dist < ball::rad && aRvec.y>0)) { // bounce off edge
				tempVEL.y *= -1.0f;
				if (Rvec.x > 0)	tempVEL.x += 1.0f;
				else tempVEL.x -= 1.0f;
				tempVEL = glm::normalize(tempVEL);
				if ((glfwGetTime() - PLTF.get_TimeModif() ) * 0.5 + PLTF.get_DeltaModif() * 0.5 < 0.03) {
					tempVEL.x += float(PLTF.get_direction());
				}
				tempVEL = glm::normalize(tempVEL);
				if (tempVEL.y < abs(tempVEL.x) * 0.3f) {
					tempVEL.y = abs(tempVEL.x) * 0.3f;
				}
				balls[idBL].set_velocity(tempVEL);
			}
		}
	}
}

// Function that checks if there was interaction between bonus and platform and handles if so
bool level::check_bonus(const int &idBON, platform& pltf) {
	glm::vec3 pltf_lwh = pltf.get_lwh();
	glm::vec2 Rvec = pltf.get_position() - bonuses[idBON].get_position();
	if (abs(Rvec.x) < pltf_lwh.x + bonus::lwh.x && abs(Rvec.y) < pltf_lwh.y + bonus::lwh.y) {
		unsigned int bonus_state = bonuses[idBON].get_state();
		if (bonus_state == 0) {
			pltf.set_xscale(1.5f);
		}
		else if (bonus_state == 1 && balls.size() < 2) {
			ball tempBLL = balls[0];
			glm::vec2 velBLL = balls[0].get_velocity();
			velBLL.y += 1.0f;
			tempBLL.set_velocity(velBLL);
			balls.push_back(tempBLL);
			pltf.set_xscale(1.0f);
		}
		else if (bonus_state == 2) {
			pltf.set_xscale(0.75f);
		}
		return true;
	}
	return false;
}

// Check for losing condition
void level::check_lose() {
	if (balls.size() == 1 && balls[0].out_of_bounds() ) {
		end_level = true;
		win_cond = false;
		Sleep(100);
	}
}

// Remove all objects if any still present
void level::clean_level() {
	bricks.clear();
	bonuses.clear();
	balls.clear();

	end_level = true;
	grade = 1;
	score = 0;
	win_cond = false;
}
