COMPILER_F = -O2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

game: main.c header.h
	@echo "building game"
	gcc main.c -o game.exe $(COMPILER_F) && ./game.exe

clean: 
	@echo "removing"
	@rm -rf game.exe