#ifndef MOVLANCIFOLIUM_H
#define MOVLANCIFOLIUM_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stack>
#include <list>

#include "GeneBase.h"

#define MAXBORDSIZE 26 // 最大棋盤規模

// for tmpbord[][]
#define TB_EMPTYPOINT 0 // 空點
#define TB_BLACKSTONE 1 // 黑子
#define TB_WHITESTONE 2 // 白子
#define TB_SAMECOLOUR 3 // 同色
#define TB_DIFFCOLOUR 4 //
#define TB_LIFTSTONES 5
#define TB_DIFFNOLIFT 6

// for lift
#define CANNOTPLACE 0 //
#define LIFTSTONES  1
#define NOLIFTS     2

struct MovLancifolium {
	int siz;
	char ston[MAXBORDSIZE][MAXBORDSIZE];
	char calcbord[MAXBORDSIZE][MAXBORDSIZE];

	struct GnNode *root; // 根
	struct GnNode *curNode; // 當前指針

	int conflict; // 劫爭
	int confmove; // 劫爭熱點

	void init();
	void deleteroot(struct GnNode *tmproot);
	void clearall();
    MovLancifolium();

	int findStoneBlock(int colour, int tmpcolour, int pos, int tmpsiz, int &stonnum); // 落子判斷函數

	int configDropStone(int colour, int mov); // 著子判斷函數

	int savetree(int colour, int mov); // 保留

	int regainMove(); // 回退，考慮添加棋子
};

#endif // MOVLANCIFOLIUM_H
