#!/bin/sh

########################################
# MAIN PROGRAM #########################
########################################

g++ -std=c++14 main.cpp \
    analyser.cpp \
    ../src_shared/filehandler.cpp \
    ../src_shared/variant.cpp \
-o ../bin/analysis


########################################
# TESTS ################################
########################################

if [ ! -d "tests/bin" ]; then
    mkdir tests/bin
fi

g++ -std=c++14 tests/analyser.cpp \
    analyser.cpp \
    ../src_shared/filehandler.cpp \
    ../src_shared/variant.cpp \
-o tests/bin/test_analyser
