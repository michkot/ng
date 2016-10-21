clang++ -S -g -emit-llvm input1.cpp -o - | opt -lowerswitch -S -o input3.ll
clang++ -S -g -emit-llvm input2.cpp -o - | opt -lowerswitch -S -o input3.ll
clang++ -S -g -emit-llvm input3.cpp -o - | opt -lowerswitch -S -o input3.ll
clang++ -S -g -emit-llvm input3.cpp -o - | opt -lowerswitch -mem2reg -S -o input3m.ll
