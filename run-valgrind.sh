#! /bin/bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./build/c2xcam/test/c2xcam_UnitTest