VPATH = Scheduler/src

CC = clang
CFLAGS = -std=gnu11 -I Scheduler/src
CXX = clang++
CXXFLAGS = -std=gnu++17 -I Scheduler/src

all: all_to_largest.o job_info.o main.o resource_info.o socket_client.o system_config.o -ltinyxml
	$(CXX) $(CXXFLAGS) -o Scheduler/bin/client $^

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

socket_client.o: socket_client.c socket_client.h
	$(CC) $(CFLAGS) -c $< -o $@

system_config.o: system_config.cpp system_config.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

job_info.o: job_info.cpp job_info.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

resource_info.o: resource_info.cpp resource_info.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

all_to_largest.o: all_to_largest.c all_to_largest.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o

