#include "hmm.h"
#include <math.h>

int main()
{
	vector<HMM*>* HMM_head;
	HMM_head = load_models("modellist.txt", NUM_MODEL);
	dump_models(HMM_head, 5);
/*
	HMM* hmm = new HMM();
	hmm->loadHMM("../model_init.txt");
	hmm->dumpHMM(stderr);
*/

	return 0;
}