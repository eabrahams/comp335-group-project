VPATH = Scheduler/src:Scheduler/tst

#this is set to the default install location for the ubuntu package, change as required
GTEST_DIR = /usr/src/gtest

BINARY = c-client

TEST = gtest-runner

CC = clang
CFLAGS = -std=gnu11
CXX = clang++
CXXFLAGS = -std=gnu++17

all: all_to_largest.o job_info.o main.o resource_info.o socket_client.o system_config.o stringhelper.o cpp_util.o -ltinyxml -lpcre2-8
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARY) $^

main.o: main.c

socket_client.o: socket_client.c socket_client.h

system_config.o: system_config.cpp system_config.h

job_info.o: job_info.cpp job_info.h

resource_info.o: resource_info.cpp resource_info.h

all_to_largest.o: all_to_largest.c all_to_largest.h

stringhelper.o: stringhelper.c stringhelper.h

cpp_util.o: cpp_util.cpp cpp_util.h

test: gtest-all.o gtest_main.o job_info.test.o job_info.o resource_info.test.o resource_info.o -ltinyxml -lpcre2-8 -lpthread
	$(CXX) $(CPPFLAGSE) $(CXXFLAGS) -o $(TEST) $^
	./$(TEST)

gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $^

gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $^

job_info.test.o: job_info.test.cpp

resource_info.test.o: resource_info.test.cpp

clean:
	rm -f *.o

clean-all:
	rm -f *.o $(BINARY) $(TEST)
