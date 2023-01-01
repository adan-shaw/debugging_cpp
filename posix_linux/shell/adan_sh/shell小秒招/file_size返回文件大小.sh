#!/bin/sh

file_size() {
	stat -c %s $1 | tr -d '\n'
}

file_size "./file_size.sh"
