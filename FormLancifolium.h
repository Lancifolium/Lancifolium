#ifndef FORMLANCIFOLIUM_H
#define FORMLANCIFOLIUM_H

#include "DevLancifolium.h"
#include "MovLancifolium.h"

struct BranchSave {
	char ston[MAXBORDSIZE][MAXBORDSIZE];
	GnNode *branch;
};

typedef struct __FormLancifolium : public MovLancifolium {
	struct GnNode *root; // sig tree, 由DevLancifolium讀取
	// 仔細考慮如何組織
	// 判斷是否有脫先的
	struct GnNode *nodemap[MAXBORDSIZE][MAXBORDSIZE]; // clear before use (memset 0)
	std::stack<struct BranchSave> prebranch; // pre branch

	int readTree(char *filename); // read sig tree here

	int saveBranch();
	int regainBranch();
	int regainMove();

	int backroot(); // back to root
	int temp();
} FormLancifolium;

#endif // FORMLANCIFOLIUM_H
