#ifndef graphics_window_hpp
#define graphics_window_hpp

#include <string>

class Window {
	public:
		Window(std::string);
		void drawBubble(std::string, int, int, int, int);
		void getBubbleSize(std::string, int*, int*);
		bool process();
		void setPaintCallback(void(*)(Window*));
		~Window();

	private:

};

#endif
