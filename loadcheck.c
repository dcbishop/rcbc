// This just loads a file into memory then nukes it for memleak checking.

#include "rcbc.h"
#include "console.h"

/**
 * The glorious main function.
 */
int main(int argc, char** argv) {
	char* filename;

	if(argc < 2) {
		filename = "data/models/monkey-test.dae"; // Default model to view
		WARNING("No model file specified, using trying default '%s'.", filename);
	} else {
		filename = argv[1];
	}

	int* test = malloc(sizeof(int));

	List* listtest = NEW(List);
	ListAdd(listtest, test);

	//RCBC_Init();
	//Model* model = RCBC_LoadFile(filename, images);
	
	List_FreeData(listtest);
	DELETE(listtest);

	//DELETE(model);
	//DELETE(images);
}
