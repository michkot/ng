; ModuleID = 'input.cpp'
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: nounwind uwtable
define void @_Z4fce2v() #0 {
  %x = alloca i32, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %supernumber = alloca i32, align 4
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 8
  store i32 1, i32* %supernumber, align 4
  %4 = load i32, i32* %supernumber, align 4
  %5 = load i32, i32* %2, align 4
  %6 = add nsw i32 %4, %5
  store i32 %6, i32* %supernumber, align 4
  %7 = load i32, i32* %supernumber, align 4
  %8 = icmp sle i32 %7, 2
  br i1 %8, label %9, label %12

; <label>:9                                       ; preds = %0
  %10 = load i32, i32* %supernumber, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* %supernumber, align 4
  br label %13

; <label>:12                                      ; preds = %0
  call void @_Z4fce2v()
  br label %13

; <label>:13                                      ; preds = %12, %9
  %14 = load i32, i32* %supernumber, align 4
  ret i32 %14
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
