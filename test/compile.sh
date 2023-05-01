#!/usr/bin/bash
clang -S -emit-llvm -O0 $1.c -o $1.ll && 
llc $1.ll -o $1.s &&
cd .. &&
/usr/lib/llvm-14/bin/opt -load-pass-plugin build/lib/libremove_ret.so -passes="remove-rets" test/$1.ll -disable-output |& tee test/ret_analysis.txt &&
cd test &&

# One of:
# ../src/remove_rets.py $1.s $1-output.s ret_analysis.txt &&
cp $1.s $1-output.s &&

as $1-output.s -o $1.o &&
ld $1.o -o $1 &&
objdump -d $1 > $1.dump
