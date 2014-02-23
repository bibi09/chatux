#ifndef __CHATUX_CLIENT__
#define __CHATUX_CLIENT__

#include <easysocket/Client.h>
#include <chatux/Chatux.h>
#include <chatux/NoCopy.h>

class ChatClient : public es::Client, private NoCopy {
	private:
		/** @brief  Unique instance of the chat client. */
        static ChatClient* Instance ;
		/** @brief	To know if the instance has correctly been configurated. */
		static bool IsConfigurated ;
		/** @brief	IP of the server to connect to. */
		static std::string ServerIP ;
		/** @brief	Port of the server to connect to. */
		static unsigned short ServerPort ;


		/** @brief	Create a new client to chat. */
		ChatClient() ;

		/** @brief	Destroy the chat client. */
		virtual ~ChatClient() throw() ;


	public:
		/**
		 * @brief  	Get the ChatClient instance.
		 * @return	The instance of the singleton on success, NULL if the server
		 *			has not correctly been configurated.
		 */
        static ChatClient* getInstance() ;

		/**
		 * @brief	Configure the client with the server IP and port to connect
		 *			to.
		 * @param	serverIP	IP of the server.
		 * @param	serverPort	Port of the server.
		 * @return	TRUE if the server is correctly configurated, FALSE else.
		 */
		static bool configure(const std::string& serverIP,
        					  unsigned short serverPort) ;


		/**
		 * @brief	Message to send to the server.
		 * @param	The client sends a message to display on the chat.
		 */
		void echo(const std::wstring& message) ;


	private:
		/** @brief  Intermediate function to easily create a thread. */
		static void* manageThread(void*)	;

        /**
         * @brief  Function to implement in the specific client to create
         *         the background client thread.
         */
        virtual void createThread() ;

        /** @brief  Run the client thread. Call stop() to stop it. */
        virtual void run() ;


		/**
		 * @brief	Send a message with the given protocol.
		 * @param	protocol	Protocol to use.
		 * @param	message		Message to send.
		 */
		void send(PROTOCOL_FLAGS protocol, const std::wstring& message) ;
} ;

#endif
