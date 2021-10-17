#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"

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

					std::string instruction_line;
					llvm::raw_string_ostream rso(instruction_line);
					rso << instr;
					errs() << "\t\tInstruction:" << instruction_line << "\n";

					errs() << "\t\tOpCode: " << instr.getOpcodeName() << "\n";
					// TODO: Is there a way to do this more elegantly?
					switch (instr.getOpcode()) {

						// Group Add, Sub and Mul as arithmetic instructions
						case Instruction::Add:
						case Instruction::Sub:
						case Instruction::Mul:
							errs() << "\t\tArithmetic Instruction\n";
							break;

						// Group load Store as instructions that deal with variables and memory
						case Instruction::Load:
						case Instruction::Store:
							errs() << "\t\tLoad/Store instructions, interacts with variables memeory\n";
							break;
						
						// Function calls
						case Instruction::Call:
							errs() << "\t\tFunction call\n";
							errs() << "\t\tCalling: ";
							instr.getOperand(numops - 1)->printAsOperand(errs(), false);
							errs() << "\n";
							break;
						
						// Branch instructions
						case Instruction::Br:
							errs() << "\t\tBranch Instruction\n";
							break;
						
						// And/Or/Xor boolean logic
						case Instruction::And:
						case Instruction::Or:
						case Instruction::Xor:
							errs() << "\t\tLogical (Boolean AND/OR/XOR) Instruction\n";
							break;
						
						// Alloca allocates storage
						case Instruction::Alloca:
							errs() << "\t\tAllocate instruction: asks compiler to ask OS to reserve memory\n";
							break;
						
						// Return 
						case Instruction::Ret:
							errs() << "\t\tReturns from a function\n";
							break;
						
						// Comparison instruction
						case Instruction::ICmp: {
							errs() << "\t\tComparison instruction\n";
							CmpInst* cii = dyn_cast<CmpInst>(&instr);
							// TODO: simplify ugly switch case if possible?
							switch (cii->getPredicate()) {
								case CmpInst::ICMP_EQ:
									errs() << "\t\tComparing for equality\n";
									break;
								case CmpInst::ICMP_NE:
									errs() << "\t\tComparing for not equality\n";
									break;
								case CmpInst::ICMP_SGE:
								case CmpInst::ICMP_UGE:
									errs() << "\t\tComparing Greater Than Equals\n";
									break;
								case CmpInst::ICMP_ULE:
								case CmpInst::ICMP_SLE:
									errs() << "\t\tComparing Less Than Equals\n";
									break;
								case CmpInst::ICMP_SLT:
								case CmpInst::ICMP_ULT:
									errs() << "\t\tComparing Less Than\n";
									break;
								case CmpInst::ICMP_SGT:
								case CmpInst::ICMP_UGT:
									errs() << "\t\tComparing Greater Than\n";
									break;
								default:
									errs() << "\t\tA kind of comparison Idon't know about\n";
									break;
							}
							break;
						}

						// Floating point arithmetic
						case Instruction::FAdd:
						case Instruction::FMul:
						case Instruction::FSub:
						case Instruction::FDiv: {
							errs() << "\t\tFloating point arithmetic\n";
							break;
						}

						// Floating point compares
						case Instruction::FCmp: {
							errs() << "\t\tFloating point comparison\t";
							CmpInst* cif = dyn_cast<CmpInst>(&instr);
							errs() << "\t\tComparison Type: " << CmpInst::getPredicateName(cif->getPredicate()).str() << "\n";
							break;
						}

					}

					// Check if the  instrruction returns something
					// if it does there might be an LHS
					if (!instr.getType()->isVoidTy()) {
						errs() << "\t\tThis statement is also an assignment since it is of the form (\%a = <instr> %b \%c)\n";
						errs() << "\t\t\t[LHS] %" << bbno + lvalue_counter << "\n";
					}
					// Iterate over the rest of the operands to get the RHS
					for (size_t i = 0; i < numops; i++) {
						errs() << "\t\t\t[RHS] ";
						errs() << "Operand " << i << ": ";
						instr.getOperand(i)->printAsOperand(errs(), false);
						errs() << "\n";
					}
					// Increment the lvalue counter of instructions that return an lvalue
					// We need to do this since the lvalues of IR instructions are generated on the fly
					// Meaning they don't actually ecist as named values but as intermediaries for the compiler
					// LLVM stores no record of these values so we have to generate them
					if (!instr.getType()->isVoidTy()) {
						lvalue_counter += 1;
					}
				}
			}
			// Did we modify the source?
			return false;
		}
	};
}

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass", false, false);

static RegisterStandardPasses Y(PassManagerBuilder::EP_EarlyAsPossible, [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
	PM.add(new Hello());
});