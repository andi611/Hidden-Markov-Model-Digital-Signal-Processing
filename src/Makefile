.PHONY: clean all

TARGET1 = test
TARGET2 = train

# type make/make all to compile test_hmm
all: $(TARGET1) $(TARGET2)

CC = g++
CFLAGS = -ansi -std=c++11 -O3 
LDFLAGS = -lm # link to math library

$(TARGET1): test_hmm.cpp
	$(CC) hmm.cpp test_hmm.cpp $(CFLAGS) $(LDFLAGS) -o $(TARGET1) 

$(TARGET2): train_hmm.cpp
	$(CC) hmm.cpp train_hmm.cpp $(CFLAGS) $(LDFLAGS) -o $(TARGET2) 

# type make clean to remove the compiled file
clean:
	$(RM) $(TARGET1) $(TARGET2)
