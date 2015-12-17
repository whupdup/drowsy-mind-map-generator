#include "window.hpp"

#define WINVER 0x0600
#define _WIN_WINNT 0x0A00

#include <windows.h>
#include <windowsx.h>
#include <Wingdi.h>

#include <iostream>

static const int FONT_SIZE = 16;
static const char szAppName[] = "myWindowClass";
static MSG Msg;
static void (*paintCallback)(Window*);
static Window *currentWindow;
static HWND lhWnd;

void blankCallback(Window *window) { }
HDC *currentDC;

void Window::getBubbleSize(std::string text, unsigned int complexity, int *x, 
		int *y) {
	SIZE size;
	TEXTMETRIC metric;
	HFONT hFont = CreateFont(FONT_SIZE+complexity*2,0,0,0,FW_NORMAL,FALSE,FALSE,
			FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY,DEFAULT_PITCH,TEXT("Arial"));
	SelectObject(*currentDC, hFont);
	SetTextCharacterExtra(*currentDC, 0);
	GetTextMetrics(*currentDC, &metric);
	GetTextExtentPoint(*currentDC, text.c_str(), text.length(), &size);
	*x = (size.cx + metric.tmOverhang) * 1.05 + 20;
	*y = (FONT_SIZE) * 1.4;
	DeleteObject(hFont);
}

void Window::resize(int x, int y) {
	SetWindowPos(lhWnd, NULL, 0, 0, x, y, SWP_SHOWWINDOW);
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
	SelectObject(*currentDC, hPen);
	MoveToEx(*currentDC, x1, y1, NULL);
	LineTo(*currentDC, x2, y2);
	DeleteObject(hPen);
}

void Window::drawBubble(std::string text, unsigned int complexity, int x1, 
		int y1, int x2, int y2) {
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 127, 127));
	SelectObject(*currentDC, hPen);
	SelectObject(*currentDC, hBrush);
	Ellipse(*currentDC, x1, y1, x2 + x1, y2 + y1);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2 + x1;
	rect.bottom = y2 + y1;
	SetTextColor(*currentDC, RGB(0, 0, 0));
	SetBkMode(*currentDC, TRANSPARENT);
	HFONT hFont = CreateFont(FONT_SIZE + complexity*2,0,0,0,FW_NORMAL,FALSE,
			FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY,DEFAULT_PITCH,TEXT("Arial"));
	SelectObject(*currentDC, hFont);

	DrawText(*currentDC, text.c_str(), text.length(), &rect, 
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DeleteObject(hFont);
}

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam) {
	lhWnd = hWnd;
	HDC hDC;
	PAINTSTRUCT Ps;
	HBRUSH newBrush;
	HPEN newPen;

	switch(Msg)
	{
		case WM_LBUTTONDOWN:
			currentWindow->mouse1Down = true;
			currentWindow->startPosX = GET_X_LPARAM(lParam);
			currentWindow->startPosY =  GET_Y_LPARAM(lParam);
			break;
		case WM_LBUTTONUP:
			currentWindow->mouse1Down = false;
			break;
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &Ps);
			currentDC = &hDC;
			paintCallback(currentWindow);
			EndPaint(hWnd, &Ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;
		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

Window::Window(std::string name) {
	mouse1Down = false; // "non-static data member intializers only available with -std=c++11"
	mousePosX = 0;
	mousePosY = 0;

	startPosX = 0;
	startPosY = 0;

	offsetX = 0;
	offsetY = 0;

	WNDCLASSEX WndCls;

	WndCls.cbSize        = sizeof(WndCls);
	WndCls.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	WndCls.lpfnWndProc   = WindProcedure;
	WndCls.cbClsExtra    = 0;
	WndCls.cbWndExtra    = 0;
	WndCls.hInstance     = NULL;
	WndCls.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndCls.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndCls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndCls.lpszMenuName  = NULL;
	WndCls.lpszClassName = szAppName;
	WndCls.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndCls);

	CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
			szAppName, name.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT, Window::WIDTH, Window::HEIGHT,
			NULL, NULL, NULL, NULL);

	paintCallback = blankCallback;
	currentWindow = this;
}

void Window::setPaintCallback(void(*callback)(Window*)) {
	paintCallback = callback;
}

void Window::repaint()
{
	currentWindow->offsetX = currentWindow->startPosX - currentWindow->mousePosX;
	currentWindow->offsetY = currentWindow->startPosY - currentWindow->mousePosY;

	RedrawWindow(lhWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);
	//SendMessage(lhWnd, WM_PAINT, 0, 0);
}

bool Window::process() {
	bool cont = GetMessage(&Msg, NULL, 0, 0) > 0;

	POINT p;

	if (GetCursorPos(&p))
	{
		ScreenToClient(lhWnd, &p);
		mousePosX = p.x;
		mousePosY = p.y;
	}

	if (cont) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return cont;
}
