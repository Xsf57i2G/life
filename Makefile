build:
	cc -Wall -Wextra ./src/*.c -lSDL2 -lSDL2_image -o ./bin/main

run:
	./bin/main

clean:
	rm -rf ./bin/main