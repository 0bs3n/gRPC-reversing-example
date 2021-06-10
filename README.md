This repo exists and a useful tool for testing out various gRPC functionalities and seeing what the corresponding binary looks like. Made for reverse engineering practice, but feel free to use it however. Includes a client and a server that will talk to each other. Also handy for building gRPC stuff with make rather than cmake, couldn't find a working Makefile for doing so online -- this one works.

## Build
```bash
# tested with gnu make
make
```

## Run
```bash
# in one terminal
./server

# in another terminal
./client <string to manipulate>
```
