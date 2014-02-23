#include <easysocket/InternetSocket.h>
#include <sys/ioctl.h>
#include <iostream>

using namespace std ;
using namespace es ;

InternetSocket::InternetSocket(int type,
                               const string& protocol)
                                    throw (SocketException) : m_isBound(false),
                                                              m_isPrepared(false),
                                                              m_isListening(false) {
    // Check the type
    if ((type != SOCK_STREAM) && (type != SOCK_DGRAM) && (type != SOCK_SEQPACKET)) {
        throw SocketException("Unknown socket type") ;
    }

    // Get the number of the protocol
    int protocol_number ;
    if (!protocol.empty())
        protocol_number = getprotobyname(protocol.c_str()) -> p_proto ;
    else
        protocol_number = 0 ;

    // Get a socket descriptor
    m_socketDesc = socket(AF_INET, type, protocol_number) ;
    if (m_socketDesc == -1) {
        throw SocketException("Unable to get a socket descriptor") ;
    }
    else {
		int enabled = 1 ;
		setsockopt(m_socketDesc, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled)) ;
    }
}

InternetSocket::~InternetSocket() {
    ::close(m_socketDesc) ;
}


void InternetSocket::prepare(const std::string& ip,
                             unsigned short port) throw (SocketException) {
//    int ip_addr = (ip.empty()) ? htonl(INADDR_ANY) : inet_addr(ip.c_str()) ;
//    if (ip_addr == -1)
//        throw SocketException("Undefined IP address") ;

	#if SOCKET_FAMILY_USED == AF_INET
		m_address.sin_family = AF_INET ;
		m_address.sin_port = htons(port) ;
		bzero(&(m_address.sin_zero), 8) ;

		if (!ip.empty()) {
			in_addr buffer ;
			int success = inet_pton(AF_INET, ip.c_str(), &buffer) ;
			if (success <= 0)
				throw SocketException("Undefined IPv4 address") ;

			m_address.sin_addr = buffer ;
		}
		else {
			m_address.sin_addr.s_addr = htonl(INADDR_ANY) ;
		}
	#elif SOCKET_FAMILY_USED == AF_INET6
		m_address.sin6_family = AF_INET6 ;
		m_address.sin6_port = htons(port) ;

		if (!ip.empty()) {
			in6_addr buffer ;
			int success = inet_pton(AF_INET6, ip.c_str(), &buffer) ;
			if (success <= 0)
				throw SocketException("Undefined IPv6 address") ;

			m_address.sin6_addr = buffer ;
		}
		else {
			m_address.sin6_addr = in6addr_any ;
		}
	#endif

    m_isPrepared = true ;
}


void InternetSocket::bindPrepare() throw (SocketException) {
    // Bind it to the descriptor
    if (::bind(m_socketDesc,
               (struct sockaddr*) &m_address,
               sizeof m_address) == -1) {
        throw SocketException("Unable to bind the socket") ;
    }

    m_isBound = true ;
}


void InternetSocket::bind(const std::string& ip,
                          unsigned short port) throw (SocketException) {
    prepare(ip, port) ;
    bindPrepare() ;
}


int InternetSocket::connect(const std::string& ip,
                            unsigned short port) throw (SocketException) {
    if (ip.empty())
        throw SocketException("No IP address of the server to connect to...") ;

	#if SOCKET_FAMILY_USED == AF_INET
		m_address.sin_family = AF_INET ;
		m_address.sin_addr.s_addr = inet_addr(ip.c_str()) ;
		m_address.sin_port = htons(port) ;
		bzero(&(m_address.sin_zero), 8) ;

		// Connect to the server
		if (::connect(m_socketDesc,
		              (struct sockaddr*) &m_address,
		              sizeof m_address) == -1) {
		    throw SocketException("The server may not be listening.") ;
		}
    #elif SOCKET_FAMILY_USED == AF_INET6
		in6_addr buffer ;
		unsigned char success = inet_pton(AF_INET6, ip.c_str(), &buffer) ;
		if (!success)
			throw SocketException("Undefined IPv6 address") ;

		m_address.sin6_family = AF_INET6 ;
		m_address.sin6_addr = buffer ;
		m_address.sin6_port = htons(port) ;

		// Connect to the server
		if (::connect(m_socketDesc,
		              (struct sockaddr*) &m_address,
		              sizeof m_address) == -1) {
		    throw SocketException("The server may not be listening.") ;
		}
    #endif

    return m_socketDesc ;
}


void InternetSocket::listen(int max) throw (SocketException) {
    if (!m_isBound) {
        throw SocketException("The socket descriptor is not binded to an IP and a port... Call bind() before listen().") ;
    }

    if (max < 1) {
        throw SocketException("Bad amount of waiting connections...") ;
    }

    if (::listen(m_socketDesc, max) == -1) {
        throw SocketException() ;
    }

    m_isListening = true ;
}


int InternetSocket::accept(struct sockaddr_in* client_addr) throw (SocketException) {
    if (!m_isListening) {
        throw SocketException("Not listening... Call listen() before accept().") ;
    }

    socklen_t addrlen = sizeof client_addr ;

    int client = ::accept(m_socketDesc,
                          (sockaddr*) client_addr,
                          &addrlen) ;
    if (client == -1)
        throw SocketException("Error on accepting client...") ;

    return client ;
}


int InternetSocket::send(const int sockfd,
                         const void* message,
                         const int length,
                         const int flags) throw (SocketException) {
    int len_sent = ::send(sockfd, message, length, flags) ;
    if (len_sent == -1)
        throw SocketException("Error while sending message") ;

    return len_sent ;
}


int InternetSocket::send(const int sockfd,
                         const std::string& message,
                         const int flags) throw (SocketException) {
    // Compute message.size() + 1 to include the '\0' character
    int len_sent = ::send(sockfd, message.c_str(), message.size() + 1, flags) ;
    if (len_sent == -1)
        throw SocketException("Error while sending message") ;

    return len_sent ;
}


int InternetSocket::write(const int sockfd,
                          const void* message,
                          const int length) {
    return ::write(sockfd, message, length) ;
}


int InternetSocket::write(const int sockfd,
                          const std::string& message) {
    return send(sockfd, message, 0) ;
}


int InternetSocket::recv(const int sockfd,
                         void* buffer,
                         unsigned int length,
                         unsigned int flags) throw (SocketException) {
    int len_recv = ::recv(sockfd, buffer, length, flags) ;
    if (len_recv == -1)
        throw SocketException("Error while receiving message") ;

    return len_recv ;
}


int InternetSocket::recv(const int sockfd,
                         std::string& buffer,
                         unsigned int flags) throw (SocketException) {
    static const unsigned short MAX_LENGTH = 4096 ;
    unsigned short length = 0 ;
    char letter = 0 ;
    ssize_t readBytes = 0 ;


    // Reserve a minimum amount of space to avoid reallocations
    buffer.reserve(256) ;
    do {
        // Read the message byte after byte
        readBytes = ::recv(sockfd, &letter, 1, flags) ;

        // Insert the character at the end of the string
        if (readBytes == 1) {
            buffer.push_back(letter) ;
            length++ ;
        }
    } while ((readBytes == 1) && (letter != '\0') && (length < MAX_LENGTH)) ;

    if (readBytes == -1)
        throw SocketException("Error while receiving message") ;

    return buffer.size() ;
}


int InternetSocket::read(const int sockfd,
                         void* buffer,
                         const unsigned int length) {
    return ::read(sockfd, buffer, length) ;
}


int InternetSocket::read(const int sockfd,
                         std::string& buffer) {
    return recv(sockfd, buffer, 0) ;
}


int InternetSocket::sendto(const int sockfd,
                           const std::string& to_ip,
                           unsigned short port,
                           const void* message,
                           unsigned int length,
                           unsigned int flags) throw (SocketException) {
    struct sockaddr_in to ;
    to.sin_family = AF_INET ;
    to.sin_port = port ;
    bzero(&(to.sin_zero), 8) ;
    
    if (inet_aton(to_ip.c_str(), &to.sin_addr) == 0)
        throw SocketException("Bad IP format") ;

    int len_sent = ::sendto(sockfd,
                            message,
                            length,
                            flags,
                            ((sockaddr*) &to),
                            sizeof(struct sockaddr)) ;

    if (len_sent == -1)
        throw SocketException("Error while sending message (UDP)") ;

    return len_sent ;            
}


int InternetSocket::recvfrom(int sockfd,
                             sockaddr& from,
                             socklen_t& from_length,
                             void* buffer,
                             unsigned int length,
                             unsigned int flags) throw (SocketException) {
    int len_recv = ::recvfrom(sockfd,
                              buffer,
                              length,
                              flags,
                              &from,
                              &from_length) ;

    if (len_recv == -1)
        throw SocketException("Error while receiving message (UDP)") ;

    return len_recv ;
}


int InternetSocket::recvfrom(int sockfd,
                             sockaddr& from,
                             socklen_t& from_length,
                             std::string& buffer,
                             unsigned int flags) throw (SocketException) {
    static const unsigned short MAX_LENGTH = 4096 ;
    unsigned short length = 0 ;
    char letter = 0 ;
    ssize_t readBytes = 0 ;

    // Reserve a minimum amount of space to avoid reallocations
    buffer.reserve(256) ;
    do {
        // Read the message byte after byte
        readBytes = ::recvfrom(sockfd,
                               &letter,
                               1,
                               flags,
                               &from,
                               &from_length) ;

        // Insert the character at the end of the string
        if (readBytes == 1) {
            buffer.push_back(letter) ;
            length++ ;
        }
    } while ((readBytes == 1) && (letter != '\0') && (length < MAX_LENGTH)) ;

    if (readBytes == -1)
        throw SocketException("Error while receiving message (UDP)") ;

    return buffer.size() ;
}


void InternetSocket::close() {
    ::close(m_socketDesc) ;
}


void InternetSocket::shutdown(SHUTDOWN_HOW how) throw (SocketException) {
    if (::shutdown(m_socketDesc, how) != 0)
         throw SocketException("Error on shuting down") ;
}



int InternetSocket::getSocket() const {
    return m_socketDesc ;
}


#if SOCKET_FAMILY_USED == AF_INET
const sockaddr_in& InternetSocket::getAddress() const {
    return m_address ;
}
#elif SOCKET_FAMILY_USED == AF_INET6
const sockaddr_in6& InternetSocket::getAddress() const {
    return m_address ;
}
#endif

bool InternetSocket::isBound() const {
    return m_isBound ;
}

bool InternetSocket::isPrepared() const {
    return m_isPrepared ;
}

bool InternetSocket::isListening() const {
    return m_isListening ;
}
