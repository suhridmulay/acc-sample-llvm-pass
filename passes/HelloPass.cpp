#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
	struct Hello : public FunctionPass {
		static char ID;
		Hello() : FunctionPass(ID) {}

		bool runOnFunction(Function &F) override {
			// Log the function name to stderr
			errs() << "Analysing Function: " << F.getName() << "\n";

			// Iterate over all basic blocks
			for (llvm::BasicBlock &bb: F) {
				// Lvalue counter for within basic blocks
				// Use this to get the lvalue of the current llvm IR instruction
				// Eg: in %5 = add %3 %4 this helps us generate %5
				size_t lvalue_counter = 1;

				// Work around to get the basic block number
				// Init a temporary string
				std::string temp;
				// Make a llvm ostream out of the string
				llvm::raw_string_ostream rso(temp);
				// Print the basic block as operand (%0 style) to the ostream
				// Consequently it gets printed to the temporary string
				bb.printAsOperand(rso, false);
				// Remove the %age sign and get the basic block number
				size_t bbno = std::stoi(temp.erase(0, 1));

				// Logging out the basic block number
				errs() << "\tBasic Block: " << bbno << "\n";

				// Iterate over all instructions in the basic block
				for (llvm::Instruction &instr: bb) {
					size_t numops = instr.getNumOperands();
					errs() << "\t\t" << instr.getOpcodeName() << "\n";

					// Check if the  instrruction returns something
					// if it does there might be an LHS
					if (!instr.getType()->isVoidTy()) {
						errs() << "\t\t\t[LHS] %" << bbno + lvalue_counter << "\n";
					}
					for (size_t i = 0; i < numops; i++) {
						errs() << "\t\t\t[RHS] ";
						errs() << "Operand " << i << ": ";
						instr.getOperand(i)->printAsOperand(errs(), false);
						errs() << "\n";
					}
					if (!instr.getType()->isVoidTy()) {
						lvalue_counter += 1;
					}
				}
			}
			return false;
		}
	};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);

static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible, [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
	PM.add(new Hello());
});