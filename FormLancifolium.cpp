#include "FormLancifolium.h"
#include "DevLancifolium.h"

int __FormLancifolium::readTree(char *filename) {
	GnTree sig;
	DevLancifolium dev;
	dev.gntree = &sig;
	//DevLancifolium sig;
	if (dev.configManual(filename)) { // read file error
		return 1;
	}
	root = sig.treeroot;
}

