VPATH = Scheduler/src

BINARY = ds-client

CC = clang
CFLAGS = -std=gnu11 -Wall -Wextra -pedantic
CXX = clang++
CXXFLAGS = -std=gnu++17

.PHONY: all
all: $(BINARY)

$(BINARY): algorithms.o job_info.o main.o resource_info.o socket_client.o system_config.o -ltinyxml
	$(CXX) $(CXXFLAGS) -I Scheduler/src -o $@ $^

main.o: main.c

socket_client.o: socket_client.c socket_client.h

system_config.o: system_config.cpp system_config.h

job_info.o: job_info.cpp job_info.h

resource_info.o: resource_info.cpp resource_info.h

algorithms.o: algorithms.c algorithms.h

clean:
	rm -f *.o

clean-all:
	rm -f *.o $(BINARY)

