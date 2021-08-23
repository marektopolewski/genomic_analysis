#!/bin/sh

########################################
# MAIN PROGRAM #########################
########################################

g++ -std=c++14 main.cpp \
    cigar.cpp \
    constants.cpp \
    referencehandler.cpp \
    sequencehandler.cpp \
    varianthandler.cpp \
    ../src_shared/filehandler.cpp \
    ../src_shared/variant.cpp \
-o ../bin/variantcall


########################################
# TESTS ################################
########################################

if [ ! -d "tests/bin" ]; then
    mkdir tests/bin
fi

g++ -std=c++14 tests/sequencehandler.cpp \
    sequencehandler.cpp \
    ../src_shared/filehandler.cpp \
-o tests/bin/test_sequencehandler

g++ -std=c++14 tests/referencehandler.cpp \
    referencehandler.cpp \
    ../src_shared/filehandler.cpp \
-o tests/bin/test_referencehandler

g++ -std=c++14 tests/varianthandler.cpp \
    cigar.cpp \
    varianthandler.cpp \
    ../src_shared/filehandler.cpp \
    ../src_shared/variant.cpp \
-o tests/bin/test_varianthandler
