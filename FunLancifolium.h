#ifndef FUNLANCIFOLIUM
#define FUNLANCIFOLIUM

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cctype>
#include <list>
#include <vector>
#include <stack>
#include <algorithm>

#include "FileBuff.h"

inline int iswhite(char tmpc) { /* 判斷空白字符 */
	if (tmpc == ' ' || tmpc == '\n' || tmpc == '\t'
		|| tmpc == 13 || tmpc == 10) return 1;
	return 0;
}

/* 升序排序所用比較函數 */
inline bool sortcmp(const int &tmpa, const int &tmpb) {
	return tmpa < tmpb;
} /* 此函數多處用到，不可刪除 */

inline int operatecase(char *operate) { /* 專門用來 switch 操縱符的 */
	int tmpi = 0; int tmpj = 0;
	while (operate[tmpi] != '\0') {
		tmpj = tmpj * 100 + operate[tmpi] - '@'; /* @是A前面的一個字符, 令A從1開始 */
		tmpi++;
	}
	return tmpj;
}

/* 棋譜格式調整函數 */
inline int ManualAdjustment_old0(char *infile, char *oufile) {
	FileOpenBuff fil;
	FileSaveBuff wrl;
	if (!fil.openfile(infile)) return 0;
	wrl.openfile(oufile);

	int deep, tmpi, mod; /* 兩個域, 控制域(方括號外) mod = 0; 內容域(方括號內) mod = 1 */
	char tmpc, tmpb;
	deep = 0; tmpb = '0'; mod = 0;
	while (tmpc = fil.getc()) {
		if ((tmpc == '(') && (mod == 0)) {
			tmpc = fil.getc();
			while (iswhite(tmpc)) { /* 修復stonebase空括號對的bug */
				tmpc = fil.getc();
			}
			if (tmpc != ')') {
				wrl.putc('\n');
				for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
				wrl.puts("(\n"); // (\n !
				deep++;
				for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
			}
			else wrl.putc('(');
			wrl.putc(tmpc);
		}
		else if ((tmpc == ')') && (mod == 0)) {
			wrl.putc('\n');
			deep--;
			for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
			wrl.putc(')');
		}
		else if ((tmpc == '\t') || (tmpc == '\n') || (tmpc == ' ')) { /* 空白字符處理, 這個不好 */
			if (mod == 1) wrl.putc(' ');
		}
		else if ((tmpc == '[') && (tmpb != '\\') && (mod == 0)) { /* 進入內容域 */
			wrl.putc('['); mod = 1;
		}
		else if ((tmpc == ']') && (tmpb != '\\') && (mod == 1)) {
			wrl.putc(']'); mod = 0;
		}
		else if (tmpc == EOF) { /* reaching end */
			wrl.putc(EOF); break;
		}
		else wrl.putc(tmpc);

		tmpb = tmpc;
	}
	return 1;
} /* finished ManualAdjustment */

/* 棋譜格式調整函數 */
inline int ManualAdjustment(char *infile, char *oufile) {
	FileOpenBuff fil;
	FileSaveBuff wrl;
	if (!fil.openfile(infile)) return 0;
	wrl.openfile(oufile);

	int deep, tmpi, mod; /* 兩個域, 控制域(方括號外) mod = 0; 內容域(方括號內) mod = 1 */
	char tmpc, tmpb;
	deep = 0; tmpb = '0'; mod = 0;

	tmpc = fil.getc(); // get one character

	while (true) {
		if (mod == 0) { // 控制域
			while (iswhite(tmpc)) tmpc = fil.getc(); /* 忽略空白 */

			switch (tmpc) { //
			case '(': // (
				tmpc = fil.getc();
				while (iswhite(tmpc)) tmpc = fil.getc(); /* 修復stonebase空括號對的bug */
				if (tmpc != ')') {
					wrl.putc('\n');
					for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
					wrl.puts("(\n"); deep++;
					for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
				}
				else wrl.putc('(');
				wrl.putc(tmpc);
				break;
			case ')': // )
				wrl.putc('\n');
				deep--;
				for (tmpi = 0; tmpi < deep; tmpi++) wrl.putc(' ');
				wrl.putc(')'); break;
			case '[': // [
				wrl.putc('['); mod = 1; break;
			case EOF: // EOF
				wrl.putc(EOF); return 1;
			default:
				wrl.putc(tmpc); break;
			} // finished switch (tmpc)
		} // finished if
		else { // 內容域
			switch (tmpc) {
			case EOF: // EOF
				wrl.putc(EOF); return 0; // error here
			/*
			case '\n': // 需要考慮, 此處將換行符進行了轉義處理
				wrl.puts("\\n"); tmpb = tmpc;
				break; //*/
			case ']':
				if (tmpb != '\\') mod = 0;
			default:
				wrl.putc(tmpc); tmpb = tmpc;
				break;
			}
		} // finished else
		tmpc = fil.getc(); // get next character
	} // finished while loop

	return 1; // will never reach here
} /* finished ManualAdjustment */

#endif // FUNLANCIFOLIUM
