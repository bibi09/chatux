#ifndef __EASYLOG_LOGGER__
#define __EASYLOG_LOGGER__

extern "C" {
	#include <syslog.h>
	#include <stdarg.h>
	#include <stdlib.h>
}

/**
 * @brief	Log system writing into the syslogs.
 * @author	Denis CARLUS, widely inspired from Adrien RICCIARDI's code.
 */
class Logger {
	public:
		/**
		 * @brief	Initialize syslog.
		 * @param	Name of the program.
		 */
		static void init(char* program) ;

		/**
		 * @brief	Print a log.
		 * @param	priority	Priority of the log.
		 * @param	format		Format of the message.
		 */
		static void print(int priority, char* format, ...) ;


		/** @brief	Close the connection to syslog. */
		static void close() ;
} ;

#endif
