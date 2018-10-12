#ifndef HMM_HEADER_
#define HMM_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <float.h>
using namespace std;

#ifndef MAX_STATE
#	define MAX_STATE 	10
#endif

#ifndef MAX_OBSERV
#	define MAX_OBSERV	26
#endif

#ifndef MAX_SEQ
#	define	MAX_SEQ		200
#endif

#ifndef MAX_LINE
#	define MAX_LINE 	256
#endif

#ifndef NUM_MODEL
#	define NUM_MODEL 	5
#endif

FILE* open_or_die(const char* filename, const char* ht);

/******************************************/
/*        class Hidden Markov Model       */
/******************************************/
class HMM 
{
public:
	// Constructor
	HMM() { initialize_HMM(); };

	// model I/O
	void load_HMM (const char *filename);	// load model
	void dump_HMM (FILE *fp, bool display=false) const;			// store model

	string get_name() const { return _model_name; }
	void set_name(const string name) { _model_name = name; }

private:
	// private functions
	void initialize_HMM();


    // helper functions


	// class members
	string 						_model_name;
	int 						_state_num; 	//number of state
	int 						_observ_num;	//number of observation
	vector<double>				_initial;		//initial prob
	vector< vector<double> > 	_transition;	//transition prob
	vector< vector<double> > 	_observation;	//observation prob


};


void HMM::initialize_HMM () 
{
	_initial = vector<double> (MAX_STATE, FLT_MAX);
	_transition = vector< vector<double> >(MAX_STATE, vector<double>(MAX_STATE, FLT_MAX));
	_observation = vector< vector<double> >(MAX_OBSERV, vector<double>(MAX_STATE, FLT_MAX));
}


void HMM::load_HMM(const char* filename)
{
   	int i, j;
   	FILE* fp = open_or_die(filename, "r");

   	string str(filename);
   	_model_name = str;

   	char token[MAX_LINE] = "";
   	while(fscanf( fp, "%s", token ) > 0 )
   	{
	  	if(token[0] == '\0' || token[0] == '\n' ) continue;

	  	if(strcmp( token, "initial:" ) == 0 ) {
		 	fscanf(fp, "%d", &_state_num );
		 	for(i = 0 ; i < _state_num ; i++ )
				fscanf(fp, "%lf", &( _initial[i] ) );
	  	}
	  	else if(strcmp( token, "transition:" ) == 0 ) {
		 	fscanf(fp, "%d", &_state_num );
		 	for(i = 0 ; i < _state_num ; i++ )
				for(j = 0 ; j < _state_num ; j++ )
			   		fscanf(fp, "%lf", &(_transition[i][j]));
	  	}
	  	else if(strcmp( token, "observation:" ) == 0 ) {
		 	fscanf(fp, "%d", &_observ_num );
		 	for(i = 0 ; i < _observ_num ; i++ )
				for(j = 0 ; j < _state_num ; j++ )
			   		fscanf(fp, "%lf", &( _observation[i][j]) );
	  	}
   	}
}


void HMM::dump_HMM(FILE *fp, bool display) const
{
   	int i, j;

   	if (display) {
   		fprintf(fp, "\n===============================================\n");
   		fprintf(fp, "model name: %s\n", _model_name.c_str() );
   	}
   	fprintf(fp, "initial: %d\n", _state_num );
   	for(i = 0 ; i < _state_num - 1; i++ )
	  	fprintf(fp, "%.5lf ", _initial[i]);
   	fprintf(fp, "%.5lf\n", _initial[ _state_num - 1 ] );

   	fprintf(fp, "\ntransition: %d\n", _state_num );
   	for(i = 0 ; i < _state_num ; i++ ) {
	 	for(j = 0 ; j < _state_num - 1 ; j++ )
		 	fprintf( fp, "%.5lf ", _transition[i][j] );
	  	fprintf(fp,"%.5lf\n", _transition[i][_state_num - 1]);
   	}
   	fprintf(fp, "\nobservation: %d\n", _observ_num );
   	for(i = 0 ; i < _observ_num ; i++ ) {
	  	for(j = 0 ; j < _state_num - 1 ; j++ )
		 	fprintf( fp, "%.5lf ", _observation[i][j] );
	  	fprintf(fp,"%.5lf\n", _observation[i][_state_num - 1]);
   	}
}


/*************************************/
/*        I/O functions        */
/*************************************/
FILE* open_or_die(const char* filename, const char* ht)
{
   	FILE* fp = fopen(filename, ht);
   	if( fp == NULL ){
	  	perror(filename);
	  	exit(1);
   	}
   	return fp;
}


vector<HMM*>* load_models(const char* model_list, const int max_num)
{
   	FILE *fp = open_or_die(model_list, "r");

   	int count = 0;
   	char model_name[MAX_LINE] = "";
   	vector<HMM*>* HMM_HEAD = new vector<HMM*>();
   	HMM* hmm;

   	while(fscanf(fp, "%s", model_name) == 1) {
   		hmm = new HMM();
	  	hmm->load_HMM(model_name);
	  	HMM_HEAD->push_back(hmm);
	  	++count;
	  	if(count >= max_num)
		 	return HMM_HEAD;
   	}
   	fclose(fp);
   	return HMM_HEAD;
}


vector<HMM*>* load_new_models(const char* model_init, const char* model_list, const int max_num)
{	
	cout << "Initializing new models for training..." << endl;
   	FILE *fp = open_or_die(model_list, "r");

   	int count = 0;
   	char model_name[MAX_LINE] = "";
   	vector<HMM*>* HMM_HEAD = new vector<HMM*>();
   	HMM* hmm;

   	while(fscanf(fp, "%s", model_name) == 1) {
   		hmm = new HMM();
	  	hmm->load_HMM(model_init);
	  	string str(model_name);
	  	hmm->set_name(str);
	  	HMM_HEAD->push_back(hmm);
	  	++count;
	  	if(count >= max_num)
		 	return HMM_HEAD;
   	}
   	fclose(fp);
   	return HMM_HEAD;
}


void dump_models(vector<HMM*>* HMM_HEAD, const int num, bool to_file)
{
   	for(int i = 0; i < num ; ++i ) { 
   		if (to_file) {
	  		FILE* fp = open_or_die(HMM_HEAD->at(i)->get_name().c_str(), "w");
	  		HMM_HEAD->at(i)->dump_HMM(fp, false);
   		}
   		else
	  		HMM_HEAD->at(i)->dump_HMM(stderr, true);
   	}
}


#endif

