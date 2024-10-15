.PHONY: all clean test

all:
	cd src && make all
	cd tests && make all

test:
	cd tests && make test

clean:
	cd src && make clean
	cd tests && make clean
