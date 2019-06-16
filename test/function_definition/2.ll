; ModuleID = 'my JIT'
source_filename = "my JIT"

define i32 @b(i32 %a, i32 %b) {
b_block:
  %a1 = alloca i32
  store i32 %a, i32* %a1
  %b2 = alloca i32
  store i32 %b, i32* %b2
  %0 = load i32, i32* %a1
  %ui2f_tmp = uitofp i32 %0 to float
  %add_result = alloca float
  %add_tmp = alloca float
  %1 = fadd float %ui2f_tmp, 1.005000e+02
  store float %1, float* %add_tmp
  %2 = load float, float* %add_tmp
  store float %2, float* %add_result
  %3 = load float, float* %add_result
  %4 = load i32, i32* %b2
  %ui2f_tmp3 = uitofp i32 %4 to float
  %div_result = alloca float
  %div_tmp = alloca float
  %5 = fdiv float %3, %ui2f_tmp3
  store float %5, float* %div_tmp
  %6 = load float, float* %div_tmp
  store float %6, float* %div_result
  %7 = load float, float* %div_result
  %f2ui_tmp = fptoui float %7 to i32
  ret i32 %f2ui_tmp
}

define i32 @main() {
main_block:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 3, i32* %b
  ret i32 100
}
