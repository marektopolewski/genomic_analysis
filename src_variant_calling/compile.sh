#!/bin/sh

########################################
# MAIN PROGRAM #########################
########################################

g++ -std=c++14 main.cpp \
    cigar.cpp \
    constants.cpp \
    filehandler.cpp \
    referencehandler.cpp \
    sequencehandler.cpp \
    varianthandler.cpp \
-o ../bin/variantcall


########################################
# TESTS ################################
########################################

if [ ! -d "tests/bin" ]; then
    mkdir tests/bin
fi

g++ -std=c++14 tests/sequencehandler.cpp \
    filehandler.cpp \
    sequencehandler.cpp \
-o tests/bin/test_sequencehandler

g++ -std=c++14 tests/referencehandler.cpp \
    filehandler.cpp \
    referencehandler.cpp \
-o tests/bin/test_referencehandler

g++ -std=c++14 tests/varianthandler.cpp \
    cigar.cpp \
    filehandler.cpp \
    varianthandler.cpp \
-o tests/bin/test_varianthandler
