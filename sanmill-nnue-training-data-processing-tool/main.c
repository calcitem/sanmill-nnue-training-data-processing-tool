#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// ���建������С
#define MAX_LINE_LENGTH 80

/*
 * �ó�����ȡ��Ŀ¼�µ� unique.text �ļ���ʶ������У������зָ��ļ���
 * Ȼ���ڱ�Ŀ¼�´���һ����Ϊ cleaned ��Ŀ¼�����ָ�õ��ļ����������Ŀ¼�¡�
 * �ļ��������ִ�1��ʼ��������׺�� .txt�� 
 */

int main(void)
{
    // �����ļ�ָ��
    FILE *in = NULL, *out = NULL;

    // ���建����
    static char buffer[MAX_LINE_LENGTH];

    // �������ļ�
    if (fopen_s(&in, "unique.text", "r") != 0) {
        printf("Failed to open input file\n");
        return 1;
    }

    // ����Ŀ¼
    CreateDirectory(L"cleaned", NULL);

    // ��ȡÿһ��
    int fileNum = 0;
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        // ȥ�����з�
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // �����ǰ��Ϊ���У�������ļ�
        if (buffer[0] == '\0') {
            fileNum++;

            // �رյ�ǰ�ļ�
            if (out != NULL) {
                fclose(out);
            }

            // �����ļ�
            char fileName[MAX_LINE_LENGTH];
            sprintf_s(fileName, MAX_LINE_LENGTH, "cleaned\\%d.txt", fileNum);
            if (fopen_s(&out, fileName, "w") != 0) {
                printf("Failed to create output file\n");
                fclose(in);
                return 1;
            }
        } else {
            // �������ļ�δ�򿪣�������ļ���
            if (out == NULL) {
                fileNum++;

                // �����ļ�
                char fileName[MAX_LINE_LENGTH];
                sprintf_s(fileName, MAX_LINE_LENGTH, "cleaned\\%d.txt", fileNum);
                if (fopen_s(&out, fileName, "w") != 0) {
                    printf("Failed to create output file\n");
                    fclose(in);
                    return 1;
                }
            }

            // ����ǰ��д���ļ�
            fprintf(out, "%s\n", buffer);
        }
    }

    // �ر������ļ�
    if (out != NULL) {
        fclose(out);
    }
    fclose(in);

    return 0;
}
