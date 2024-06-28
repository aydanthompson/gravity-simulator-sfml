compile: ./src/main.cpp ./src/body.cpp
	g++ -Iinclude -c ./src/main.cpp ./src/body.cpp
	g++ main.o body.o -o app.exe -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app.exe

combined: compile run
