; ModuleID = 'asd'
source_filename = "asd"

define i32 @main(i32) {
entry:
  %argc = alloca i32
  store i32 %0, i32* %argc
  ret i32 0

temp:                                             ; No predecessors!
  ret i32 0
}
