clang++ -S -g -emit-llvm input1.cpp -o - | opt -lowerswitch -S -o input1.ll
clang++ -S -g -emit-llvm input2.cpp -o - | opt -lowerswitch -S -o input2.ll
clang++ -S -g -emit-llvm input3.cpp -o - | opt -lowerswitch -S -o input3.ll
clang++ -S -g -emit-llvm input3.cpp -o - | opt -lowerswitch -mem2reg -S -o input3m.ll
# clang++ -S -g -emit-llvm input-int-conv.cpp -o - | opt -lowerswitch -S -o input-int-conv.ll
clang++ -S -g -emit-llvm input-logop.cpp -o - | opt -lowerswitch -S -o input-logop.ll

# the following set-up skips -mem2reg and -simplecfg for retaining switch-less, phi-less output
passes="-lowerswitch -globalopt -demanded-bits -branch-prob -inferattrs -ipsccp -dse -loop-simplify -scoped-noalias -barrier -adce -deadargelim -memdep -licm -globals-aa -rpo-functionattrs -basiccg -loop-idiom
-forceattrs -early-cse -instcombine -sccp "
clang -S -g -emit-llvm examples/01_minimal_case.c -o - | opt $passes -S -o examples/01_minimal_case.ll
