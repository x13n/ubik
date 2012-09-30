CCFLAGS=-I../include

all: test

run: test
	./test

test:
	g++ test.cpp $(CCFLAGS) -o test

clean:
	@rm test 2> /dev/null || true
