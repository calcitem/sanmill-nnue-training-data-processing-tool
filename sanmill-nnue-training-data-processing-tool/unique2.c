#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 定义缓冲区大小
#define MAX_LINE_LENGTH 80
#define MAX_LINES (200 * 100000)

// 定义缓冲区
static char buffer[MAX_LINE_LENGTH];

// 定义结构体，用于传递线程参数
typedef struct {
    // 指向行指针数组的指针
    char*** lines;

    // 行数
    size_t* numLines;

    // 要保存去重后的行的开始下标
    size_t start;

    // 要保存去重后的行的结束下标
    size_t end;
} ThreadParams;

// 定义去重线程的函数
unsigned int __stdcall UniqueThread(void* param)
{
    // 获取线程参数
    ThreadParams* params = (ThreadParams*)param;

    // 取出线程参数中的变量
    char*** lines = params->lines;
    size_t* numLines = params->numLines;
    size_t start = params->start;
    size_t end = params->end;

    // 去重并保存
    for (size_t i = start; i <= end; i++) {
        // 判断当前行是否重复, 重复就写入 D 标记
        for (size_t j = 0; j < i; j++) {
            if (strcmp((*lines)[i], (*lines)[j]) == 0 && (*lines)[i][0] != '\0') {               
                (*lines)[i][0] = 'D';
                (*lines)[i][1] = '\0';
                break;
            }
        }
    }

    return 0;
}

int unique2(void)
{
    // 定义文件指针
    FILE *in, *out;

    // 定义行指针
    static char** lines;

    // 定义行数
    size_t numLines = 0;
    size_t numLines0Unique = 0;
    size_t numLines1Unique = 0;

    // 为所有行分配内存
    lines = (char**)malloc(MAX_LINES * sizeof(char*));
    if (lines == NULL) {
        printf("Failed to allocate memory for lines\n");
        return 1;
    }

    *lines = (char*)malloc(MAX_LINES * MAX_LINE_LENGTH);
    if (*lines == NULL) {
        printf("Failed to allocate memory for lines\n");
        free(lines);
        return 1;
    }

    for (size_t i = 0; i < MAX_LINES; i++) {
        lines[i] = (*lines) + i * MAX_LINE_LENGTH;
    }

    // 打开输入文件
    if (fopen_s(&in, "all.text", "r") != 0) {
        printf("Failed to open input file\n");
        free(*lines);
        free(lines);
        return 1;
    }

    // 创建输出文件
    if (fopen_s(&out, "unique.text", "w") != 0) {
        printf("Failed to create output file\n");
        free(*lines);
        free(lines);
        return 1;
    }

    // 读取每一行
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // 去掉换行符
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // 将行写入行指针数组
        lines[numLines++] = _strdup(buffer);
    }

    // 创建线程参数
    ThreadParams params0;
    params0.lines = &lines;
    params0.numLines = &numLines;
    params0.start = 0;
    params0.end = numLines / 2;

    ThreadParams params1;
    params1.lines = &lines;
    params1.numLines = &numLines;
    params1.start = numLines / 2 + 1;
    params1.end = numLines - 1;

    // 创建线程
    HANDLE thread0 = (HANDLE)_beginthreadex(NULL, 0, UniqueThread, &params0, 0, NULL);
    HANDLE thread1 = (HANDLE)_beginthreadex(NULL, 0, UniqueThread, &params1, 0, NULL);

    // 等待线程结束
    if (thread0 != 0)
        WaitForSingleObject(thread0, INFINITE);

    if (thread1 != 0)
        WaitForSingleObject(thread1, INFINITE);

    // 关闭线程句柄
    if (thread0 != 0)
        CloseHandle(thread0);
    if (thread1 != 0)
        CloseHandle(thread1);

    // 写入文件
    for (int i = 0; i < numLines; i++) {
        if (strcmp(lines[i], "D")) {
            fputs(lines[i], out);
            fputs("\n", out);
        }
    }

    // 释放内存
    for (size_t i = 0; i < numLines; i++) {
        free(lines[i]);
    }

    // free(*lines);
    // free(lines);

    // 关闭文件
    fclose(in);
    fclose(out);

    return 0;
}
