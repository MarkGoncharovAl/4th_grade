; ModuleID = 'math'
source_filename = "math"

@pixels = common global [300 x [300 x i16]] zeroinitializer, align 16
@new_pixels = common global [300 x [300 x i16]] zeroinitializer, align 16

define i32 @initPixels([300 x i16]* %0) {
  br label %2

2:                                                ; preds = %63, %1
  %3 = phi i64 [ 0, %1 ], [ %73, %63 ]
  %4 = phi <8 x i32> [ <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>, %1 ], [ %74, %63 ]
  %5 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799>
  %6 = udiv <8 x i32> %5, <i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300>
  %7 = trunc <8 x i32> %6 to <8 x i16>
  %8 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 %3
  %9 = bitcast i16* %8 to <8 x i16>*
  store <8 x i16> %7, <8 x i16>* %9, align 16
  %10 = sub <8 x i16> <i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799>, %7
  %11 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 %3
  %12 = bitcast i16* %11 to <8 x i16>*
  store <8 x i16> %10, <8 x i16>* %12, align 8
  %13 = or i64 %3, 8
  %14 = icmp eq i64 %13, 296
  br i1 %14, label %15, label %63

15:                                               ; preds = %2
  store i16 12628, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i16 296), align 16
  store i16 171, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 296), align 8
  store i16 12671, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 297), align 2
  store i16 128, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 297), align 2
  store i16 12713, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 298), align 4
  store i16 86, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 298), align 4
  store i16 12756, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 299), align 2
  store i16 43, i16* getelementptr inbounds ([300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 299), align 2
  br label %16

16:                                               ; preds = %16, %15
  %17 = phi i64 [ 0, %15 ], [ %33, %16 ]
  %18 = trunc i64 %17 to i32
  %19 = mul i32 %18, 12799
  %20 = udiv i32 %19, 300
  %21 = trunc i32 %20 to i16
  %22 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %17, i64 0
  store i16 %21, i16* %22, align 16
  %23 = sub i16 12799, %21
  %24 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %17, i64 299
  store i16 %23, i16* %24, align 2
  %25 = or i64 %17, 1
  %26 = trunc i64 %25 to i32
  %27 = mul i32 %26, 12799
  %28 = udiv i32 %27, 300
  %29 = trunc i32 %28 to i16
  %30 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %25, i64 0
  store i16 %29, i16* %30, align 8
  %31 = sub i16 12799, %29
  %32 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %25, i64 299
  store i16 %31, i16* %32, align 2
  %33 = add i64 %17, 2
  %34 = icmp eq i64 %33, 300
  br i1 %34, label %35, label %16

35:                                               ; preds = %35, %16
  %36 = phi i64 [ 0, %16 ], [ %40, %35 ]
  %37 = or i64 %36, 1
  %38 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %37, i64 1
  %39 = bitcast i16* %38 to i8*
  call void @llvm.memset.p0i8.i64(i8* %39, i8 0, i64 596, i1 false)
  %40 = add i64 %36, 2
  %41 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %40, i64 1
  %42 = bitcast i16* %41 to i8*
  call void @llvm.memset.p0i8.i64(i8* %42, i8 0, i64 596, i1 false)
  %43 = icmp eq i64 %40, 298
  br i1 %43, label %44, label %35

44:                                               ; preds = %44, %35
  %45 = phi i64 [ 0, %35 ], [ %60, %44 ]
  %46 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %45, i64 0
  %47 = bitcast i16* %46 to i8*
  %48 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %45, i64 0
  %49 = bitcast i16* %48 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %47, i8* %49, i64 600, i1 false)
  %50 = add i64 %45, 1
  %51 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %50, i64 0
  %52 = bitcast i16* %51 to i8*
  %53 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %50, i64 0
  %54 = bitcast i16* %53 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %52, i8* %54, i64 600, i1 false)
  %55 = add i64 %45, 2
  %56 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %55, i64 0
  %57 = bitcast i16* %56 to i8*
  %58 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %55, i64 0
  %59 = bitcast i16* %58 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %57, i8* %59, i64 600, i1 false)
  %60 = add i64 %45, 3
  %61 = icmp eq i64 %60, 300
  br i1 %61, label %62, label %44

62:                                               ; preds = %44
  ret i32 0

63:                                               ; preds = %2
  %64 = mul <8 x i32> %4, <i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799, i32 12799>
  %65 = add <8 x i32> %64, <i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392, i32 102392>
  %66 = udiv <8 x i32> %65, <i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300, i32 300>
  %67 = trunc <8 x i32> %66 to <8 x i16>
  %68 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 0, i64 %13
  %69 = bitcast i16* %68 to <8 x i16>*
  store <8 x i16> %67, <8 x i16>* %69, align 16
  %70 = sub <8 x i16> <i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799, i16 12799>, %67
  %71 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 299, i64 %13
  %72 = bitcast i16* %71 to <8 x i16>*
  store <8 x i16> %70, <8 x i16>* %72, align 8
  %73 = add i64 %3, 16
  %74 = add <8 x i32> %4, <i32 16, i32 16, i32 16, i32 16, i32 16, i32 16, i32 16, i32 16>
  br label %2
}

; Function Attrs: nounwind uwtable
define i32 @updatePixels([300 x i16]* %0) {
  br label %2

2:                                                ; preds = %10, %1
  %3 = phi i32 [ 0, %1 ], [ %11, %10 ]
  br label %4

4:                                                ; preds = %13, %2
  %5 = phi i64 [ 1, %2 ], [ %7, %13 ]
  %6 = add i64 %5, -1
  %7 = add i64 %5, 1
  %8 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 1
  %9 = load i16, i16* %8, align 2
  br label %15

10:                                               ; preds = %13
  %11 = add i32 %3, 1
  %12 = icmp eq i32 %11, 25
  br i1 %12, label %43, label %2

13:                                               ; preds = %15
  %14 = icmp eq i64 %7, 299
  br i1 %14, label %10, label %4

15:                                               ; preds = %15, %4
  %16 = phi i16 [ %9, %4 ], [ %31, %15 ]
  %17 = phi i64 [ 1, %4 ], [ %29, %15 ]
  %18 = sext i16 %16 to i32
  %19 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %6, i64 %17
  %20 = load i16, i16* %19, align 2
  %21 = sext i16 %20 to i32
  %22 = add i64 %17, -1
  %23 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 %22
  %24 = load i16, i16* %23, align 2
  %25 = sext i16 %24 to i32
  %26 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %7, i64 %17
  %27 = load i16, i16* %26, align 2
  %28 = sext i16 %27 to i32
  %29 = add i64 %17, 1
  %30 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %5, i64 %29
  %31 = load i16, i16* %30, align 2
  %32 = sext i16 %31 to i32
  %33 = shl i32 %18, 2
  %34 = sub i32 %21, %33
  %35 = add i32 %34, %25
  %36 = add i32 %35, %28
  %37 = add i32 %36, %32
  %38 = sdiv i32 %37, 5
  %39 = trunc i32 %38 to i16
  %40 = add i16 %16, %39
  %41 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %5, i64 %17
  store i16 %40, i16* %41, align 2
  %42 = icmp eq i64 %29, 299
  br i1 %42, label %13, label %15

43:                                               ; preds = %10, %47
  %44 = phi i64 [ %48, %47 ], [ 1, %10 ]
  %45 = trunc i64 %44 to i32
  br label %50

46:                                               ; preds = %47
  ret i32 0

47:                                               ; preds = %50
  %48 = add i64 %44, 1
  %49 = icmp eq i64 %48, 299
  br i1 %49, label %46, label %43

50:                                               ; preds = %50, %43
  %51 = phi i64 [ 1, %43 ], [ %57, %50 ]
  %52 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @new_pixels, i64 0, i64 %44, i64 %51
  %53 = load i16, i16* %52, align 2
  %54 = getelementptr [300 x [300 x i16]], [300 x [300 x i16]]* @pixels, i64 0, i64 %44, i64 %51
  store i16 %53, i16* %54, align 2
  %55 = trunc i64 %51 to i32
  %56 = tail call i32 @putPixel(i32 %45, i32 %55, i16 signext %53)
  %57 = add i64 %51, 1
  %58 = icmp eq i64 %57, 299
  br i1 %58, label %47, label %50
}

declare i32 @putPixel(i32, i32, i16 signext)

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nounwind willreturn }