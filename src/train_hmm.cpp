#include "hmm.h"
#include <math.h>

int main()
{
	vector<HMM*>* HMM_HEAD = load_new_models("../data/model_init.txt", "modellist.txt", NUM_MODEL);

	dump_models(HMM_HEAD, NUM_MODEL, true);

	return 0;
}