#include <chatux/Chatux.h>
#include <chatux/Window.h>
#include <chatux/ChatWindow.h>
#include <chatux/TypeWindow.h>
#include <chatux/ChatClient.h>
#include <chatux/ChatServer.h>
#include <easylog/Logger.h>

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <term.h>
#include <unistd.h>
#include <iostream>

using namespace std ;


Context ChatuxContext ;
ChatWindow* GlobalChatWindow ;
TypeWindow* GlobalTypeWindow ;
bool RunningApplication ;


/** @brief	Initialization. */
static void init() {
	setlocale(LC_ALL, "") ;
	// Initialize NCURSES
	Window::setupCurses() ;
}


/***
 * @brief	Convert the alias to wchar.
 * @param	alias	Alias entered by the user.
 * @warning	This is just a stabilization function, it does not convert special
 *			chars.
 */
static wchar_t* aliasToWChar(char* alias) {
	static wchar_t* walias ;
	string input = alias ;
	wstring output(input.begin(), input.end()) ;
	walias = (wchar_t*) output.c_str() ;
	return walias ;
}

/**
 * @brief	Set up the context of the application from the given arguments.
 * @param	argc	Count of arguments.
 * @param	argv	Values of the arguments.
 * @return	0 if an error occured.
 *			d if the application is a dedicated server.
 *			s if the application is a server and a client.
 *			c if the application is a client only.
 */
static char setContext(int argc, char** argv) {
	if (argc < 5)
		return 0 ;

	char connectionType = tolower(argv[1][0]) ;
	if ((connectionType != 's') && (connectionType != 'c'))
		return 0 ;

	wchar_t* wargv2 = aliasToWChar(argv[2]) ;
	ChatuxContext.Alias[USER_ALIAS_LENGTH] = L'\0' ;
	size_t originalAliasLength = strlen(argv[2]) ;
	if (originalAliasLength > USER_ALIAS_LENGTH)
		wmemcpy(ChatuxContext.Alias, wargv2, USER_ALIAS_LENGTH) ;
	else {
		size_t diffSize = USER_ALIAS_LENGTH - originalAliasLength ;
		wmemset(ChatuxContext.Alias, L' ', diffSize) ;
		wmemcpy(ChatuxContext.Alias + diffSize, wargv2, originalAliasLength) ;
	}

	ChatuxContext.IP = argv[3] ;
	ChatuxContext.Port = atoi(argv[4]) ;


	/** Create the required threads. **/
	if (connectionType == 's') {
		// Create the server thread	
		if (!ChatServer::configure(ChatuxContext.Port)) {
			cerr << "The server thread could not be initialized." << endl ;
			exit(EXIT_FAILURE) ;
		}
	}

	// Create the client thread
	if (!ChatClient::configure(ChatuxContext.IP, ChatuxContext.Port)) {
		cerr << "The client thread could not be initialized." << endl ;
		exit(EXIT_FAILURE) ;
	}

	return connectionType ;
}


/** @brief	Launch the windows creation and the loop of the application. */
static void launch() {
	// Create the windows
	int width = 0, height = 0 ;
	getmaxyx(stdscr, height, width) ;

	short typeHeight = 1 ;
	short typeOffset = height - typeHeight ;
	ChatWindow chatWin(width, typeOffset - 1, 0, 0) ;
	TypeWindow typeWin(width, typeHeight, 0, typeOffset) ;
	GlobalChatWindow = &chatWin ;
	GlobalTypeWindow = &typeWin ;


	// Run the application
	RunningApplication = true ;
	chatWin.display() ;
	typeWin.display() ;
	while(RunningApplication) {
		chatWin.update() ;
		typeWin.update() ;
	}


	// Clear the application context
	GlobalTypeWindow = 0 ;
	GlobalChatWindow = 0 ;
}



/** @brief Main function */
int main(int argc, char** argv) {
	Logger::init(argv[0]) ;

	char type = setContext(argc, argv) ;
	if (type) {
		switch(type) {
			case 'd' :
				ChatServer::getInstance() ;
				break ;
			case 's' :
				ChatServer::getInstance() -> start() ;
				// No break since the server launches a client too
			case 'c' :
				ChatClient::getInstance() -> start() ;
				break ;
		}

		init() ;
		launch() ;
	}
	else {
		WRITE_COLOR(TEXT_RED, "Bad command, expected arguments not provided:\n") ;
		cerr << argv[0] << " <Type> <Alias> <IP> <Port>" << endl ;
		cerr << endl ;
		cerr << "* Type of application: S to launch a server, C to be a client" << endl ;
		cerr << "* Alias: 1 to " << USER_ALIAS_LENGTH << " character-length" << endl ;
		cerr << "* Server IP:   Formatted as xxx.xxx.xxx.xxx" << endl ;
		cerr << "* Server Port: From 0 to 65535" << endl ;
		cerr << endl ;
		WRITE_COLOR(TEXT_CYAN, "Example for server + client: ") ;
		cerr << argv[0] << " S ItIsMe 92.123.12.123 1234" << endl ;
		WRITE_COLOR(TEXT_CYAN, "Example for client only:     ") ;
		cerr << argv[0] << " C ItIsMe 92.123.12.123 1234" << endl ;
		cerr << endl ;
		WRITE_COLOR(TEXT_YELLOW, "You may want to use \"run_chatux.sh\" for an interactive setup.\n") ;
		exit(EXIT_FAILURE) ;
	}

	Logger::close() ;
	exit(EXIT_SUCCESS) ;
}
