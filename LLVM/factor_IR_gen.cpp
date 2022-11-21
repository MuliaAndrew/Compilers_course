#include <iostream>
#include <fstream>
#include <llvm-14/llvm/IR/IRBuilder.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Module.h>
#include <llvm-14/llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>

int main()
{
    using namespace llvm;

    // Making the context
    LLVMContext context;
    Module *module = new Module("factorinal", context);

    // Making IR builber tool
    IRBuilder<> builder{context};

    // Attribute builder tool
    AttrBuilder attr_builder{context};

    // Definintion main function
    FunctionType *main_type = FunctionType::get(builder.getInt32Ty(), false);
    Function *main = Function::Create(main_type, GlobalValue::ExternalLinkage, "main", module);

    // Entry point in main function
    BasicBlock *entry = BasicBlock::Create(context, "enrty", main);
    builder.SetInsertPoint(entry);

    // Declaring @ str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
    Value *str1 = builder.CreateGlobalString("%d");

    // Declaring @ str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
    Value *str2 = builder.CreateGlobalString("%d\n");

    // %0 = alloca i32, align 4
    auto var0_alloca_entry = builder.CreateAlloca(builder.getInt32Ty());

    // %1 = bitcast i32* %0 to i8
    auto var1_bitcast_entry = builder.CreateBitCast(var0_alloca_entry, builder.getInt8Ty()->getPointerTo());

    // Declaring void @ llvm.lifetime.start.poi8(i64 4, i8* nonnull %1)
    std::vector<Type *> poi8_args;
    poi8_args.push_back(builder.getInt64Ty());
    poi8_args.push_back(builder.getInt8Ty()->getPointerTo());
    ArrayRef<Type *> poi8_args_ref{poi8_args};
    FunctionType *poi8_type = FunctionType::get(builder.getVoidTy(), poi8_args_ref, false);
    FunctionCallee lt_start_poi8 = module->getOrInsertFunction("llvm.lifetime.start.poi8", poi8_type);

    // call void @ llvm.lifetime.start.p0i8(i64 4, i8* nonnull %2)
    std::vector<Value *> lt_start_poi8_args;
    lt_start_poi8_args.push_back(ConstantInt::get(builder.getInt64Ty(), 4));
    lt_start_poi8_args.push_back(var1_bitcast_entry);
    ArrayRef<Value *> lt_start_poi8_args_ref{lt_start_poi8_args};
    auto lt_start_poi8_call = builder.CreateCall(lt_start_poi8, lt_start_poi8_args_ref);

    // Declaring i32 @__isoc99_scanf(i8* nocapture noundef readonly, ...)
    std::vector<Type *> scanf_args_types;
    scanf_args_types.push_back(builder.getInt8Ty()->getPointerTo());
    ArrayRef<Type *> scanf_args_types_ref{scanf_args_types};
    FunctionType *scanf_type = FunctionType::get(builder.getInt32Ty(), scanf_args_types_ref, true);
    AttributeList scanf_attr_list;
    attr_builder.addAttribute(Attribute::NoCapture);
    attr_builder.addAttribute(Attribute::NoUndef);
    attr_builder.addAttribute(Attribute::ReadOnly);
    scanf_attr_list = scanf_attr_list.addAttributesAtIndex(context, 1, attr_builder);
    FunctionCallee scanf = module->getOrInsertFunction("__isoc99_scanf", scanf_type, scanf_attr_list);

    // %2 = call i32 (i8*, ...) @ __isoc99_scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* noundef nonnull %1)
    auto var2_scanf_call_entry = builder.CreateCall(scanf, {builder.CreateConstInBoundsGEP2_64(str1->getType()->getContainedType(0), str1, 0, 0), var0_alloca_entry});

    // %3 = load i32, i32* %0, align 4
    auto var3_load_entry = builder.CreateLoad(builder.getInt32Ty(), var0_alloca_entry);

    // %4 = icmp slt i32 %3, 1
    auto var4_icmp_slt_entry = builder.CreateICmpSLT(var3_load_entry, ConstantInt::get(builder.getInt32Ty(), 1));

    // BasicBlock %retBB
    BasicBlock *retBB = BasicBlock::Create(context, "retBB", main);

    // BasicBlock %AriphmeticBB
    BasicBlock *ABB = BasicBlock::Create(context, "AriphmeticBB", main);

    // br i1 %4, label %retBB, label %ABB
    builder.CreateCondBr(var4_icmp_slt_entry, retBB, ABB);

    // Switching to ret BasicBlock
    builder.SetInsertPoint(retBB);

    // %5 = phi i32 [ 1, %0 ], [ %12, %9 ]
    auto var5_phi_retBB = builder.CreatePHI(builder.getInt32Ty(), 2);

    // Declaring i32 @ printf(i8* nocapture noundef readonly, ...)
    FunctionCallee printf = module->getOrInsertFunction("printf", scanf_type);

    // %6 = call i32 (i8*, ...) @printf(i8* noundef nonnull dereferenceable(1) getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), i32 noundef %7)
    auto var6_call_printf_retBB = builder.CreateCall(printf, {builder.CreateConstInBoundsGEP2_64(str2->getType()->getContainedType(0), str2, 0, 0), var5_phi_retBB});

    // Declaring void @ llvm.lifetime.end.p0i8(i64, i8* nonnull)
    AttributeList lt_end_attrs;
    attr_builder.clear();
    attr_builder.addAttribute(Attribute::NonNull);
    lt_end_attrs = lt_end_attrs.addAttributesAtIndex(context, 2, attr_builder);
    ArrayRef<Type *> lt_end_type_ref{builder.getInt64Ty(), builder.getInt8PtrTy()};
    FunctionType* lt_end_type = FunctionType::get(builder.getVoidTy(), lt_end_type_ref, false);
    FunctionCallee lt_end_poi8 = module->getOrInsertFunction("llvm.lifetime.end.poi8", lt_end_type, lt_end_attrs);

    // call void @ llvm.lifetime.end.p0i8(i64 4, i8* nonnull %2)
    builder.CreateCall(lt_end_poi8, {ConstantInt::get(builder.getInt64Ty(), 0), var1_bitcast_entry});

    // ret i32 0
    builder.CreateRet(ConstantInt::get(builder.getInt32Ty(), 0));

    // Switching to ariphmetic BasicBlock
    builder.SetInsertPoint(ABB);

    // %7 = phi i32 [ %13, %9 ], [ 1, %0 ]
    auto var7_phi_ABB = builder.CreatePHI(builder.getInt32Ty(), 2);

    // %8 = phi i32 [ %12, %9 ], [ 1, %0 ]
    auto var8_phi_ABB = builder.CreatePHI(builder.getInt32Ty(), 2);

    // %9 = mul nsw i32 %7, %8
    auto var9_mul_ABB = builder.CreateMul(var7_phi_ABB, var8_phi_ABB, "", false, true);

    // %10 = add nuw i32 %7, 1
    auto var10_add_ABB = builder.CreateAdd(var7_phi_ABB, ConstantInt::get(builder.getInt32Ty(), 1), "", true, false);

    // %11 = icmp eq i32 %10, %4
    auto var11_icmp_ABB = builder.CreateICmpEQ(var7_phi_ABB, var3_load_entry);

    // br i1 %11, label %retBB, label %ariphmeticBB
    builder.CreateCondBr(var11_icmp_ABB, retBB, ABB);
    
    //Additional definition to phi functions
    var5_phi_retBB->addIncoming(ConstantInt::get(builder.getInt32Ty(), 1), entry);
    var5_phi_retBB->addIncoming(var9_mul_ABB, ABB);

    var7_phi_ABB->addIncoming(var10_add_ABB, ABB);
    var7_phi_ABB->addIncoming(ConstantInt::get(builder.getInt32Ty(), 1), entry);

    var8_phi_ABB->addIncoming(var9_mul_ABB, ABB);
    var8_phi_ABB->addIncoming(ConstantInt::get(builder.getInt32Ty(), 1), entry);
    
    // Printing the readable IR
    std::cout << "[LLVM IR]:\n";
    std::string s;
    raw_string_ostream os{s};
    module->print(os, nullptr);
    std::cout << s << "\n";
    std::ofstream IR_out;
    IR_out.open("factor.ll");
    IR_out << s;

    // Execution Engine
    // std::cout << "\n[EE] starting run code:\n";
    // ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    // std::vector<GenericValue> noargs;
    // GenericValue prog_ret = ee->runFunction(main, noargs); 
    // std::cout << "\n[EE] code was run\n";
}