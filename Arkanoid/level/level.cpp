#include "level.h"

level::level() {
	end_level = true;
	grade = 1;
	score = 0;
	win_cond = false;
}

void level::load_level(const unsigned int &_grade) {
	grade = _grade;
	std::fstream inputF;
	glm::vec2 _pos = glm::vec2(0.0,0.0);
	int state = 0;
	try {
		_background.set_state(grade);
	}
	catch(int i){
		std::cout << "Error: " << i << "Falied background loading.\n";
	}
	//open, read file with code for vertex shader, create and compile, close file
	try
	{
		if (grade == 1 || grade == 2) inputF.open("level1_distribution.dat", std::fstream::in);
		else {
			std::cout << "This level is not yet implemented.\n";
			throw 103;
		}
		while (inputF >> _pos.x >> _pos.y >> state) {
			brick tempBrick( _pos, state );
			bricks.push_back(tempBrick);
		}
		inputF.close();
		if (brick::get_count() != bricks.size()) {
			std::cout << "Something wrong with brick count.\n";
			exit(EXIT_FAILURE);
		}
	}
	catch (std::fstream::failure e) {
		std::cout << "ERROR while reading level1_distribution.dat file: " << std::endl;
	}
	_ball.set_position(glm::vec3(0.0f, -8.25f, 0.0f));
	_ball.set_velocity(glm::vec3(0.6f,0.8f,0.0f));
	_ball.set_speed(0.0f);
}

void level::play_level(GLFWwindow* window, Shader &_SO) {
	glfwSetCursorPosCallback(window, player::mouse_callback);
	if (bricks.size() > 0) end_level = false;
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

	int i = 0;
	while (!end_level) {
		level_process_input(window);

		float timeVal = glfwGetTime();
		deltaTime = timeVal - lastFrame;
		lastFrame = timeVal;
		std::cout << "Your score is: " << score << "\r";

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// set the default color to which the screen is reset
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen

		_background.draw(_SO);

		i = 0;
		bricks[0].prepare_to_draw(_SO);
		while (i < bricks.size()) {
			if (check_bounce_brick(bricks[i], _ball)) {
				bricks.erase(bricks.begin() + i);
				score += 10*grade;
				//std::cout << "Score: " << score << std::endl;
				//std::cout << "No. of bricks left: " << bricks.size() << std::endl;
				continue;
			}
			bricks[i].draw(_SO);
			++i;
		}
		if (bricks.size() == 0) {
			end_level = true;
			win_cond = true;
		}

		_player.draw(_SO);

		_ball.prepare_to_draw(_SO);
		_ball.draw(_SO);
		_ball.propagate(deltaTime);
		check_bounce_platform(_player.plat, _ball);
		check_lose(_ball);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void level::level_process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // if the escape key was pressed then
		end_level = true;
		glfwSetWindowShouldClose(window, true);	// set the variable for window closing to true
		Sleep(10);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { // if the escape key was pressed then
		_ball.set_speed(10.0f * grade);
	}
}

bool level::check_bounce_brick(const brick& BRCK, ball& BLL) {
	glm::vec3 Rvec = BLL.futurePosition - BRCK.position;
	glm::vec3 aRvec = glm::vec3(abs(Rvec.x) - BRCK.lwh.x, abs(Rvec.y) - BRCK.lwh.y, 0.0f);
	float dist = glm::distance(aRvec, glm::vec3(0.0f, 0.0f, 0.0f));
	if ((abs(Rvec.x) < BRCK.lwh.x + BLL.rad) && (abs(Rvec.y) < BRCK.lwh.y)) {	// bounce off side
		BLL.velocity.x *= -1.;
		BLL.velocity = glm::normalize(BLL.velocity);
		return true;
	}
	else if ((abs(Rvec.x) < BRCK.lwh.x) && (abs(Rvec.y) < BRCK.lwh.y + BLL.rad)) { // bounce off top/bottom
		BLL.velocity.y *= -1.;
		BLL.velocity = glm::normalize(BLL.velocity);
		return true;
	}
	else if (dist < BLL.rad) {	// bounce of corner
		if (Rvec.x < 0) {
			aRvec.x *= -1.;
		}
		if (Rvec.y < 0) {
			aRvec.y *= -1.;
		}
		aRvec = glm::normalize(aRvec);
		float vdotn = glm::dot(BLL.velocity, aRvec);
		BLL.velocity -= 2.0f * vdotn * aRvec;
		BLL.velocity = glm::normalize(BLL.velocity);
		return true;
	}
	return false;
}

void level::check_bounce_platform(const platform& PLTF, ball& BLL) {
	if (BLL.velocity.y < 0.0f) {
		glm::vec3 Rvec = BLL.futurePosition - PLTF.position;
		glm::vec3 aRvec = glm::vec3(abs(Rvec.x) - PLTF.lwh.x, abs(Rvec.y) - PLTF.lwh.y, 0.0f);
		float dist = glm::distance(aRvec, glm::vec3(0.0f, 0.0f, 0.0f));
		if ((abs(Rvec.x) < PLTF.lwh.x / 2.0f) && (abs(Rvec.y) < PLTF.lwh.y + BLL.rad)) { // bounce off top/bottom
			BLL.velocity.y *= -1.;
			BLL.velocity = glm::normalize(BLL.velocity);
			if ((glfwGetTime() - PLTF.TimeModif) * 0.5 + PLTF.DeltaModif * 0.5 < 0.03) {
				BLL.velocity.x += float(PLTF.direction);
			}
			BLL.velocity = glm::normalize(BLL.velocity);
		}
		else if (((abs(Rvec.x) < PLTF.lwh.x) && (abs(Rvec.y) < PLTF.lwh.y + BLL.rad)) || (dist < BLL.rad && aRvec.y>0)) { // bounce off edge
			BLL.velocity.y *= -1.0f;
			if (Rvec.x > 0)	BLL.velocity.x += 1.0f;
			else BLL.velocity.x -= 1.0f;
			BLL.velocity = glm::normalize(BLL.velocity);
			if ((glfwGetTime() - PLTF.TimeModif) * 0.5 + PLTF.DeltaModif * 0.5 < 0.03) {
				BLL.velocity.x += float(PLTF.direction);
			}
			BLL.velocity = glm::normalize(BLL.velocity);
		}
	}
}

void level::check_lose(const ball& BLL) {
	if (BLL.position.y < -10.0f) {
		end_level = true;
		win_cond = false;
		Sleep(100);
	}
}