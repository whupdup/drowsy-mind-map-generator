#include <iostream>
#include <SFML/Window.hpp>

int main(int argc, char **argv) {
	sf::VideoMode videoMode(640, 480);
	sf::Window window(videoMode, "Basic window");
	window.display();
	window.close();
	return 0;
}
