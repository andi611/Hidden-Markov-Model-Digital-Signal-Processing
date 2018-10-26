/****************************************************************************
  FileName     [ train_hmm.cpp ]
  Synopsis     [ Implementation of the training of a discrete hidden markov model ]
  Author       [ Ting-Wei Liu (Andi611) ]
  Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
****************************************************************************/
#include "hmm.h"


/*****************************************/
/*          Forward Declaration          */
/*****************************************/
void train(HMM* hmm, int iteration, string model_data_path);
void train_with_iteration_test(vector<HMM*>* HMM_HEAD, int iteration, string data_path);


/***********************************/
/*          Main Function          */
/***********************************/
int main(int argc, char** argv)
{	
	int iteration = atoi(argv[1]);

	// Usage: ./train 100 ../data/model_init.txt ../data/seq_model_01.txt model_01.txt
	if (argc == 5) {

		char* model_init = argv[2];
		char* model_name = argv[4];
		string data_path = argv[3];

		HMM* hmm = new HMM();
		hmm->load_HMM(model_init);
		hmm->set_name(model_name);
		train(hmm, iteration, data_path);
	}

	// Usage: ./train 100 ../data/model_init.txt ../data/ modellist.txt all
	else if (argc == 6) {

		string all = argv[5];
		assert(all == "all");
		char* model_init = argv[2];
		char* model_list = argv[4];
		string data_path = argv[3];
		cout << "Training all models in: " << model_list << endl;

		vector<HMM*>* HMM_HEAD = initialize_new_models(model_init, model_list, NUM_MODEL);
		HMM* hmm;

		for (int m = 0; m < HMM_HEAD->size(); ++m) {
			hmm = HMM_HEAD->at(m);
			string model_data_path = data_path + "seq_" + hmm->get_name();
			train(hmm, iteration, model_data_path);
		}

		vector<vector<char>> test_data = read_data("../data/testing_data1.txt");
		double acc = test(HMM_HEAD, &test_data, "../data/testing_result1.txt");
	}
	// Usage: ./train 3000 ../data/model_init.txt ../data/ modellist.txt all test
	else if (argc == 7) {

		string all = argv[5];
		string test = argv[6];
		assert(all == "all");
		assert(test == "test");
		char* model_init = argv[2];
		char* model_list = argv[4];
		string data_path = argv[3];
		cout << "Training all models with iteration-wise test in: " << model_list << endl;

		vector<HMM*>* HMM_HEAD = initialize_new_models(model_init, model_list, NUM_MODEL);
		train_with_iteration_test(HMM_HEAD, iteration, data_path);
	}
	else {
		cout << "[HMM Train Usage]:" << endl;
		cout << " - Train single model: /train iteration ../data/model_init.txt ../data/seq_model_xx.txt model_xx.txt" << endl;
		cout << " - Train all models:   /train iteration ../data/model_init.txt ../data/ modellist.txt all" << endl;
		exit(-1);
	}
	return 0;
}


/****************************************/
/*          Training Functions          */
/****************************************/
void train(HMM* hmm, int iteration, string model_data_path)
{	
	cout << endl << "===============================================" << endl;
	cout << "Training new model: " << hmm->get_name() << endl;
	vector<vector<char>> data = read_data(model_data_path);
	hmm->train_HMM(iteration, &data);
	hmm->dump_HMM(stderr, true);
	hmm->dump_HMM(open_file(hmm->get_name().c_str(), "w"), false);
}


void train_with_iteration_test(vector<HMM*>* HMM_HEAD, int iteration, string data_path)
{	
	cout << endl << "===============================================" << endl;

	// initialize variables
	HMM* hmm;
	double acc;
	double max_acc = 0;
	int max_iteration = 0;

	// initialize files
	ofstream acc_file;
	acc_file.open("../data/acc.txt");

	// read all data
	vector<vector<vector<char>>> datas;
	for (int m = 0; m < HMM_HEAD->size(); ++m) {
		hmm = HMM_HEAD->at(m);
		string model_data_path = data_path + "seq_" + hmm->get_name();
		datas.push_back(read_data(model_data_path));
	}
	vector<vector<char>> test_data = read_data("../data/testing_data1.txt");

	// train with iteration-wise test
	for (int iter = 0; iter < iteration; iter++) {
		
		// train all models 1 iteration
		for (int m = 0; m < HMM_HEAD->size(); ++m) {
			cout << "\r" << "Computing model: " << m << '/' << HMM_HEAD->size() << "..." << flush; 
			hmm = HMM_HEAD->at(m);
			hmm->train_HMM(1, &datas.at(m), false);
		}
		
		// test with all models
		acc = test(HMM_HEAD, &test_data, "../data/testing_result1.txt", false);
		acc_file << acc << ' ' << iter+1 << '\n';
		cout << "\r" << "[HMM training] Iteration: " << iter + 1 << '/' << iteration << ", Accuracy: " << acc;
		
		// save best
		if (max_acc < acc) {
			max_acc = acc;
			max_iteration = iter + 1;
			dump_models(HMM_HEAD);
			cout << " - Saved." << endl;
		}
		else cout << endl;
	}
	acc_file.close();
	cout << "\r" << "[HMM training] Iteration: " << iteration << '/' << iteration << ", Accuracy: " << acc << "... Complete." << endl;
	cout << "[HMM training] Max Accuracy " << max_acc << " achieved at Iteration: " << max_iteration << endl;
}


void HMM::train_HMM(const int& iteration, vector<vector<char>>* data, bool print_log)
{
	vector<char>* obs;

	for (int iter = 0; iter < iteration; iter++) {
		if (print_log) cout << "\r" << "HMM training: " << iter+1 << '/' << iteration << "... " << flush;
		
		// accumulaters
		vector<double>* pi_accumulate = new vector<double>(N_S, 0.0);
		vector<double>* gamma_accumulate = new vector<double>(N_S, 0.0);
		vector<vector<double>>* epsilon_accumulate = new vector<vector<double>>(N_S, vector<double>(N_S, 0.0));
		vector<vector<double>>* B_num_accumulate = new vector<vector<double>>(N_S, vector<double>(N_O, 0.0));
		vector<double>* B_den_accumulate = new vector<double>(N_S, 0.0);

		// iterate through all observation sequence in training data
		for (int s = 0; s < data->size(); ++s) {
			obs = &data->at(s);

			// computations
			vector<vector<double>>* alpha = compute_alpha(obs);
			vector<vector<double>>* beta = compute_beta(obs);
			vector<vector<double>>* gamma = compute_gamma(alpha, beta);
			vector<vector<vector<double>>>* epsilon = compute_epsilon(obs, alpha, beta);

			// update
			accumulate_update(obs, gamma, epsilon, pi_accumulate, gamma_accumulate, epsilon_accumulate, B_num_accumulate, B_den_accumulate);
			delete alpha; delete beta; delete gamma; delete epsilon;
		}
		apply_update(pi_accumulate, gamma_accumulate, epsilon_accumulate, B_num_accumulate, B_den_accumulate, (double)data->size());
		delete pi_accumulate; delete gamma_accumulate; delete epsilon_accumulate; delete B_num_accumulate; delete B_den_accumulate;
	}
	if (print_log) cout << "\r" << "HMM training: " << iteration << '/' << iteration << "... " << " Complete." << endl;
}


vector<vector<double>>* HMM::compute_alpha(vector<char>* obs) const
{
	vector<vector<double>>* alpha = new vector<vector<double>>(T, vector<double>(N_S, 0.0));
	for (int i = 0; i < N_S; ++i)
		alpha(0, i) = PI(i) * B(i, o(0));
	for (int t = 0; t < T-1; ++t)
		for (int i = 0; i < N_S; ++i) {
			for (int j = 0; j < N_S; ++j)
				alpha(t+1, i) += alpha(t, j) * A(j, i);
			alpha(t+1, i) *= B(i, o(t+1));
		}
	return alpha;
}


vector<vector<double>>* HMM::compute_beta(vector<char>* obs) const
{
	vector<vector<double>>* beta = new vector<vector<double>>(T, vector<double>(N_S, 0.0));
	for (int i = 0; i < N_S; ++i)
		beta(T-1, i) = 1.0;
	for (int t = T-2; t >= 0; --t)
		for (int i = 0; i < N_S; ++i)
			for (int j = 0; j < N_S; ++j)
				beta(t, i) += A(i, j) * B(j, o(t+1)) * beta(t+1, j);
	return beta;
}


vector<vector<double>>* HMM::compute_gamma(vector<vector<double>>* alpha, vector<vector<double>>* beta) const
{
	vector<vector<double>>* gamma = new vector<vector<double>>(alpha->size(), vector<double>(N_S, 0.0));
	for (int t = 0; t < alpha->size(); ++t) {
		double gamma_base = 0.0;
		for (int i = 0; i < N_S; ++i) {
			gamma(t, i) = (alpha(t, i) * beta(t, i));
			gamma_base += gamma(t, i);
		}
		for (int i = 0; i < N_S; ++i)
			gamma(t, i) /= gamma_base;
	}
	return gamma;
}


vector<vector<vector<double>>>* HMM::compute_epsilon(vector<char>* obs, vector<vector<double>>* alpha, vector<vector<double>>* beta) const
{
	vector<vector<vector<double>>>* epsilon = new vector<vector<vector<double>>>(T-1, vector<vector<double>>(N_S, vector<double>(N_S, 0.0)));
	for (int t = 0; t < T-1; ++t) {
		double epsilon_base = 0.0;
		for (int i = 0; i < N_S; ++i)
			for (int j = 0; j < N_S; ++j) {
				epsilon(t, i, j) = alpha(t, i) * A(i, j) * B(j, o(t+1)) * beta(t+1, j);
				epsilon_base += epsilon(t, i, j);
			}
		for (int i = 0; i < N_S; ++i)
			for (int j = 0; j < N_S; ++j)
				epsilon(t, i, j) /= epsilon_base;
	}
	return epsilon;
}


void HMM::accumulate_update(vector<char>* obs,
							vector<vector<double>>* gamma, 
							vector<vector<vector<double>>>* epsilon, 
							vector<double>* pi_accumulate,
							vector<double>* gamma_accumulate,
							vector<vector<double>>* epsilon_accumulate,
							vector<vector<double>>* B_num_accumulate,
							vector<double>* B_den_accumulate) const
{	
	for (int i = 0; i < N_S; ++i) {	
		pi_accumulate->at(i) += gamma(0, i);
		for (int t = 0; t < T-1; ++t) {	
			gamma_accumulate->at(i) += gamma(t, i);
			for (int j = 0; j < N_S; ++j)
				epsilon_accumulate->at(i).at(j) += epsilon(t, i, j);
		}
	}
	for (int i = 0; i < N_S; ++i)
		for (int t = 0; t < T; ++t) {
			B_num_accumulate->at(i).at(o(t)) += gamma(t, i);
			B_den_accumulate->at(i) += gamma(t, i);
		}
}


void HMM::apply_update(vector<double>* pi_accumulate,
					   vector<double>* gamma_accumulate,
					   vector<vector<double>>* epsilon_accumulate,
					   vector<vector<double>>* B_num_accumulate,
					   vector<double>* B_den_accumulate,
					   double num_sample)
{
	// update pi
	for (int i = 0; i < N_S; i++)
		PI(i) = pi_accumulate->at(i) / num_sample;
	// update _transition
	for (int i = 0; i < N_S; ++i)
		for (int j = 0; j < N_S; ++j)
			A(i, j) = epsilon_accumulate->at(i).at(j) / gamma_accumulate->at(i);
	// update _observation
	for (int i = 0; i < N_S; ++i)
		for (int t = 0; t < N_O; ++t)
			B(i, t) = B_num_accumulate->at(i).at(t) / B_den_accumulate->at(i);
}
