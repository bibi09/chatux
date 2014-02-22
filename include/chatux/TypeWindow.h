#ifndef __CHATUX_TYPEWINDOW__
#define __CHATUX_TYPEWINDOW__

#include "Window.h"
#include <list>
#include <string>


/**
 * @brief	A TypeWindow displays messages from chat, i.e. current user and
 *			other ones that are chatting.
 * @author	Denis CARLUS
 * @version	1.0	2014/02/17
 */
class TypeWindow : public Window {
	private:
		/** @brief	Text the user is typing. */
		std::string m_typedText ;
		/** @brief	List of the texts already sent. */
		std::list<std::string> m_memory ;
		/** @brief	Pointer to the memory instructions already sent. */
		std::list<std::string>::iterator m_memoryPosition ;
		

	public:
		/**
		 * @brief	Create a TypeWindow.
		 * @param	width	Width of the window.
		 * @param	height	Height of the window.
		 * @param	posX	Position of the window on X axis.
		 * @param	posY	Position of the window on Y axis.
		 */
		TypeWindow(unsigned short width = 0,
				   unsigned short height = 0,
			   	   unsigned short posX = 0,
			   	   unsigned short posY = 0) ;

		/** @brief	Destroy a TypeWindow. */
		virtual ~TypeWindow() ;


		/** @brief	Update the window content. */
		virtual void update() ;

		/** @brief	Display the window. */
		virtual void display() ;

		/** @brief	Clear the window content. */
		virtual void eraseContent() ;
} ;

#endif
