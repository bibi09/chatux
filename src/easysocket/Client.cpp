#include <easysocket/Client.h>
#include <easysocket/SocketException.h>
#include <vector>
#include <iostream>

using namespace std ;
using namespace es ;


Client::Client(const string& serverIP, unsigned short serverPort) {
    assert(pthread_mutex_init(&m_clientState, NULL) == 0) ;

    pthread_mutex_lock(&m_clientState) ;
    	try {
		    m_socket.connect(serverIP.c_str(), serverPort) ;
		    m_isRunning = false ;

		    srand(time(0)) ;
		    randomSession() ;
        }
        catch(SocketException ex) {
        	cerr << ex.what() << endl ;
        }
    pthread_mutex_unlock(&m_clientState) ;
}


Client::~Client() {
    pthread_mutex_destroy(&m_clientState) ;
}


const string& Client::getSession() {
    return m_session ;
}


void Client::start() {
    pthread_mutex_lock(&m_clientState) ;
		m_isRunning = true ;
		createThread() ;
    pthread_mutex_unlock(&m_clientState) ;
}


void Client::stop() {
    pthread_mutex_lock(&m_clientState) ;
    	m_isRunning = false ;
    pthread_mutex_unlock(&m_clientState) ;
}


void Client::randomSession() {
	const unsigned char MAX_LENGTH = 32 ;
	m_session.reserve(MAX_LENGTH) ;
	for (unsigned char pos = 0 ; pos < MAX_LENGTH ; pos++) {
		if (rand() % 2 == 0) {
			// Write a number, codes from 48 to 57 in ASCII table
			m_session.push_back((char) ((rand() % 10) + 48)) ;
		}
		else {
			// Write a lowercase letter, codes from 97 to 122 in ASCII table
			m_session.push_back((char) ((rand() % 26) + 97)) ;
		}
	}
}
