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

void drawRect(float x, float y, float w, float h) {
	GLfloat vertices[] = {
		 x, y+h,  0.0f,  // 0. left-bottom
		 x+w, y+h,  0.0f,  // 1. right-bottom
		 x,  y,  0.0f,  // 2. left-top
		 x+w,  y,  0.0f   // 3. right-top
	};
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Game::render() {
	drawRect(5, 5, 200, 200);
}

void Game::input(int EVENT_TYPE, unsigned char k, int x, int y) {
}

#endif