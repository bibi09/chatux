#include <chatux/ChatClient.h>
#include <chatux/ChatWindow.h>
#include <chatux/TypeWindow.h>

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

	waddstr(m_cursesWindowPtr, WINDOW_TIPS) ;
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
						waddstr(m_cursesWindowPtr, m_typedText.c_str()) ;
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
					waddstr(m_cursesWindowPtr, m_typedText.c_str()) ;
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
		wechochar(m_cursesWindowPtr, key) ;
		key = wgetch(m_cursesWindowPtr) ;
		m_typedText.push_back(key) ;
		wechochar(m_cursesWindowPtr, key) ;
		m_typedText.push_back(key) ;
	}
	else {
		// ASCII!
		wechochar(m_cursesWindowPtr, key) ;
		m_typedText.push_back(key) ;
	}
}

void TypeWindow::display() {
	short bgColor = getColorPair(NAME_COLOR_TYPE) ;
	wbkgd(m_cursesWindowPtr, COLOR_PAIR(bgColor)) ;
}

void TypeWindow::eraseContent() {
	wclear(m_cursesWindowPtr) ;
	waddstr(m_cursesWindowPtr, WINDOW_TIPS) ;
}
