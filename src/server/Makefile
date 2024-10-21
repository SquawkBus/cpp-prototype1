ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

CXX = clang++
# CXX = g++
INCLUDES = -I${SRC_DIR} -I${EXT_DIR} -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib
CXXFLAGS = -g -std=c++23 -Wall ${INCLUDES}
LDLIBS = ${LIBS} -lssl -lcrypto

HEADERS = \
	${SRC_DIR}/io/bio.hpp \
	${SRC_DIR}/io/poll_handler.hpp \
	${SRC_DIR}/io/poller.hpp \
	${SRC_DIR}/io/tcp_listener_poll_handler.hpp \
	${SRC_DIR}/io/tcp_listener_socket.hpp \
	${SRC_DIR}/io/tcp_server_socket.hpp \
	${SRC_DIR}/io/tcp_socket.hpp \
	${SRC_DIR}/io/tcp_socket_poll_handler.hpp \
	${SRC_DIR}/io/tcp_stream.hpp \
	${SRC_DIR}/io/file.hpp \
	${SRC_DIR}/io/ssl.hpp \
	${SRC_DIR}/io/ssl_ctx.hpp \
	${SRC_DIR}/logging/log.hpp \
	${SRC_DIR}/utils/match.hpp \
	${SRC_DIR}/utils/utils.hpp

SOURCES = \
	server.cpp \
	distributor.cpp \
	interactor.cpp \
	hub.cpp \
	subscription_manager.cpp \
	publisher_manager.cpp \
	notification_manager.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)

.PHONEY: default
default: all

.PHONEY: all
all: server

server: $(OBJECTS)
	$(LINK.cc) $(OBJECTS) $(LOADLIBES) $(LDLIBS) -o $@

server.o: \
	server.cpp \
	distributor.hpp \
	$(HEADERS)

distributor.o: \
	distributor.cpp \
	distributor.hpp \
	$(HEADERS)

interactor.o: \
	interactor.cpp \
	interactor.hpp \
	$(HEADERS)

hub.o: \
	hub.cpp \
	hub.hpp \
	subscription_manager.hpp \
	publisher_manager.hpp \
	notification_manager.hpp \
	$(HEADERS)

subscription_manager.o: \
	subscription_manager.cpp \
	subscription_manager.hpp \
	notification_manager.hpp \
	$(HEADERS)

publisher_manager.o: \
	publisher_manager.cpp \
	publisher_manager.hpp \
	$(HEADERS)

notification_manager.o: \
	notification_manager.cpp \
	notification_manager.hpp \
	$(HEADERS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) server
