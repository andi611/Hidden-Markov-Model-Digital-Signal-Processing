#include "hmm.h"
#include <math.h>

int main()
{
/*
	HMM hmms[5];
	load_models( "modellist.txt", hmms, 5);
	dump_models( hmms, 5);
*/
	HMM hmm_initial;
	loadHMM( &hmm_initial, "model_init.txt" );
	dumpHMM( stderr, &hmm_initial );

	printf("%f\n", log(1.5) ); // make sure the math library is included
	return 0;
}
