#!/bin/sh

echo "Compiling.."
./compile.sh
echo "done."

echo ""
echo "Running tests.."
./tests/bin/test_analyser
echo "done."
