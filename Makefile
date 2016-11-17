#why won't it work ;=;
all: scrabble

#puts everything in place
scrabble:  wildcard.o board.o instruct_holder.o trie.o
	g++ wildcard.o board.o instruct_holder.o trie.o -o scrabble -std=c++11

#game
wildcard.o: wildcard.cpp
	g++ -c wildcard.cpp -std=c++11
	
#board
board.o: board.cpp
	g++ -c board.cpp -std=c++11
	
#instruct_holder
instruct_holder.o: instruct_holder.cpp
	g++ -c instruct_holder.cpp -std=c++11
	
#trie
trie.o: trie.cpp
	g++ -c trie.cpp -std=c++11

#variables
%: %.cc
	g++ -std=c++11 $< -o $@

%: %.c
	gcc $< -o $@

#cleans the project
clean:
	rm *.o scrabble