; ModuleID = '../math/math.c'
source_filename = "../math/math.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@pixels = common dso_local local_unnamed_addr global [300 x [300 x i16]] zeroinitializer, align 16
@new_pixels = common dso_local local_unnamed_addr global [300 x [300 x i16]] zeroinitializer, align 16

; Function Attrs: nofree norecurse nounwind uwtable
define dso_local i32 @initPixels([300 x i16]* nocapture readnone %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %63, %1
  %3 = phi i64 [ 0, %1 ], [ %73, %63 ]
  %4 = phi <8 x i32> [ <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>, %1 ], [ %74, %63 ]
  %5 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799>
  %6 = udiv <8 x i32> %5, <i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300>
  %7 = trunc <8 x i32> %6 to <8 x i16>
  %8 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 %3
  %9 = bitcast i16* %8 to <8 x i16>*
  store <8 x i16> %7, <8 x i16>* %9, align 16, !tbaa !2
  %10 = sub nuw nsw <8 x i16> <i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799>, %7
  %11 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 %3
  %12 = bitcast i16* %11 to <8 x i16>*
  store <8 x i16> %10, <8 x i16>* %12, align 8, !tbaa !2
  %13 = or i64 %3, 8
  %14 = icmp eq i64 %13, 296
  br i1 %14, label %15, label %63, !llvm.loop !6

15:                                               ; preds = %2
  store i16 12628, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 296), align 16, !tbaa !2
  store i16 171, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 296), align 8, !tbaa !2
  store i16 12671, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 297), align 2, !tbaa !2
  store i16 128, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 297), align 2, !tbaa !2
  store i16 12713, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 298), align 4, !tbaa !2
  store i16 86, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 298), align 4, !tbaa !2
  store i16 12756, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 299), align 2, !tbaa !2
  store i16 43, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 299), align 2, !tbaa !2
  br label %16

16:                                               ; preds = %16, %15
  %17 = phi i64 [ 0, %15 ], [ %33, %16 ]
  %18 = trunc i64 %17 to i32
  %19 = mul nuw nsw i32 %18, 12799
  %20 = udiv i32 %19, 300
  %21 = trunc i32 %20 to i16
  %22 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %17, i64 0
  store i16 %21, i16* %22, align 16, !tbaa !2
  %23 = sub nuw nsw i16 12799, %21
  %24 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %17, i64 299
  store i16 %23, i16* %24, align 2, !tbaa !2
  %25 = or i64 %17, 1
  %26 = trunc i64 %25 to i32
  %27 = mul nuw nsw i32 %26, 12799
  %28 = udiv i32 %27, 300
  %29 = trunc i32 %28 to i16
  %30 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %25, i64 0
  store i16 %29, i16* %30, align 8, !tbaa !2
  %31 = sub nuw nsw i16 12799, %29
  %32 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %25, i64 299
  store i16 %31, i16* %32, align 2, !tbaa !2
  %33 = add nuw nsw i64 %17, 2
  %34 = icmp eq i64 %33, 300
  br i1 %34, label %35, label %16

35:                                               ; preds = %16, %35
  %36 = phi i64 [ %40, %35 ], [ 0, %16 ]
  %37 = or i64 %36, 1
  %38 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %37, i64 1
  %39 = bitcast i16* %38 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull align 2 dereferenceable(596) %39, i8 0, i64 596, i1 false)
  %40 = add nuw nsw i64 %36, 2
  %41 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %40, i64 1
  %42 = bitcast i16* %41 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull align 2 dereferenceable(596) %42, i8 0, i64 596, i1 false)
  %43 = icmp eq i64 %40, 298
  br i1 %43, label %44, label %35

44:                                               ; preds = %35, %44
  %45 = phi i64 [ %60, %44 ], [ 0, %35 ]
  %46 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %45, i64 0
  %47 = bitcast i16* %46 to i8*
  %48 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %45, i64 0
  %49 = bitcast i16* %48 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 dereferenceable(600) %47, i8* nonnull align 8 dereferenceable(600) %49, i64 600, i1 false)
  %50 = add nuw nsw i64 %45, 1
  %51 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %50, i64 0
  %52 = bitcast i16* %51 to i8*
  %53 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %50, i64 0
  %54 = bitcast i16* %53 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 dereferenceable(600) %52, i8* nonnull align 8 dereferenceable(600) %54, i64 600, i1 false)
  %55 = add nuw nsw i64 %45, 2
  %56 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %55, i64 0
  %57 = bitcast i16* %56 to i8*
  %58 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %55, i64 0
  %59 = bitcast i16* %58 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 8 dereferenceable(600) %57, i8* nonnull align 8 dereferenceable(600) %59, i64 600, i1 false)
  %60 = add nuw nsw i64 %45, 3
  %61 = icmp eq i64 %60, 300
  br i1 %61, label %62, label %44

62:                                               ; preds = %44
  ret i32 0

63:                                               ; preds = %2
  %64 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799>
  %65 = add <8 x i32> %64, <i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392>
  %66 = udiv <8 x i32> %65, <i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300>
  %67 = trunc <8 x i32> %66 to <8 x i16>
  %68 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 %13
  %69 = bitcast i16* %68 to <8 x i16>*
  store <8 x i16> %67, <8 x i16>* %69, align 16, !tbaa !2
  %70 = sub nuw nsw <8 x i16> <i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799>, %67
  %71 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 %13
  %72 = bitcast i16* %71 to <8 x i16>*
  store <8 x i16> %70, <8 x i16>* %72, align 8, !tbaa !2
  %73 = add nuw nsw i64 %3, 16
  %74 = add <8 x i32> %4, <i32 16, i32 16, i32 16, i32 16, i32 16, i32 16, i32 16, i32 16>
  br label %2
}

; Function Attrs: nounwind uwtable
define dso_local i32 @updatePixels([300 x i16]* nocapture readnone %0) local_unnamed_addr #1 {
  br label %2

2:                                                ; preds = %10, %1
  %3 = phi i32 [ 0, %1 ], [ %11, %10 ]
  br label %4

4:                                                ; preds = %13, %2
  %5 = phi i64 [ 1, %2 ], [ %7, %13 ]
  %6 = add nsw i64 %5, -1
  %7 = add nuw nsw i64 %5, 1
  %8 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 1
  %9 = load i16, i16* %8, align 2, !tbaa !2
  br label %15

10:                                               ; preds = %13
  %11 = add nuw nsw i32 %3, 1
  %12 = icmp eq i32 %11, 25
  br i1 %12, label %45, label %2

13:                                               ; preds = %15
  %14 = icmp eq i64 %7, 299
  br i1 %14, label %10, label %4

15:                                               ; preds = %15, %4
  %16 = phi i16 [ %9, %4 ], [ %32, %15 ]
  %17 = phi i64 [ 1, %4 ], [ %30, %15 ]
  %18 = sext i16 %16 to i32
  %19 = sitofp i16 %16 to double
  %20 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %6, i64 %17
  %21 = load i16, i16* %20, align 2, !tbaa !2
  %22 = sext i16 %21 to i32
  %23 = add nsw i64 %17, -1
  %24 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 %23
  %25 = load i16, i16* %24, align 2, !tbaa !2
  %26 = sext i16 %25 to i32
  %27 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %7, i64 %17
  %28 = load i16, i16* %27, align 2, !tbaa !2
  %29 = sext i16 %28 to i32
  %30 = add nuw nsw i64 %17, 1
  %31 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 %30
  %32 = load i16, i16* %31, align 2, !tbaa !2
  %33 = sext i16 %32 to i32
  %34 = shl nsw i32 %18, 2
  %35 = sub nsw i32 %22, %34
  %36 = add nsw i32 %35, %26
  %37 = add nsw i32 %36, %29
  %38 = add nsw i32 %37, %33
  %39 = sitofp i32 %38 to double
  %40 = fmul double %39, 2.400000e-01
  %41 = fadd double %40, %19
  %42 = fptosi double %41 to i16
  %43 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %5, i64 %17
  store i16 %42, i16* %43, align 2, !tbaa !2
  %44 = icmp eq i64 %30, 299
  br i1 %44, label %13, label %15

45:                                               ; preds = %10, %49
  %46 = phi i64 [ %50, %49 ], [ 1, %10 ]
  %47 = trunc i64 %46 to i32
  br label %52

48:                                               ; preds = %49
  ret i32 0

49:                                               ; preds = %52
  %50 = add nuw nsw i64 %46, 1
  %51 = icmp eq i64 %50, 299
  br i1 %51, label %48, label %45

52:                                               ; preds = %52, %45
  %53 = phi i64 [ 1, %45 ], [ %59, %52 ]
  %54 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %46, i64 %53
  %55 = load i16, i16* %54, align 2, !tbaa !2
  %56 = getelementptr inbounds [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %46, i64 %53
  store i16 %55, i16* %56, align 2, !tbaa !2
  %57 = trunc i64 %53 to i32
  %58 = tail call i32 @putPixel(i32 %47, i32 %57, i16 signext %55) #4
  %59 = add nuw nsw i64 %53, 1
  %60 = icmp eq i64 %59, 299
  br i1 %60, label %49, label %52
}

declare dso_local i32 @putPixel(i32, i32, i16 signext) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #3

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #3

attributes #0 = { nofree norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind willreturn }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}
!2 = !{!3, !3, i64 0}
!3 = !{!"short", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.isvectorized", i32 1}
