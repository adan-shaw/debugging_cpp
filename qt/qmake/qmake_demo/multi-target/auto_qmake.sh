#!/bin/sh

mkdir buildA
cd buildA
qmake -makefile ../multi-target.pro CONFIG+=A
make
cd ..

mkdir buildB
cd buildB
qmake -makefile ../multi-target.pro CONFIG+=B
make
cd ..

