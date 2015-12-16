#include "window.hpp"

#define WINVER 0x0600
#define _WIN_WINNT 0x0A00

#include <windows.h>
#include <Wingdi.h>

static const int FONT_SIZE = 20;
static const char szAppName[] = "myWindowClass";
static MSG Msg;
static void (*paintCallback)(Window*);
static Window *currentWindow;

void blankCallback(Window *window) { }
HDC *currentDC;

void Window::getBubbleSize(std::string text, int *x, int *y) {
	SIZE size;
	TEXTMETRIC metric;
	HFONT hFont = CreateFont(FONT_SIZE,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
			DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY,DEFAULT_PITCH,TEXT("Arial"));
	SelectObject(*currentDC, hFont);
	SetTextCharacterExtra(*currentDC, 0);
	GetTextMetrics(*currentDC, &metric);
	GetTextExtentPoint(*currentDC, text.c_str(), text.length(), &size);
	*x = (size.cx + metric.tmOverhang) * 1.05 + 20;
	*y = (FONT_SIZE) * 1.4;
}

void Window::drawBubble(std::string text, int x1, int y1, int x2, int y2) {
	// TODO: Calculate size automatically and return it
	// Use that later to determine how to position bubbles
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd144938(v=vs.85).aspx

	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(50, 50, 50));
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
	HFONT hFont = CreateFont(FONT_SIZE,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
			DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("Arial"));
	SelectObject(*currentDC, hFont);

	DrawText(*currentDC, text.c_str(), text.length(), &rect, 
			DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT Msg,
		WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT Ps;
	HBRUSH newBrush;
	HPEN newPen;

	switch(Msg)
	{
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
			CW_USEDEFAULT, CW_USEDEFAULT, 420, 220,
			NULL, NULL, NULL, NULL);

	paintCallback = blankCallback;
	currentWindow = this;
}

void Window::setPaintCallback(void(*callback)(Window*)) {
	paintCallback = callback;
}

bool Window::process() {
	bool cont = GetMessage(&Msg, NULL, 0, 0);

	if (cont) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return cont;
}
