#############################
# Chatux - Chat on terminal #
#############################
# Chatux Makefile           #
# Author: Denis CARLUS      #
# Version 1, 2014/02/17     #
# Version 2, 2014/02/21     #
#############################

# Compilation options
CC      		= g++
CCOPTS  		=
CHATUX_OPTS		= -W -Wall
EASYSOCKET_OPTS = -fPIC -shared -W -Wall

# FILES AND DIRECTORIES
ROOT_DIR		= $(realpath ./)

# Sources of the project
SOURCES_DIR		= $(ROOT_DIR)/src

CHATUX_SRC_DIR	= $(SOURCES_DIR)/chatux
CHATUX_SOURCES	= $(wildcard $(CHATUX_SRC_DIR)/*.cpp)
CHATUX_OBJECTS	= $(CHATUX_SOURCES:.cpp=.o)

EASYSOCK_SRC_DIR= $(SOURCES_DIR)/easysocket
EASYSOCK_SOURCES= $(wildcard $(EASYSOCK_SRC_DIR)/*.cpp)
EASYSOCK_OBJECTS= $(EASYSOCK_SOURCES:.cpp=.o)

# Binary and compiled files path
BINARY_DIR		= $(ROOT_DIR)/bin
EXECUTABLE_NAME	= chatux
EXECUTABLE		= $(BINARY_DIR)/$(EXECUTABLE_NAME)

LIBRARIES_DIR	= $(ROOT_DIR)/lib
EASYSOCKET_NAME = libeasysocket.so
EASYSOCKET_OUT	= $(LIBRARIES_DIR)/$(EASYSOCKET_NAME)

# Libraries used paths
INCLUDE_DIR		= $(ROOT_DIR)/include
INCLUDES		= -I/usr/include/ -I$(INCLUDE_DIR)

CHATUX_LIBS		= -lcurses -lpthread -L$(LIBRARIES_DIR) -leasysocket
EASYSOCKET_LIBS	= -lpthread


# RULES
all: easysocket chatux

%.o: %.cpp
	@echo "Compiling" $(notdir $@)
	@$(CC) $(CCOPTS) $(INCLUDES) -c $< -o $@


# Rules for EasySocket
easysocket: setCCOptionsForEasySocket $(EASYSOCK_OBJECTS)
	@echo "Linking to" $(EASYSOCKET_NAME)
	@$(CC) $(CCOPTS) $(INCLUDES) $(EASYSOCKET_LIBS) $(EASYSOCK_OBJECTS) -o $(EASYSOCKET_OUT)

setCCOptionsForEasySocket:
	@echo "--- Compilation of the EasySocket library ---"
	$(eval CCOPTS := $(EASYSOCKET_OPTS))


# Rules for Chatux
chatux: setCCOptionsForChatux $(CHATUX_OBJECTS)
	@echo "Linking to" $(EXECUTABLE_NAME)
	@$(CC) $(CCOPTS) $(INCLUDES) $(CHATUX_LIBS) $(CHATUX_OBJECTS) -o $(EXECUTABLE)

setCCOptionsForChatux:
	@echo -e "\n--- Compilation of Chatux ---"
	$(eval CCOPTS := $(CHATUX_OPTS))


# General rules
clean:
	@echo "Clearing project..."
	@rm -rf $(EXECUTABLE) $(EASYSOCKET_OUT) $(CHATUX_OBJECTS) $(EASYSOCK_OBJECTS)
	@echo "Done!"

exec:
	@cd $(BINARY_DIR) && ./run_chatux.sh
