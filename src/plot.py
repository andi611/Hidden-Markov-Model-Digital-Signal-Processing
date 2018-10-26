# -*- coding: utf-8 -*- #
"""*********************************************************************************************"""
#   FileName     [ plot.py ]
#   Synopsis     [ Implement of two basic algorithms to perform frequent pattern mining: 1. Apriori, 2. Eclat. 
#   			   Find all itemsets with support > min_support. ]
#   Author       [ Ting-Wei Liu (Andi611) ]
#   Copyright    [ Copyleft(c), NTUEE, NTU, Taiwan ]
"""*********************************************************************************************"""


###############
# IMPORTATION #
###############
import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt


##################
# CONFIGURATIONS #
##################
def get_config():
	parser = argparse.ArgumentParser(description='plot argument parser')
	parser.add_argument('--input_path', type=str, default='../data/training_log.txt', help='input data path')
	parser.add_argument('--output_path', type=str, default='../data/acc.jpeg', help='output data path')
	args = parser.parse_args()
	return args


#############
# READ DATA #
#############
def read_data(data_path, skip_header=False):
	x_data = []
	y_data = []
	if not os.path.isfile(data_path): raise ValueError('Invalid data path.')
	with open(data_path, 'r', encoding='utf-8') as f:
		file = csv.reader(f, delimiter=' ', quotechar='\r')
		if skip_header: next(file, None)  # skip the headers
		for row in file:
			x_data.append(int(row[1]))
			y_data.append(float(row[0]))
	return x_data, y_data


########
# MAIN #
########
"""
	main function that runs the two algorithms
"""
def main():
	args = get_config()
	x_data, y_data = read_data(args.input_path)

	fig = plt.figure(figsize=(20,7))
	plt.plot(x_data, y_data, 'o')
	plt.ylabel('Accuracy')
	plt.xlabel('Iteration')
	plt.title('Iteration v.s. Iteration')
	plt.grid()
	plt.tight_layout()
	plt.savefig(args.output_path)
	plt.close(fig)

if __name__ == '__main__':
	main()

