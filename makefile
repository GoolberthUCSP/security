all: test/main.cpp virus.cpp
	g++ -o test/main test/main.cpp
	g++ -o test/virus virus.cpp

run: test/main test/virus
	./test/virus

clean:
	rm -f test/main test/virus