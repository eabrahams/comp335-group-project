# comp335-group-project
Repo for Macquarie University COMP335 Assignment

This project aims to implement a job scheduler for a distributed system.

Developers:

Ellis Abrahams (eabrahams)

Jonathan Stacey (Ourous)

Armam Hassan (emoxeon)

## Stage 1
This stage implements the allToLargest algorithm.

## Compilation
### For building:
* C and C++ compiler that supports C11 and C++17. (Clang 5 or GCC 8)
* GNU Make

### External Libraries:
* tinyxml
* PCRE2

### Install on Ubuntu 16.04:
```bash
 # add-apt-repository ppa:ubuntu-toolchain-r/test
 # apt-get update
 # apt-get install gcc-8 g++-8 libpcre2-dev libtinyxml-dev make
 # update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
```

### Build
```bash
make
```

### Run
```bash
./ds-client [-a ALGORITHM] # in same directory as server
```
