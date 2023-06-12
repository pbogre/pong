NOTICE: if you are looking to play this, please look at issue #2

# Instructions

CMake is required to build this project.

## Server

- `cd server`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./server`

## Client

- `cd client`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./client`

## Config

The configuration file is located in `client/src/config.h`

- `IP` is a `string` representing the IP to connect to
- `PORT` is an `int` representing the port of the pong server
- `username` is a `string` representing the username you wish to play with

# To-Do

- [ ] desync handling and warnings testing
- [x] ~~score desync handling~~
- [x] ~~desync warning system~~
