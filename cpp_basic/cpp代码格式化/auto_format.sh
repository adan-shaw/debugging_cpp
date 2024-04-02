#!/bin/sh

find . -name "*.cpp" -exec indent -nce -ut -npro -nip -nlp -npsl -i2 -ts2 -sob -ss -l0 -bl -bli 0 `pwd`/{} \;
find . -name "*.c" -exec indent -nce -ut -npro -nip -nlp -npsl -i2 -ts2 -sob -ss -l0 -bl -bli 0 `pwd`/{} \;
find . -name "*.h" -exec indent -nce -ut -npro -nip -nlp -npsl -i2 -ts2 -sob -ss -l0 -bl -bli 0 `pwd`/{} \;
find . -name "*.hpp" -exec indent -nce -ut -npro -nip -nlp -npsl -i2 -ts2 -sob -ss -l0 -bl -bli 0 `pwd`/{} \;

find . -name "*~" -exec rm {} \;
