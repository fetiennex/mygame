LIBS=-lsfml-graphics -lsfml-window -lsfml-system
CXX := g++

all: thegame

%.o: %.cpp
	$(CXX) -c $< -o $@

# %.o: %.hpp
# 	$(CXX) -c $< -o $@

thegame: edge.o form.o point.o world.o player.o main.o utils.o
	@echo "** Building the game"
	$(CXX) -o thegame edge.o form.o point.o world.o player.o main.o utils.o $(LIBS)
clean:
	@echo "** Removing object files and executable..."
	rm -f thegame *.o