ifeq ($(OS),Windows_NT)
COMMAND=g++ -g -Wall -o PityBoy_Editor main.cpp -DSFML_STATIC -lsfml-graphics -lsfml-window -lsfml-system -lboost_filesystem -lboost_system  -static-libgcc -static-libstdc++
else
COMMAND=	g++ -g -Wall -o PityBoy_Editor main.cpp -DSFML_STATIC -lsfml-graphics -lsfml-window -lsfml-system -lboost_filesystem -lboost_system -static-libgcc -static-libstdc++
endif


all: main.cpp
	$(COMMAND)

