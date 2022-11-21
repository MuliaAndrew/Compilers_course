; ModuleID = 'factorinal'
source_filename = "factorinal"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
enrty:
  %0 = alloca i32, align 4
  %1 = bitcast i32* %0 to i8*
  call void @llvm.lifetime.start.poi8(i64 4, i8* %1)
  %2 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i64 0, i64 0), i32* %0)
  %3 = load i32, i32* %0, align 4
  %4 = icmp slt i32 %3, 1
  br i1 %4, label %retBB, label %AriphmeticBB

retBB:                                            ; preds = %AriphmeticBB, %enrty
  %5 = phi i32 [ 1, %enrty ], [ %9, %AriphmeticBB ]
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i64 0, i64 0), i32 %5)
  call void @llvm.lifetime.end.poi8(i64 0, i8* %1)
  ret i32 0

AriphmeticBB:                                     ; preds = %AriphmeticBB, %enrty
  %7 = phi i32 [ %10, %AriphmeticBB ], [ 1, %enrty ]
  %8 = phi i32 [ %9, %AriphmeticBB ], [ 1, %enrty ]
  %9 = mul nsw i32 %7, %8
  %10 = add nuw i32 %7, 1
  %11 = icmp eq i32 %7, %3
  br i1 %11, label %retBB, label %AriphmeticBB
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.poi8(i64 immarg, i8* nocapture) #0

declare i32 @__isoc99_scanf(i8* nocapture noundef readonly, ...)

declare i32 @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.poi8(i64 immarg, i8* nocapture) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
