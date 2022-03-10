#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/PassManager.h"

#include "llvm/Support/raw_ostream.h"
#include <unordered_map>
using namespace std;

using namespace llvm;

namespace {

// This method implements what the pass does
void visitor(Function &F){

    // Here goes what you want to do with a pass
    
	string func_name = "main";
	errs() << "ValueNumbering: " << F.getName() << "\n";
		
	unordered_map<string, int> hashTable;
	int valueNumbering = 1;
    for (auto& basic_block : F)
    {
		valueNumbering = 1;
        for (auto& inst : basic_block)
        {
			if(inst.getOpcode() == Instruction::Store){
				errs() << inst << "\t\t\t\t";
				string oper1 = string(inst.getOperand(0)->getName());
				string oper2 = string(inst.getOperand(1)->getName());

				//if operand is a temporary register
				if(oper1 == ""){
					oper1 = string(dyn_cast<User>(inst.getOperand(0))->getOperand(0)->getName());
                }

				//if operand is a constant
				if(isa<ConstantInt>(inst.getOperand(0))){
					int temp = (dyn_cast<ConstantInt>(inst.getOperand(0)))->getSExtValue();
					oper1 = to_string(temp);
                }

				if(isa<ConstantInt>(inst.getOperand(1))){
					int temp = (dyn_cast<ConstantInt>(inst.getOperand(1)))->getSExtValue();
					oper2 = to_string(temp);
                }

				unordered_map<string ,int>::const_iterator hashLookUp = hashTable.find(oper1);
	
				if(hashLookUp == hashTable.end()){
					hashTable.insert(make_pair(oper1, valueNumbering));
					hashTable.erase(oper2);
					hashTable.insert(make_pair(oper2, valueNumbering));
					valueNumbering++;
				}
				else{
					hashTable.erase(oper2);
					hashTable.insert(make_pair(oper2, hashLookUp->second));
					hashLookUp = hashTable.find(oper1);
					errs() << hashLookUp->second << " = ";
					hashLookUp = hashTable.find(oper2);
					errs() << hashLookUp->second << "\n";
					continue;
				}
				hashLookUp = hashTable.find(oper1);
				errs() << hashLookUp->second << " = ";
				hashLookUp = hashTable.find(oper2);
				errs() << hashLookUp->second << "\n";

			}
			if(inst.getOpcode() == Instruction::Load){
				errs() << inst << "\t\t\t\t";
				//errs() << inst.getOperand(0)->getName()[0] << "\n";
				string temp = string(inst.getOperand(0)->getName());
				unordered_map<string ,int>::const_iterator hashLookUp = hashTable.find(temp);

				if(hashLookUp == hashTable.end()){
					hashTable.insert(make_pair(temp, valueNumbering));
					valueNumbering++;
				}
				else{
					errs() << hashLookUp->second << " = " << hashLookUp->second << "\n";
					continue;
				}
				hashLookUp = hashTable.find(temp);
				errs() << hashLookUp->second << " = " << hashLookUp->second << "\n";
			}
			if(inst.isBinaryOp()){
				if(inst.getOpcode() == Instruction::Add || inst.getOpcode() == Instruction::Sub || inst.getOpcode() == Instruction::Mul){
					errs() << inst << "\t\t\t\t";
					string valName = string(inst.getName());

					string oper1 = string(dyn_cast<User>(inst.getOperand(0))->getOperand(0)->getName());
					string oper2 = string(dyn_cast<User>(inst.getOperand(1))->getOperand(0)->getName());

					if(isa<ConstantInt>(inst.getOperand(0))){
						int temp = (dyn_cast<ConstantInt>(inst.getOperand(0)))->getSExtValue();
						oper1 = to_string(temp);
					}

					if(isa<ConstantInt>(inst.getOperand(1))){
						int temp = (dyn_cast<ConstantInt>(inst.getOperand(1)))->getSExtValue();
						oper2 = to_string(temp);
					}
					//checks if hash contains operand 1
					unordered_map<string ,int>::const_iterator hashLookUp = hashTable.find(oper1);
					if(hashLookUp == hashTable.end()){
						hashTable.insert(make_pair(oper1, valueNumbering));
						valueNumbering++;				
					}

					//checks if hash contains operand 2
					hashLookUp = hashTable.find(oper2);
					if(hashLookUp == hashTable.end()){
						hashTable.insert(make_pair(oper2, valueNumbering));
						valueNumbering++;
					}

					//formulate instruction into a string
					hashLookUp = hashTable.find(oper1);
					int temp1 = hashLookUp->second;
					hashLookUp = hashTable.find(oper2);
					int temp2 = hashLookUp->second;
					string op1 = to_string(temp1);
					string op2 = to_string(temp2);
					string instr = "";
					if(inst.getOpcode() == Instruction::Add){
						instr = op1 + " add " + op2;
                    }
					if(inst.getOpcode() == Instruction::Sub){
						instr = op1 + " sub " + op2;
					}
					if(inst.getOpcode() == Instruction::Mul){
						instr = op1 + " mul " + op2;
					}

					//checks if hash contains instruction
					hashLookUp = hashTable.find(instr);
					if(hashLookUp == hashTable.end()){
						hashTable.insert(make_pair(instr, valueNumbering));
						valueNumbering++;
					}
					else{
						hashTable.insert(make_pair(valName, hashLookUp->second));
						hashLookUp = hashTable.find(valName);
						errs() << hashLookUp->second << " = ";
						hashLookUp = hashTable.find(instr);
						errs() << hashLookUp->first << " (redundant)\n";
						continue;
                    }
					hashLookUp = hashTable.find(instr);
					hashTable.insert(make_pair(valName, hashLookUp->second));
					hashLookUp = hashTable.find(valName);
					errs() << hashLookUp->second << " = ";
					hashLookUp = hashTable.find(instr);
					errs() << hashLookUp->first << "\n";					
				}
			}
	
        } // end for inst
    } // end for block

   
        
}


// New PM implementation
struct ValueNumberingPass : public PassInfoMixin<ValueNumberingPass> {

  // The first argument of the run() function defines on what level
  // of granularity your pass will run (e.g. Module, Function).
  // The second argument is the corresponding AnalysisManager
  // (e.g ModuleAnalysisManager, FunctionAnalysisManager)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
  	visitor(F);
	return PreservedAnalyses::all();

	
  }
  
    static bool isRequired() { return true; }
};
}



//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "ValueNumberingPass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "value-numbering"){
            FPM.addPass(ValueNumberingPass());
            return true;
          }
          return false;
        });
    }};
}
