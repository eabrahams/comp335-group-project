VPATH = Scheduler/src:Scheduler/tst

#this is set to the default install location for the ubuntu package, change as required
GTEST_DIR = /usr/src/gtest

BINARY = c-client

TEST = gtest-runner

CC = clang
CFLAGS = -std=gnu11
CXX = clang++
CXXFLAGS = -std=gnu++17

all: algorithms.o job_info.o main.o resource_info.o socket_client.o system_config.o stringhelper.o cpp_util.o worst_fit.o -ltinyxml -lpcre2-8
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(BINARY) $^

main.o: main.c

socket_client.o: socket_client.c socket_client.h

system_config.o: system_config.cpp system_config.h

job_info.o: job_info.cpp job_info.h

resource_info.o: resource_info.cpp resource_info.h

algorithms.o: algorithms.c algorithms.h

stringhelper.o: stringhelper.c stringhelper.h

cpp_util.o: cpp_util.cpp cpp_util.h

worst_fit.o: worst_fit.cpp worst_fit.h

test: gtest-all.o gtest_main.o system_config.test.o job_info.test.o resource_info.test.o worst_fit.test.o system_config.o job_info.o resource_info.o socket_client.o stringhelper.o cpp_util.o worst_fit.o -ltinyxml -lpcre2-8 -lpthread
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(TEST) $^
	./$(TEST)

gtest-all.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $^

gtest_main.o: $(GTEST_DIR)/src/gtest_main.cc
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c $^

system_config.test.o: system_config.test.cpp

job_info.test.o: job_info.test.cpp

resource_info.test.o: resource_info.test.cpp

worst_fit.test.o: worst_fit.test.cpp

clean:
	rm -f *.o

clean-all:
	rm -f *.o $(BINARY) $(TEST)
