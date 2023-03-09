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

define i32 @main() {
  %arr = alloca [7 x i32], align 4
  %x = alloca i32, align 4
  %x1 = load i32, i32* %x, align 4
  store i32 0, i32* %x, align 4
  br label %1

1:                                                ; preds = %3, %0
  %x2 = load i32, i32* %x, align 4
  %2 = icmp slt i32 %x2, 7
  br i1 %2, label %3, label %7

3:                                                ; preds = %1
  %x3 = load i32, i32* %x, align 4
  %x4 = load i32, i32* %x, align 4
  %4 = add i32 %x4, 1
  %5 = getelementptr [7 x i32], [7 x i32]* %arr, i32 0, i32 %x3
  store i32 %4, i32* %5, align 4
  %x5 = load i32, i32* %x, align 4
  %x6 = load i32, i32* %x, align 4
  %6 = add i32 %x6, 1
  store i32 %6, i32* %x, align 4
  br label %1

7:                                                ; preds = %1
  %x7 = load i32, i32* %x, align 4
  store i32 0, i32* %x, align 4
  br label %8

8:                                                ; preds = %10, %7
  %x8 = load i32, i32* %x, align 4
  %9 = icmp slt i32 %x8, 7
  br i1 %9, label %10, label %14

10:                                               ; preds = %8
  %x9 = load i32, i32* %x, align 4
  %11 = getelementptr [7 x i32], [7 x i32]* %arr, i32 0, i32 %x9
  %12 = load i32, i32* %11, align 4
  call void @__print_int(i32 %12)
  %x10 = load i32, i32* %x, align 4
  %x11 = load i32, i32* %x, align 4
  %13 = add i32 %x11, 1
  store i32 %13, i32* %x, align 4
  br label %8

14:                                               ; preds = %8
  ret i32 0
}
