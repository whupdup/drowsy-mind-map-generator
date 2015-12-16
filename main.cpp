#include "graphics/window.hpp"

void paint(Window *window) {
	window->drawBubble(std::string("hey"), 20, 50, 100, 35);
}

int main(int argc, char **argv) {
	Window *window = new Window(std::string("Hello world!"));
	window->setPaintCallback(paint);

	while (window->process()) {

	}

	return 0;
}

