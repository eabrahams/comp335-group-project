# comp335-group-project
Repo for Macquarie University COMP335 Assignment

This project aims to implement a job scheduler for a distributed system.

Developers:

Ellis Abrahams (eabrahams)

Jonathan Stacey (Ourous)

Armam Hassan (emoxeon)

## Stage 2
This stage implements the First-Fit, Best-Fit and Worst-Fit algorithms.

In this branch are the Best-Fit and Worst-Fit algorithms, implemented in C.

## Compilation
### For building:
* C and C++ compiler that supports C11 and C++11 (Clang 3.4+ or GCC 4.8.1+)
* GNU Make

### External Libraries:
* tinyxml
* PCRE2

NOTE: # means running in a shell as root (replace with sudo when running as normal user)
### Install up-to-date GCC and libraries on Ubuntu 16.04:
```bash
 # add-apt-repository ppa:ubuntu-toolchain-r/test
 # apt-get update
 # apt-get install gcc-8 g++-8 libpcre2-dev libtinyxml-dev make
 # update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
```

### Install Clang and libraries on Ubuntu 16.04:
```bash
 # apt-get update
 # apt-get install clang4.0 libpcre2-dev libtinyxml-dev make
 # update-alternatives --install /usr/bin/clang clang /usr/bin/clang-4.0 400 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-4.0
```

### Build
```bash
make
```

### Run
```bash
./ds-client [-a ALGORITHM] # in same directory as server, while server is running
```
