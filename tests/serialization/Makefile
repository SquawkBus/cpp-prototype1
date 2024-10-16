ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g

.PHONY: all clean test

all: test_data_packet test_frame_buffer test_network_bytes

test: all
	./test_data_packet -s
	./test_frame_buffer -s
	./test_network_bytes -s

test_data_packet: test_data_packet.cpp
test_data_packet.cpp: ${SRC_DIR}/serialization/data_packet.hpp

test_frame_buffer: test_frame_buffer.cpp
test_frame_buffer.cpp: ${SRC_DIR}/serialization/frame_buffer.hpp

test_network_bytes: test_network_bytes.cpp
test_network_bytes.cpp: ${SRC_DIR}/serialization/network_bytes.hpp

clean:
	rm -f test_data_packet.o test_data_packet
	rm -f test_frame_buffer.o test_frame_buffer
	rm -f test_network_bytes.o test_network_bytes
