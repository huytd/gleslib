#ifndef _GOBJECT_H_
#define _GOBJECT_H_

// EMULATE MOBILE DEVICE EVENT
#define EVENT_TAP 1				// Mouse Click
#define EVENT_TOUCH_BEGIN 2		// Mouse Down
#define EVENT_TOUCH_END 3		// Mouse Up
#define EVENT_TOUCH_MOVE 4		// Mouse Move when Mouse Down Enabled
#define EVENT_PINCH_IN 5		// Mouse Wheel up
#define EVENT_PINCH_OUT 6		// Mouse Wheel down
// FOR PC ONLY
#define EVENT_KEY_UP 7			// Keyboard Up 
#define EVENT_KEY_DOWN 8		// Keyboard Down

class GObject
{
public:
	virtual void init(){}
	virtual void loop(){}
	virtual void render(){}
	virtual void input(int EVENT_TYPE, unsigned char k = 0, int x = -1, int y = -1){}
};
#endif