#include <chatux/Window.h>
#include <iostream>
#include <assert.h>

using namespace std ;

bool Window::InitializedNCurses = false ;
unsigned char Window::RunningWindows = 0 ;
map<string, unsigned char> Window::ColorPairs ;


Window::Window(unsigned short width, unsigned short height,
			   unsigned short posX, unsigned short posY,
			   bool scroll) {
	if (!InitializedNCurses) {
		setupCurses() ;
	}

	m_cursesWindowPtr = subwin(stdscr, height, width, posY, posX) ;
	m_scrollContent = scroll ;
	scrollok(m_cursesWindowPtr, m_scrollContent) ;
	leaveok(m_cursesWindowPtr, true) ;
	RunningWindows = RunningWindows + 1 ;
}

Window::~Window() {
	delwin(m_cursesWindowPtr) ;
	RunningWindows = RunningWindows - 1 ;

	if (RunningWindows == 0)
		endwin() ;
}


void Window::refresh() {
	wrefresh(m_cursesWindowPtr) ;
}

void Window::setColorPair(const string& name,
				  short foreground,
				  short background) {
	// Do not alter the default color
	if (name == DEFAULT_COLOR_NAME)
		return ;


	short colorID ;
	if (ColorPairs.count(name) == 0) {
		// Add the color pair
		colorID = ColorPairs.size() + 1 ;
		init_pair(colorID, foreground, background) ;
		ColorPairs[name] = colorID ;
	}
	else {
		colorID = ColorPairs[name] ;
		init_pair(colorID, foreground, background) ;
	}
}

short Window::getColorPair(const std::string& name) {
	if (ColorPairs.count(name) == 0)
		return ColorPairs[DEFAULT_COLOR_NAME] ;
	else
		return ColorPairs[name] ;
}


void Window::setupCurses() {
	initscr() ;
	start_color() ;

	short defaultColorID = 0 ;
	init_pair(defaultColorID, COLOR_WHITE, COLOR_BLACK) ;
	ColorPairs[DEFAULT_COLOR_NAME] = defaultColorID ;
	bkgd(COLOR_PAIR(defaultColorID)) ;

	curs_set(0) ;
	noecho() ;
	halfdelay(1) ;
	nonl() ;

	InitializedNCurses = true ;
}


void Window::moveCursor(Direction dir) {
	// Get the maximal position of the cursor in the window.
	int maxX, maxY ;
	getmaxyx(m_cursesWindowPtr, maxY, maxX) ;
	// Get the current position of the cursor in the window.
	int curX, curY, nextX, nextY ;
	getyx(m_cursesWindowPtr, curY, curX) ;

	// Make the cursor move.
	switch(dir) {
		case CURSOR_UP:
			if (curY > 0 || m_scrollContent) {
				nextY = curY - 1 ;
				// Make the cursor go up if possible.
				move(nextY, curX) ;
				wmove(m_cursesWindowPtr, nextY, curX) ;
			}
			break ;

		case CURSOR_DOWN:
			if (curY < maxY || m_scrollContent) {
				nextY = curY + 1 ;
				// Make the cursor go down if possible.
				move(nextY, curX) ;
				wmove(m_cursesWindowPtr, nextY, curX) ;
			}
			break ;

		case CURSOR_LEFT:
			if (curX > 0) {
				nextX = curX - 1 ;
				// Make the cursor go left if possible.
				move(curY, nextX) ;
				wmove(m_cursesWindowPtr, curY, nextX) ;
			}
			else if (curY > 0 || m_scrollContent) {
				nextY = curY - 1 ;
				nextX = maxX - 1 ;
				// Make the cursor go at the end of previous line if possible.
				move(nextY, nextX) ;
				wmove(m_cursesWindowPtr, nextY, nextX) ;
			}
			break ;

		case CURSOR_RIGHT:
			if (curX < maxX) {
				nextX = curX + 1 ;
				// Make the cursor go right if possible.
				move(curY, nextX) ;
				wmove(m_cursesWindowPtr, curY, nextX) ;
			}
			else if (curY < maxY || m_scrollContent) {
				nextY = curY + 1 ;
				// Make the cursor go at the beginning of next line if possible.
				move(nextY, 0) ;
				wmove(m_cursesWindowPtr, nextY, 0) ;
			}
			break ;
	}

	refresh() ;
}
