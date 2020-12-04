CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lpthread
# All Targets
all: Client

# Tool invocations
# Executable "StompBookClubClient" depends on the files inside bin folder
Client: bin/Book.o bin/Client.o bin/connectionHandler.o bin/KeyboardReader.o bin/readFromServ.o bin/StompBookClubClient.o
	@echo 'Building target: Client'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/StompBookClubClient bin/Book.o bin/Client.o bin/connectionHandler.o bin/KeyboardReader.o bin/readFromServ.o bin/StompBookClubClient.o -lboost_system -lpthread
	@echo 'Finished building target: Client'
	@echo ' '

# Depends on the source and header files
bin/Book.o: src/Book.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Book.o src/Book.cpp

bin/Client.o: src/Client.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Client.o src/Client.cpp
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp
bin/KeyboardReader.o: src/KeyboardReader.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/KeyboardReader.o src/KeyboardReader.cpp
bin/readFromServ.o: src/readFromServ.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/readFromServ.o src/readFromServ.cpp
bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/StompBookClubClient.o src/StompBookClubClient.cpp


#Clean the build directory
clean: 
	rm -f bin/*
