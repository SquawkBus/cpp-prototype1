ROOT_DIR=../../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g

.PHONY: all clean test

all: \
	test_authorization_request \
	test_authorization_response \
	test_forwarded_multicast_data \
	test_forwarded_subscription_request \
	test_forwarded_unicast_data \
	test_multicast_data \
	test_notification_request \
	test_subscription_request \
	test_unicast_data


test: all
	./test_authorization_request -s
	./test_authorization_response -s
	./test_forwarded_multicast_data -s
	./test_forwarded_subscription_request -s
	./test_forwarded_unicast_data -s
	./test_multicast_data -s
	./test_notification_request -s
	./test_subscription_request -s
	./test_unicast_data -s

test_authorization_request: test_authorization_request.cpp
test_authorization_request.cpp: ${SRC_DIR}/feed_bus/messages/authorization_request.hpp

test_authorization_response: test_authorization_response.cpp
test_authorization_response.cpp: ${SRC_DIR}/feed_bus/messages/authorization_response.hpp

test_forwarded_multicast_data: test_forwarded_multicast_data.cpp
test_forwarded_multicast_data.cpp: ${SRC_DIR}/feed_bus/messages/forwarded_multicast_data.hpp

test_forwarded_subscription_request: test_forwarded_subscription_request.cpp
test_forwarded_subscription_request.cpp: ${SRC_DIR}/feed_bus/messages/forwarded_subscription_request.hpp

test_forwarded_unicast_data: test_forwarded_unicast_data.cpp
test_forwarded_unicast_data.cpp: ${SRC_DIR}/feed_bus/messages/forwarded_unicast_data.hpp

test_multicast_data: test_multicast_data.cpp
test_multicast_data.cpp: ${SRC_DIR}/feed_bus/messages/multicast_data.hpp

test_notification_request: test_notification_request.cpp
test_notification_request.cpp: ${SRC_DIR}/feed_bus/messages/notification_request.hpp

test_subscription_request: test_subscription_request.cpp
test_subscription_request.cpp: ${SRC_DIR}/feed_bus/messages/subscription_request.hpp

test_unicast_data: test_unicast_data.cpp
test_unicast_data.cpp: ${SRC_DIR}/feed_bus/messages/unicast_data.hpp

clean:
	rm -f test_authorization_request.o test_authorization_request
	rm -f test_authorization_response.o test_authorization_response
	rm -f test_forwarded_multicast_data.o test_forwarded_multicast_data
	rm -f test_forwarded_subscription_request.o test_forwarded_subscription_request
	rm -f test_forwarded_unicast_data.o test_forwarded_unicast_data
	rm -f test_multicast_data.o test_multicast_data
	rm -f test_notification_request.o test_notification_request
	rm -f test_subscription_request.o test_subscription_request
	rm -f test_unicast_data.o test_unicast_data
