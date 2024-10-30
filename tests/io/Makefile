ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g

.PHONY: all clean test

all: test_endpoint

test: all
	./test_endpoint -s

test_endpoint: test_endpoint.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_endpoint.o: test_endpoint.cpp ${SRC_DIR}/io/endpoint.hpp

clean:
	rm -f test_endpoint.o test_endpoint
