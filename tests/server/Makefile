ROOT_DIR=../..
SRC_DIR=${ROOT_DIR}/src
EXT_DIR=${ROOT_DIR}/externals

INCLUDES=-I${SRC_DIR} -I${EXT_DIR}
CXXFLAGS=${INCLUDES} -std=c++23 -g
LDLIBS = -lssl -lcrypto -luuid

.PHONY: all clean test

all: test_uuid

test: all
	./test_uuid -s

test_uuid: test_uuid.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
test_uuid.o: test_uuid.cpp ${SRC_DIR}/server/uuid.hpp

clean:
	rm -f test_uuid.o test_uuid
