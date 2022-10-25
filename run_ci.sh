#!/usr/bin/bash

set -e

# Simple test
mkdir -p build && cd build && cmake .. -GNinja && ninja run_test
cd -
echo "Simple test job passed"
set -e

# UB Sanitizer test
mkdir -p build_ub && cd build_ub && cmake .. -GNinja -DTHREAD_SAN_ENABLE=false -DUB_SAN_ENABLE=true && ninja run_test
cd -
echo "UB Sanitizer job passed"
set -e

# Thread Sanitizer test
mkdir -p build_thread && cd build_thread && cmake .. -GNinja -DTHREAD_SAN_ENABLE=true -DUB_SAN_ENABLE=false && ninja run_test
cd -
echo "Thread Sanitizer job passed"
echo "ALL JOBS PASSED"