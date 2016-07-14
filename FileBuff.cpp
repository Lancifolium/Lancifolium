#include "FileBuff.h"

FileOpenBuff::FileOpenBuff() {
	this->indx = 0;
	this->reallen = 0;
	this->fileopen = NULL;
}

int FileOpenBuff::openfile(char *filename)  { /* 成功將返回0 */
	fileopen = fopen(filename, "r");
	if (fileopen) {
		reallen = fread(buff, 1, MAXFILEBUFF, fileopen);
		indx = 0; return 0;
	}
	else return 1;
}

char FileOpenBuff::getc() {
	if (indx < reallen) return buff[indx++];
	else {
		reallen = fread(buff, 1, MAXFILEBUFF, fileopen);
		if (reallen == 0) { // finished file reading
			fclose(fileopen);
			return EOF;
		}
		indx = 0;
		return this->getc();
	}
}

FileSaveBuff::FileSaveBuff() {
	this->indx = 0; this->fileopen = NULL;
}

int FileSaveBuff::openfile(char *filename) { /* 成功將返回0 */
	fileopen = fopen(filename, "w");
	if (fileopen) {
		indx = 0; return 0;
	}
	else return 1;
}

void FileSaveBuff::putc(char chr) { /* 輸入EOF結束流並保存文件 */
	if (indx < MAXFILEBUFF && chr != EOF) buff[indx++] = chr;
	else if (chr == EOF) {
		if (indx > 0) fwrite(buff, 1, indx, fileopen); /* finished writing */
		fclose(fileopen);
	}
	else { // case that indx == MAXFILEBUFF and chr != EOF
		fwrite(buff, indx, 1, fileopen);
		indx = 0;
		this->putc(chr);
	}
}

void FileSaveBuff::puts(const char *str) { /* */
	if (strlen(str) > (MAXFILEBUFF - indx)) {
		fwrite(buff, 1, indx, fileopen); /**/
		indx = 0;
		fwrite(str, 1, strlen(str), fileopen);
	}
	else {
		strcpy((buff + indx), str);
		indx += strlen(str);
	}
}
