; ModuleID = 'input.cpp'
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: nounwind uwtable
define void @_Z4fce2v() #0 {
  %x = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %x, metadata !32, metadata !33), !dbg !34
  ret void, !dbg !35
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %supernumber = alloca i32, align 4
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !36, metadata !33), !dbg !37
  store i8** %argv, i8*** %3, align 8
  call void @llvm.dbg.declare(metadata i8*** %3, metadata !38, metadata !33), !dbg !39
  call void @llvm.dbg.declare(metadata i32* %supernumber, metadata !40, metadata !33), !dbg !41
  store i32 1, i32* %supernumber, align 4, !dbg !41
  %4 = load i32, i32* %supernumber, align 4, !dbg !42
  %5 = load i32, i32* %2, align 4, !dbg !43
  %6 = add nsw i32 %4, %5, !dbg !44
  store i32 %6, i32* %supernumber, align 4, !dbg !45
  %7 = load i32, i32* %supernumber, align 4, !dbg !46
  %8 = icmp sle i32 %7, 2, !dbg !48
  br i1 %8, label %9, label %12, !dbg !49

; <label>:9                                       ; preds = %0
  %10 = load i32, i32* %supernumber, align 4, !dbg !50
  %11 = add nsw i32 %10, 1, !dbg !50
  store i32 %11, i32* %supernumber, align 4, !dbg !50
  br label %13, !dbg !51

; <label>:12                                      ; preds = %0
  call void @_Z4fce2v(), !dbg !52
  br label %13

; <label>:13                                      ; preds = %12, %9
  %14 = load i32, i32* %supernumber, align 4, !dbg !53
  ret i32 %14, !dbg !54
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!28, !29, !30}
!llvm.ident = !{!31}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !1, producer: "clang version 3.7.0 (tags/RELEASE_370/final)", isOptimized: false, runtimeVersion: 0, emissionKind: 1, enums: !2, subprograms: !3, imports: !14)
!1 = !DIFile(filename: "input.cpp", directory: "C:\5CPrograms\5CPortableGit\5CGIT\5CFIT\5Cbordel\5Cbordel1")
!2 = !{}
!3 = !{!4, !7}
!4 = !DISubprogram(name: "fce2", linkageName: "_Z4fce2v", scope: !1, file: !1, line: 3, type: !5, isLocal: false, isDefinition: true, scopeLine: 3, flags: DIFlagPrototyped, isOptimized: false, function: void ()* @_Z4fce2v, variables: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{null}
!7 = !DISubprogram(name: "main", scope: !1, file: !1, line: 7, type: !8, isLocal: false, isDefinition: true, scopeLine: 7, flags: DIFlagPrototyped, isOptimized: false, function: i32 (i32, i8**)* @main, variables: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !11}
!10 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!11 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !12, size: 64, align: 64)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64, align: 64)
!13 = !DIBasicType(name: "char", size: 8, align: 8, encoding: DW_ATE_signed_char)
!14 = !{!15, !21, !25}
!15 = !DIImportedEntity(tag: DW_TAG_imported_module, scope: !16, entity: !18, line: 56)
!16 = !DINamespace(name: "__gnu_debug", scope: null, file: !17, line: 54)
!17 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cinclude\5Cc++\5C5.3.0\5Cdebug/debug.h", directory: "C:\5CPrograms\5CPortableGit\5CGIT\5CFIT\5Cbordel\5Cbordel1")
!18 = !DINamespace(name: "__debug", scope: !19, file: !17, line: 48)
!19 = !DINamespace(name: "std", scope: null, file: !20, line: 194)
!20 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cinclude\5Cc++\5C5.3.0\5Cx86_64-w64-mingw32\5Cbits/c++config.h", directory: "C:\5CPrograms\5CPortableGit\5CGIT\5CFIT\5Cbordel\5Cbordel1")
!21 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !22, entity: !23, line: 44)
!22 = !DINamespace(name: "__gnu_cxx", scope: null, file: !20, line: 220)
!23 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", scope: !19, file: !20, line: 196, baseType: !24)
!24 = !DIBasicType(name: "long long unsigned int", size: 64, align: 64, encoding: DW_ATE_unsigned)
!25 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !22, entity: !26, line: 45)
!26 = !DIDerivedType(tag: DW_TAG_typedef, name: "ptrdiff_t", scope: !19, file: !20, line: 197, baseType: !27)
!27 = !DIBasicType(name: "long long int", size: 64, align: 64, encoding: DW_ATE_signed)
!28 = !{i32 2, !"Dwarf Version", i32 4}
!29 = !{i32 2, !"Debug Info Version", i32 3}
!30 = !{i32 1, !"PIC Level", i32 2}
!31 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
!32 = !DILocalVariable(tag: DW_TAG_auto_variable, name: "x", scope: !4, file: !1, line: 4, type: !10)
!33 = !DIExpression()
!34 = !DILocation(line: 4, column: 9, scope: !4)
!35 = !DILocation(line: 5, column: 1, scope: !4)
!36 = !DILocalVariable(tag: DW_TAG_arg_variable, name: "argc", arg: 1, scope: !7, file: !1, line: 7, type: !10)
!37 = !DILocation(line: 7, column: 14, scope: !7)
!38 = !DILocalVariable(tag: DW_TAG_arg_variable, name: "argv", arg: 2, scope: !7, file: !1, line: 7, type: !11)
!39 = !DILocation(line: 7, column: 27, scope: !7)
!40 = !DILocalVariable(tag: DW_TAG_auto_variable, name: "supernumber", scope: !7, file: !1, line: 8, type: !10)
!41 = !DILocation(line: 8, column: 9, scope: !7)
!42 = !DILocation(line: 9, column: 19, scope: !7)
!43 = !DILocation(line: 9, column: 33, scope: !7)
!44 = !DILocation(line: 9, column: 31, scope: !7)
!45 = !DILocation(line: 9, column: 17, scope: !7)
!46 = !DILocation(line: 10, column: 8, scope: !47)
!47 = distinct !DILexicalBlock(scope: !7, file: !1, line: 10, column: 8)
!48 = !DILocation(line: 10, column: 20, scope: !47)
!49 = !DILocation(line: 10, column: 8, scope: !7)
!50 = !DILocation(line: 11, column: 20, scope: !47)
!51 = !DILocation(line: 11, column: 9, scope: !47)
!52 = !DILocation(line: 13, column: 9, scope: !47)
!53 = !DILocation(line: 14, column: 12, scope: !7)
!54 = !DILocation(line: 14, column: 5, scope: !7)
