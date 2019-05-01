VPATH = Scheduler/src

CC = clang
CFLAGS = -std=gnu11
CXX = clang++
CXXFLAGS = -std=gnu++17

all: all_to_largest.o job_info.o main.o resource_info.o socket_client.o system_config.o -ltinyxml
	$(CXX) $(CXXFLAGS) -I Scheduler/src -o Scheduler/bin/client $^

main.o: main.c

socket_client.o: socket_client.c socket_client.h

system_config.o: system_config.cpp system_config.h

job_info.o: job_info.cpp job_info.h

resource_info.o: resource_info.cpp resource_info.h

all_to_largest.o: all_to_largest.c all_to_largest.h

clean:
	rm -rf *.o

