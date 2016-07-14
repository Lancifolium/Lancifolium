#include "DevLancifolium.h"
#include "GeneBase.h"
#include "FileBuff.h"
#include "FunLancifolium.h"

int main(int argc, char *argv[]) {
	GnTree sig, siu;
	DevLancifolium dev;
    printf("----------------\n");

	if (argc < 3) {
		printf("exe [file 1] [file 2]\n"); return 1;
	}

	dev.gntree = &sig;
	if (dev.configManual(argv[1]) == 1) {
		return 2;
	}

	dev.gntree = &siu;
	if (dev.configManual(argv[2]) == 1) {
		return 3;
	}

	sig.joinManuals(siu);

	printf("----------------\n");
	sig.writetree("kkk.sgf");
	sig.deleteroot(sig.treeroot);
	siu.deleteroot(siu.treeroot);

	/*
    if (argc < 2) {
		printf("exe [filename]\n"); return 1;
	}
    if (dev.configManual(argv[1]) == 1) {

	}
	else {
	//sig.printtree();
		//sig.printpool();
		sig.writetree("kkk.sgf");
		sig.deleteroot(sig.treeroot);
	} //*/
}
