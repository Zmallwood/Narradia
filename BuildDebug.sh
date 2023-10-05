#!/bin/sh
./StopServer.sh; cmake -B Build/ . && cmake --build Build --target debug

