#!/bin/bash
# Tested on Mac OS Monterey clang-14 and Ubuntu 24.04 with gcc-13
# Use 'DISABLE_TEST=1 ./build_and_test_and_run.sh' to skip test running
# Use 'TEST_ONLY=1 ./build_and_test_and_run.sh' to skip server running

set -e

mkdir -p build
(cd build && cmake .. && make)

if [ -z "${DISABLE_TEST:-}" ] || [ "${DISABLE_TEST}" = "0" ]; then
    build/at_command_server_test
fi

if [ -z "${TEST_ONLY:-}" ] || [ "${TEST_ONLY}" = "0" ]; then
    build/at_command_server
fi
