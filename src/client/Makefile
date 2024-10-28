ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

CXX = clang++
# CXX = g++
INCLUDES = -I${SRC_DIR} -I${EXT_DIR} -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib
CXXFLAGS = -g -std=c++23 -Wall ${INCLUDES}
LDLIBS = ${LIBS} -lssl -lcrypto

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
	${SRC_DIR}/utils/match.hpp \
	${SRC_DIR}/utils/utils.hpp

HEADERS = \
	${MESSAGE_HEADERS} \
	${IO_HEADERS} \
	${LOGGING_HEADERS} \
	${UTIL_HEADERS}

.PHONEY: default
default: all

.PHONEY: all
all: client

client: client.o topic_client.o
	$(LINK.cc) client.o topic_client.o $(LOADLIBES) $(LDLIBS) -o $@

client.o: client.cpp $(HEADERS)
topic_client.o: topic_client.cpp topic_client.hpp $(HEADERS)

.PHONY: clean
clean:
	rm -f client.o topic_client.o utils.o client
