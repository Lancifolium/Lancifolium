#ifndef DEVLANCIFOLIUM_H
#define DEVLANCIFOLIUM_H

#include <algorithm>
#include "GeneBase.h"
#include "FunLancifolium.h"
#include "FileBuff.h"

#define BUFFER_LENGTH 999999 /* 評論長度 */

typedef struct DevLancifolium { // 全局結構
	int siz; // 棋盤規格

	//struct GnNode *root; // 根節點
	GnTree *gntree; // 棋譜

	char reader;
	FileOpenBuff read;

	struct GnNode *curNode; // 當前節點指針
	std::stack<struct GnNode *> branchStack; // 分支棧

	/* 初始 */
	void init();
	void clearall();
	DevLancifolium();
	~DevLancifolium();


	/* 棋譜信息函數 */
	int dealSize();

	int dealEncode(); // deal encode

	// 節點內容函數
	int dealAddStones(struct GnNode *tmpnode, int colour);

	int dealMove(struct GnNode *tmpnode, int colour);

	int dealCommentNodename(struct GnNode *tmpnode, int tmpkind);

	int dealLabels(struct GnNode *tmpnode); // 字母標籤 LB

	int dealLabeldrago(struct GnNode *tmpnode); // L, only for drago

	int dealShapes(struct GnNode *tmpnode, int form); // 方塊、三角、圓、叉

	/* 節點處理函數 */
	int configNode();

	/* 讀取棋譜主函數 */
	int configManual(char *filename);
} DevLancifolium;

#endif // DEVLANCIFOLIUM_H
