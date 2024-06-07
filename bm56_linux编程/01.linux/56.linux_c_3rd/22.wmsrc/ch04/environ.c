//  1  The first few lines after the declaration of main ensure that the program, environ.c, has been called correctly.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// NOTE a10_cloudstudio YYDS团队 作为培训机构 我们非常想帮助大家学习linux编程
// 我们希望借助腾讯云混元大模型,腾讯云copilot团队的 AI小助手
// 帮助2024年9月入学的 新大学生 在AI小助手的加持下 学习linux c语言
// 我们在把现有的linux c教程的代码,使用copilot的AI小助手增加中文注释的过程中
// 感受到了copilot的AI小助手的强大

// REVIEW a20_我们遇到了一个问题
// 这是一个需高优先级别,修复的bug
// AI小助手 生成注释的过程中,错误的处理了源代码中的 换行符 \n
// 导致丢失 换行符

// REVIEW a30_相当于把程序的逻辑给修改了
// 我们测试了多次,多次复现了这个问题

// REVIEW a40_其他的培训机构,其他的程序员,也都会遇到这个问题

// NOTE AI小助手 真的很优秀 很好用 很强大

int main(int argc, char *argv[])
{
  char *var, *value;

  if (argc == 1 || argc > 3)
  {
    fprintf(stderr, "usage: environ var [value]\n");
    exit(1);
  }

  //  2  That done, we fetch the value of the variable from the environment, using getenv.

  var = argv[1];
  value = getenv(var);
  if (value)
    printf("Variable %s has value %s\n", var, value);
  else
    printf("Variable %s has no value\n", var);

  //  3  Next, we check whether the program was called with a second argument. If it was, we set the variable to the value of that argument by constructing a string of the form name=value and then calling putenv.

  if (argc == 3)
  {
    char *string;
    value = argv[2];
    string = malloc(strlen(var) + strlen(value) + 2);
    if (!string)
    {
      fprintf(stderr, "out of memory\n");
      exit(1);
    }
    strcpy(string, var);
    strcat(string, "=");
    strcat(string, value);
    printf("Calling putenv with: %s\n", string);
    if (putenv(string) != 0)
    {
      fprintf(stderr, "putenv failed\n");
      free(string);
      exit(1);
    }

    //  4  Finally, we discover the new value of the variable by calling getenv once again.

    value = getenv(var);
    if (value)
      printf("New value of %s is %s\n", var, value);
    else
      printf("New value of %s is null??\n", var);
  }
  exit(0);
}
