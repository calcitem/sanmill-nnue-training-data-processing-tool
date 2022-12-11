#include <stdio.h>
#include <string.h>
#include <windows.h>

int merge(void)
{
    // 定义文件指针
    FILE *in, *out;

    // 定义目录查询句柄和文件信息结构体
    HANDLE hFind;
    WIN32_FIND_DATAA fileInfo;

    // 定义目录查询模板
    char fileTemplate[] = "*.txt";

    // 查询本目录下所有 .txt 文件
    hFind = FindFirstFileA(fileTemplate, &fileInfo);

    // 判断是否找到了文件
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found\n");
        return 1;
    }

    // 创建输出文件
    if (fopen_s(&out, "all.text", "w") != 0) {
        printf("Failed to create output file\n");
        return 1;
    }

    // 查询到的文件不是 "." 或 ".."
    if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0) {
        // 打开该文件
        if (fopen_s(&in, fileInfo.cFileName, "r") != 0) {
            printf("Failed to open file: %s\n", fileInfo.cFileName);
            return 1;
        }

        // 将该文件的内容写入输出文件
        char c;
        while ((c = fgetc(in)) != EOF) {
            fputc(c, out);
        }
        // 空一行
        fputc('\n', out);

        // 关闭该文件
        fclose(in);
    }

    // 查找下一个文件
    while (FindNextFileA(hFind, &fileInfo)) {
        // 查询到的文件不是 "." 或 ".."
        if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0) {
            // 打开该文件
            if (fopen_s(&in, fileInfo.cFileName, "r") != 0) {
                printf("Failed to open file: %s\n", fileInfo.cFileName);
                return 1;
            }

            // 将该文件的内容写入输出文件
            char c;
            while ((c = fgetc(in)) != EOF) {
                fputc(c, out);
            }

            // 空一行
            fputc('\n', out);

            // 关闭该文件
            fclose(in);
        }
    }

    // 关闭目录查询句柄
    FindClose(hFind);

    // 关闭输出文件
    fclose(out);

    return 0;
}
