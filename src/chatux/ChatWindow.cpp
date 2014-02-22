#include <chatux/Chatux.h>
#include <chatux/ChatWindow.h>
#include <time.h>

using namespace std ;

#define NAME_COLOR_CHATTIME		"ChatTime"
#define NAME_COLOR_CHATME		"ChatMe"
#define NAME_COLOR_CHATOTHERS	"ChatOthers"
#define NAME_COLOR_CHATMSG		"ChatMsg"

// Context containing data on the user and application
extern Context ChatuxContext ;


ChatWindow::ChatWindow(unsigned short width, unsigned short height,
			   	   	   unsigned short posX, unsigned short posY)
			: Window(width, height, posX, posY, true) {
	setColorPair(NAME_COLOR_CHATTIME, COLOR_YELLOW, COLOR_BLACK) ;
	setColorPair(NAME_COLOR_CHATME, COLOR_YELLOW, COLOR_BLACK) ;
	setColorPair(NAME_COLOR_CHATOTHERS, COLOR_CYAN, COLOR_BLACK) ;
	setColorPair(NAME_COLOR_CHATMSG, COLOR_RED, COLOR_BLACK) ;
}

ChatWindow::~ChatWindow() {}



void ChatWindow::update() {
	this -> refresh() ;
}

void ChatWindow::display() {
	short defaultColor = getColorPair() ;
	wbkgd(m_cursesWindowPtr, COLOR_PAIR(defaultColor)) ;
}

void ChatWindow::echo(const string& alias, const string& msg) {
	// Display the date of the message
	wattron(m_cursesWindowPtr, COLOR_PAIR(getColorPair(NAME_COLOR_CHATTIME))) ;
		waddstr(m_cursesWindowPtr, "[") ;
		waddstr(m_cursesWindowPtr, getTime()) ;
		waddstr(m_cursesWindowPtr, "] ") ;
	wattroff(m_cursesWindowPtr, COLOR_PAIR(getColorPair(NAME_COLOR_CHATTIME))) ;

	// Display the alias of the user who sent the message
	wattron(m_cursesWindowPtr, COLOR_PAIR(getColorPair(NAME_COLOR_CHATOTHERS))) ;
		waddstr(m_cursesWindowPtr, alias.c_str()) ;
		waddstr(m_cursesWindowPtr, " : ") ;
	wattroff(m_cursesWindowPtr, COLOR_PAIR(getColorPair(NAME_COLOR_CHATOTHERS))) ;

	waddstr(m_cursesWindowPtr, msg.c_str()) ;
	waddch(m_cursesWindowPtr, '\n') ;
}

void ChatWindow::eraseContent() {
}


char* ChatWindow::getTime() {
	static const unsigned char TIME_BUFFER_SIZE = 9 ;
	static char timeBuffer[TIME_BUFFER_SIZE] ;
	time_t now ;
	struct tm* timeinfo ;

	time(&now) ;
	timeinfo = localtime(&now) ;
	strftime(timeBuffer, TIME_BUFFER_SIZE, "%T", timeinfo) ;
	return timeBuffer ;
}
