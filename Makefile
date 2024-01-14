build:
	gcc -std=c99 ./src/*.c -lSDL2 -lSDL2_ttf -lSDL2_mixer -lm -o ./bin/raycast;

run:
	./bin/raycast;

dev:
	make build && make run

clean:
	rm ./bin/raycast;
