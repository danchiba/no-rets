; ModuleID = 'hello.c'
source_filename = "hello.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @foo(i32 noundef %a) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %rem = srem i32 %0, 2
  %cmp = icmp eq i32 %rem, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %a.addr, align 4
  %sub = sub nsw i32 %1, 1
  store i32 %sub, ptr %retval, align 4
  br label %return

if.else:                                          ; preds = %entry
  %2 = load i32, ptr %a.addr, align 4
  %div = sdiv i32 %2, 2
  store i32 %div, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.else, %if.then
  %3 = load i32, ptr %retval, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @bar(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %mul = mul nsw i32 %0, 3
  store i32 %mul, ptr %b, align 4
  %1 = load i32, ptr %b, align 4
  %call = call i32 @foo(i32 noundef %1)
  ret i32 %call
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fez() #0 {
entry:
  %call = call i32 @bar(i32 noundef 2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @_start(i32 noundef %argc, ptr noundef %argv) #0 {
entry:
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca ptr, align 8
  %a = alloca i32, align 4
  %b = alloca i64, align 8
  store i32 %argc, ptr %argc.addr, align 4
  store ptr %argv, ptr %argv.addr, align 8
  %0 = load i32, ptr %argc.addr, align 4
  %call = call i32 @foo(i32 noundef %0)
  store volatile i32 %call, ptr %a, align 4
  %1 = load i32, ptr %argc.addr, align 4
  %call1 = call i32 @bar(i32 noundef %1)
  %conv = sext i32 %call1 to i64
  store i64 %conv, ptr %b, align 8
  call void asm sideeffect "movq $$60, %rax\0A\09syscall\0A\09", "~{rax},~{rdi},~{dirflag},~{fpsr},~{flags}"() #1, !srcloc !4
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 17.0.0 (https://github.com/llvm/llvm-project.git 603c286334b07f568d39f6706c848f576914f323)"}
!4 = !{i64 1841, i64 1859, i64 1912}
