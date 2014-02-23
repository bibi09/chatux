#include <chatux/ChatServer.h>
#include <easylog/Logger.h>
#include <iostream>

using namespace es ;
using namespace std ;

ChatServer* ChatServer::Instance = 0 ;
bool ChatServer::IsConfigurated = false ;
unsigned short ChatServer::ServerPort = 0 ;


ChatServer::ChatServer() : Server(ServerPort) {
	#if DEBUG
	Logger::print(LOG_DEBUG, (char*) "[Server] Binded to port %d", ServerPort) ;
	#endif
}

ChatServer::~ChatServer() throw() {
	#if DEBUG
	Logger::print(LOG_DEBUG, (char*) "[Server] Shutting down") ;
	#endif

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

	#if DEBUG
	if (!IsConfigurated)
		Logger::print(LOG_ERR, (char*) "[Server] Not correctly configurated") ;
	#endif

	return IsConfigurated ;
}


void* ChatServer::manageClient(int socket) {
	// Received buffer message.
	wchar_t received[BUFFER_MESSAGE_SIZE] ;
	// Buffer to write the message to send.
	wchar_t sent[BUFFER_MESSAGE_SIZE] ;
	// Real size of the buffer (lower of equal to the maximal size).
	int realSize, sizeSent ;
	// Switch to safely stop the thread.
	bool continueThread = true ;
	// Alias linked to the client.
	wstring alias ;

	// Length of the message header.
	static const int headerLength = 1 + CLIENT_ID_SIZE ;
	static const int headerAliasLength = 1 + CLIENT_ID_SIZE + USER_ALIAS_LENGTH ;

	// Cache some offsets.
	wchar_t* offsetRecv1 = received + 1 ;
	wchar_t* offsetSent1 = sent + 1 ;
	wchar_t* offsetSentID = offsetSent1 + CLIENT_ID_SIZE ;
	wchar_t* offsetSentAlias = offsetSentID + USER_ALIAS_LENGTH ;

	while (continueThread
			&& (realSize = m_socket.recv(socket,
										 received,
										 BUFFER_MESSAGE_SIZE_BYTES))) {
		// Parse the protocol and make action according to this
		switch (received[0]) {
			case PROTOCOL_DISCONNECT:
				// Set the message to send and notify all the clients
//				wmemcpy(received + 1, m_aliases[socket].c_str(), m_aliases[socket].size()) ;
//				sendAll(received, sizeof(wchar_t) * (m_aliases[socket].size() + 1)) ;
				continueThread = false ;
				break ;

			case PROTOCOL_ECHO:
				{
					if (alias.empty()) {
						// The client is not known, drop its message and stop
						// the communication with it.
						continueThread = false ;
					}

					// Compute the total size of the sent message
					size_t messageLength = headerLength + wcslen(offsetRecv1) ;
					sizeSent = headerAliasLength + messageLength ;

					// Construction of the message:
					// [ PROTOCOL | ID | ALIAS | MESSAGE ]
					sent[0] = PROTOCOL_ECHO ;
					memcpy(offsetSent1, &socket, sizeof(socket)) ;
					wmemcpy(offsetSentID, alias.c_str(), USER_ALIAS_LENGTH) ;
					wmemcpy(offsetSentAlias, offsetRecv1, messageLength) ;

					// Notify all the clients
					sendAll(sent, sizeof(wchar_t) * sizeSent) ;
				}
				break ;

			case PROTOCOL_ALIAS:
				// Manage alias setting
				if (alias.empty())
					alias = offsetRecv1 ;
				break ;
		}
	}

	return 0 ;
}


void ChatServer::send(int client_socket, void* message, size_t messageSize) {
	wchar_t* trueMessage = (wchar_t*) message ;
	// Socket of the original client who sent the message
	int originalClientSocket ;
	memcpy(&originalClientSocket, trueMessage + 1, sizeof(originalClientSocket)) ;

	if (originalClientSocket == client_socket) {
		wchar_t specialMessage[messageSize] ;
		memcpy(specialMessage, trueMessage, messageSize) ;
		specialMessage[0] = PROTOCOL_ECHO_SELF ;
		m_socket.write(client_socket, specialMessage, messageSize) ;
	}
	else {
		m_socket.write(client_socket, message, messageSize) ;
	}
}
