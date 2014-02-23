#include <chatux/ChatClient.h>
#include <chatux/ChatWindow.h>
#include <chatux/TypeWindow.h>
#include <stdlib.h>

#define MEMORY_SIZE		16
#define NAME_COLOR_TYPE	"typeText"
#define WINDOW_TIPS		">> "

using namespace std ;


//extern ChatWindow* GlobalChatWindow ;


TypeWindow::TypeWindow(unsigned short width, unsigned short height,
			   	   	   unsigned short posX, unsigned short posY)
			: Window(width, height, posX, posY, false) {
	keypad(m_cursesWindowPtr, TRUE) ;
	setColorPair(NAME_COLOR_TYPE, COLOR_WHITE, COLOR_BLACK) ;

	Window::printStr(WINDOW_TIPS) ;
	wattron(m_cursesWindowPtr, COLOR_PAIR(getColorPair(NAME_COLOR_TYPE))) ;
	m_typedText.reserve(128) ;
	m_memoryPosition = m_memory.end() ;
}

TypeWindow::~TypeWindow() {}


void TypeWindow::update() {
	static unsigned char key ;

	key = wgetch(m_cursesWindowPtr) ;
	if (key == ((unsigned char) ERR))
		return ;

	if (key > 1 && key < 6) {
		// An arrow key has been pressed.
		switch(key) {
			case 2:			// DOWN
				if (m_memoryPosition != m_memory.end()) {
					m_memoryPosition++ ;
					eraseContent() ;
					if (m_memoryPosition != m_memory.end()) {
						m_typedText = *m_memoryPosition ;
						Window::printStr(m_typedText) ;
					}
					else {
						m_typedText.clear() ;
					}
				}
				break ;

			case 3:			// UP
				if (m_memoryPosition != m_memory.begin()) {
					m_memoryPosition-- ;
					eraseContent() ;
					m_typedText = *m_memoryPosition ;
					Window::printStr(m_typedText) ;
				}
				break ;
		}
	}
	else if (key == 10 || key == 13) {
		if (!m_typedText.empty()) {
			// Send message and clear the typed text!
			ChatClient::getInstance() -> echo(m_typedText) ;
			eraseContent() ;

			if (m_memory.size() > MEMORY_SIZE)
				m_memory.pop_front() ;
			m_memory.push_back(m_typedText) ;
			m_memoryPosition = m_memory.end() ;

			m_typedText.clear() ;
		}
	}
	else if (key == 7 || key == 127) {
		if (m_typedText.size() > 0) {
			moveCursor(CURSOR_LEFT) ;
			// Backspace, remove the last character
			wdelch(m_cursesWindowPtr) ;
			m_typedText.erase(m_typedText.size() - 1, 1) ;
		}
	}
	else if (key >= 0b11000010) {
		// UTF8: Characters are sent on two bytes (or more, but not supported).
		static char utf8Chars[2] ;
		utf8Chars[0] = key ;
		utf8Chars[1] = wgetch(m_cursesWindowPtr) ;
		wchar_t utf8WChar ;
		mbstowcs(&utf8WChar, utf8Chars, sizeof(utf8Chars)) ;
		m_typedText.push_back(utf8WChar) ;
		Window::printChar(utf8WChar) ;
	}
	else {
		// ASCII!
		wchar_t utf8WChar ;
		mbstowcs(&utf8WChar, (char*) &key, sizeof(key)) ;
		Window::printChar(utf8WChar) ;
		m_typedText.push_back(utf8WChar) ;
	}
}

void TypeWindow::display() {
	short bgColor = getColorPair(NAME_COLOR_TYPE) ;
	wbkgd(m_cursesWindowPtr, COLOR_PAIR(bgColor)) ;
}

void TypeWindow::eraseContent() {
	wclear(m_cursesWindowPtr) ;
	Window::printStr(WINDOW_TIPS) ;
}
