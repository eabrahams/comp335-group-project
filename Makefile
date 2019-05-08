VPATH = Scheduler/src

BINARY = c-client

CC = clang
CFLAGS = -std=gnu11
CXX = clang++
CXXFLAGS = -std=gnu++17

all: all_to_largest.o job_info.o main.o resource_info.o socket_client.o system_config.o stringhelper.o -ltinyxml -lpcre2-8
	$(CXX) $(CXXFLAGS) -I Scheduler/src -o $(BINARY) $^

main.o: main.c

socket_client.o: socket_client.c socket_client.h

system_config.o: system_config.cpp system_config.h

job_info.o: job_info.cpp job_info.h

resource_info.o: resource_info.cpp resource_info.h

all_to_largest.o: all_to_largest.c all_to_largest.h

stringhelper.o: stringhelper.c stringhelper.h

clean:
	rm -f *.o

clean-all:
	rm -f *.o $(BINARY)

