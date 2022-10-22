#include "llvm-10/llvm/IR/IRBuilder.h"
#include "llvm-10/llvm/IR/LLVMContext.h"
#include "llvm-10/llvm/IR/Module.h"
#include "llvm-10/llvm/Support/TargetSelect.h"
#include "llvm-10/llvm/Support/raw_ostream.h"
#include <fstream>
#include <iostream>

llvm::GlobalVariable *PIXELS = nullptr;
llvm::GlobalVariable *NEW_PIXELS = nullptr;
llvm::LLVMContext context;
llvm::IRBuilder<> builder(context);

void createGlobal(llvm::Module *module, llvm::IRBuilder<> &builder) {
  llvm::Type *cur_type = llvm::ArrayType::get(
      llvm::ArrayType::get(builder.getInt16Ty(), 300), 300);

  module->getOrInsertGlobal("pixels", cur_type);
  PIXELS = module->getNamedGlobal("pixels");
  PIXELS->setLinkage(llvm::GlobalValue::CommonLinkage);
  PIXELS->setAlignment(llvm::MaybeAlign(16));
  PIXELS->setInitializer(llvm::ConstantAggregateZero::get(cur_type));

  module->getOrInsertGlobal("new_pixels", cur_type);
  NEW_PIXELS = module->getNamedGlobal("new_pixels");
  NEW_PIXELS->setLinkage(llvm::GlobalValue::CommonLinkage);
  NEW_PIXELS->setAlignment(llvm::MaybeAlign(16));
  NEW_PIXELS->setInitializer(llvm::ConstantAggregateZero::get(cur_type));
}

llvm::Value *getConst(int type, int value) {
  llvm::Type *Ty = nullptr;
  switch (type) {
  case 1:
    Ty = builder.getInt1Ty();
    break;
  case 8:
    Ty = builder.getInt8Ty();
    break;
  case 16:
    Ty = builder.getInt16Ty();
    break;
  case 32:
    Ty = builder.getInt32Ty();
    break;
  case 64:
    Ty = builder.getInt64Ty();
    break;
  default:
    std::abort();
    break;
  }
  return llvm::ConstantInt::get(Ty, value);
}

int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  // ; ModuleID = 'math'
  // source_filename = "math"
  llvm::Module *module = new llvm::Module("math", context);

  // @pixels = common global [300 x [300 x i16]] zeroinitializer, align 16
  // @new_pixels = global[300 x[300 x i16]] zeroinitializer , align 16
  createGlobal(module, builder);

  // define i32 @initPixels([300 x i16]* %0) {
  llvm::FunctionType *funcType = llvm::FunctionType::get(
      builder.getInt32Ty(),
      llvm::PointerType::get(llvm::ArrayType::get(builder.getInt16Ty(), 300),
                             0),
      false);
  llvm::Function *mainFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "initPixels", module);

  llvm::BasicBlock *block1 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block2 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block15 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block16 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block35 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block44 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block62 = llvm::BasicBlock::Create(context, "", mainFunc);
  llvm::BasicBlock *block63 = llvm::BasicBlock::Create(context, "", mainFunc);

  // 1:
  builder.SetInsertPoint(block1);

  // br label %2
  llvm::BranchInst *branchInst = builder.CreateBr(block2);

  // 2:
  builder.SetInsertPoint(block2);

  // %3 = phi i64 [ 0, %1 ], [ %73, %63 ]
  llvm::PHINode *var3 = builder.CreatePHI(builder.getInt64Ty(), 2);
  var3->addIncoming(llvm::ConstantInt::get(builder.getInt64Ty(), 0), block1);

  // %4 = phi <8 x i32> [ <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32
  // 7>, %1 ], [ %74, %63 ]
  llvm::VectorType *var4Ty = llvm::VectorType::get(builder.getInt32Ty(), 8);
  llvm::PHINode *var4 = builder.CreatePHI(var4Ty, 2);
  var4->addIncoming(
      llvm::ConstantDataVector::get(
          context, llvm::ArrayRef<uint32_t>{0, 1, 2, 3, 4, 5, 6, 7}),
      block1);

  // %5 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32
  // 12799, i32 12799, i32 12799, i32 12799>
  llvm::Value *var5 = builder.CreateMul(
      var4, llvm::ConstantDataVector::get(
                context, llvm::ArrayRef<uint32_t>{12799, 12799, 12799, 12799,
                                                  12799, 12799, 12799, 12799}));

  // %6 = udiv <8 x i32> %5, <i32 300, i32 300, i32 300, i32 300, i32 300, i32
  // 300, i32 300, i32 300>
  llvm::Value *var6 = builder.CreateUDiv(
      var5, llvm::ConstantDataVector::get(
                context, llvm::ArrayRef<uint32_t>{300, 300, 300, 300, 300, 300,
                                                  300, 300}));

  // %7 = trunc <8 x i32> % 6 to <8 x i16>
  llvm::Value *var7 =
      builder.CreateTrunc(var6, llvm::VectorType::get(builder.getInt16Ty(), 8));

  // %8 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64
  // 0, i64 0, i64 %3
  llvm::Value *var8TyIn = llvm::ConstantInt::get(builder.getInt64Ty(), 0);
  llvm::Value *var8 = builder.CreateGEP(
      PIXELS, llvm::ArrayRef<llvm::Value *>{var8TyIn, var8TyIn, var3});

  // %9 = bitcast i16* %8 to <8 x i16>*
  llvm::Value *var9 = builder.CreateBitCast(
      var8, llvm::PointerType::get(
                llvm::VectorType::get(builder.getInt16Ty(), 8), 0));

  // store <8 x i16> %7, <8 x i16>* %9, align 16
  builder.CreateAlignedStore(var7, var9, 16);

  // %10 = sub <8 x i16> <i16 12799, i16 12799, i16 12799, i16 12799, i16 12799,
  // i16 12799, i16 12799, i16 12799>, %7
  llvm::Value *var10 = builder.CreateSub(
      llvm::ConstantDataVector::get(
          context, llvm::ArrayRef<uint16_t>{12799, 12799, 12799, 12799, 12799,
                                            12799, 12799, 12799}),
      var7);

  // %11 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64
  // 0, i64 299, i64 %3
  llvm::Value *var11TyIn1 = llvm::ConstantInt::get(builder.getInt64Ty(), 0);
  llvm::Value *var11TyIn2 = llvm::ConstantInt::get(builder.getInt64Ty(), 299);
  llvm::Value *var11 = builder.CreateGEP(
      PIXELS, llvm::ArrayRef<llvm::Value *>{var11TyIn1, var11TyIn2, var3});

  // %12 = bitcast i16* %11 to <8 x i16>*
  llvm::Value *var12 = builder.CreateBitCast(
      var11, llvm::PointerType::get(
                 llvm::VectorType::get(builder.getInt16Ty(), 8), 0));

  // store <8 x i16> %10, <8 x i16>* %12, align 8
  builder.CreateAlignedStore(var10, var12, 8);

  // %13 = or i64 %3, 8
  llvm::Value *var13 =
      builder.CreateOr(var3, llvm::ConstantInt::get(builder.getInt64Ty(), 8));

  // %14 = icmp eq i64 % 13 , 296
  llvm::Value *var14 = builder.CreateICmpEQ(
      var13, llvm::ConstantInt::get(builder.getInt64Ty(), 296));

  // br i1 %14, label %15, label %63
  builder.CreateCondBr(var14, block15, block63);

  // 15:
  builder.SetInsertPoint(block15);

  // store i16 12628, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x
  // i16]]* @pixels, i64 0, i64 0, i64 296), align 16
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 12628),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{
                            var8TyIn, var8TyIn,
                            llvm::ConstantInt::get(builder.getInt16Ty(), 296)}),
      16);

  // store i16 171, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x
  // i16]]* @pixels, i64 0, i64 299, i64 296), align 8
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 171),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 299),
                                                      getConst(64, 296)}),
      8);

  // store i16 12671 , i16* getelementptr ([300 x[300 x i16]] , [300 x[300 x
  // i16]] * @pixels , i64 0 , i64 0 , i64 297) , align 2
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 12671),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 0),
                                                      getConst(64, 297)}),
      2);

  // store i16 128, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x
  // i16]]* @pixels, i64 0, i64 299, i64 297), align 2
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 128),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 299),
                                                      getConst(64, 297)}),
      2);

  // store i16 12713, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x
  // i16]]* @pixels, i64 0, i64 0, i64 298), align 4
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 12713),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 0),
                                                      getConst(64, 298)}),
      4);

  // store i16 86, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x i16]]*
  // @pixels, i64 0, i64 299, i64 298), align 4
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 86),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 299),
                                                      getConst(64, 298)}),
      4);

  // store i16 12756, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x
  // i16]]* @pixels, i64 0, i64 0, i64 299), align 2
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 12756),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 0),
                                                      getConst(64, 299)}),
      2);

  // store i16 43, i16* getelementptr ([300 x [300 x i16]], [300 x [300 x i16]]*
  // @pixels, i64 0, i64 299, i64 299), align 2
  builder.CreateAlignedStore(
      llvm::ConstantInt::get(builder.getInt16Ty(), 43),
      builder.CreateGEP(PIXELS,
                        llvm::ArrayRef<llvm::Value *>{getConst(64, 0),
                                                      getConst(64, 299),
                                                      getConst(64, 299)}),
      2);

  // br label %16
  llvm::BranchInst *branchInst2 = builder.CreateBr(block16);

  // 16:
  builder.SetInsertPoint(block16);

  // %17 = phi i64 [ 0, %15 ], [ %33, %16 ]
  llvm::PHINode *var17 = builder.CreatePHI(builder.getInt64Ty(), 2);
  var17->addIncoming(getConst(64, 0), block15);

  // %18 = trunc i64 %17 to i32
  llvm::Value *var18 = builder.CreateTrunc(var17, builder.getInt32Ty());

  // %19 = mul i32 %18, 12799
  llvm::Value *var19 = builder.CreateMul(var18, getConst(32, 12799));

  // %20 = udiv i32 %19, 300
  llvm::Value *var20 = builder.CreateUDiv(var19, getConst(32, 300));

  // %21 = trunc i32 %20 to i16
  llvm::Value *var21 = builder.CreateTrunc(var20, builder.getInt16Ty());

  // %22 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64
  // 0, i64 %17, i64 0
  llvm::Value *var22 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var17, getConst(64, 0)});

  // store i16 %21, i16* %22, align 16
  builder.CreateAlignedStore(var21, var22, 16);

  // %23 = sub i16 12799, %21
  llvm::Value *var23 = builder.CreateSub(getConst(16, 12799), var21);

  // %24 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64
  // 0, i64 %17, i64 299
  llvm::Value *var24 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var17, getConst(64, 299)});

  // store i16 %23, i16* %24, align 2
  builder.CreateAlignedStore(var23, var24, 2);

  // %25 = or i64 %17, 1
  llvm::Value *var25 = builder.CreateOr(var17, getConst(64, 1));

  // %26 = trunc i64 %25 to i32
  llvm::Value *var26 = builder.CreateTrunc(var25, builder.getInt32Ty());

  // %27 = mul i32 %26, 12799
  llvm::Value *var27 = builder.CreateMul(var26, getConst(32, 12799));

  // %28 = udiv i32 %27, 300
  llvm::Value *var28 = builder.CreateUDiv(var27, getConst(32, 300));

  // %29 = trunc i32 %28 to i16
  llvm::Value *var29 = builder.CreateTrunc(var28, builder.getInt16Ty());

  // %30 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64
  // 0, i64 %25, i64 0
  llvm::Value *var30 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var25, getConst(64, 0)});

  // store i16 %29, i16* %30, align 8
  builder.CreateAlignedStore(var29, var30, 8);

  // %31 = sub nuw nsw i16 12799, %29
  llvm::Value *var31 = builder.CreateSub(getConst(16, 12799), var29);

  // %32 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]*
  // @pixels, i64 0, i64 %25, i64 299
  llvm::Value *var32 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var25, getConst(64, 299)});

  // store i16 %31, i16* %32, align 2
  builder.CreateAlignedStore(var31, var32, 2);

  // %33 = add i64 %17, 2
  llvm::Value *var33 = builder.CreateAdd(var17, getConst(64, 2));
  var17->addIncoming(var33, block16);

  // %34 = icmp eq i64 %33, 300
  llvm::Value *var34 = builder.CreateICmpEQ(var33, getConst(64, 300));

  // br i1 %34, label %35, label %16
  builder.CreateCondBr(var34, block35, block16);

  // 35:
  builder.SetInsertPoint(block35);

  // % 36 = phi i64[% 40 , % 35] , [0 , % 16]
  llvm::PHINode *var36 = builder.CreatePHI(builder.getInt64Ty(), 2);
  var36->addIncoming(getConst(64, 0), block16);

  // % 37 = or i64 % 36 , 1
  llvm::Value *var37 = builder.CreateOr(var36, getConst(64, 1));

  // % 38 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 % 37 , i64 1
  llvm::Value *var38 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var37, getConst(64, 1)});

  // % 39 = bitcast i16 * %38 to i8 *
  llvm::Value *var39 =
      builder.CreateBitCast(var38, builder.getInt8Ty()->getPointerTo());

  // call void @llvm.memset.p0i8.i64 (i8 * %39 , i8 0 , i64 596 , i1 false)
  llvm::FunctionType *llvmMemsetTy = llvm::FunctionType::get(
      builder.getVoidTy(),
      llvm::ArrayRef<llvm::Type *>{
          llvm::PointerType::get(builder.getInt8Ty(), 0), builder.getInt8Ty(),
          builder.getInt64Ty(), builder.getInt1Ty()},
      false);
  llvm::FunctionCallee llvmMemsetCall =
      module->getOrInsertFunction("llvm.memset.p0i8.i64", llvmMemsetTy);
  builder.CreateCall(llvmMemsetCall, llvm::ArrayRef<llvm::Value *>{
                                         var39, getConst(8, 0),
                                         getConst(64, 596), getConst(1, 0)});

  // % 40 = add i64 % 36 , 2
  llvm::Value *var40 = builder.CreateAdd(var36, getConst(64, 2));
  var36->addIncoming(var40, block35);

  // % 41 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 % 40 , i64 1
  llvm::Value *var41 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var40, getConst(64, 1)});

  // % 42 = bitcast i16 * %41 to i8 *
  llvm::Value *var42 =
      builder.CreateBitCast(var41, builder.getInt8Ty()->getPointerTo());

  // call void @llvm.memset.p0i8.i64 (i8 * %42 , i8 0 , i64 596 , i1 false)
  builder.CreateCall(llvmMemsetCall, llvm::ArrayRef<llvm::Value *>{
                                         var42, getConst(8, 0),
                                         getConst(64, 596), getConst(1, 0)});

  // % 43 = icmp eq i64 % 40 , 298
  llvm::Value *var43 = builder.CreateICmpEQ(
      var40, llvm::ConstantInt::get(builder.getInt64Ty(), 298));

  // br i1 % 43 , label % 44 , label % 35
  builder.CreateCondBr(var43, block44, block35);

  // 44:; preds = % 35 , % 44
  builder.SetInsertPoint(block44);

  // % 45 = phi i64[% 60 , % 44] , [0 , % 35]
  llvm::PHINode *var45 = builder.CreatePHI(builder.getInt64Ty(), 2);
  var45->addIncoming(getConst(64, 0), block35);

  // % 46 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @new_pixels ,
  // i64 0 , i64 % 45 , i64 0
  llvm::Value *var46 = builder.CreateGEP(
      NEW_PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var45, getConst(64, 0)});

  // % 47 = bitcast i16 * %46 to i8 *
  llvm::Value *var47 =
      builder.CreateBitCast(var46, builder.getInt8Ty()->getPointerTo());

  // %48 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 % 45 , i64 0
  llvm::Value *var48 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var45, getConst(64, 0)});

  // % 49 = bitcast i16 * %48 to i8 *
  llvm::Value *var49 =
      builder.CreateBitCast(var48, builder.getInt8Ty()->getPointerTo());

  // call void @llvm.memcpy.p0i8.p0i8.i64 (i8 * %47 , i8 * %49 , i64 600 , i1
  // false)
  llvm::FunctionType *llvmMemsetTy2 = llvm::FunctionType::get(
      builder.getVoidTy(),
      llvm::ArrayRef<llvm::Type *>{builder.getInt8Ty()->getPointerTo(),
                                   builder.getInt8Ty()->getPointerTo(),
                                   builder.getInt64Ty(), builder.getInt1Ty()},
      false);
  llvm::FunctionCallee llvmMemsetCall2 =
      module->getOrInsertFunction("llvm.memcpy.p0i8.p0i8.i64", llvmMemsetTy2);
  builder.CreateCall(llvmMemsetCall2,
                     llvm::ArrayRef<llvm::Value *>{
                         var47, var49, getConst(64, 600), getConst(1, 0)});

  // % 50 = add i64 % 45 , 1
  llvm::Value *var50 = builder.CreateAdd(var45, getConst(64, 1));

  // % 51 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @new_pixels ,
  // i64 0 , i64 % 50 , i64 0
  llvm::Value *var51 = builder.CreateGEP(
      NEW_PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var50, getConst(64, 0)});

  // % 52 = bitcast i16 * %51 to i8 *
  llvm::Value *var52 =
      builder.CreateBitCast(var51, builder.getInt8Ty()->getPointerTo());

  // %53 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 % 50 , i64 0
  llvm::Value *var53 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var50, getConst(64, 0)});

  // % 54 = bitcast i16 * %53 to i8 *
  llvm::Value *var54 =
      builder.CreateBitCast(var53, builder.getInt8Ty()->getPointerTo());

  // call void @llvm.memcpy.p0i8.p0i8.i64 (i8 * %52 , i8 * %54 , i64 600 , i1
  // false)
  builder.CreateCall(llvmMemsetCall2,
                     llvm::ArrayRef<llvm::Value *>{
                         var52, var54, getConst(64, 600), getConst(1, 0)});

  // % 55 = add i64 % 45 , 2
  llvm::Value *var55 = builder.CreateAdd(var45, getConst(64, 2));

  // % 56 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @new_pixels ,
  // i64 0 , i64 % 55 , i64 0
  llvm::Value *var56 = builder.CreateGEP(
      NEW_PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var55, getConst(64, 0)});

  // % 57 = bitcast i16 * %56 to i8 *
  llvm::Value *var57 =
      builder.CreateBitCast(var56, builder.getInt8Ty()->getPointerTo());

  // %58 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 % 55 , i64 0
  llvm::Value *var58 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), var55, getConst(64, 0)});

  // % 59 = bitcast i16 * %58 to i8 *
  llvm::Value *var59 =
      builder.CreateBitCast(var58, builder.getInt8Ty()->getPointerTo());

  // call void @llvm.memcpy.p0i8.p0i8.i64 (i8 * %57 , i8 * %59 , i64 600 , i1
  // false)
  builder.CreateCall(llvmMemsetCall2,
                     llvm::ArrayRef<llvm::Value *>{
                         var57, var59, getConst(64, 600), getConst(1, 0)});

  // % 60 = add i64 % 45 , 3
  llvm::Value *var60 = builder.CreateAdd(var45, getConst(64, 3));
  var45->addIncoming(var60, block44);

  // % 61 = icmp eq i64 % 60 , 300
  llvm::Value *var61 = builder.CreateICmpEQ(
      var60, llvm::ConstantInt::get(builder.getInt64Ty(), 300));

  // br i1 % 61 , label % 62 , label % 44
  builder.CreateCondBr(var61, block62, block44);

  // 62:
  builder.SetInsertPoint(block62);

  // ret i32 0
  builder.CreateRet(getConst(32, 0));

  // 63:
  builder.SetInsertPoint(block63);

  // %64 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32
  // 12799, i32 12799, i32 12799, i32 12799>
  llvm::Value *var64 = builder.CreateMul(
      var4, llvm::ConstantDataVector::get(
                context, llvm::ArrayRef<uint32_t>{12799, 12799, 12799, 12799,
                                                  12799, 12799, 12799, 12799}));

  // % 65 = add <8 x i32> % 64 , <i32 102392 , i32 102392 , i32 102392 , i32
  // 102392 , i32 102392 , i32 102392 , i32 102392 , i32 102392>
  llvm::Value *var65 = builder.CreateAdd(
      var64,
      llvm::ConstantDataVector::get(
          context, llvm::ArrayRef<uint32_t>{102392, 102392, 102392, 102392,
                                            102392, 102392, 102392, 102392}));

  // % 66 = udiv <8 x i32> % 65 , <i32 300 , i32 300 , i32 300 , i32 300 , i32
  // 300 , i32 300 , i32 300 , i32 300>
  llvm::Value *var66 = builder.CreateUDiv(
      var65, llvm::ConstantDataVector::get(
                 context, llvm::ArrayRef<uint32_t>{300, 300, 300, 300, 300, 300,
                                                   300, 300}));

  // % 67 = trunc <8 x i32> % 66 to <8 x i16>
  llvm::Value *var67 = builder.CreateTrunc(
      var66, llvm::VectorType::get(builder.getInt16Ty(), 8));

  // % 68 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 0 , i64 % 13
  llvm::Value *var68 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), getConst(64, 0), var13});

  // % 69 = bitcast i16 * %68 to <8 x i16>*
  llvm::Value *var69 = builder.CreateBitCast(
      var68, llvm::PointerType::get(
                 llvm::VectorType::get(builder.getInt16Ty(), 8), 0));

  // store <8 x i16> % 67 , <8 x i16>*%69 , align 16
  builder.CreateAlignedStore(var67, var69, 16);

  // % 70 = sub <8 x i16> <i16 12799 , i16 12799 , i16 12799 , i16 12799 , i16
  // 12799 , i16 12799 , i16 12799 , i16 12799> , % 67
  llvm::Value *var70 = builder.CreateSub(
      llvm::ConstantDataVector::get(
          context, llvm::ArrayRef<uint16_t>{12799, 12799, 12799, 12799, 12799,
                                            12799, 12799, 12799}),
      var67);

  // % 71 = getelementptr[300 x[300 x i16]] , [300 x[300 x i16]] * @pixels , i64
  // 0 , i64 299 , i64 % 13
  llvm::Value *var71 = builder.CreateGEP(
      PIXELS,
      llvm::ArrayRef<llvm::Value *>{getConst(64, 0), getConst(64, 299), var13});

  // % 72 = bitcast i16 * %71 to <8 x i16>*
  llvm::Value *var72 = builder.CreateBitCast(
      var71, llvm::PointerType::get(
                 llvm::VectorType::get(builder.getInt16Ty(), 8), 0));

  // store <8 x i16> % 70 , <8 x i16>*%72 , align 8
  builder.CreateAlignedStore(var70, var72, 8);

  // %73 = add i64 %3, 16
  llvm::Value *var73 =
      builder.CreateAdd(var3, llvm::ConstantInt::get(builder.getInt64Ty(), 16));
  var3->addIncoming(var73, block63);

  // %74 = add <8 x i32> %4, <i32 16, i32 16, i32 16, i32 16, i32 16, i32 16,
  // i32 16, i32 16>
  llvm::Value *var74 = builder.CreateAdd(
      var4,
      llvm::ConstantDataVector::get(
          context, llvm::ArrayRef<uint32_t>{16, 16, 16, 16, 16, 16, 16, 16}));
  var4->addIncoming(var74, block63);

  // br label %2
  builder.CreateBr(block2);

  //-----------------------------------------------------------------------------------

  // Dump LLVM IR
  std::string s;
  llvm::raw_string_ostream os(s);
  module->print(os, nullptr);
  os.flush();
  std::ofstream file;
  file.open("../math/my_math.ll");
  file << s;
  file.close();

  return 0;
}
