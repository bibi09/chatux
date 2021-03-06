#ifndef __CHATUX_CONTEXT__
#define __CHATUX_CONTEXT__

#define CHATUX_VERSION 22

#define USER_ALIAS_LENGTH 8
typedef struct {
	wchar_t Alias[USER_ALIAS_LENGTH+1] ;
	char* IP ;
	unsigned short Port ;
} Context ;

#define CLIENT_ID_SIZE 		sizeof(int) / sizeof(wchar_t)
#define BUFFER_MESSAGE_SIZE	255
#define BUFFER_MESSAGE_SIZE_BYTES sizeof(wchar_t) * BUFFER_MESSAGE_SIZE
/** @brief	Flags of the protocol used. */
enum PROTOCOL_FLAGS {PROTOCOL_SERVER_DOWN	= 0,		// The server is down
					 PROTOCOL_DISCONNECT	= 1,		// A client is disconnected
					 PROTOCOL_ECHO 			= 2,		// A client sends a message on chat
					 PROTOCOL_ECHO_SELF		= 3,		// A client receives a message from himself
					 PROTOCOL_COMMAND		= 4,		// A client sends a command
					 PROTOCOL_ALIAS			= 5} ;		// A client changes its alias

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
