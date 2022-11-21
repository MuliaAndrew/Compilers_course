#include <iostream>
#include <fstream>
#include <llvm-14/llvm/IR/IRBuilder.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Module.h>
#include <llvm-14/llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>


int main () {
    using namespace llvm;
    
    LLVMContext context;
    Module* module = new Module("top", context);

    IRBuilder<> builder{context};

    FunctionType* func_type = FunctionType::get(builder.getVoidTy(), 0);
    Function* main = Function::Create(func_type, Function::ExternalLinkage, "main", module);

    BasicBlock *entrybb = BasicBlock::Create(context, "entry", main);

    builder.SetInsertPoint(entrybb);
    
    Value* hello_world = builder.CreateGlobalString("Hello World!\n");
    
    std::vector<Type *> puts_args;
    puts_args.push_back(builder.getInt8Ty()->getPointerTo());
    ArrayRef<Type *> puts_args_ref{puts_args};
    FunctionType* puts_type = FunctionType::get(builder.getInt32Ty(), puts_args_ref, 0);
    FunctionCallee puts = module->getOrInsertFunction("puts", puts_type);

    builder.CreateCall(puts, hello_world);
    builder.CreateRet(ConstantInt::get(builder.getInt32Ty(), 0));

    std::cout << "\n[LLVM IR]:\n";
    std::string s;
    raw_string_ostream os{s};
    module->print(os, nullptr);
    os.flush();
    std::cout << s;

    std::cout << "\n[EE] starting run code\n";
    ExecutionEngine* ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    std::vector<GenericValue> noargs;
    GenericValue prog_ret = ee->runFunction(main, noargs);
    std::cout << "[EE] Code was run\n";

    return 0;
}