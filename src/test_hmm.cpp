/****************************************************************************
  FileName     [ test_hmm.cpp ]
  Synopsis     [ Implementation of the testing of a discrete hidden markov model ]
  Author       [ Ting-Wei Liu (Andi611) ]
  Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
****************************************************************************/
#include "hmm.h"


/***********************************/
/*          Main Function          */
/***********************************/
int main(int argc, char** argv)
{	
	// Usage: ./test modellist.txt ../data/testing_data1.txt ../data/testing_result1.txt
	if (argc != 4) {
		cout << "[HMM Test Usage]:" << endl;
		cout << " - Test with all models: /train modellist.txt ../data/testing_data1.txt ../data/testing_result1.txt" << endl;
        exit(-1);
    }

	char* model_list = argv[1];
	string data_path = argv[2];
	string output_path = argv[3];
	cout << "Testing with all models in: " << model_list << endl;

	// read testing data
	vector<vector<char>> test_data = read_data(data_path);

	// load models
	vector<HMM*>* HMM_HEAD = load_models(model_list, NUM_MODEL);
	assert(HMM_HEAD->size() == NUM_MODEL);

	// test with models
	double acc = test(HMM_HEAD, &test_data, output_path);
	return 0;
}

