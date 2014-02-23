#include <easylog/Logger.h>


void Logger::init(char* program) {
	openlog(program, LOG_CONS, LOG_DAEMON) ;
}


void Logger::print(int priority, char* format, ...) {
	va_list arguments ;
	va_start(arguments, format) ;
	vsyslog(priority, format, arguments) ;
	va_end(arguments) ;
}


void Logger::close() {
	closelog() ;
}
