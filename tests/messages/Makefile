ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g

HEADERS = \
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
	
.PHONY: all clean test

all: \
	test_authenticate \
	test_data_packet \
	test_forwarded_multicast_data \
	test_forwarded_subscription_request \
	test_forwarded_unicast_data \
	test_multicast_data \
	test_notification_request \
	test_subscription_request \
	test_unicast_data


test: all
	./test_authenticate -s
	./test_data_packet -s
	./test_forwarded_multicast_data -s
	./test_forwarded_subscription_request -s
	./test_forwarded_unicast_data -s
	./test_multicast_data -s
	./test_notification_request -s
	./test_subscription_request -s
	./test_unicast_data -s

test_authenticate: test_authenticate.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_authenticate.o: test_authenticate.cpp ${HEADERS}

test_data_packet: test_data_packet.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_data_packet.o: test_data_packet.cpp ${HEADERS}

test_forwarded_multicast_data: test_forwarded_multicast_data.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_forwarded_multicast_data.o: test_forwarded_multicast_data.cpp ${HEADERS}

test_forwarded_subscription_request: test_forwarded_subscription_request.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_forwarded_subscription_request.o: test_forwarded_subscription_request.cpp ${HEADERS}

test_forwarded_unicast_data: test_forwarded_unicast_data.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_forwarded_unicast_data.o: test_forwarded_unicast_data.cpp ${HEADERS}

test_multicast_data: test_multicast_data.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_multicast_data.o: test_multicast_data.cpp ${HEADERS}

test_notification_request: test_notification_request.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_notification_request.o: test_notification_request.cpp ${HEADERS}

test_subscription_request: test_subscription_request.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_subscription_request.o: test_subscription_request.cpp ${HEADERS}

test_unicast_data: test_unicast_data.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_unicast_data.o: test_unicast_data.cpp ${HEADERS}

clean:
	rm -f test_authenticate.o test_authenticate
	rm -f test_data_packet.o test_data_packet
	rm -f test_forwarded_multicast_data.o test_forwarded_multicast_data
	rm -f test_forwarded_subscription_request.o test_forwarded_subscription_request
	rm -f test_forwarded_unicast_data.o test_forwarded_unicast_data
	rm -f test_multicast_data.o test_multicast_data
	rm -f test_notification_request.o test_notification_request
	rm -f test_subscription_request.o test_subscription_request
	rm -f test_unicast_data.o test_unicast_data
