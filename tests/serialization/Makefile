ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g

.PHONY: all clean test

all: test_b64 test_frame_buffer test_network_bytes

test: all
	./test_b64 -s
	./test_frame_buffer -s
	./test_network_bytes -s

test_b64: test_b64.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_b64.o: test_b64.cpp ${SRC_DIR}/serialization/b64.hpp

test_frame_buffer: test_frame_buffer.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_frame_buffer.o: test_frame_buffer.cpp ${SRC_DIR}/serialization/frame_buffer.hpp

test_network_bytes: test_network_bytes.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_network_bytes.o: test_network_bytes.cpp ${SRC_DIR}/serialization/network_bytes.hpp

clean:
	rm -f test_b64.o test_b64
	rm -f test_frame_buffer.o test_frame_buffer
	rm -f test_network_bytes.o test_network_bytes
