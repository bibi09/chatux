#include <chatux/ChatClient.h>
#include <chatux/ChatWindow.h>
#include <chatux/TypeWindow.h>
#include <easylog/Logger.h>
#include <sstream>
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
	#if DEBUG
	Logger::print(LOG_DEBUG,
				  (char*) "[Client] Connection to %s:%d",
				  ServerIP.c_str(),
				  ServerPort) ;
	#endif

	send(PROTOCOL_ALIAS, ChatuxContext.Alias) ;
}

ChatClient::~ChatClient() throw() {
	#if DEBUG
	Logger::print(LOG_DEBUG, (char*) "[Client] Disconnection") ;
	#endif
}


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

	#if DEBUG
	if (!IsConfigurated)
		Logger::print(LOG_ERR, (char*) "[Client] Not correctly configurated") ;
	#endif

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
	static wchar_t buffer[BUFFER_MESSAGE_SIZE] ;
	// Buffer to save only the alias of the user who sent the message.
	static wchar_t alias[USER_ALIAS_LENGTH] ;
	// Real size of the received message.
	int messageSize = 0 ;
	// Socket descriptor from which the message are read.
	int socketfd = m_socket.getSocket() ;

	// This thread is waiting for new messages from the server
	while (m_isRunning) {
		messageSize = m_socket.recv(socketfd,
									&buffer,
									BUFFER_MESSAGE_SIZE_BYTES) ;

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
					wstring tmpAlias ;
					wmemcpy(alias, buffer + 1, USER_ALIAS_LENGTH) ;
					tmpAlias = alias ;
					tmpAlias[USER_ALIAS_LENGTH] = L'\0' ;

					static wchar_t* offsetMessage = buffer + 1 + USER_ALIAS_LENGTH ;
					GlobalChatWindow -> echo(tmpAlias, offsetMessage) ;
					}
					break ;
			}
		}
	}
}


void ChatClient::send(PROTOCOL_FLAGS protocol, const wstring& message) {
	static const unsigned int MAX_MESSAGE_LENGTH = BUFFER_MESSAGE_SIZE - 2 ;
	if (message.size() > MAX_MESSAGE_LENGTH) {
		Logger::print(LOG_WARNING, (char*) "[%s:%d] Message too long and dropped.",
								   __FILE__,
								   __LINE__) ;
		return ;
	}

	// The socket of the client to send the message to the server
	static int socketServer = m_socket.getSocket() ;

	// Buffer to send messages.
	static wchar_t buffer[BUFFER_MESSAGE_SIZE] ;
	static wchar_t* offsetStr = buffer + 1 ;
	buffer[0] = (wchar_t) protocol ;
	wmemcpy(offsetStr, message.c_str(), (message.size() + 1)) ;
	m_socket.write(socketServer, buffer, sizeof(wchar_t) * (message.size() + 2)) ;
}


void ChatClient::echo(const std::wstring& message) {
	send(PROTOCOL_ECHO, message) ;
}
