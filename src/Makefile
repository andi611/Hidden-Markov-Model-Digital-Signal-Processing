.PHONY: clean all

TARGET1 = test_hmm
TARGET2 = train_hmm

# type make/make all to compile test_hmm
all: $(TARGET1) $(TARGET2)

CC = g++
CFLAGS = -ansi -std=c++11 -O3 
LDFLAGS = -lm # link to math library

$(TARGET1): test_hmm.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET1) $(TARGET1).cpp

$(TARGET2): train_hmm.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET2) $(TARGET2).cpp

# type make clean to remove the compiled file
clean:
	$(RM) $(TARGET1) $(TARGET2)
