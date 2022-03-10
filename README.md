# LLVM - Local Value Numbering

- Project for CS201 - Compiler Construction
- Main file modified is Pass/Transform/ValueNumbering/ValueNumbering.cpp in function void visitor(Function &F)
- Purpose of this project is to develop a pass for a LLVM such that the pass performs local value numbering to the basic blocks of the program
- Given a c program, convert it to a LLVM IR file .ll: clang -S -fno-discard-value-names -emit-llvm [C_FILE_NAME].c -o [C_FILE_NAME].ll  
- To run the pass on the c program: opt -load-pass-plugin ../Pass/build/libLLVMValueNumberingPass.so  -passes=value-numbering [C FILE NAME].ll
- Test programs are provided in test folder
