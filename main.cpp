#include <iostream>
#include "Classifier.h"

using namespace std;

int main(int argc, char** argv)
{  
    if (argc != 6) {
        cerr << "Error: input 5 arguments\n";
        return 0;
    }

    Classifier classifier;

    classifier.train(argv[1]); //"data/train_dataset_20k.csv"
    classifier.predict(argv[2]); // "data/test_dataset_10k.csv"
    classifier.evaluatePredictions(argv[3], argv[4], argv[5]); // "data/test_dataset_sentiment_10k.csv", "results.csv", "accuracy.txt"

    return 0;
}