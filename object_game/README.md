# mygame
compile cpp:


g++ -c edge.cpp
g++ -c form.cpp
g++ -c point.cpp
g++ -c world.cpp
g++ -c main.cpp
g++ -c utils.cpp
g++ edge.o form.o point.o world.o main.o utils.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system