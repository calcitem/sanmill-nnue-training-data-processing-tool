#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_LINE_LENGTH 80
#define MAX_LINES 512

// 定义缓冲区
static char buffer[MAX_LINE_LENGTH];

int main(void)
{
    // 定义文件指针
    FILE *in, *out;

    // 定义行指针
    // static char** lines;
    static char* lines[MAX_LINES];

    // 定义行数
    int numLines = 0;

#if 0
    // 为所有行分配内存
    lines = (char**)malloc(MAX_LINES * sizeof(char*));
     if (lines == NULL) {
        printf("Failed to allocate memory for lines\n");
        return 1;
    }
#endif

    *lines = (char*)malloc(MAX_LINES * MAX_LINE_LENGTH);
    if (*lines == NULL) {
        printf("Failed to allocate memory for lines\n");
        free(lines);
        return 1;
    }

    for (int i = 0; i < MAX_LINES; i++) {
        lines[i] = (*lines) + i * MAX_LINE_LENGTH;
    }

    // 打开输入文件
    if (fopen_s(&in, "all.txt", "r") != 0) {
        printf("Failed to open input file\n");
        free(*lines);
        free(lines);
        return 1;
    }

    // 创建输出文件
    if (fopen_s(&out, "unique.txt", "w") != 0) {
        printf("Failed to create output file\n");
        free(*lines);
        free(lines);
        return 1;
    }

    // 读取每一行
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // 去掉换行符
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // 判断当前行是否重复
        BOOL isDuplicate = FALSE;
        for (int i = 0; i < numLines; i++) {
            // 如果当前行和之前的行相同，则设置标记
            if (strcmp(buffer, lines[i]) == 0) {
                isDuplicate = TRUE;
                break;
            }
        }

        // 如果当前行是空行，或者与之前的行不同，则将其写入行指针
        if (buffer[0] == '\0' || isDuplicate == FALSE) {
            // 如果行指针为 NULL，则退出程序
            if (lines == NULL) {
                printf("Failed to allocate memory for lines\n");
                break;
            }

            // 如果行指针指向的内存已满，则退出循环
            if (numLines >= MAX_LINES) {
                break;
            }

            
            // 将当前行存储到行指针
            strcpy_s(lines[numLines], MAX_LINE_LENGTH, buffer);
                numLines++;           
        }
    }

    // 将行指针指向的内存中的每一行写入输出文件
    for (int i = 0; i < numLines; i++) {
        // 如果行指针为 NULL，则退出程序
        if (lines == NULL) {
            printf("Failed to allocate memory for lines\n");
            break;
        }

        fputs(lines[i], out);
        fputc('\n', out);
    }

    // 释放内存
    if (lines != NULL && *lines != NULL) {
       free(*lines);
       // free(lines);
    }

out:
    // 关闭文件
    fclose(in);
    fclose(out);

    return 0;
}
