#include <test.h>

extern int lib_dynamic_f1(void);
extern int lib_dynamic_f2(void);

extern int static_lib_f1(void);
extern int static_lib_f2(void);

extern int src1_f1(void);
extern int src2_f1(void);

extern int src1_src1_f1(void);
extern int src2_src2_f1(void);

int main(void){
  printf("%d\n",lib_dynamic_f1());
  printf("%d\n",lib_dynamic_f2());

  printf("%d\n",static_lib_f1());
  printf("%d\n",static_lib_f2());

  printf("%d\n",src1_f1());
  printf("%d\n",src2_f1());

  printf("%d\n",src1_src1_f1());
  printf("%d\n",src2_src2_f1());
  return 0;
}
