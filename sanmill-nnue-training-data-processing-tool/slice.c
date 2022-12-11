#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "config.h"

/*
 * 该程序会读取本目录下的 unique.text 文件，识别出空行，按空行分割文件。
 * 然后，在本目录下创建一个名为 cleaned 的目录，将分割好的文件保存在这个目录下。
 * 文件名按数字从1开始递增，后缀是 .txt。 
 */

int slice(void)
{
    // 定义文件指针
    FILE *in = NULL, *out = NULL;

    // 定义缓冲区
    static char buffer[MAX_LINE_LENGTH];

    // 打开输入文件
    if (fopen_s(&in, "unique.text", "r") != 0) {
        printf("Failed to open input file\n");
        return 1;
    }

    // 创建目录
    CreateDirectory(L"cleaned", NULL);

    // 读取每一行
    int fileNum = 0;
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // 去掉换行符
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // 如果当前行为空行，则打开新文件
        if (buffer[0] == '\0') {
            fileNum++;

            // 关闭当前文件
            if (out != NULL) {
                fclose(out);
            }

            // 打开新文件
            char fileName[MAX_LINE_LENGTH];
            sprintf_s(fileName, MAX_LINE_LENGTH, "cleaned\\%d.txt", fileNum);
            if (fopen_s(&out, fileName, "w") != 0) {
                printf("Failed to create output file\n");
                fclose(in);
                return 1;
            }
        } else {
            // 如果输出文件未打开，则打开新文件。
            if (out == NULL) {
                fileNum++;

                // 打开新文件
                char fileName[MAX_LINE_LENGTH];
                sprintf_s(fileName, MAX_LINE_LENGTH, "cleaned\\%d.txt", fileNum);
                if (fopen_s(&out, fileName, "w") != 0) {
                    printf("Failed to create output file\n");
                    fclose(in);
                    return 1;
                }
            }

            // 将当前行写入文件
            fprintf(out, "%s\n", buffer);
        }
    }

    // 关闭所有文件
    if (out != NULL) {
        fclose(out);
    }
    fclose(in);

    // 删除最后一个文件，如果它是空文件
    if (fileNum > 0) {
        char fileName[MAX_LINE_LENGTH];
        sprintf_s(fileName, MAX_LINE_LENGTH, "cleaned\\%d.txt", fileNum);

        // 检查最后一个文件是否为空
        FILE* lastFile = NULL;
        if (fopen_s(&lastFile, fileName, "r") == 0) {
            if (fgets(buffer, sizeof(buffer), lastFile) == NULL) {
                // 如果最后一个文件是空文件，则删除它
                fclose(lastFile);
                remove(fileName);
            }
        }
    }

    return 0;
}
