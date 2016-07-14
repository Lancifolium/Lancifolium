#ifndef GENEBASE_H
#define GENEBASE_H


#include "FunLancifolium.h"

using std::string;
using std::vector;

#define EMPTYPOINT 0 // 沒有落子
#define BLACKSTONE 1 // 黑子
#define WHITESTONE 2 // 白子
#define NONE_MOV -1 // 沒有走子記爲-1

#define TRIANGLE 1 /* 三角形 */
#define DIAMOND 2 /* 方塊 */
#define FORK 3 /* 叉 */
#define CIRCLE 4 /* 圓 */

/*
 * 重要原則：成功則返回0
 * 正確返回true
 */

typedef struct GnNode { // 讀取棋譜文件所用節點，亦當可用於戰鬥
	struct GnNode *parent; // 父節點

	vector<struct GnNode *> nxt; // 分支節點線性表

	unsigned char stoneProp; // 0空節點，1黑走子，2白走子
	int mov; // 走子：百位以上爲橫坐標，百位以下爲縱坐標，例如909即坐標天元，最大爲2626

	vector<int> addblacks; // 添加黑子
	vector<int> addwhites; // 添加白子

	vector<int> labels; // 標籤(LB) 長度 labanum，萬位以上爲標籤對應數字值，千百爲縱坐標，個十爲橫坐標

	string *comment; // 評論
	string *nodename; // 節點名稱

	vector<int> liftsave; // 提子保存
	unsigned char liftcolour; // 提子顏色


	void init(struct GnNode *par = NULL); // 此處默認參數在後面不能加
	GnNode();
	GnNode(struct GnNode *par);

	int insertNextNode(struct GnNode *tmpnxt);
	int insertAddStones(int tmpmov, int colour);

	/* 判斷 */
	bool operator==(const GnNode &tmpnode) const;

	int jud_nextmov(int tmpmov); // 判斷下一個落子是否在nxt中，

	const char *displaylabels();

	void printing();
	void printbase();
} GnNode;

typedef struct __GnTree { // 保存提子信息，GnNode中已經包含了這個功能
	std::list<string> comment; // 評論池
	std::list<string> nodename; // 節點名池
	GnNode *treeroot; // 棋譜樹
	int siz; // 棋盤規格
	string encode; // 編碼

	void init();
	__GnTree();
	void insertcomment(char buff[], GnNode *tmpnode, int kind);
	void deleteroot(GnNode *tmproot);

	int joinsame(GnNode *dst, GnNode *src); // 同一個棋譜內
	int joinsame_diff(GnNode *dst, GnNode *src); // 位於不同棋譜
	int adjustnxtlist(vector<GnNode *> &nxt);
	int adjustManual(GnNode *tmpnode);
	int joinManuals(struct __GnTree &sig);

	int wreverse(int deep, GnNode *cur, FileSaveBuff &wril);
	void writetree(char *filename);

	int reverse(int deep, GnNode *cur); //  遞歸遍歷棋譜樹，並輸出節點信息
	void printtree(); // 打印樹
	void printpool(); // 打印nodename和comment池
} GnTree;

#endif // !GENEBASE_H
