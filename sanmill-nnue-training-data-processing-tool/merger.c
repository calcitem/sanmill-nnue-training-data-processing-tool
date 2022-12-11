#include <stdio.h>
#include <string.h>
#include <windows.h>

int mergeFiles(void)
{
    // �����ļ�ָ��
    FILE *in, *out;

    // ����Ŀ¼��ѯ������ļ���Ϣ�ṹ��
    HANDLE hFind;
    WIN32_FIND_DATAA fileInfo;

    // ����Ŀ¼��ѯģ��
    char fileTemplate[] = "*.txt";

    // ��ѯ��Ŀ¼������ .txt �ļ�
    hFind = FindFirstFileA(fileTemplate, &fileInfo);

    // �ж��Ƿ��ҵ����ļ�
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found\n");
        return 1;
    }

    // ��������ļ�
    if (fopen_s(&out, "all.txt", "w") != 0) {
        printf("Failed to create output file\n");
        return 1;
    }

    // ��ѯ�����ļ����� "." �� ".."
    if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0) {
        // �򿪸��ļ�
        if (fopen_s(&in, fileInfo.cFileName, "r") != 0) {
            printf("Failed to open file: %s\n", fileInfo.cFileName);
            return 1;
        }

        // �����ļ�������д������ļ�
        char c;
        while ((c = fgetc(in)) != EOF) {
            fputc(c, out);
        }
        // ��һ��
        fputc('\n', out);

        // �رո��ļ�
        fclose(in);
    }

    // ������һ���ļ�
    while (FindNextFileA(hFind, &fileInfo)) {
        // ��ѯ�����ļ����� "." �� ".."
        if (strcmp(fileInfo.cFileName, ".") != 0 && strcmp(fileInfo.cFileName, "..") != 0) {
            // �򿪸��ļ�
            if (fopen_s(&in, fileInfo.cFileName, "r") != 0) {
                printf("Failed to open file: %s\n", fileInfo.cFileName);
                return 1;
            }

            // �����ļ�������д������ļ�
            char c;
            while ((c = fgetc(in)) != EOF) {
                fputc(c, out);
            }

            // ��һ��
            fputc('\n', out);

            // �رո��ļ�
            fclose(in);
        }
    }

    // �ر�Ŀ¼��ѯ���
    FindClose(hFind);

    // �ر�����ļ�
    fclose(out);

    return 0;
}

int unique(void)
{
    // �����ļ�ָ��
    FILE *in, *out;

    // ����������
    static char lines[MAX_LINES][MAX_LINE_LENGTH];

    // ��������
    int numLines = 0;

    // �������ļ�
    if (fopen_s(&in, "all.txt", "r") != 0) {
        printf("Failed to open input file\n");
        return 1;
    }

    // ��������ļ�
    if (fopen_s(&out, "unique.txt", "w") != 0) {
        printf("Failed to create output file\n");
        return 1;
    }

    // ��ȡÿһ��
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // ȥ�����з�
        buffer[strcspn(buffer, "\r\n")] = '\0';
        // �����ǰ���ǿ��У�������֮ǰ���в�ͬ������д��������
        if (buffer[0] == '\0' || strcmp(buffer, lines[numLines]) != 0) {
            // ������������������˳�ѭ��
            if (numLines >= MAX_LINES) {
                break;
            }

            // ����ǰ�д洢��������
            strcpy_s(lines[numLines], sizeof(lines[numLines]), buffer);
            numLines++;
        }
    }

    // ���������е�ÿһ��д������ļ�
    for (int i = 0; i < numLines; i++) {
        fputs(lines[i], out);
        fputc('\n', out);
    }

    // �ر��ļ�
    fclose(in);
    fclose(out);

    return 0;
}
