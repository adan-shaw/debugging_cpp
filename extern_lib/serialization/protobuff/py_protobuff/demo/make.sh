#!/bin/sh

protoc ./data.proto --python_out=. 

py=python
#py=python3

$py ./print_data.py
