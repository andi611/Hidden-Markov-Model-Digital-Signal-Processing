/****************************************************************************
  FileName     [ hmm.cpp ]
  Synopsis     [ .cpp file for hmm.h ]
  Author       [ Ting-Wei Liu (Andi611) ]
  Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
****************************************************************************/
#include "hmm.h"
#include <string.h>
using namespace std;


/************************************/
/*        class HMM Functions       */
/************************************/
void HMM::initialize_HMM() 
{
	_initial = vector<double> (MAX_STATE, FLT_MAX);
	_transition = vector<vector<double>>(MAX_STATE, vector<double>(MAX_STATE, FLT_MAX));
	_observation = vector<vector<double>>(MAX_STATE, vector<double>(MAX_OBSERV, FLT_MAX));
}


void HMM::load_HMM(const char* filename)
{
   	int i, j;
   	int f;
   	FILE* fp = open_file(filename, "r");

   	string str(filename);
   	_model_name = str;

   	char token[MAX_LINE] = "";
   	while (fscanf( fp, "%s", token ) > 0 )
   	{
	  	if (token[0] == '\0' || token[0] == '\n' ) continue;

	  	if (strcmp( token, "initial:" ) == 0 ) {
		 	f = fscanf(fp, "%d", &_state_num );
	  		if (!_max_allocate) _initial = vector<double> (_state_num, FLT_MAX);
		 	for (i = 0 ; i < _state_num ; i++ )
				f = fscanf(fp, "%lf", &( _initial[i] ) );
	  	}
	  	else if (strcmp( token, "transition:" ) == 0 ) {
		 	f = fscanf(fp, "%d", &_state_num );
		 	if (!_max_allocate) _transition = vector<vector<double>>(_state_num, vector<double>(_state_num, FLT_MAX));
		 	for (i = 0 ; i < _state_num ; i++ )
				for (j = 0 ; j < _state_num ; j++ )
			   		f = fscanf(fp, "%lf", &(_transition[i][j]));
	  	}
	  	else if (strcmp( token, "observation:" ) == 0 ) {
		 	f = fscanf(fp, "%d", &_observ_num );
			if (!_max_allocate) _observation = vector<vector<double>>(_state_num, vector<double>(_observ_num, FLT_MAX));
		 	for (i = 0 ; i < _observ_num ; i++ )
				for (j = 0 ; j < _state_num ; j++ )
			   		f = fscanf(fp, "%lf", &( _observation[i][j]) );
	  	}
   	}
}


void HMM::dump_HMM(FILE *fp, bool display) const
{
   	int i, j;

   	if (display) {
   		fprintf(fp, "===============================================\n");
   		fprintf(fp, "model name: %s\n", _model_name.c_str() );
   	}
   	fprintf(fp, "initial: %d\n", _state_num );
   	for (i = 0 ; i < _state_num - 1; i++ )
	  	fprintf(fp, "%.5lf ", _initial[i]);
   	fprintf(fp, "%.5lf\n", _initial[ _state_num - 1 ] );

   	fprintf(fp, "\ntransition: %d\n", _state_num );
   	for (i = 0 ; i < _state_num ; i++ ) {
	 	for (j = 0 ; j < _state_num - 1 ; j++ )
		 	fprintf( fp, "%.5lf ", _transition[i][j] );
	  	fprintf(fp,"%.5lf\n", _transition[i][_state_num - 1]);
   	}
   	fprintf(fp, "\nobservation: %d\n", _observ_num );
   	for (i = 0 ; i < _observ_num ; i++ ) {
	  	for (j = 0 ; j < _state_num - 1 ; j++ )
		 	fprintf( fp, "%.5lf ", _observation[i][j] );
	  	fprintf(fp,"%.5lf\n", _observation[i][_state_num - 1]);
   	}
}


double HMM::test_HMM(vector<char>* obs) const
{
	// viterbi algorithm
	vector<vector<double>>* delta = new vector<vector<double>>(T, vector<double>(N_S, 0));
	for (int i = 0; i < N_S; ++i)
		delta(0, i) = PI(i) * B(i, o(0));
	for (int t = 0; t < T-1; ++t)
		for (int j = 0; j < N_S; ++j) {
			for (int i = 0; i < N_S; ++i)
				if (delta(t+1, j) < (delta(t, i) * A(i, j)))
					delta(t+1, j) = delta(t, i) * A(i, j);
			delta(t+1, j) *= B(j, o(t+1));
		}
	double viterbi_prob = 0;
	for (int i = 0; i < N_S; ++i)
		if (viterbi_prob < delta(T-1, i))
			viterbi_prob = delta(T-1, i);
	delete delta;
	return viterbi_prob;
}


/***************************************/
/*          Testing Functions          */
/***************************************/
double test(vector<HMM*>* HMM_HEAD, vector<vector<char>>* data, const string& output_path, bool print_log){
	
	vector<double>* max_prob = new vector<double>(data->size(), 0);
	vector<int>* model = new vector<int>(data->size(), 0);
	vector<char>* obs;
	
	// run test
	for (int i = 0; i < data->size(); ++i) {
		if (print_log) cout << "\r" << "HMM testing: " << i << '/' << data->size() << "... " << flush;
		obs = &data->at(i);
		
		double prob = 0;
		for (int m = 0; m < HMM_HEAD->size(); ++m) {
			prob = HMM_HEAD->at(m)->test_HMM(obs);
			if (max_prob->at(i) < prob){
				max_prob->at(i) = prob;
				model->at(i) = m;
			}
		}
	}
	if (print_log) cout << "\r" << "HMM testing: " << data->size() << '/' << data->size() << "... " << " Complete." << endl;

	// output results
	ofstream file;
	file.open(output_path.c_str());
	for (int i = 0; i < data->size(); ++i)
		file << HMM_HEAD->at(model->at(i))->get_name() << ' ' << max_prob->at(i) << '\n';
	file.close();

	double acc = compute_acc("../data/testing_answer.txt", output_path);
	if (print_log) cout << "Testing accuracy: " << acc << endl;
	delete max_prob;
	delete model;
	return acc;
}


double compute_acc(const string& answer_path, const string& result_path)
{
	// opening file
	ifstream ifs_ans(answer_path.c_str());
	ifstream ifs_res(result_path.c_str());
	assert(ifs_ans);
	assert(ifs_res);

	// read file
	string str_buf;
	vector<string> answer;
	vector<string> result;

	while (getline(ifs_ans, str_buf)) {
		string ans = str_buf.substr(0, 12);
		answer.push_back(ans);
	}
	while (getline(ifs_res, str_buf)) {
		string res = str_buf.substr(0, 12);
		result.push_back(res);
	}

	// compute score
	assert(answer.size() == result.size());
	double acc = 0;
	for (int i = 0; i < answer.size(); ++i)
		if (answer.at(i) == result.at(i))
			acc += 1;
	return (acc / (double)answer.size());
}


/*************************************/
/*           I/O Functions           */
/*************************************/
FILE* open_file(const char* filename, const char* ht)
{
	FILE* fp = fopen(filename, ht);
	if ( fp == NULL ){
		perror(filename);
		exit(1);
	}
	return fp;
}


vector<vector<char>> read_data(const string& data_path)
{
	// opening file
	ifstream ifs(data_path.c_str());
	assert(ifs);

	// read file
	string str_buf;
	vector<vector<char>> data;

	while (getline(ifs, str_buf)) {
		vector<char> line;
		for (int i = 0; i < str_buf.size(); ++i)
			line.push_back(str_buf[i]);
		data.push_back(line);
	}
	ifs.close();
	return data;
}


vector<HMM*>* load_models(const char* model_list, const int max_num)
{
	FILE *fp = open_file(model_list, "r");

	int count = 0;
	char model_name[MAX_LINE] = "";
	vector<HMM*>* HMM_HEAD = new vector<HMM*>();
	HMM* hmm;

	while (fscanf(fp, "%s", model_name) == 1) {
		hmm = new HMM();
		hmm->load_HMM(model_name);
		HMM_HEAD->push_back(hmm);
		++count;
		if (count >= max_num)
			return HMM_HEAD;
	}
	fclose(fp);
	return HMM_HEAD;
}


vector<HMM*>* initialize_new_models(const char* model_init, const char* model_list, const int max_num)
{	
	FILE *fp = open_file(model_list, "r");

	int count = 0;
	char model_name[MAX_LINE] = "";
	vector<HMM*>* HMM_HEAD = new vector<HMM*>();
	HMM* hmm;

	while (fscanf(fp, "%s", model_name) == 1) {
		hmm = new HMM();
		hmm->load_HMM(model_init);
		hmm->set_name(model_name);
		HMM_HEAD->push_back(hmm);
		++count;
		if (count >= max_num)
			return HMM_HEAD;
	}
	fclose(fp);
	return HMM_HEAD;
}


void dump_models(vector<HMM*>* HMM_HEAD, bool to_file)
{
   	for(int i = 0; i < HMM_HEAD->size(); ++i ) { 
   		if (to_file) {
	  		HMM_HEAD->at(i)->dump_HMM(open_file(HMM_HEAD->at(i)->get_name().c_str(), "w"), false);
   		}
   		else
	  		HMM_HEAD->at(i)->dump_HMM(stderr, true);
   	}
}

