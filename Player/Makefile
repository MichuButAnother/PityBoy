# Are these literally the same?
ifeq ($(OS), Windows_NT)
	COMMAND = g++ -g -Wall -o PityBoy main.cpp -DSFML_STATIC -lsfml-graphics -lsfml-window -lsfml-system -lboost_filesystem -lboost_system -llua5.4 -static-libgcc -static-libstdc++
else
	COMMAND = g++ -g -Wall -o PityBoy main.cpp -DSFML_STATIC -lsfml-graphics -lsfml-window -lsfml-system -lboost_filesystem -lboost_system -llua5.4 -static-libgcc -static-libstdc++
endif

all: main.cpp
	$(COMMAND)