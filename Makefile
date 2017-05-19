CXX = clang++
CXXFLAGS = -g3 -std=c++11 -Wall -Wextra
LDFLAGS = -g3

crypter: main.o crypter.o
		${CXX} ${LDFLAGS} -o crypter main.o crypter.o

main.o: main.cpp crypter.h
crypter.o: crypter.h crypter.cpp