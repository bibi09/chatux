#ifndef __CHATUX_CHATWINDOW__
#define __CHATUX_CHATWINDOW__

#include "Chatux.h"
#include "Window.h"
#include <string>
#include <map>

/**
 * @brief	A ChatWindow displays messages from chat, i.e. current user and
 *			other ones that are chatting.
 * @author	Denis CARLUS
 * @version	1.0	2014/02/17
 */
class ChatWindow : public Window {
	public:
		/**
		 * @brief	Create a ChatWindow.
		 * @param	width	Width of the window.
		 * @param	height	Height of the window.
		 * @param	posX	Position of the window on X axis.
		 * @param	posY	Position of the window on Y axis.
		 */
		ChatWindow(unsigned short width = 0,
				   unsigned short height = 0,
			   	   unsigned short posX = 0,
			   	   unsigned short posY = 0) ;

		/** @brief	Destroy a ChatWindow. */
		virtual ~ChatWindow() ;


		/** @brief	Update the window content. */
		virtual void update() ;

		/** @brief	Display the window. */
		virtual void display() ;

		/**
		 * @brief	Add a message to the chat window.
		 * @param	alias	Alias of the author of the message.
		 * @param	msg		The message to display.
		 */
		virtual void echo(PROTOCOL_FLAGS protocol,
						  const std::wstring& alias,
						  const std::wstring& msg) ;

		/** @brief	Clear the window content. */
		virtual void eraseContent() ;


	protected:
		/**
		 * @brief	Get the current time as a string.
		 * @return	Time as a string.
		 */
		char* getTime() ;
} ;

#endif
