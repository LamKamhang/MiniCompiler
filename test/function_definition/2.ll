; ModuleID = 'my JIT'
source_filename = "my JIT"

define i32 @main() {
main_block:
  %a = alloca i32
  store i32 3, i32* %a
  ret i32 100
}
