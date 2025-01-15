CC=g++
STD=c++17

build/main: build/main.o build/Hand.o build/Deck.o build/Card.o build/Player.o build/Eights.o build/randomutils.o build/CardCollection.o build/socket.o
	$(CC) -o $@ $^ -std=$(STD) -lncursesw 

build/test: build/test.o build/socket.o build/Card.o build/Deck.o build/CardCollection.o build/randomutils.o build/messages.o build/Hand.o
	$(CC) -o $@ $^ -std=$(STD) -lncursesw

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CC) -MM -MT $@ $< > build/$*.d
	$(CC) -g -c -o $@ $< -std=$(STD)

-include build/*.d

.PHONY: test all clean

test: build/test
	./build/test

test-all: build/test
	./build/test --no-skip

all: build/test build/main

clean:
	rm -rf build
