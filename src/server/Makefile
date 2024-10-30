ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

CXX = clang++
# CXX = g++
INCLUDES = -I${SRC_DIR} -I${EXT_DIR} -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib
CXXFLAGS = -g -std=c++23 -Wall ${INCLUDES}
LDLIBS = ${LIBS} -lssl -lcrypto -luuid

MESSAGE_HEADERS = \
	${SRC_DIR}/messages/_authenticate.hpp \
	${SRC_DIR}/messages/_data_packet.hpp \
	${SRC_DIR}/messages/_forwarded_multicast_data.hpp \
	${SRC_DIR}/messages/_forwarded_subscription_request.hpp \
	${SRC_DIR}/messages/_forwarded_unicast_data.hpp \
	${SRC_DIR}/messages/_message_inline.hpp \
	${SRC_DIR}/messages/_message_type.hpp \
	${SRC_DIR}/messages/_message.hpp \
	${SRC_DIR}/messages/_multicast_data.hpp \
	${SRC_DIR}/messages/_notification_request.hpp \
	${SRC_DIR}/messages/_subscription_request.hpp \
	${SRC_DIR}/messages/_unicast_data.hpp \
	${SRC_DIR}/messages/messages.hpp

IO_HEADERS = \
	${SRC_DIR}/io/bio.hpp \
	${SRC_DIR}/io/endpoint.hpp \
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
	${SRC_DIR}/io/ssl_ctx.hpp

LOGGING_HEADERS = \
	${SRC_DIR}/logging/log.hpp

UTIL_HEADERS = \
	${SRC_DIR}/utils/utils.hpp

HEADERS = \
	${MESSAGE_HEADERS} \
	${IO_HEADERS} \
	${LOGGING_HEADERS} \
	${UTIL_HEADERS}

SOURCES = \
	server.cpp \
	distributor.cpp \
	interactor.cpp \
	hub.cpp \
	subscription_manager.cpp \
	publisher_manager.cpp \
	notification_manager.cpp \
	authorization.o \
	cmd_line.o

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
	cmd_line.hpp \
	authorization.hpp \
	role.hpp \
	$(HEADERS)

distributor.o: \
	distributor.cpp \
	distributor.hpp \
	$(HEADERS)

interactor.o: \
	interactor.cpp \
	interactor.hpp \
	uuid.hpp \
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

authorization.o: \
	authorization.cpp \
	authorization.hpp \
	role.hpp

cmd_line.o: \
	cmd_line.cpp \
	cmd_line.hpp \
	authorization.hpp \
	role.hpp \
	${IO_HEADERS}

.PHONY: clean
clean:
	rm -f $(OBJECTS) server
