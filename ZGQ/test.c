#include<stdio.h>
#include"print.h"
#include"hello.h"
#include"Add.h"
int main()
{
  int a,b;
  printf("请输入两个计算的数\n");
  int d=scanf("%d %d",&a,&b);
  printf("%d\n",d);
  int c=Add(a,b);
  hello();
  print(c);
  printf("结束！");
  printf("终于学会make编译了！");
  return 0; 

}
