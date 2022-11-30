#!/bin/sh


if [ -d "./Python-3.11.0" ];then
	rm -r ./Python-3.11.0
fi
tar xf ./cpython_src/Python-3.11.0\[20221024\].tar.xz
