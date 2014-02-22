#include <easysocket/Server.h>
#include <string>

using namespace std ;
using namespace es ;

Server::Server(unsigned short port, unsigned short max_clients) {
    m_port = port ;
    m_maxClients = max_clients ;
    m_isRunning = false ;

    assert(pthread_mutex_init(&m_serverState, NULL) == 0) ;
}

Server::~Server() {
	// Stop all the clients threads
	map<int, pthread_t>::iterator it ;
	pthread_mutex_lock(&m_serverState) ;
		// ENTER CRITICAL SECTION!
		for (it = m_clients.begin() ; it != m_clients.end() ; it++)
			pthread_cancel(it -> second) ;
		// EXIT CRITICAL SECTION!
	pthread_mutex_unlock(&m_serverState) ;

    pthread_mutex_destroy(&m_serverState) ;
    m_socket.close() ;
}


void Server::init() {
    m_socket.prepare("", m_port) ;
}

void Server::start() throw(SocketException) {
	try {
		// Bind the socket to the machine
		if (m_socket.isPrepared())
		    m_socket.bind() ;
		else
		    m_socket.bind("", m_port) ;

		pthread_mutex_lock(&m_serverState) ;
		    // ENTER CRITICAL SECTION!
		    m_isRunning = true ;
		    m_socket.listen() ;
		    // EXIT CRITICAL SECTION!
		pthread_mutex_unlock(&m_serverState) ;


		// Wait for new connections
		pthread_t clientThread ;// Thread to manage the client connection
		struct sockaddr_in client ;
		ThreadParams params ;   // Parameters given to the new client thread

		while(m_isRunning) {
			if (m_clients.size() < m_maxClients) {
				params.server = this ;
				params.client = m_socket.accept(&client) ;

				// Create the thread...
				assert(pthread_create(&clientThread, 0, &Server::initClient, &params) == 0) ;
				m_clients[params.client] = clientThread ;
		    }
		}
	}
    catch(SocketException ex) {
    	cerr << ex.what() << endl ;
    }
}


void Server::pause() throw(SocketException) {
    pthread_mutex_lock(&m_serverState) ;
        // ENTER CRITICAL SECTION!
        m_isRunning = false ;
        m_socket.close() ;
        // EXIT CRITICAL SECTION!
    pthread_mutex_unlock(&m_serverState) ;
}


void* Server::initClient(void* param) {
    ThreadParams* infos = (ThreadParams*) param ;
	Server* self = infos -> server ;

    void* output = self -> manageClient(infos -> client) ;
    (self -> m_clients).erase(infos -> client) ;
    return output ;
}


void Server::sendAll(void* message, size_t messageSize) {
	map<int, pthread_t> copyClients ;

	pthread_mutex_lock(&m_serverState) ;
		// ENTER CRITICAL SECTION!
		copyClients.insert(m_clients.begin(), m_clients.end()) ;
		// EXIT CRITICAL SECTION!
	pthread_mutex_unlock(&m_serverState) ;


	map<int, pthread_t>::iterator it ;
	for (it = copyClients.begin() ; it != copyClients.end() ; it++)
		send(it -> first, message, messageSize) ;
}
