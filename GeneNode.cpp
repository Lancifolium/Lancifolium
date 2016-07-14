#include "GeneBase.h"

void GnNode::init(struct GnNode *par) { // 初始化（此處的默認參數不能加）
    parent = par; // 父節點

	nxt.clear();
	nxt.reserve(0); // 初始分配空間爲0

	stoneProp = EMPTYPOINT; // 空節點，脫先
	mov = NONE_MOV; // 走子和走子自然手數，沒有走子記爲-1
	liftcolour = 0;


	addblacks.clear(); // 這些以後試著刪除
	addblacks.reserve(0);
	addwhites.clear();
	addwhites.reserve(0);

	labels.clear();
    labels.reserve(0);

	comment = nodename = NULL;

	liftsave.clear();
    liftsave.reserve(0);
} // finished init

GnNode::GnNode() {
    init(NULL);
}

GnNode::GnNode(struct GnNode *par) {
	init(par);
}

int GnNode::insertNextNode(struct GnNode *tmpnxt) { // 挿入後續節點
	tmpnxt->parent = this;
	nxt.push_back(tmpnxt);
} // finished insertNextNode

int GnNode::insertAddStones(int tmpmov, int colour) {
	if (colour == 1) { // black stones
		addblacks.push_back(tmpmov);
	}
	else { // white stones
		addwhites.push_back(tmpmov);
	}
}

bool GnNode::operator==(const GnNode &tmpnode) const {
	if (this->stoneProp != tmpnode.stoneProp) return false;
	if (this->mov != tmpnode.mov) return false;
	/* add black stones */
	if (this->addblacks.size() != tmpnode.addblacks.size()) return false;
	else {
		for (int tmpi = 0; tmpi < this->addblacks.size(); tmpi++) {
			if (this->addblacks[tmpi] != tmpnode.addblacks[tmpi]) return false;
		}
	}
	/* add white stones */
	if (this->addwhites.size() != tmpnode.addwhites.size()) return false;
	else {
		for (int tmpi = 0; tmpi < this->addwhites.size(); tmpi++) {
			if (this->addwhites[tmpi] != tmpnode.addwhites[tmpi]) return false;
		}
	}
	printf("mov|%d|%d, stp|%d|%d, adb|%d|%d, adw|%d|%d\n",
		this->mov, tmpnode.mov, this->stoneProp, tmpnode.stoneProp,
		this->addblacks.size(), tmpnode.addblacks.size(),
		this->addwhites.size(), tmpnode.addwhites.size()); /* --------------------- */
	return true;
}

int GnNode::jud_nextmov(int tmpmov) { /* 判斷下一手是否在子節點中 */
	for (int tmpi = 0; tmpi < nxt.size(); tmpi++) {
		if (nxt[tmpi]->mov == tmpmov) return 1;
	} /* 先實現再說，這裡用不著高明算法 */
	return 0;
}

const char *GnNode::displaylabels() { // 這個功能寫弱一點，就當不會出現奇葩標籤好了
	// 這個還有問題，沒有區別很多東西，主要還是跟drago有關
	char buf[999999];
	int buft[360], tmpt = 0; // triangle
	int bufd[360], tmpd = 0; // diamond
	int buff[360], tmpf = 0; // fork
	int bufc[360], tmpc = 0; // circle
	int bufa[360], tmpa = 0; // alphabet
	for (int tmpi = 0; tmpi < this->labels.size(); tmpi++) {
		switch (this->labels[tmpi] / 10000) {
		case TRIANGLE: buft[tmpt++] = this->labels[tmpi] % 10000; break;
		case  DIAMOND: bufd[tmpd++] = this->labels[tmpi] % 10000; break;
		case     FORK: buff[tmpf++] = this->labels[tmpi] % 10000; break;
		case   CIRCLE: bufc[tmpc++] = this->labels[tmpi] % 10000; break;
		default:
			bufa[tmpa++] = this->labels[tmpi]; break;
		}
	}

	int tmp = 0;
	if (tmpa > 0) {
		buf[tmp++] = 'L'; buf[tmp++] = 'B';
		for (int tmpi = 0; tmpi < tmpa; tmpi++) {
			buf[tmp++] = '[';
			buf[tmp++] = (bufa[tmpi] % 10000) / 100 + 'a';
			buf[tmp++] = (bufa[tmpi] % 100) + 'a';
			buf[tmp++] = ':';
			buf[tmp++] = bufa[tmpi] / 10000;
			buf[tmp++] = ']';
		}
	}
	if (tmpt > 0) { // 三角形
		buf[tmp++] = 'T'; buf[tmp++] = 'R';
		for (int tmpi = 0; tmpi < tmpt; tmpi++) {
			buf[tmp++] = '[';
			buf[tmp++] = buft[tmpi] / 100 + 'a';
			buf[tmp++] = buft[tmpi] % 100 + 'a';
			buf[tmp++] = ']';
		}
	}
	if (tmpd > 0) { // 方形
		buf[tmp++] = 'T'; buf[tmp++] = 'R';
		for (int tmpi = 0; tmpi < tmpd; tmpi++) {
			buf[tmp++] = '[';
			buf[tmp++] = bufd[tmpi] / 100 + 'a';
			buf[tmp++] = bufd[tmpi] % 100 + 'a';
			buf[tmp++] = ']';
		}
	}
	if (tmpf > 0) { // 菱形
		buf[tmp++] = 'T'; buf[tmp++] = 'R';
		for (int tmpi = 0; tmpi < tmpf; tmpi++) {
			buf[tmp++] = '[';
			buf[tmp++] = buff[tmpi] / 100 + 'a';
			buf[tmp++] = buff[tmpi] % 100 + 'a';
			buf[tmp++] = ']';
		}
	}
	if (tmpc > 0) { // 圓形
		buf[tmp++] = 'T'; buf[tmp++] = 'R';
		for (int tmpi = 0; tmpi < tmpc; tmpi++) {
			buf[tmp++] = '[';
			buf[tmp++] = bufc[tmpi] / 100 + 'a';
			buf[tmp++] = bufc[tmpi] % 100 + 'a';
			buf[tmp++] = ']';
		}
	}
	buf[tmp] = '\0';
	return buf;
}

void GnNode::printing() {
	printf("\n|%p|%p|[%d](%d)", parent, this, mov, stoneProp);
}

void GnNode::printbase() {
	//printf("|%d|%d|%d|", stoneProp, mov, nxtnum);
    printf("[%d", mov);
    if (this->nodename != NULL) std::cout << "|" << *this->nodename;
    if (this->comment != NULL) std::cout << "|" << *this->comment;
    printf("]");
}

