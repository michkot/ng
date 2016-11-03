; ModuleID = '<stdin>'
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: norecurse nounwind uwtable
define i32 @main() #0 !dbg !17 {
  %1 = alloca i32, align 4
  %int32 = alloca i32, align 4
  %uint32 = alloca i32, align 4
  %int64 = alloca i64, align 8
  %uint64 = alloca i64, align 8
  %a = alloca i64, align 8
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %f = alloca i32, align 4
  %g = alloca i64, align 8
  %h = alloca i64, align 8
  %i = alloca i64, align 8
  %j = alloca i64, align 8
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata i32* %int32, metadata !291, metadata !292), !dbg !293
  store i32 -1, i32* %int32, align 4, !dbg !293
  call void @llvm.dbg.declare(metadata i32* %uint32, metadata !294, metadata !292), !dbg !295
  store i32 -1, i32* %uint32, align 4, !dbg !295
  call void @llvm.dbg.declare(metadata i64* %int64, metadata !296, metadata !292), !dbg !297
  store i64 -1, i64* %int64, align 8, !dbg !297
  call void @llvm.dbg.declare(metadata i64* %uint64, metadata !298, metadata !292), !dbg !299
  store i64 -1, i64* %uint64, align 8, !dbg !299

  ;long long a = int32 + uint32;
  call void @llvm.dbg.declare(metadata i64* %a, metadata !300, metadata !292), !dbg !301
  %2 = load i32, i32* %int32, align 4, !dbg !302
  %3 = load i32, i32* %uint32, align 4, !dbg !303
  %4 = add i32 %2, %3, !dbg !304
  %5 = zext i32 %4 to i64, !dbg !302
  store i64 %5, i64* %a, align 8, !dbg !301

  ;int b = int32 + uint32;
  call void @llvm.dbg.declare(metadata i32* %b, metadata !305, metadata !292), !dbg !306
  %6 = load i32, i32* %int32, align 4, !dbg !307
  %7 = load i32, i32* %uint32, align 4, !dbg !308
  %8 = add i32 %6, %7, !dbg !309
  store i32 %8, i32* %b, align 4, !dbg !306

  ;int c = int64 + uint32;
  call void @llvm.dbg.declare(metadata i32* %c, metadata !310, metadata !292), !dbg !311
  %9 = load i64, i64* %int64, align 8, !dbg !312
  %10 = load i32, i32* %uint32, align 4, !dbg !313
  %11 = zext i32 %10 to i64, !dbg !313
  %12 = add nsw i64 %9, %11, !dbg !314
  %13 = trunc i64 %12 to i32, !dbg !312
  store i32 %13, i32* %c, align 4, !dbg !311

  ;int d = int32 + uint64;
  call void @llvm.dbg.declare(metadata i32* %d, metadata !315, metadata !292), !dbg !316
  %14 = load i32, i32* %int32, align 4, !dbg !317
  %15 = sext i32 %14 to i64, !dbg !317
  %16 = load i64, i64* %uint64, align 8, !dbg !318
  %17 = add i64 %15, %16, !dbg !319
  %18 = trunc i64 %17 to i32, !dbg !317
  store i32 %18, i32* %d, align 4, !dbg !316

  ;unsigned e = int64 + uint32;
  call void @llvm.dbg.declare(metadata i32* %e, metadata !320, metadata !292), !dbg !321
  %19 = load i64, i64* %int64, align 8, !dbg !322
  %20 = load i32, i32* %uint32, align 4, !dbg !323
  %21 = zext i32 %20 to i64, !dbg !323
  %22 = add nsw i64 %19, %21, !dbg !324
  %23 = trunc i64 %22 to i32, !dbg !322
  store i32 %23, i32* %e, align 4, !dbg !321

  ;unsigned f = int32 + uint64;
  call void @llvm.dbg.declare(metadata i32* %f, metadata !325, metadata !292), !dbg !326
  %24 = load i32, i32* %int32, align 4, !dbg !327
  %25 = sext i32 %24 to i64, !dbg !327
  %26 = load i64, i64* %uint64, align 8, !dbg !328
  %27 = add i64 %25, %26, !dbg !329
  %28 = trunc i64 %27 to i32, !dbg !327
  store i32 %28, i32* %f, align 4, !dbg !326

  ;long long g = int64 + uint32;
  call void @llvm.dbg.declare(metadata i64* %g, metadata !330, metadata !292), !dbg !331
  %29 = load i64, i64* %int64, align 8, !dbg !332
  %30 = load i32, i32* %uint32, align 4, !dbg !333
  %31 = zext i32 %30 to i64, !dbg !333
  %32 = add nsw i64 %29, %31, !dbg !334
  store i64 %32, i64* %g, align 8, !dbg !331
  
  ;long long h = int32 + uint64;
  call void @llvm.dbg.declare(metadata i64* %h, metadata !335, metadata !292), !dbg !336
  %33 = load i32, i32* %int32, align 4, !dbg !337
  %34 = sext i32 %33 to i64, !dbg !337
  %35 = load i64, i64* %uint64, align 8, !dbg !338
  %36 = add i64 %34, %35, !dbg !339
  store i64 %36, i64* %h, align 8, !dbg !336
  
  ;unsigned long long i = int64 + uint32;
  call void @llvm.dbg.declare(metadata i64* %i, metadata !340, metadata !292), !dbg !341
  %37 = load i64, i64* %int64, align 8, !dbg !342
  %38 = load i32, i32* %uint32, align 4, !dbg !343
  %39 = zext i32 %38 to i64, !dbg !343
  %40 = add nsw i64 %37, %39, !dbg !344
  store i64 %40, i64* %i, align 8, !dbg !341

  ;unsigned long long j = int32 + uint64;
  call void @llvm.dbg.declare(metadata i64* %j, metadata !345, metadata !292), !dbg !346
  %41 = load i32, i32* %int32, align 4, !dbg !347
  %42 = sext i32 %41 to i64, !dbg !347
  %43 = load i64, i64* %uint64, align 8, !dbg !348
  %44 = add i64 %42, %43, !dbg !349
  store i64 %44, i64* %j, align 8, !dbg !346
  ret i32 0, !dbg !350
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { norecurse nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!287, !288, !289}
!llvm.ident = !{!290}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !1, producer: "clang version 3.8.0 (tags/RELEASE_380/final)", isOptimized: false, runtimeVersion: 0, emissionKind: 1, enums: !2, retainedTypes: !3, subprograms: !16, imports: !21)
!1 = !DIFile(filename: "input-int-conv.cpp", directory: "C:\5CApps\5CGIT\5Cng")
!2 = !{}
!3 = !{!4, !6, !11}
!4 = !DICompositeType(tag: DW_TAG_structure_type, name: "_div_t", file: !5, line: 59, size: 64, align: 32, flags: DIFlagFwdDecl, identifier: "_ZTS6_div_t")
!5 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cx86_64-w64-mingw32\5Cinclude\5Cstdlib.h", directory: "C:\5CApps\5CGIT\5Cng")
!6 = !DICompositeType(tag: DW_TAG_structure_type, name: "_ldiv_t", file: !5, line: 64, size: 64, align: 32, elements: !7, identifier: "_ZTS7_ldiv_t")
!7 = !{!8, !10}
!8 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !"_ZTS7_ldiv_t", file: !5, line: 65, baseType: !9, size: 32, align: 32)
!9 = !DIBasicType(name: "long int", size: 32, align: 32, encoding: DW_ATE_signed)
!10 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !"_ZTS7_ldiv_t", file: !5, line: 66, baseType: !9, size: 32, align: 32, offset: 32)
!11 = !DICompositeType(tag: DW_TAG_structure_type, file: !5, line: 653, size: 128, align: 64, elements: !12, identifier: "_ZTS7lldiv_t")
!12 = !{!13, !15}
!13 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !"_ZTS7lldiv_t", file: !5, line: 653, baseType: !14, size: 64, align: 64)
!14 = !DIBasicType(name: "long long int", size: 64, align: 64, encoding: DW_ATE_signed)
!15 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !"_ZTS7lldiv_t", file: !5, line: 653, baseType: !14, size: 64, align: 64, offset: 64)
!16 = !{!17}
!17 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 4, type: !18, isLocal: false, isDefinition: true, scopeLine: 5, flags: DIFlagPrototyped, isOptimized: false, variables: !2)
!18 = !DISubroutineType(types: !19)
!19 = !{!20}
!20 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!21 = !{!22, !26, !28, !32, !36, !41, !49, !53, !57, !70, !74, !78, !82, !86, !91, !95, !99, !103, !107, !115, !119, !123, !125, !129, !134, !140, !144, !149, !151, !159, !163, !166, !168, !172, !176, !180, !184, !188, !193, !198, !199, !200, !201, !204, !205, !206, !207, !208, !209, !210, !215, !219, !225, !229, !233, !237, !241, !243, !245, !249, !253, !257, !261, !265, !267, !269, !271, !275, !279, !283, !285}
!22 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !25, line: 124)
!23 = !DINamespace(name: "std", scope: null, file: !24, line: 196)
!24 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cinclude\5Cc++\5C6.2.0\5Cx86_64-w64-mingw32\5Cbits/c++config.h", directory: "C:\5CApps\5CGIT\5Cng")
!25 = !DIDerivedType(tag: DW_TAG_typedef, name: "div_t", file: !5, line: 62, baseType: !"_ZTS6_div_t")
!26 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !27, line: 125)
!27 = !DIDerivedType(tag: DW_TAG_typedef, name: "ldiv_t", file: !5, line: 67, baseType: !"_ZTS7_ldiv_t")
!28 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !29, line: 127)
!29 = !DISubprogram(name: "abort", scope: !5, file: !5, line: 329, type: !30, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!30 = !DISubroutineType(types: !31)
!31 = !{null}
!32 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !33, line: 128)
!33 = !DISubprogram(name: "abs", scope: !5, file: !5, line: 338, type: !34, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!34 = !DISubroutineType(types: !35)
!35 = !{!20, !20}
!36 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !37, line: 129)
!37 = !DISubprogram(name: "atexit", scope: !5, file: !5, line: 349, type: !38, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!38 = !DISubroutineType(types: !39)
!39 = !{!20, !40}
!40 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !30, size: 64, align: 64)
!41 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !42, line: 135)
!42 = !DISubprogram(name: "atof", scope: !5, file: !5, line: 352, type: !43, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!43 = !DISubroutineType(types: !44)
!44 = !{!45, !46}
!45 = !DIBasicType(name: "double", size: 64, align: 64, encoding: DW_ATE_float)
!46 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !47, size: 64, align: 64)
!47 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !48)
!48 = !DIBasicType(name: "char", size: 8, align: 8, encoding: DW_ATE_signed_char)
!49 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !50, line: 136)
!50 = !DISubprogram(name: "atoi", scope: !5, file: !5, line: 355, type: !51, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!51 = !DISubroutineType(types: !52)
!52 = !{!20, !46}
!53 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !54, line: 137)
!54 = !DISubprogram(name: "atol", scope: !5, file: !5, line: 357, type: !55, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!55 = !DISubroutineType(types: !56)
!56 = !{!9, !46}
!57 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !58, line: 138)
!58 = !DISubprogram(name: "bsearch", scope: !5, file: !5, line: 361, type: !59, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!59 = !DISubroutineType(types: !60)
!60 = !{!61, !62, !62, !64, !64, !67}
!61 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 64)
!62 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !63, size: 64, align: 64)
!63 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!64 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !65, line: 35, baseType: !66)
!65 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cx86_64-w64-mingw32\5Cinclude\5Ccrtdefs.h", directory: "C:\5CApps\5CGIT\5Cng")
!66 = !DIBasicType(name: "long long unsigned int", size: 64, align: 64, encoding: DW_ATE_unsigned)
!67 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !68, size: 64, align: 64)
!68 = !DISubroutineType(types: !69)
!69 = !{!20, !62, !62}
!70 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !71, line: 139)
!71 = !DISubprogram(name: "calloc", scope: !5, file: !5, line: 455, type: !72, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!72 = !DISubroutineType(types: !73)
!73 = !{!61, !64, !64}
!74 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !75, line: 140)
!75 = !DISubprogram(name: "div", scope: !5, file: !5, line: 367, type: !76, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!76 = !DISubroutineType(types: !77)
!77 = !{!25, !20, !20}
!78 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !79, line: 141)
!79 = !DISubprogram(name: "exit", scope: !5, file: !5, line: 315, type: !80, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!80 = !DISubroutineType(types: !81)
!81 = !{null, !20}
!82 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !83, line: 142)
!83 = !DISubprogram(name: "free", scope: !5, file: !5, line: 456, type: !84, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!84 = !DISubroutineType(types: !85)
!85 = !{null, !61}
!86 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !87, line: 143)
!87 = !DISubprogram(name: "getenv", scope: !5, file: !5, line: 368, type: !88, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!88 = !DISubroutineType(types: !89)
!89 = !{!90, !46}
!90 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !48, size: 64, align: 64)
!91 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !92, line: 144)
!92 = !DISubprogram(name: "labs", scope: !5, file: !5, line: 339, type: !93, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!93 = !DISubroutineType(types: !94)
!94 = !{!9, !9}
!95 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !96, line: 145)
!96 = !DISubprogram(name: "ldiv", scope: !5, file: !5, line: 378, type: !97, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!97 = !DISubroutineType(types: !98)
!98 = !{!27, !9, !9}
!99 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !100, line: 146)
!100 = !DISubprogram(name: "malloc", scope: !5, file: !5, line: 457, type: !101, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!101 = !DISubroutineType(types: !102)
!102 = !{!61, !64}
!103 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !104, line: 148)
!104 = !DISubprogram(name: "mblen", scope: !5, file: !5, line: 380, type: !105, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!105 = !DISubroutineType(types: !106)
!106 = !{!20, !46, !64}
!107 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !108, line: 149)
!108 = !DISubprogram(name: "mbstowcs", scope: !5, file: !5, line: 388, type: !109, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!109 = !DISubroutineType(types: !110)
!110 = !{!64, !111, !114, !64}
!111 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !112)
!112 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !113, size: 64, align: 64)
!113 = !DIBasicType(name: "wchar_t", size: 16, align: 16, encoding: DW_ATE_unsigned)
!114 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !46)
!115 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !116, line: 150)
!116 = !DISubprogram(name: "mbtowc", scope: !5, file: !5, line: 386, type: !117, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!117 = !DISubroutineType(types: !118)
!118 = !{!20, !111, !114, !64}
!119 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !120, line: 152)
!120 = !DISubprogram(name: "qsort", scope: !5, file: !5, line: 362, type: !121, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!121 = !DISubroutineType(types: !122)
!122 = !{null, !61, !64, !64, !67}
!123 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !124, line: 158)
!124 = !DISubprogram(name: "rand", scope: !5, file: !5, line: 391, type: !18, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!125 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !126, line: 159)
!126 = !DISubprogram(name: "realloc", scope: !5, file: !5, line: 458, type: !127, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!127 = !DISubroutineType(types: !128)
!128 = !{!61, !61, !64}
!129 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !130, line: 160)
!130 = !DISubprogram(name: "srand", scope: !5, file: !5, line: 393, type: !131, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!131 = !DISubroutineType(types: !132)
!132 = !{null, !133}
!133 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!134 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !135, line: 161)
!135 = !DISubprogram(name: "strtod", scope: !5, file: !5, line: 405, type: !136, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!136 = !DISubroutineType(types: !137)
!137 = !{!45, !114, !138}
!138 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !139)
!139 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !90, size: 64, align: 64)
!140 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !141, line: 162)
!141 = !DISubprogram(name: "strtol", scope: !5, file: !5, line: 439, type: !142, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!142 = !DISubroutineType(types: !143)
!143 = !{!9, !114, !138, !20}
!144 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !145, line: 163)
!145 = !DISubprogram(name: "strtoul", scope: !5, file: !5, line: 441, type: !146, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!146 = !DISubroutineType(types: !147)
!147 = !{!148, !114, !138, !20}
!148 = !DIBasicType(name: "long unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!149 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !150, line: 164)
!150 = !DISubprogram(name: "system", scope: !5, file: !5, line: 445, type: !51, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!151 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !152, line: 166)
!152 = !DISubprogram(name: "wcstombs", scope: !5, file: !5, line: 450, type: !153, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!153 = !DISubroutineType(types: !154)
!154 = !{!64, !155, !156, !64}
!155 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !90)
!156 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !157)
!157 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !158, size: 64, align: 64)
!158 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !113)
!159 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !160, line: 167)
!160 = !DISubprogram(name: "wctomb", scope: !5, file: !5, line: 448, type: !161, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!161 = !DISubroutineType(types: !162)
!162 = !{!20, !90, !113}
!163 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !165, line: 220)
!164 = !DINamespace(name: "__gnu_cxx", scope: null, file: !24, line: 222)
!165 = !DIDerivedType(tag: DW_TAG_typedef, name: "lldiv_t", file: !5, line: 653, baseType: !"_ZTS7lldiv_t")
!166 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !167, line: 226)
!167 = !DISubprogram(name: "_Exit", scope: !5, file: !5, line: 320, type: !80, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!168 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !169, line: 230)
!169 = !DISubprogram(name: "llabs", scope: !5, file: !5, line: 657, type: !170, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!170 = !DISubroutineType(types: !171)
!171 = !{!14, !14}
!172 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !173, line: 236)
!173 = !DISubprogram(name: "lldiv", scope: !5, file: !5, line: 655, type: !174, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!174 = !DISubroutineType(types: !175)
!175 = !{!165, !14, !14}
!176 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !177, line: 247)
!177 = !DISubprogram(name: "atoll", scope: !5, file: !5, line: 666, type: !178, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!178 = !DISubroutineType(types: !179)
!179 = !{!14, !46}
!180 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !181, line: 248)
!181 = !DISubprogram(name: "strtoll", scope: !5, file: !5, line: 662, type: !182, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!182 = !DISubroutineType(types: !183)
!183 = !{!14, !114, !138, !20}
!184 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !185, line: 249)
!185 = !DISubprogram(name: "strtoull", scope: !5, file: !5, line: 663, type: !186, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!186 = !DISubroutineType(types: !187)
!187 = !{!66, !114, !138, !20}
!188 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !189, line: 251)
!189 = !DISubprogram(name: "strtof", scope: !5, file: !5, line: 412, type: !190, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!190 = !DISubroutineType(types: !191)
!191 = !{!192, !114, !138}
!192 = !DIBasicType(name: "float", size: 32, align: 32, encoding: DW_ATE_float)
!193 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !164, entity: !194, line: 252)
!194 = !DISubprogram(name: "strtold", scope: !5, file: !5, line: 423, type: !195, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!195 = !DISubroutineType(types: !196)
!196 = !{!197, !114, !138}
!197 = !DIBasicType(name: "long double", size: 128, align: 128, encoding: DW_ATE_float)
!198 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !165, line: 260)
!199 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !167, line: 262)
!200 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !169, line: 264)
!201 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !202, line: 265)
!202 = !DISubprogram(name: "div", linkageName: "_ZN9__gnu_cxx3divExx", scope: !164, file: !203, line: 233, type: !174, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!203 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cinclude\5Cc++\5C6.2.0\5Ccstdlib", directory: "C:\5CApps\5CGIT\5Cng")
!204 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !173, line: 266)
!205 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !177, line: 268)
!206 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !189, line: 269)
!207 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !181, line: 270)
!208 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !185, line: 271)
!209 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !194, line: 272)
!210 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !211, line: 75)
!211 = !DISubprogram(name: "memchr", scope: !212, file: !212, line: 46, type: !213, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!212 = !DIFile(filename: "C:\5CApps\5Cmsys64\5Cmingw64\5Cx86_64-w64-mingw32\5Cinclude\5Cstring.h", directory: "C:\5CApps\5CGIT\5Cng")
!213 = !DISubroutineType(types: !214)
!214 = !{!61, !62, !20, !64}
!215 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !216, line: 76)
!216 = !DISubprogram(name: "memcmp", scope: !212, file: !212, line: 49, type: !217, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!217 = !DISubroutineType(types: !218)
!218 = !{!20, !62, !62, !64}
!219 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !220, line: 77)
!220 = !DISubprogram(name: "memcpy", scope: !212, file: !212, line: 50, type: !221, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!221 = !DISubroutineType(types: !222)
!222 = !{!61, !223, !224, !64}
!223 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !61)
!224 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !62)
!225 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !226, line: 78)
!226 = !DISubprogram(name: "memmove", scope: !212, file: !212, line: 66, type: !227, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!227 = !DISubroutineType(types: !228)
!228 = !{!61, !61, !62, !64}
!229 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !230, line: 79)
!230 = !DISubprogram(name: "memset", scope: !212, file: !212, line: 53, type: !231, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!231 = !DISubroutineType(types: !232)
!232 = !{!61, !61, !20, !64}
!233 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !234, line: 80)
!234 = !DISubprogram(name: "strcat", scope: !212, file: !212, line: 62, type: !235, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!235 = !DISubroutineType(types: !236)
!236 = !{!90, !155, !114}
!237 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !238, line: 81)
!238 = !DISubprogram(name: "strcmp", scope: !212, file: !212, line: 63, type: !239, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!239 = !DISubroutineType(types: !240)
!240 = !{!20, !46, !46}
!241 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !242, line: 82)
!242 = !DISubprogram(name: "strcoll", scope: !212, file: !212, line: 72, type: !239, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!243 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !244, line: 83)
!244 = !DISubprogram(name: "strcpy", scope: !212, file: !212, line: 61, type: !235, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!245 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !246, line: 84)
!246 = !DISubprogram(name: "strcspn", scope: !212, file: !212, line: 80, type: !247, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!247 = !DISubroutineType(types: !248)
!248 = !{!64, !46, !46}
!249 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !250, line: 85)
!250 = !DISubprogram(name: "strerror", scope: !212, file: !212, line: 82, type: !251, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!251 = !DISubroutineType(types: !252)
!252 = !{!90, !20}
!253 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !254, line: 86)
!254 = !DISubprogram(name: "strlen", scope: !212, file: !212, line: 64, type: !255, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!255 = !DISubroutineType(types: !256)
!256 = !{!64, !46}
!257 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !258, line: 87)
!258 = !DISubprogram(name: "strncat", scope: !212, file: !212, line: 85, type: !259, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!259 = !DISubroutineType(types: !260)
!260 = !{!90, !155, !114, !64}
!261 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !262, line: 88)
!262 = !DISubprogram(name: "strncmp", scope: !212, file: !212, line: 86, type: !263, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!263 = !DISubroutineType(types: !264)
!264 = !{!20, !46, !46, !64}
!265 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !266, line: 89)
!266 = !DISubprogram(name: "strncpy", scope: !212, file: !212, line: 89, type: !259, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!267 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !268, line: 90)
!268 = !DISubprogram(name: "strspn", scope: !212, file: !212, line: 95, type: !247, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!269 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !270, line: 91)
!270 = !DISubprogram(name: "strtok", scope: !212, file: !212, line: 97, type: !235, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!271 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !272, line: 92)
!272 = !DISubprogram(name: "strxfrm", scope: !212, file: !212, line: 104, type: !273, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!273 = !DISubroutineType(types: !274)
!274 = !{!64, !155, !114, !64}
!275 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !276, line: 93)
!276 = !DISubprogram(name: "strchr", scope: !212, file: !212, line: 68, type: !277, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!277 = !DISubroutineType(types: !278)
!278 = !{!90, !46, !20}
!279 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !280, line: 94)
!280 = !DISubprogram(name: "strpbrk", scope: !212, file: !212, line: 92, type: !281, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!281 = !DISubroutineType(types: !282)
!282 = !{!90, !46, !46}
!283 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !284, line: 95)
!284 = !DISubprogram(name: "strrchr", scope: !212, file: !212, line: 93, type: !277, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!285 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !23, entity: !286, line: 96)
!286 = !DISubprogram(name: "strstr", scope: !212, file: !212, line: 96, type: !281, isLocal: false, isDefinition: false, flags: DIFlagPrototyped, isOptimized: false)
!287 = !{i32 2, !"Dwarf Version", i32 4}
!288 = !{i32 2, !"Debug Info Version", i32 3}
!289 = !{i32 1, !"PIC Level", i32 2}
!290 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
!291 = !DILocalVariable(name: "int32", scope: !17, file: !1, line: 6, type: !20)
!292 = !DIExpression()
!293 = !DILocation(line: 6, column: 9, scope: !17)
!294 = !DILocalVariable(name: "uint32", scope: !17, file: !1, line: 7, type: !133)
!295 = !DILocation(line: 7, column: 14, scope: !17)
!296 = !DILocalVariable(name: "int64", scope: !17, file: !1, line: 9, type: !14)
!297 = !DILocation(line: 9, column: 15, scope: !17)
!298 = !DILocalVariable(name: "uint64", scope: !17, file: !1, line: 10, type: !66)
!299 = !DILocation(line: 10, column: 24, scope: !17)
!300 = !DILocalVariable(name: "a", scope: !17, file: !1, line: 12, type: !14)
!301 = !DILocation(line: 12, column: 15, scope: !17)
!302 = !DILocation(line: 12, column: 19, scope: !17)
!303 = !DILocation(line: 12, column: 27, scope: !17)
!304 = !DILocation(line: 12, column: 25, scope: !17)
!305 = !DILocalVariable(name: "b", scope: !17, file: !1, line: 13, type: !20)
!306 = !DILocation(line: 13, column: 9, scope: !17)
!307 = !DILocation(line: 13, column: 13, scope: !17)
!308 = !DILocation(line: 13, column: 21, scope: !17)
!309 = !DILocation(line: 13, column: 19, scope: !17)
!310 = !DILocalVariable(name: "c", scope: !17, file: !1, line: 15, type: !20)
!311 = !DILocation(line: 15, column: 9, scope: !17)
!312 = !DILocation(line: 15, column: 13, scope: !17)
!313 = !DILocation(line: 15, column: 21, scope: !17)
!314 = !DILocation(line: 15, column: 19, scope: !17)
!315 = !DILocalVariable(name: "d", scope: !17, file: !1, line: 16, type: !20)
!316 = !DILocation(line: 16, column: 9, scope: !17)
!317 = !DILocation(line: 16, column: 13, scope: !17)
!318 = !DILocation(line: 16, column: 21, scope: !17)
!319 = !DILocation(line: 16, column: 19, scope: !17)
!320 = !DILocalVariable(name: "e", scope: !17, file: !1, line: 18, type: !133)
!321 = !DILocation(line: 18, column: 14, scope: !17)
!322 = !DILocation(line: 18, column: 18, scope: !17)
!323 = !DILocation(line: 18, column: 26, scope: !17)
!324 = !DILocation(line: 18, column: 24, scope: !17)
!325 = !DILocalVariable(name: "f", scope: !17, file: !1, line: 19, type: !133)
!326 = !DILocation(line: 19, column: 14, scope: !17)
!327 = !DILocation(line: 19, column: 18, scope: !17)
!328 = !DILocation(line: 19, column: 26, scope: !17)
!329 = !DILocation(line: 19, column: 24, scope: !17)
!330 = !DILocalVariable(name: "g", scope: !17, file: !1, line: 21, type: !14)
!331 = !DILocation(line: 21, column: 15, scope: !17)
!332 = !DILocation(line: 21, column: 19, scope: !17)
!333 = !DILocation(line: 21, column: 27, scope: !17)
!334 = !DILocation(line: 21, column: 25, scope: !17)
!335 = !DILocalVariable(name: "h", scope: !17, file: !1, line: 22, type: !14)
!336 = !DILocation(line: 22, column: 15, scope: !17)
!337 = !DILocation(line: 22, column: 19, scope: !17)
!338 = !DILocation(line: 22, column: 27, scope: !17)
!339 = !DILocation(line: 22, column: 25, scope: !17)
!340 = !DILocalVariable(name: "i", scope: !17, file: !1, line: 24, type: !66)
!341 = !DILocation(line: 24, column: 24, scope: !17)
!342 = !DILocation(line: 24, column: 28, scope: !17)
!343 = !DILocation(line: 24, column: 36, scope: !17)
!344 = !DILocation(line: 24, column: 34, scope: !17)
!345 = !DILocalVariable(name: "j", scope: !17, file: !1, line: 25, type: !66)
!346 = !DILocation(line: 25, column: 24, scope: !17)
!347 = !DILocation(line: 25, column: 28, scope: !17)
!348 = !DILocation(line: 25, column: 36, scope: !17)
!349 = !DILocation(line: 25, column: 34, scope: !17)
!350 = !DILocation(line: 27, column: 5, scope: !17)
