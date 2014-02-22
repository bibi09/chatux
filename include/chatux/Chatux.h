#ifndef __CHATUX_CONTEXT__
#define __CHATUX_CONTEXT__

#define CHATUX_VERSION 14

#define USER_ALIAS_LENGTH 8
typedef struct {
	char Alias[USER_ALIAS_LENGTH+1] ;
	char* IP ;
	unsigned short Port ;
} Context ;


#define BUFFER_MESSAGE_SIZE	255
/** @brief	Flags of the protocol used. */
enum PROTOCOL_FLAGS {PROTOCOL_SERVER_DOWN	= 0,		// The server is down
					 PROTOCOL_DISCONNECT	= 1,		// A client is disconnected
					 PROTOCOL_ECHO 			= 2,		// A client send a message on chat
					 PROTOCOL_ALIAS			= 3} ;		// A client changes its alias

#define TEXT_NORMAL		"\x1B[0m"
#define TEXT_RED		"\x1B[31m"
#define TEXT_GREEN  	"\x1B[32m"
#define TEXT_YELLOW 	"\x1B[33m"
#define TEXT_BLUE		"\x1B[34m"
#define TEXT_MAGENTA	"\x1B[35m"
#define TEXT_CYAN		"\x1B[36m"
#define TEXT_WHITE		"\x1B[37m"

#include <stdio.h>
#define WRITE_COLOR(color, text) printf(color text TEXT_NORMAL)

#endif
