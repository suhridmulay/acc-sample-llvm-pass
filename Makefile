CPPFLAGS=-g3 -fPIC -shared -Wall -Werror
COMPILER=clang
FILES = $(wildcard *.c)
GENERATED = $(wildcard *.ll) passes/HelloPass.so

helloPass: passes/HelloPass.cpp
	$(COMPILER) $(CPPFLAGS) passes/HelloPass.cpp -o passes/HelloPass.so

ir-1: test1.c
	$(COMPILER) -S -emit-llvm test1.c -o test-1.ll

ir-2: test2.c
	$(COMPILER) -S -emit-llvm test2.c -o test-2.ll

test: passes/HelloPass.so 
	echo "Test 1"
	opt -load passes/HelloPass.so -hello < test-1.ll > /dev/null
	echo "Test 2"
	opt -load passes/HelloPass.so -hello < test-2.ll > /dev/null

clean: $(GENERATED)
	rm $(GENERATED)
