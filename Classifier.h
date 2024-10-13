#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include "DSString.h"

using namespace std;

class Classifier {

private:
    vector<DSString> words; // words from training data
    vector<int> sentiments; // sentiments from training data
    vector<DSString> testingID;
    vector<DSString> predictions; // sentiment predictions from testing data
    vector<DSString> stopWords;
    double classifyingStart; // start time of classification

public:
    Classifier();

    void train(char* fileName);
    void predict(char* fileName);
    void evaluatePredictions(char* testingSentimentFile, char* resultsFile, char* accuracyFile);

    vector<DSString> tokenize(DSString& tweet);

    int findWord(DSString word, vector<DSString> vector);
    void readStopWords();
};

#endif