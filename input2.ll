; ModuleID = 'input2.cpp'
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %supernumber = alloca i32*, align 8
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 8
  %4 = call noalias i8* @_Znwy(i64 4) #2
  %5 = bitcast i8* %4 to i32*
  store i32 0, i32* %5, align 4
  store i32* %5, i32** %supernumber, align 8
  %6 = load i32*, i32** %supernumber, align 8
  store i32 64, i32* %6, align 4
  %7 = load i32, i32* %2, align 4
  %8 = icmp sle i32 %7, 2
  br i1 %8, label %9, label %15

; <label>:9                                       ; preds = %0
  %10 = load i32*, i32** %supernumber, align 8
  %11 = load i32, i32* %10, align 4
  %12 = load i32, i32* %2, align 4
  %13 = add nsw i32 %11, %12
  %14 = load i32*, i32** %supernumber, align 8
  store i32 %13, i32* %14, align 4
  br label %21

; <label>:15                                      ; preds = %0
  %16 = load i32*, i32** %supernumber, align 8
  %17 = load i32, i32* %16, align 4
  %18 = load i32, i32* %2, align 4
  %19 = sub nsw i32 %17, %18
  %20 = load i32*, i32** %supernumber, align 8
  store i32 %19, i32* %20, align 4
  br label %21

; <label>:21                                      ; preds = %15, %9
  %22 = load i32*, i32** %supernumber, align 8
  %23 = load i32, i32* %22, align 4
  ret i32 %23
}

; Function Attrs: nobuiltin
declare noalias i8* @_Znwy(i64) #1

attributes #0 = { uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { builtin }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"PIC Level", i32 2}
!1 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
