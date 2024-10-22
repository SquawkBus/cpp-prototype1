.PHONY: all clean test

all:
	cd serialization && make all
	cd messages && make all
	cd server && make all

clean:
	cd serialization && make clean
	cd messages && make clean
	cd server && make clean

test:
	cd serialization && make test
	cd messages && make test
	cd server && make test
	