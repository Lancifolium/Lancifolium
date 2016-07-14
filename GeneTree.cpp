#include "GeneBase.h"

void GnTree::init() {
    treeroot = NULL;
    comment.clear();
    nodename.clear();
}

GnTree::__GnTree() {
	init();
}

void GnTree::insertcomment(char buff[], GnNode *tmpnode, int kind) {
	string tmpstr = buff;
	if (kind == 1) {
		comment.push_back(tmpstr);
		tmpnode->comment = &comment.back(); // Qt
	}
	else {
		nodename.push_back(tmpstr);
		tmpnode->nodename = &nodename.back(); // Qt
	}
}

void GnTree::deleteroot(GnNode *tmproot) {
	if (tmproot != NULL) {
		for (int tmpi = 0; tmpi < tmproot->nxt.size(); tmpi++) {
			deleteroot(tmproot->nxt[tmpi]);
		}
		delete tmproot;
	}
}

/*
 * 棋譜調整
 */

int GnTree::joinsame(GnNode *dst, GnNode *src) {
	/* 評論和節點名*/
	if (dst->comment == NULL && src->comment != NULL) dst->comment = src->comment; // 反之不用考慮
	else if (dst->comment != NULL && src->comment != NULL && *dst->comment != *src->comment) {
		*dst->comment += *src->comment;
	}
	if (dst->nodename == NULL && src->nodename != NULL) dst->nodename = src->nodename;
	else if (dst->nodename != NULL && src->nodename != NULL && *dst->nodename != *src->nodename) {
		*dst->nodename += *src->nodename;
	}

	/* 標籤 */
	dst->labels.insert(dst->labels.end(), src->labels.begin(), src->labels.end());
	std::sort(dst->labels.begin(), dst->labels.end(), sortcmp);
	vector<int>::iterator iter = std::unique(dst->labels.begin(), dst->labels.end());
	dst->labels.erase(iter, dst->labels.end());

	/* 後續節點 */
	printf("$ p(%p, %p), mov(%d, %d), nxtlist(%d, %d), ", dst, src, dst->mov, src->mov, dst->nxt.size(), src->nxt.size());
	dst->nxt.insert(dst->nxt.end(), src->nxt.begin(), src->nxt.end());
	printf("finally nxt: %d; \n", dst->nxt.size());
}

int GnTree::adjustnxtlist(vector<GnNode *> &nxt) {
	vector<GnNode *>::iterator itoi, itoj;
	for (itoi = nxt.begin(); itoi != nxt.end(); itoi++) {
		for (itoj = itoi + 1; itoj != nxt.end();) {
			if (*itoi == *itoj) nxt.erase(itoj); /* 指針地址相同 */
			else if (**itoi == **itoj) { /* 兩個節點配置相同 */
				printf("mov(%d, %d)\n", (*itoi)->mov, (*itoj)->mov); /* ------------- */
				printf("nxtlist(%d, %d)\n", (*itoi)->nxt.size(), (*itoj)->nxt.size()); /* ------------- */
				//(*itoi)->joinsame(**itoj); /* 將itoj複製到itoi */
			joinsame(*itoi, *itoj); // 此處換成指針了
				/* 之前這裡出了問題，joinsame裡面是兩個*，不能是一個星 */
				nxt.erase(itoj); /* 理解，這裡不要++ */
			}
			else itoj++; /* 理解 */
		}
	}
}

int GnTree::adjustManual(GnNode *tmpnode) {
	if (tmpnode == NULL) return 1;
	adjustnxtlist(tmpnode->nxt); /* 調整當前節點的子節點列表 */
	for (vector<GnNode *>::iterator tmpi = tmpnode->nxt.begin();
			tmpi != tmpnode->nxt.end(); tmpi++) {
		adjustManual(*tmpi);
	}
	return 0;
}

int GnTree::joinManuals(struct __GnTree &sig) { /* 將另一個棋譜歸併進來 */
		if (sig.siz > this->siz) this->siz = sig.siz; /* 棋盤取大 */
		for (int tmpi = 0; tmpi < sig.treeroot->nxt.size(); tmpi++) { /* 更改sig分支的父節點 */
			sig.treeroot->nxt[tmpi]->parent = this->treeroot;
		}
		this->treeroot->nxt.insert(this->treeroot->nxt.begin(),
										   sig.treeroot->nxt.begin(),
										   sig.treeroot->nxt.end());
		delete sig.treeroot; sig.treeroot = NULL;
		adjustManual(this->treeroot); /* 調整棋譜即可 */
}

/*
 * 棋譜寫出函數
 */

int GnTree::wreverse(int deep, GnNode *cur, FileSaveBuff &wril) {
	char mov[3];
	if (cur->mov >= 0) { // mov
		mov[0] = cur->mov / 100 + 'a';
		mov[1] = cur->mov % 100 + 'a';
		mov[2] = '\0';
		switch(cur->stoneProp) {
		case 1:
			wril.puts("B["); wril.puts(mov); wril.putc(']'); break;
		case 2:
			wril.puts("W["); wril.puts(mov); wril.putc(']'); break;
		default:
			break;
		}
	}

	if (cur->addblacks.size() > 0) { // add black stones
		wril.puts("AB");
		for (int tmpi = 0; tmpi < cur->addblacks.size(); tmpi++) {
			wril.putc('[');
			wril.putc(cur->addblacks[tmpi] / 100 + 'a');
			wril.putc(cur->addblacks[tmpi] % 100 + 'a');
			wril.putc(']');
		}
	}

	if (cur->addwhites.size() > 0) { // add white stones
		wril.puts("AW");
		for (int tmpi = 0; tmpi < cur->addwhites.size(); tmpi++) {
			wril.putc('[');
			wril.putc(cur->addwhites[tmpi] / 100 + 'a');
			wril.putc(cur->addwhites[tmpi] % 100 + 'a');
			wril.putc(']');
		}
	}

	if (cur->labels.size() > 0) { // labels
		wril.puts(cur->displaylabels());
	}

	if (cur->nodename != NULL && !cur->nodename->empty()) { // nodename
		wril.puts("N[");
		wril.puts(cur->nodename->c_str());
		wril.puts("]");
	}

	if (cur->comment != NULL && !cur->comment->empty()) { // nodename
		wril.puts("C[");
		wril.puts(cur->comment->c_str());
		wril.puts("]");
	}
	// 寫完了所有東西
	if (cur->nxt.size() == 0) return 0;
	if (cur->nxt.size() == 1) { // 有一個子節點
		wril.putc(';'); wreverse(++deep, cur->nxt[0], wril);
		return 0;
	}
	if (cur->nxt.size() > 1) { // 多個子節點
		for (int tmpi = 0; tmpi < cur->nxt.size(); tmpi++) {
			wril.puts("\n(;");
			wreverse(++deep, cur->nxt[tmpi], wril);
			wril.puts(")\n");
		}
		return 0;
	}
}

void GnTree::writetree(char *filename) {
	FileSaveBuff wril;
	wril.openfile(filename);

	wril.puts("(\n ;GM[1]FF[4]ST[2]");
	wril.puts("SZ[");
	if (this->siz > 3 && this->siz < 27) {
		char tmpsize[3];
		sprintf(tmpsize, "%d",this->siz);
		wril.puts(tmpsize);
	}
	else wril.puts("19");
	wril.puts("]CA[");
	if (this->encode.empty()) wril.puts("UTF-8");
	else wril.puts(this->encode.c_str());
	wril.putc(']');


	wreverse(1, this->treeroot, wril); // 遞歸寫譜

	wril.puts(")\n");
	wril.putc(EOF); // important here
}

int GnTree::reverse(int deep, struct GnNode *cur) {
	if (cur == NULL) return 1;
	cur->printbase();
	//printf("after printbase\n");

	if (cur->nxt.size() > 1) {
		deep++;
		for (int tmpi = 0; tmpi < cur->nxt.size(); tmpi++) {
			printf("\n");
			for (int tmpj = 0; tmpj < deep; tmpj++) printf(" ");

			printf("#|%d|", deep);
			reverse(deep, cur->nxt[tmpi]);
		}
	}
	else if (cur->nxt.size() == 1) {
		//printf(", |%d|", deep);
		reverse(deep, cur->nxt[0]);
	}
	return 0;
}

void GnTree::printtree() {
	printf("-----------------------------------\n");
    reverse(0, this->treeroot);
	printf("\n");
}

void GnTree::printpool() { // 將節點池和評論池打印出來
	std::list<string>::iterator itor;
	int tmpi;

	printf("[Nodename]\n");
	itor = this->nodename.begin();
	while (itor != this->nodename.end()) {
		std::cout << "$ "<< *itor++ << std::endl;
	}
	printf("[Comment]\n");
	itor = this->comment.begin();
	while (itor != this->comment.end()) {
		std::cout << "$ "<< *itor++ << "\n";
		//system("pause"); //
	}
	printf("----------------------");
}
