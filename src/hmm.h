/****************************************************************************
  FileName     [ hmm.h ]
  Synopsis     [ Implementation of discrete hidden markov model ]
  Author       [ Ting-Wei Liu (Andi611) ]
  Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
****************************************************************************/
#ifndef HMM_HEADER_
#define HMM_HEADER_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <float.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cassert>
using namespace std;


/**************************************/
/*          Macro Definition          */
/**************************************/
#ifndef MAX_STATE
#define MAX_STATE 	10
#endif
#ifndef MAX_OBSERV
#define MAX_OBSERV	6
#endif
#ifndef MAX_SEQ
#define	MAX_SEQ		200
#endif
#ifndef MAX_LINE
#define MAX_LINE 	256
#endif
#ifndef NUM_MODEL
#define NUM_MODEL 	5
#endif
/**************************************/
#ifndef N_S
#define N_S _state_num
#endif
#ifndef N_O
#define N_O _observ_num
#endif
#ifndef PI
#define PI(i) _initial.at(i) 							// shape: (max state)
#endif
#ifndef A
#define A(i, j) _transition.at(i).at(j) 				// shape: (max state, max state)
#endif
#ifndef B
#define B(s, o) _observation.at(s).at(o) 				// shape: (max state, max observation)
#endif
#ifndef alpha
#define alpha(t, s) alpha->at(t).at(s)					// shape: (max sequence len T, num state)
#endif
#ifndef beta
#define beta(t, s) beta->at(t).at(s)					// shape: (max sequence len T, num state)
#endif
#ifndef gamma
#define gamma(t, s) gamma->at(t).at(s)					// shape: (max sequence len T, num state)
#endif
#ifndef epsilon
#define epsilon(t, i, j) epsilon->at(t).at(i).at(j)		// shape: (max sequence len T, num state)
#endif
#ifndef delta
#define delta(t, s) delta->at(t).at(s)					// shape: (max sequence len T, num state)
#endif
#ifndef o
#define o(t) obs->at(t) - 'A'							// char to index mapping: 'A' -> 0, 'B' -> 1, ... etc
#endif
#ifndef T
#define T obs->size()									// single observation sample length
#endif


/******************************************/
/*        class Hidden Markov Model       */
/******************************************/
class HMM 
{
public:
	// Constructor
	HMM(bool max_allocate = false){ if (max_allocate) initialize_HMM(); _max_allocate = max_allocate; };

	// model I/O
	void load_HMM(const char* filename);
	void dump_HMM(FILE *fp, bool display=false) const;

	// set attribute
	string get_name() const { return _model_name; }
	void set_name(const char* name) { string str(name); _model_name = str; }

	// training & testing
	void train_HMM(const int& iteration, vector<vector<char>>* data, bool print_log=true); 
	double test_HMM(vector<char>* obs) const;

private:
	void initialize_HMM();

	// compute functions
	vector<vector<double>>* compute_alpha(vector<char>* obs) const;
	vector<vector<double>>* compute_beta(vector<char>* obs) const;
	vector<vector<double>>* compute_gamma(vector<vector<double>>* alpha, vector<vector<double>>* beta) const;
	vector<vector<vector<double>>>* compute_epsilon(vector<char>* obs, vector<vector<double>>* alpha, vector<vector<double>>* beta) const;


	// update functions
	void accumulate_update(vector<char>* obs,
					  	   vector<vector<double>>* gamma, 
					 	   vector<vector<vector<double>>>* epsilon, 
						   vector<double>* pi_accumulate,
						   vector<double>* gamma_accumulate,
						   vector<vector<double>>* epsilon_accumulate,
						   vector<vector<double>>* B_num_accumulate,
						   vector<double>* B_den_accumulate) const;
	void apply_update(vector<double>* pi_accumulate,
					  vector<double>* gamma_accumulate,
					  vector<vector<double>>* epsilon_accumulate,
					  vector<vector<double>>* B_num_accumulate,
					  vector<double>* B_den_accumulate,
					  double num_sample);

	// class members
	bool						_max_allocate;  // allocate maximum memory
	string 						_model_name;	// model name
	int 						_state_num; 	// number of state
	int 						_observ_num;	// number of observation
	vector<double>				_initial;		// initial prob, shape: (max state)
	vector<vector<double>> 		_transition;	// transition prob, shape: (max state, max state)
	vector<vector<double>> 		_observation;	// observation prob, shape: (max state, max observation)
};

/***************************************/
/*          Testing Functions          */
/***************************************/
double test(vector<HMM*>* HMM_HEAD, vector<vector<char>>* data, const string& output_path, bool print_log=true);
double compute_acc(const string& answer_path, const string& result_path);


/*************************************/
/*           I/O Functions           */
/*************************************/
FILE* open_file(const char* filename, const char* ht);
vector<vector<char>> read_data(const string& data_path);
vector<HMM*>* load_models(const char* model_list, const int max_num);
vector<HMM*>* initialize_new_models(const char* model_init, const char* model_list, const int max_num);
void dump_models(vector<HMM*>* HMM_HEAD, bool to_file=true);


#endif