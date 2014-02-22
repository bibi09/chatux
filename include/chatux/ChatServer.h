#ifndef __CHATUX_SERVER__
#define __CHATUX_SERVER__

#include <easysocket/Server.h>
#include <chatux/Chatux.h>
#include <chatux/NoCopy.h>
#include <map>
#include <string>

class ChatServer : public es::Server, private NoCopy {
	private:
		/** @brief  Unique instance of the chat server. */
        static ChatServer* Instance ;
		/** @brief	To know if the instance has correctly been configurated. */
		static bool IsConfigurated ;
		/** @brief	Port of the server to bind to. */
		static unsigned short ServerPort ;


		/** @brief	Create a new server to chat. */
		ChatServer() ;

		/** @brief	Destroy the chat server. */
		virtual ~ChatServer() throw() ;


	public:
		/**
		 * @brief  	Get the ChatServer instance.
		 * @return	The instance of the singleton on success, NULL if the server
		 *			has not correctly been configurated.
		 */
        static ChatServer* getInstance() ;

		/**
		 * @brief	Configure the server with the port to bind to.
		 * @param	serverPort	Port of the server.
		 * @return	TRUE if the server is correctly configurated, FALSE else.
		 */
		static bool configure(unsigned short serverPort) ;


        /**
         * @brief Runned by the client threads.
         * @param socket     Socket of the client.
         */
        virtual void* manageClient(int socket) ;


	protected:
        /**
         * @brief   Send a message from the server to the client.
         * @param   client_socket   Socket of the client.
         * @param   message			The message to send.
         * @param	messageSize		Size of the message to send.
         */
        virtual void send(int client_socket,
                          void* message,
                          size_t messageSize) ;
} ;

#endif
