# Digital Signal Processing: Discrete HMM
- **Discrete Hidden Markov Model Implementation in C++**

## Environment
* **< g++ [gcc version 7.3.0 (GCC)] >**
* **< g++ [gcc version 4.2.1 (GCC)] >**

## Algorithm
* **Initialization**: Set θ=(A,B,π) \theta =(A,B,\pi ) with random initial conditions
* **Forward**:

## File Description
```
.
├── src/
|   ├── Makefile                g++ compiler make file
|   ├── hmm.h                   HMM implementation
|   ├── hmm.cpp                 HMM implementation
|   ├── test_hmm.cpp            Testing algorithm implementation
|   ├── train_hmm.cpp           Training algorithm implementation
|   ├── test                    Unix executable binary code for test_hmm.cpp
|   ├── train                   Unix executable binary code for train_hmm.cpp
|   ├── model_01~05.txt         Trained models
|   └── modellist.txt           Model name list
├── data/
|   ├── model_init.txt          Initial model for training
|   ├── seq_model_01~05.txt     Training data (observation sequences)
|   ├── testing_data1.txt       Testing data (observation sequences)
|   ├── testing_answer.txt      Real answer for "testing_data1.txt"
|   ├── testing_result.txt      Model generated answer for "testing_data1.txt"
|   └── testing_data2.txt       Testing data without answer
└── problem_description.pdf     Work Spec
└── Readme.md                   This File
```

## Usage
### Train all 5 models separately then test
```
└── src/
    ├── make clean
    ├── make
    ├── ./train #iteration ../data/model_init.txt ../data/seq_model_01.txt model_01.txt
    ├── ./train #iteration ../data/model_init.txt ../data/seq_model_02.txt model_02.txt
    ├── ./train #iteration ../data/model_init.txt ../data/seq_model_03.txt model_03.txt
    ├── ./train #iteration ../data/model_init.txt ../data/seq_model_04.txt model_04.txt
    ├── ./train #iteration ../data/model_init.txt ../data/seq_model_05.txt model_05.txt
    └── ./test modellist.txt ../data/testing_data1.txt ../data/testing_result1.txt
- #iteration is positive integer, which is the iteration of the Baum-Welch algorithm.
```
### Train all 5 models at once then test
```
└── src/
    ├── make clean
    ├── make
    ├──  ./train 250 ../data/model_init.txt ../data/ modellist.txt all
    └── ./test modellist.txt ../data/testing_data1.txt ../data/testing_result1.txt
- #iteration is positive integer, which is the iteration of the Baum-Welch algorithm.
```
### Train all 5 models at once, and calculate models' test score every iteration
```
└── src/
    ├── make clean
    ├── make
    ├── ./train 250 ../data/model_init.txt ../data/ modellist.txt all test
    └── ./test modellist.txt ../data/testing_data1.txt ../data/testing_result1.txt
- #iteration is positive integer, which is the iteration of the Baum-Welch algorithm.
```

## Experinment: Iteration v.s. Accuracy Plot
![]()
