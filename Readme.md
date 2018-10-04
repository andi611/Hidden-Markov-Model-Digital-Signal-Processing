# Discrete_HMM_Cplusplus
**Discrete Hidden Markov Model Implementation in C++**
**Work in Progress**

## ====== File description ====== 
+- dsp_hw1/ 
   +-  src/
   |     +-                    some hmm program
   +-  modellist.txt           model list to train
   +-  model_init.txt          initial model for training
   +-  seq_model_01~05.txt     training data observation
   +-  testing_data1.txt       testing data  observation
   +-  testing_answer.txt      answer for "testing_data1.txt"
   +-  testing_data2.txt       testing data without answer
-
## ====== Program Execute ======
src:
 make
 ./train $iter model_init.txt seq_model_01.txt model_01.txt
 ./train $iter model_init.txt seq_model_02.txt model_02.txt
 ./train $iter model_init.txt seq_model_03.txt model_03.txt
 ./train $iter model_init.txt seq_model_04.txt model_04.txt
 ./train $iter model_init.txt seq_model_05.txt model_05.txt
 ./test modellist.txt testing_data1.txt result1.txt
 ./test modellist.txt testing_data2.txt result2.txt
$iter is positive integer, which is iteration of Baum-Welch algorithm.
-
## ====== Handout  ======
  1. Include all program files  ( train.c, test.c, etc. )
  2. trained HMM:    model_01.txt ~ model_05.txt
  3. result1.txt , result2.txt
  4. acc.txt : The accuracy of testing_data.txt
  5. Document ( including your name, student ID, compile and run-time environment, iterations and what you learned from this homework. )
  
  * upload to Cieba *
     Compress all your files to hw1_[studentID].zip
     with the following format! ( Attention! There is a directory in the zip, name after your student ID.

     hw1_[studentID].zip
     +- hw1_[studentID]
        +- train.c/.cpp
        +- test.c/.cpp
        +- Makefile
        +- model_01~05.txt
        +- result1~2.txt 
        +- acc.txt
        +- Document.pdf (pdf)
