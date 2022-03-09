# Cake

Collection of useful basic (and not so basic) data structures, goodies, and other musings in C++.

## Build

The Bloom Filter is using the MD5 implementation of OpenSSL, so libssl is needed.

```
$ sudo aptitude install libssl-dev
$ cd $CAKE_HOME
$ mkdir build; cd build
$ cmake .. [-DCMAKE_BUILD_TYPE=Debug]
```