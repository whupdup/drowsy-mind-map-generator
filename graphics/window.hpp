#ifndef graphics_window_hpp
#define graphics_window_hpp

#include <string>

class Window {
	public:
		Window(std::string);
		void drawBubble(std::string, unsigned int, int, int, int, int);
		void drawLine(int, int, int, int);
		void getBubbleSize(std::string, unsigned int, int*, int*);
		bool process();
		void resize(int x, int y);
		void setPaintCallback(void(*)(Window*));
		~Window();

	private:

};

#endif
