SRCS=$(wildcard *.cpp)
NAME=utac

all:
	g++ -Ofast $(SRCS) -o $(NAME)
	x86_64-w64-mingw32-g++ -Ofast $(SRCS) -o utac.exe

debug:
	g++ $(SRCS) -o $(NAME)
	x86_64-w64-mingw32-g++ $(SRCS) -o $(NAME)	