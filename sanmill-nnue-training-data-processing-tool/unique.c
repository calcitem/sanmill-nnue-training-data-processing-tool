#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "config.h"


// 定义缓冲区
static char buffer[MAX_LINE_LENGTH];

int unique(void)
{
    // 定义文件指针
    FILE *in, *out;

    // 定义行指针
    static char** lines;

    // 定义行数
    size_t numLines = 0;

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

        // 判断当前行是否重复
        BOOL isDuplicate = FALSE;
        for (int i = 0; i < numLines; i++) {
            // 如果当前行和之前的行相同，则设置标记
            if (strncmp(buffer + 55, lines[i] + 55, 2) == 0 && /* 先比较着法序号附近 */
                strcmp(buffer, lines[i]) == 0 /* 再完整比较 */) {
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
                printf("numLines has reached the maximum number of lines\n");
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
       free(lines);
    }

    // 关闭文件
    fclose(in);
    fclose(out);

    return 0;
}
