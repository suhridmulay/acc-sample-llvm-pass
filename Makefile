CPPFLAGS=-g3 -fPIC -shared
COMPILER=clang

helloPass: passes/HelloPass.cpp
	$(COMPILER) $(CPPFLAGS) passes/HelloPass.cpp -o passes/HelloPass.so
	opt -load passes/HelloPass.so -hello < test.ll > /dev/null
	
ir: test.c
	$(COMPILER) -S -emit-llvm test.c -o test.ll
