#!/bin/bash
gcc -c bill.c fred.c
gcc -c program.c
gcc -o program program.o bill.o

ar crv libfoo.a bill.o fred.o
ranlib libfoo.a
# gcc -o program program.o libfoo.a
gcc -o program program.o -L. -lfoo