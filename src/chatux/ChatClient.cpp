#include <chatux/ChatClient.h>
#include <chatux/ChatWindow.h>
#include <chatux/TypeWindow.h>
#include <iostream>

using namespace es ;
using namespace std ;

ChatClient* ChatClient::Instance = 0 ;
bool ChatClient::IsConfigurated = false ;
string ChatClient::ServerIP ;
unsigned short ChatClient::ServerPort = 0 ;

extern bool RunningApplication ;
extern Context ChatuxContext ;
extern ChatWindow* GlobalChatWindow ;
extern TypeWindow* GlobalTypeWindow ;


ChatClient::ChatClient() : Client(ServerIP, ServerPort) {
	send(PROTOCOL_ALIAS, ChatuxContext.Alias) ;
}

ChatClient::~ChatClient() throw() {}


ChatClient* ChatClient::getInstance() {
    if (IsConfigurated && (Instance == 0))
        Instance = new ChatClient() ;
    return Instance ;
}


bool ChatClient::configure(const std::string& serverIP,
						   unsigned short serverPort) {
	if (!IsConfigurated) {
		ChatClient::ServerIP = serverIP ;
		ChatClient::ServerPort = serverPort ;
		IsConfigurated = true ;
	}

	return IsConfigurated ;
}


void* ChatClient::manageThread(void*) {
    Instance -> run() ;
    return 0 ;
}

void ChatClient::createThread() {
    pthread_t clientThread ;
    assert(pthread_create(&clientThread, 0, &ChatClient::manageThread, 0) == 0) ;
}


void ChatClient::run() {
	// Buffer to receive messages.
	static char buffer[BUFFER_MESSAGE_SIZE] ;
	// Buffer to save only the alias of the user who sent the message.
	static char alias[USER_ALIAS_LENGTH] ;
	// Real size of the received message.
	int messageSize = 0 ;
	// Socket descriptor from which the message are read.
	int socketfd = m_socket.getSocket() ;

	// This thread is waiting for new messages from the server
	while (m_isRunning) {
		messageSize = m_socket.recv(socketfd,
									&buffer,
									BUFFER_MESSAGE_SIZE) ;

		if (messageSize > 0) {
			// Display the message...
			switch(buffer[0]) {
				case PROTOCOL_SERVER_DOWN:
					m_isRunning = false ;
					RunningApplication = false ;
					break ;

				case PROTOCOL_DISCONNECT:
					break ;

				case PROTOCOL_ECHO:
					{
					static char* offsetMessage = buffer + 1 + USER_ALIAS_LENGTH ;
					memcpy(alias, buffer + 1, USER_ALIAS_LENGTH) ;
					GlobalChatWindow -> echo(alias, offsetMessage) ;
					}
					break ;

				case PROTOCOL_ALIAS:
					break ;
			}
		}
	}
}


void ChatClient::send(PROTOCOL_FLAGS protocol, const string& message) {
	static const int MAX_MESSAGE_LENGTH = BUFFER_MESSAGE_SIZE - 2 ;
	if (message.size() > MAX_MESSAGE_LENGTH)
		// @TODO write into logs!
		return ;

	// The socket of the client to send the message to the server
	static int socketServer = m_socket.getSocket() ;

	// Buffer to send messages.
	static char buffer[BUFFER_MESSAGE_SIZE] ;
	static char* offsetStr = buffer + 1 ;
	buffer[0] = (char) protocol ;
	strcpy(offsetStr, message.c_str()) ;
	m_socket.write(socketServer, buffer, message.size() + 2) ;
}


void ChatClient::echo(const std::string& message) {
	send(PROTOCOL_ECHO, message) ;
}
