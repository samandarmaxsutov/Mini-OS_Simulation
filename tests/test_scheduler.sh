#!/bin/bash

echo "==== MiniOS Automatic Testing ===="

# File system test
echo "touch a.txt" 
echo "write a.txt Hello"
echo "cat a.txt"

# Process test
echo "create p1 6"
echo "create p2 4"
echo "ps"

# Scheduler test
echo "sched rr 1"
echo "start"
sleep 5
echo "stop"
echo "top"