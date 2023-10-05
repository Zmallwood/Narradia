#!/bin/sh
./StopServer.sh; cmake -B Build/ . && cmake --build Build --target release && (Bin/Narradia -server > ServerOutput.txt & Bin/Narradia & tail -f ServerOutput.txt)
