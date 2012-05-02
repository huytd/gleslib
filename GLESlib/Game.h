#ifndef _GAME_H_
#define _GAME_H_

#include "GObject.h"

class Game: public GObject
{
public:
	void init();
	void loop();
	void render();
	void input(int EVENT_TYPE, unsigned char k = 0, int x = 0, int y = 0);
};

void Game::init() {
	
}

void Game::loop() {
}

void Game::render() {
	/*GLfloat vertices[] = {
		-1.0f, -1.0f,  0.0f,  // 0. left-bottom
		 1.0f, -1.0f,  0.0f,  // 1. right-bottom
		-1.0f,  1.0f,  0.0f,  // 2. left-top
		 1.0f,  1.0f,  0.0f   // 3. right-top
	};*/
	GLfloat vertices[] = {
		 0.0f, 480.0f,  0.0f,  // 0. left-bottom
		 320.0f, 480.0f,  0.0f,  // 1. right-bottom
		 0.0f,  0.0f,  0.0f,  // 2. left-top
		 320.0f,  0.0f,  0.0f   // 3. right-top
	};
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Game::input(int EVENT_TYPE, unsigned char k, int x, int y) {
}

#endif