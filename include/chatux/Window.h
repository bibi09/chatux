#ifndef __CHATUX_WINDOW__
#define __CHATUX_WINDOW__

#include "Position.h"
#include "interfaces/Clearable.h"
#include "interfaces/Displayable.h"
#include "interfaces/Updatable.h"

#include <map>
#include <string>
#include <curses.h>
#include <pthread.h>

#define	DEFAULT_COLOR_NAME	""


/**
 * @brief	A window is displayed on screen.
 */
class Window : public Displayable, public Updatable, public Clearable {
	protected:
		/** @brief	Directions to move the cursor. */
		enum Direction {CURSOR_UP, CURSOR_DOWN, CURSOR_LEFT, CURSOR_RIGHT} ;


	private:
		/** @brief	To know if NCurses has been initialized. */
		static bool InitializedNCurses ;
		/** @brief	Is the windowing system initialized? */
		static unsigned char RunningWindows ;
		/** @brief	Default color pair. */
		static std::map<std::string, unsigned char> ColorPairs ;


	protected:
		/** @brief	Pointer to the underlying Ncurses window. */
		WINDOW* m_cursesWindowPtr ;
		/** @brief	To know if the content should be scrolled. */
		bool m_scrollContent ;

	public:
		/**
		 * @brief	Create a new window.
		 * @param	width	Width of the window.
		 * @param	height	Height of the window.
		 * @param	posX	Position of the window on X axis.
		 * @param	posY	Position of the window on Y axis.
		 * @param	scroll	To know if the content should be scrolled.
		 */
		Window(unsigned short width = 0,
			   unsigned short height = 0,
			   unsigned short posX = 0,
			   unsigned short posY = 0,
			   bool scroll = false) ;

		/** @brief	Destroy the window. */
		virtual ~Window() ;


		/** @brief	Refresh the content of the window. */
		void refresh() ;


		/**
		 * @brief	Add or reset a named color pair.
		 * @param	name		Name of the color pair.
		 * @param	foreground	Foreground color (text).
		 * @param	background	Background color.
		 * @warning	Altering the default color has no effect.
		 */
		void setColorPair(const std::string& name,
						  short foreground,
						  short background) ;


		/**
		 * @brief	Get the color ID from its name.
		 * @param	name	Name of the color to get back.
		 * @return	The wanted color ID if found, the default color ID else.
		 */
		short getColorPair(const std::string& name = DEFAULT_COLOR_NAME) ;


		/** @brief	Set up Ncurses if not already done. */
		static void setupCurses() ;


	protected:
		/**
		 * @brief	Make the cursor of the current window in the given
		 * 			direction.
		 */
		void moveCursor(Direction dir) ;


		/**
		 * @brief	Echo a character.
		 * @param	c	Character to echo.
		 * @return	OK on success, ERR on fail.
		 */
		int printChar(char c) ;

		/**
		 * @brief	Echo a character based string.
		 * @param	str	String to echo.
		 * @return	OK on success, ERR on fail.
		 */
		int printStr(const std::string& str) ;

		/**
		 * @brief	Echo a wide character.
		 * @param	c	Character to echo.
		 * @return	OK on success, ERR on fail.
		 */
		int printChar(wchar_t c) ;

		/**
		 * @brief	Echo a wide character based string.
		 * @param	str	String to echo.
		 * @return	OK on success, ERR on fail.
		 */
		int printStr(const std::wstring& str) ;
} ;

#endif
