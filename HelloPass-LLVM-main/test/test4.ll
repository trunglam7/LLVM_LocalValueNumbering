; ModuleID = 'test4.c'
source_filename = "test4.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx11.0.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @test4(i32 %a, i32 %b, i32 %c, i32 %d) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %c.addr = alloca i32, align 4
  %d.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  store i32 %c, i32* %c.addr, align 4
  store i32 %d, i32* %d.addr, align 4
  %0 = load i32, i32* %b.addr, align 4
  %1 = load i32, i32* %c.addr, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* %a.addr, align 4
  %2 = load i32, i32* %a.addr, align 4
  %3 = load i32, i32* %d.addr, align 4
  %sub = sub nsw i32 %2, %3
  store i32 %sub, i32* %b.addr, align 4
  %4 = load i32, i32* %b.addr, align 4
  %5 = load i32, i32* %c.addr, align 4
  %add1 = add nsw i32 %4, %5
  store i32 %add1, i32* %c.addr, align 4
  %6 = load i32, i32* %a.addr, align 4
  %7 = load i32, i32* %d.addr, align 4
  %sub2 = sub nsw i32 %6, %7
  store i32 %sub2, i32* %d.addr, align 4
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+cx8,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 1}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"Homebrew clang version 13.0.0"}
