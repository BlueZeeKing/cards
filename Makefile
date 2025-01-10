CC=g++
STD=c++17

build/main: build/main.o build/Hand.o build/Deck.o build/Card.o build/Player.o build/Eights.o build/randomutils.o build/CardCollection.o
	$(CC) -o $@ $^ -std=$(STD) -lncursesw -ltinfo

build/test: build/test.o 
	$(CC) -o $@ $^ -std=$(STD) -lncursesw -ltinfo

build/%.o: src/%.cpp
	@mkdir -p build
	@$(CC) -MM -MT $@ $< > build/$*.d
	$(CC) -c -o $@ $< -std=$(STD)

-include build/*.d

.PHONY: test all clean

test: build/test
	./build/test

all: build/test build/main

clean:
	rm -rf build
