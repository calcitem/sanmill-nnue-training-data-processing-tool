#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_LINE_LENGTH 80
#define MAX_LINES 1000000

// 定义缓冲区
static char buffer[MAX_LINE_LENGTH];

int main(void)
{
    // 定义文件指针
    FILE *in, *out;

    // 定义行指针
    static char* lines;

    // 定义行数
    int numLines = 0;

    // 为所有行分配内存
    lines = (char*)malloc(MAX_LINES * MAX_LINE_LENGTH);

    // 打开输入文件
    if (fopen_s(&in, "all.txt", "r") != 0) {
        printf("Failed to open input file\n");
        return 1;
    }

    // 创建输出文件
    if (fopen_s(&out, "unique.txt", "w") != 0) {
        printf("Failed to create output file\n");
        return 1;
    }

    // 读取每一行
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // 去掉换行符
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // 如果当前行是空行，或者与之前的行不同，则将其写入行指针
        if (buffer[0] == '\0' || strcmp(buffer, lines + numLines * MAX_LINE_LENGTH) != 0) {
            // 如果行指针指向的内存已满，则退出循环
            if (numLines >= MAX_LINES) {
                break;
            }

            // 将当前行存储到行指针
            strcpy_s(lines + numLines * MAX_LINE_LENGTH, MAX_LINE_LENGTH, buffer);
            numLines++;
        }
    }

    // 将行指针指向的内存中的每一行写入输出文件
    for (int i = 0; i < numLines; i++) {
        fputs(lines + i * MAX_LINE_LENGTH, out);
        fputc('\n', out);
    }

    // 释放内存
    free(lines);

    // 关闭文件
    fclose(in);
    fclose(out);

    return 0;
}
