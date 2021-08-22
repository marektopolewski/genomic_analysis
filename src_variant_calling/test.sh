#!/bin/sh

echo "Compiling.."
./compile.sh
echo "done."

echo ""
echo "Running tests.."
./tests/bin/test_sequencehandler
./tests/bin/test_referencehandler
./tests/bin/test_varianthandler
echo "done."
