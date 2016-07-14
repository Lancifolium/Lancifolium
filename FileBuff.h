#ifndef FILEBUFF_H
#define FILEBUFF_H

#include <cstdio>
#include <cstring>

#define MAXFILEBUFF 9000


typedef struct FileOpenBuff {
	char buff[MAXFILEBUFF];
	int indx;
	int reallen;
	FILE *fileopen;

	FileOpenBuff();
	int openfile(char *filename);
	char getc(); /* 從流中獲取一個字符 */
} FileOpenBuff;

typedef struct FileSaveBuff {
	char buff[MAXFILEBUFF];
	int indx;
	FILE *fileopen;

	FileSaveBuff();
	int openfile(char *filename);
	void putc(char chr); /* 向流中寫入一個字符 &&& 寫入EOF結束文件寫入並保存 */
	void puts(const char *str); /* 向流中寫入字符串 */
} FileSaveBuff;

#endif // FILEBUFF_H
