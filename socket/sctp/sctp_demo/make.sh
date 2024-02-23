#!/bin/sh

gcc -g3 ./cli.c -lsctp -o cli

gcc -g3 ./srv.c -lsctp -o srv
