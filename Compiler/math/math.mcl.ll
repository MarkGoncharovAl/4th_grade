; ModuleID = 'pcl.module'
source_filename = "pcl.module"

@.int_string = internal global [4 x i8] c"%d\0A\00", align 1
@.int_string_scan = internal global [3 x i8] c"%d\00", align 1

declare i32 @printf(...)

define void @__print_int(i32 %0) {
  %2 = call i32 (...) @printf([4 x i8]* @.int_string, i32 %0)
  ret void
}

declare i32 @__isoc99_scanf(...)

define i32 @__scan_int() {
  %1 = alloca i32, align 4
  %2 = call i32 (...) @__isoc99_scanf([3 x i8]* @.int_string_scan, i32* %1)
  %3 = load i32, i32* %1, align 4
  ret i32 %3
}

declare i32 @putPixel(i32, i32, i32)

@pixels = common global [300 x [300 x i16]] zeroinitializer, align 16

define i32 @initPixels() {
  %SzH = alloca i32, align 4
  %SzH1 = load i32, i32* %SzH, align 4
  store i32 300, i32* %SzH, align 4
  %ColorPixel = alloca i32, align 4
  %ColorPixel2 = load i32, i32* %ColorPixel, align 4
  store i32 2560, i32* %ColorPixel, align 4
  %MaxColor = alloca i32, align 4
  %MaxColor3 = load i32, i32* %MaxColor, align 4
  %ColorPixel4 = load i32, i32* %ColorPixel, align 4
  %1 = mul i32 %ColorPixel4, 5
  %2 = sub i32 %1, 1
  store i32 %2, i32* %MaxColor, align 4
  %idx = alloca i32, align 4
  %idx5 = load i32, i32* %idx, align 4
  store i32 0, i32* %idx, align 4
  br label %3

3:                                                ; preds = %5, %0
  %idx6 = load i32, i32* %idx, align 4
  %SzH7 = load i32, i32* %SzH, align 4
  %4 = icmp slt i32 %idx6, %SzH7
  br i1 %4, label %5, label %17

5:                                                ; preds = %3
  %idx8 = load i32, i32* %idx, align 4
  %MaxColor9 = load i32, i32* %MaxColor, align 4
  %idx10 = load i32, i32* %idx, align 4
  %6 = mul i32 %MaxColor9, %idx10
  %SzH11 = load i32, i32* %SzH, align 4
  %7 = sdiv i32 %6, %SzH11
  %8 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %idx8
  store i32 %7, i32* %8, align 4
  %SzH12 = load i32, i32* %SzH, align 4
  %SzH13 = load i32, i32* %SzH, align 4
  %9 = sub i32 %SzH13, 1
  %10 = mul i32 %SzH12, %9
  %idx14 = load i32, i32* %idx, align 4
  %11 = add i32 %10, %idx14
  %MaxColor15 = load i32, i32* %MaxColor, align 4
  %MaxColor16 = load i32, i32* %MaxColor, align 4
  %idx17 = load i32, i32* %idx, align 4
  %12 = mul i32 %MaxColor16, %idx17
  %SzH18 = load i32, i32* %SzH, align 4
  %13 = sdiv i32 %12, %SzH18
  %14 = sub i32 %MaxColor15, %13
  %15 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %11
  store i32 %14, i32* %15, align 4
  %idx19 = load i32, i32* %idx, align 4
  %idx20 = load i32, i32* %idx, align 4
  %16 = add i32 %idx20, 1
  store i32 %16, i32* %idx, align 4
  br label %3

17:                                               ; preds = %3
  %idx21 = load i32, i32* %idx, align 4
  store i32 0, i32* %idx, align 4
  br label %18

18:                                               ; preds = %20, %17
  %idx22 = load i32, i32* %idx, align 4
  %SzH23 = load i32, i32* %SzH, align 4
  %19 = icmp slt i32 %idx22, %SzH23
  br i1 %19, label %20, label %33

20:                                               ; preds = %18
  %SzH24 = load i32, i32* %SzH, align 4
  %idx25 = load i32, i32* %idx, align 4
  %21 = mul i32 %SzH24, %idx25
  %MaxColor26 = load i32, i32* %MaxColor, align 4
  %idx27 = load i32, i32* %idx, align 4
  %22 = mul i32 %MaxColor26, %idx27
  %SzH28 = load i32, i32* %SzH, align 4
  %23 = sdiv i32 %22, %SzH28
  %24 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %21
  store i32 %23, i32* %24, align 4
  %SzH29 = load i32, i32* %SzH, align 4
  %25 = sub i32 %SzH29, 1
  %SzH30 = load i32, i32* %SzH, align 4
  %idx31 = load i32, i32* %idx, align 4
  %26 = mul i32 %SzH30, %idx31
  %27 = add i32 %25, %26
  %MaxColor32 = load i32, i32* %MaxColor, align 4
  %MaxColor33 = load i32, i32* %MaxColor, align 4
  %idx34 = load i32, i32* %idx, align 4
  %28 = mul i32 %MaxColor33, %idx34
  %SzH35 = load i32, i32* %SzH, align 4
  %29 = sdiv i32 %28, %SzH35
  %30 = sub i32 %MaxColor32, %29
  %31 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %27
  store i32 %30, i32* %31, align 4
  %idx36 = load i32, i32* %idx, align 4
  %idx37 = load i32, i32* %idx, align 4
  %32 = add i32 %idx37, 1
  store i32 %32, i32* %idx, align 4
  br label %18

33:                                               ; preds = %18
  %i = alloca i32, align 4
  %i38 = load i32, i32* %i, align 4
  store i32 1, i32* %i, align 4
  %j = alloca i32, align 4
  %j39 = load i32, i32* %j, align 4
  store i32 1, i32* %j, align 4
  br label %34

34:                                               ; preds = %33
  %i40 = load i32, i32* %i, align 4
  %SzH41 = load i32, i32* %SzH, align 4
  %35 = sub i32 %SzH41, 1
  %36 = icmp slt i32 %i40, %35
  br i1 %36, label %37, label %38

37:                                               ; preds = %34
  br label %38

38:                                               ; preds = %42, %37
  %j42 = load i32, i32* %j, align 4
  %SzH43 = load i32, i32* %SzH, align 4
  %40 = sub i32 %SzH43, 1
  %41 = icmp slt i32 %j42, %40
  br i1 %41, label %42, label %47

42:                                               ; preds = %39
  %SzH44 = load i32, i32* %SzH, align 4
  %i45 = load i32, i32* %i, align 4
  %43 = mul i32 %SzH44, %i45
  %j46 = load i32, i32* %j, align 4
  %44 = add i32 %43, %j46
  %45 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %44
  store i32 0, i32* %45, align 4
  %j47 = load i32, i32* %j, align 4
  %j48 = load i32, i32* %j, align 4
  %46 = add i32 %j48, 1
  store i32 %46, i32* %j, align 4
  br label %39

47:                                               ; preds = %39
  %i49 = load i32, i32* %i, align 4
  %i50 = load i32, i32* %i, align 4
  %48 = add i32 %i50, 1
  store i32 %48, i32* %i, align 4
}

define i32 @updatePixels() {
  %SzH = alloca i32, align 4
  %SzH1 = load i32, i32* %SzH, align 4
  store i32 300, i32* %SzH, align 4
  %ColorPixel = alloca i32, align 4
  %ColorPixel2 = load i32, i32* %ColorPixel, align 4
  store i32 2560, i32* %ColorPixel, align 4
  %Speed = alloca i32, align 4
  %Speed3 = load i32, i32* %Speed, align 4
  store i32 5, i32* %Speed, align 4
  %MaxColor = alloca i32, align 4
  %MaxColor4 = load i32, i32* %MaxColor, align 4
  %ColorPixel5 = load i32, i32* %ColorPixel, align 4
  %1 = mul i32 %ColorPixel5, 5
  %2 = sub i32 %1, 1
  store i32 %2, i32* %MaxColor, align 4
  %i = alloca i32, align 4
  %i6 = load i32, i32* %i, align 4
  store i32 1, i32* %i, align 4
  %j = alloca i32, align 4
  %j7 = load i32, i32* %j, align 4
  store i32 1, i32* %j, align 4
  br label %3

3:                                                ; preds = %0
  %i8 = load i32, i32* %i, align 4
  %SzH9 = load i32, i32* %SzH, align 4
  %4 = sub i32 %SzH9, 1
  %5 = icmp slt i32 %i8, %4
  br i1 %5, label %6, label %7

6:                                                ; preds = %3
  br label %8

7:                                                ; preds = %3
  %i46 = load i32, i32* %i, align 4
  store i32 1, i32* %i, align 4
  %j47 = load i32, i32* %j, align 4
  store i32 1, i32* %j, align 4
  br label %49

8:                                                ; preds = %11, %6
  %j10 = load i32, i32* %j, align 4
  %SzH11 = load i32, i32* %SzH, align 4
  %9 = sub i32 %SzH11, 1
  %10 = icmp slt i32 %j10, %9
  br i1 %10, label %11, label %47

11:                                               ; preds = %8
  %p00 = alloca i32, align 4
  %p0012 = load i32, i32* %p00, align 4
  %i13 = load i32, i32* %i, align 4
  %SzH14 = load i32, i32* %SzH, align 4
  %12 = mul i32 %i13, %SzH14
  %j15 = load i32, i32* %j, align 4
  %13 = add i32 %12, %j15
  %14 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %13
  %15 = load i32, i32* %14, align 4
  store i32 %15, i32* %p00, align 4
  %p_10 = alloca i32, align 4
  %p_1016 = load i32, i32* %p_10, align 4
  %i17 = load i32, i32* %i, align 4
  %16 = sub i32 %i17, 1
  %SzH18 = load i32, i32* %SzH, align 4
  %17 = mul i32 %16, %SzH18
  %j19 = load i32, i32* %j, align 4
  %18 = add i32 %17, %j19
  %19 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %18
  %20 = load i32, i32* %19, align 4
  store i32 %20, i32* %p_10, align 4
  %p0_1 = alloca i32, align 4
  %p0_120 = load i32, i32* %p0_1, align 4
  %i21 = load i32, i32* %i, align 4
  %SzH22 = load i32, i32* %SzH, align 4
  %21 = mul i32 %i21, %SzH22
  %j23 = load i32, i32* %j, align 4
  %22 = add i32 %21, %j23
  %23 = sub i32 %22, 1
  %24 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %23
  %25 = load i32, i32* %24, align 4
  store i32 %25, i32* %p0_1, align 4
  %p10 = alloca i32, align 4
  %p1024 = load i32, i32* %p10, align 4
  %i25 = load i32, i32* %i, align 4
  %26 = add i32 %i25, 1
  %SzH26 = load i32, i32* %SzH, align 4
  %27 = mul i32 %26, %SzH26
  %j27 = load i32, i32* %j, align 4
  %28 = add i32 %27, %j27
  %29 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %28
  %30 = load i32, i32* %29, align 4
  store i32 %30, i32* %p10, align 4
  %p01 = alloca i32, align 4
  %p0128 = load i32, i32* %p01, align 4
  %i29 = load i32, i32* %i, align 4
  %SzH30 = load i32, i32* %SzH, align 4
  %31 = mul i32 %i29, %SzH30
  %j31 = load i32, i32* %j, align 4
  %32 = add i32 %31, %j31
  %33 = add i32 %32, 1
  %34 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %33
  %35 = load i32, i32* %34, align 4
  store i32 %35, i32* %p01, align 4
  %SzH32 = load i32, i32* %SzH, align 4
  %i33 = load i32, i32* %i, align 4
  %36 = mul i32 %SzH32, %i33
  %j34 = load i32, i32* %j, align 4
  %37 = add i32 %36, %j34
  %p0035 = load i32, i32* %p00, align 4
  %p_1036 = load i32, i32* %p_10, align 4
  %p0_137 = load i32, i32* %p0_1, align 4
  %38 = add i32 %p_1036, %p0_137
  %p1038 = load i32, i32* %p10, align 4
  %39 = add i32 %38, %p1038
  %p0139 = load i32, i32* %p01, align 4
  %40 = add i32 %39, %p0139
  %p0040 = load i32, i32* %p00, align 4
  %41 = mul i32 4, %p0040
  %42 = sub i32 %40, %41
  %Speed41 = load i32, i32* %Speed, align 4
  %43 = sdiv i32 %42, %Speed41
  %44 = add i32 %p0035, %43
  %45 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %37
  store i32 %44, i32* %45, align 4
  %j42 = load i32, i32* %j, align 4
  %j43 = load i32, i32* %j, align 4
  %46 = add i32 %j43, 1
  store i32 %46, i32* %j, align 4
  br label %8

47:                                               ; preds = %8
  %i44 = load i32, i32* %i, align 4
  %i45 = load i32, i32* %i, align 4
  %48 = add i32 %i45, 1
  store i32 %48, i32* %i, align 4

49:                                               ; preds = %7
  %i48 = load i32, i32* %i, align 4
  %SzH49 = load i32, i32* %SzH, align 4
  %50 = sub i32 %SzH49, 1
  %51 = icmp slt i32 %i48, %50
  br i1 %51, label %52, label %53

52:                                               ; preds = %49
  br label %54

53:                                               ; preds = %49

54:                                               ; preds = %57, %52
  %j50 = load i32, i32* %j, align 4
  %SzH51 = load i32, i32* %SzH, align 4
  %55 = sub i32 %SzH51, 1
  %56 = icmp slt i32 %j50, %55
  br i1 %56, label %57, label %64

57:                                               ; preds = %54
  %tmp = alloca i32, align 4
  %tmp52 = load i32, i32* %tmp, align 4
  %i53 = load i32, i32* %i, align 4
  %j54 = load i32, i32* %j, align 4
  %i55 = load i32, i32* %i, align 4
  %SzH56 = load i32, i32* %SzH, align 4
  %58 = mul i32 %i55, %SzH56
  %j57 = load i32, i32* %j, align 4
  %59 = add i32 %58, %j57
  %60 = getelementptr [90000 x i32], [90000 x i32]* %pixels, i32 0, i32 %59
  %61 = load i32, i32* %60, align 4
  %62 = call i32 @putPixel(i32 %i53, i32 %j54, i32 %61)
  store i32 %62, i32* %tmp, align 4
  %j58 = load i32, i32* %j, align 4
  %j59 = load i32, i32* %j, align 4
  %63 = add i32 %j59, 1
  store i32 %63, i32* %j, align 4
  br label %54

64:                                               ; preds = %54
  %i60 = load i32, i32* %i, align 4
  %i61 = load i32, i32* %i, align 4
  %65 = add i32 %i61, 1
  store i32 %65, i32* %i, align 4
}
