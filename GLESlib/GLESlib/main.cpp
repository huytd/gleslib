#include <stdio.h>
#include <windows.h>
#include <TCHAR.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <time.h>
#include <math.h>
#include "../Game.h"

#define	WINDOW_CLASS _T("PVRShellClass")
#define WINDOW_WIDTH	326
#define WINDOW_HEIGHT	486
#define GAME_WIDTH 320
#define GAME_HEIGHT 480

#define TAP_TIME 600

bool	is_gameEnd = false;
time_t sTime, eTime;

GObject* gameMain = 0;
int mx, my;
bool LMBDOWN, RMBDOWN;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
			is_gameEnd = true;
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if (gameMain) gameMain->input(EVENT_KEY_DOWN, wParam);
			break;
		case WM_KEYUP:
			if (gameMain) gameMain->input(EVENT_KEY_UP, wParam);
			break;
		case WM_MOUSEMOVE:
			mx=(short)LOWORD(lParam);
			my=(short)HIWORD(lParam);
			LMBDOWN=wParam & MK_LBUTTON;
			RMBDOWN=wParam & MK_RBUTTON;
			if (gameMain && (LMBDOWN || RMBDOWN)) gameMain->input(EVENT_TOUCH_MOVE,0, mx, my);
			break;
		case WM_LBUTTONDOWN:
			sTime = time(NULL);
			mx=(short)LOWORD(lParam);
			my=(short)HIWORD(lParam);
			if (gameMain) gameMain->input(EVENT_TOUCH_BEGIN, 0, mx, my);
			break;
		case WM_RBUTTONDOWN:
			sTime = time(NULL);
			mx=(short)LOWORD(lParam);
			my=(short)HIWORD(lParam);
			if (gameMain) gameMain->input(EVENT_TOUCH_BEGIN, 0, mx, my);
			break;
		case WM_LBUTTONUP:
			eTime = time(NULL);
			mx=(short)LOWORD(lParam);
			my=(short)HIWORD(lParam);
			if (gameMain) {
				gameMain->input(EVENT_TOUCH_END, 0, mx, my);
				if (eTime - sTime <= TAP_TIME) gameMain->input(EVENT_TAP, 0, mx, my);
			}
			break;
		case WM_RBUTTONUP:
			eTime = time(NULL);
			mx=(short)LOWORD(lParam);
			my=(short)HIWORD(lParam);
			if (gameMain) {
				gameMain->input(EVENT_TOUCH_END, 0, mx, my);
				if (eTime - sTime <= TAP_TIME) gameMain->input(EVENT_TAP, 0, mx, my);
			}
			break;
		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool TestEGLError(HWND hWnd, char* pszLocation)
{
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		_stprintf(pszStr, _T("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

float DEGREES_TO_RADIANS(int Degree) {
	return ( Degree / 180.0 ) * 3.141592653589793;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow)
{
	int				i;

	// Remove unreferenced formal parameter warning by referencing these parameters.
	(void) nCmdShow;
	(void) lpCmdLine;
	(void) hPrevInstance;

	// Windows variables
	HWND				hWnd	= 0;
	HDC					hDC		= 0;

	// EGL variables
	EGLDisplay			eglDisplay	= 0;
	EGLConfig			eglConfig	= 0;
	EGLSurface			eglSurface	= 0;
	EGLContext			eglContext	= 0;
	NativeWindowType	eglWindow	= 0;
	EGLint				pi32ConfigAttribs[128];
	GLuint	ui32Vbo = 0; // Vertex buffer object handle

	// Register the windows class
	WNDCLASS sWC;
    sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = hInstance;
    sWC.hIcon = 0;
    sWC.hCursor = 0;
    sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = WINDOW_CLASS;
	unsigned int nWidth = WINDOW_WIDTH;
	unsigned int nHeight = WINDOW_HEIGHT;

	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, _T("Failed to register the window class"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, nWidth, nHeight);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( WINDOW_CLASS, _T("GLESLib - Device Emulator"), WS_VISIBLE | WS_SYSMENU,
						 0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, _T("Failed to create the device context"), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}
	eglDisplay = eglGetDisplay((NativeDisplayType) hDC);
    if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}
	i = 0;
	pi32ConfigAttribs[i++] = EGL_RED_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
	pi32ConfigAttribs[i++] = 6;
	pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
	pi32ConfigAttribs[i++] = 0;
	pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[i++] = EGL_NONE;
	EGLint iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		goto cleanup;
	}
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
    if(eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}
	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		goto cleanup;
	}
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		goto cleanup;
	}

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		goto cleanup;
	}
	
	// Init the main game object (MGO)
	gameMain = new Game();

	glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Convert from right-handed to left-handed coordinate system
	glOrthof(0, GAME_WIDTH, GAME_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
	
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // clear blue

	// Call initialize code of MGO
	gameMain->init();

	while(true)
	{
		if (is_gameEnd) break;
		glClear(GL_COLOR_BUFFER_BIT);
		if (!TestEGLError(hWnd, "glClear"))
		{
			goto cleanup;
		}
		
		gameMain->render();
		gameMain->loop();

		eglSwapBuffers(eglDisplay, eglSurface);
		if (!TestEGLError(hWnd, "eglSwapBuffers"))
		{
			goto cleanup;
		}

		// Managing the window messages
		MSG msg;
		PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


cleanup:
	glDeleteBuffers(1, &ui32Vbo);
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);
	// Release the device context
	if (hDC) ReleaseDC(hWnd, hDC);
	// Destroy the eglWindow
	if (hWnd) DestroyWindow(hWnd);
	return 0;
}

