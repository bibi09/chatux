#include <chatux/ChatServer.h>
#include <iostream>

using namespace es ;
using namespace std ;

ChatServer* ChatServer::Instance = 0 ;
bool ChatServer::IsConfigurated = false ;
unsigned short ChatServer::ServerPort = 0 ;


ChatServer::ChatServer() : Server(ServerPort) {}

ChatServer::~ChatServer() throw() {
	char buffer = PROTOCOL_SERVER_DOWN ;
	sendAll(&buffer, sizeof(buffer)) ;
}


ChatServer* ChatServer::getInstance() {
    if (IsConfigurated && (Instance == 0))
        Instance = new ChatServer() ;
    return Instance ;
}

bool ChatServer::configure(unsigned short serverPort) {
	if (!IsConfigurated) {
		ChatServer::ServerPort = serverPort ;
		IsConfigurated = true ;
	}

	return IsConfigurated ;
}


void* ChatServer::manageClient(int socket) {
	// Received buffer message.
	char received[BUFFER_MESSAGE_SIZE] ;
	// Buffer to write the message to send.
	char sent[BUFFER_MESSAGE_SIZE] ;
	// Real size of the buffer (lower of equal to the maximal size).
	int realSize, sizeSent ;
	// Switch to safely stop the thread.
	bool continueThread = true ;
	// Alias linked to the client.
	string alias ;

	while(continueThread
			&& (realSize = m_socket.recv(socket, received, BUFFER_MESSAGE_SIZE))) {
		// Parse the protocol and make action according to this
		switch(received[0]) {
			case PROTOCOL_DISCONNECT:
				// Set the message to send and notify all the clients
//				memcpy(received + 1, m_aliases[socket].c_str(), m_aliases[socket].size()) ;
//				sendAll(received, m_aliases[socket].size() + 1) ;
				continueThread = false ;
				break ;

			case PROTOCOL_ECHO:
				{
				if (alias.empty())
					// The client is not known, drop its message and stop the
					// communication with it.
					continueThread = false ;

				// Notify all the clients
				sizeSent = sizeof(char) + USER_ALIAS_LENGTH + realSize - 1 ;
				sent[0] = PROTOCOL_ECHO ;
				memcpy(sent + 1, alias.c_str(), USER_ALIAS_LENGTH) ;
				memcpy(sent + 1 + USER_ALIAS_LENGTH, received + 1, realSize - 1) ;
				sendAll(sent, sizeSent) ;
				}
				break ;

			case PROTOCOL_ALIAS:
				// Manage alias setting
				if (alias.empty())
					alias = received + 1 ;
				break ;
		}
	}

	return 0 ;
}

void ChatServer::send(int client_socket, void* message, size_t messageSize) {
	m_socket.write(client_socket, message, messageSize) ;
}
