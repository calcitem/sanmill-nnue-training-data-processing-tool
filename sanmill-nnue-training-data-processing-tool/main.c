#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "config.h"

int merge(void);
int unique(void);
int unique2(void);
int slice(void);

void runAndTime(void (*func)(void))
{
    // 计算函数名
    const char* funcName = __FUNCTION__;

    // 开始计时
    ULONGLONG startTime = GetTickCount64();

    // 调用函数
    (*func)();

    // 计算耗时
    ULONGLONG timeElapsed = GetTickCount64() - startTime;

    // 计算分秒
    int minutes = (int)(timeElapsed / 60000);
    int seconds = (int)((timeElapsed % 60000) / 1000);

    // 输出耗时
    printf("%s() took %lld ms (%dmin %ds) to run\n", funcName, timeElapsed, minutes, seconds);
}

int main()
{
    runAndTime(merge);
    runAndTime(unique);
    runAndTime(slice);

    return 0;
}
