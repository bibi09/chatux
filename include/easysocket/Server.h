#ifndef __EASYSOCKET_SERVER_H__
#define __EASYSOCKET_SERVER_H__

#include "InternetSocket.h"

#include <assert.h>
#include <map>
#include <pthread.h>

namespace es {
    /**
     * @brief A server listens for new connections and manage the connected
     *		  clients.
     * @version 1.0     08/07/2013
     * @version	1.1		21/02/2014
     * @author Denis CARLUS
     */
    class Server {
        protected:
            /** @brief Structure containing parameters required by clients threads. */
            struct ThreadParams {
                Server* server ;
                int client ;
            } ;


            /** @brief Listening port. */
            unsigned short m_port ;
            /** @brief Maximal amount of clients to listen. */
            unsigned short m_maxClients ;
            /** @brief Socket of the server. */
            InternetSocket m_socket ;
            /** @brief Used to pause the server and restart it. */
            bool m_isRunning ;
            /** @brief List of the client sockets and threads. */
            std::map<int, pthread_t> m_clients ;


            /**
             * @brief Mutex used to generate a critical section when pausing or
             * resuming the server.
             */
            pthread_mutex_t m_serverState ;


        public:
            /**
             * @brief Creation of a server.
             * @param port          Port to listen to.
             * @param max_clients   Maximal amount of clients to listen.
             */
            Server(unsigned short port, unsigned short max_clients = 20) ;

            /** @brief Destruction of a server. */
            virtual ~Server() ;


            /** @brief Prepare the socket without binding it to the machine. */
            virtual void init() ;

            /** @brief Start the server. */
            virtual void start() throw(SocketException) ;

            /** @brief Pause the server. */
            virtual void pause() throw(SocketException) ;


        protected:
            /**
             * @brief Runned by the client threads and calls manageClient().
             * @param param     ThreadParams structure given as void pointer.
             */
            static void* initClient(void* param) ;

            /**
             * @brief Runned by the client threads.
             * @param infos     ThreadParams structure given to know the used sockets.
             */
            virtual void* manageClient(int socket) = 0 ;


			/**
			 * @brief	Send a message to all clients.
             * @param   message			The message to send.
             * @param	messageSize		Size of the message to send.
			 */
			void sendAll(void* message, size_t messageSize) ;

            /**
             * @brief   Send a message from the server to the client.
             * @param   client_socket   Socket of the client.
             * @param   message			The message to send.
             * @param	messageSize		Size of the message to send.
             */
            virtual void send(int client_socket,
                              void* message,
                              size_t messageSize) = 0 ;
    } ;
}

#endif
