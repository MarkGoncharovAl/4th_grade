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
  %x = alloca i32, align 4
  %x1 = load i32, i32* %x, align 4
  store i32 5, i32* %x, align 4
  %x2 = load i32, i32* %x, align 4
  %x3 = load i32, i32* %x, align 4
  %1 = add i32 %x3, 1
  store i32 %1, i32* %x, align 4
  %y = alloca i32, align 4
  %y4 = load i32, i32* %y, align 4
  %x5 = load i32, i32* %x, align 4
  %2 = sub i32 %x5, 1
  store i32 %2, i32* %y, align 4
  %y6 = load i32, i32* %y, align 4
  call void @__print_int(i32 %y6)
  ret i32 0
}
