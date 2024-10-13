#include "Classifier.h"

Classifier::Classifier() {}

void Classifier::train(char* fileName) {
    auto start = chrono::high_resolution_clock::now();
    readStopWords();

    bool isPositive;
    ifstream inFS(fileName);
    char line[1000];

    // read training file
    if (!inFS.is_open())
        cerr << "Error reading input file\n";

    inFS.getline(line,1000); // ignore header text in data

    while(inFS.getline(line, 1000, ',')) {;
        // store sentiments in isPositive
        DSString sentiment(line);
        if(sentiment[0] == '0')
            isPositive = 0;
        else
            isPositive = 1;

        // read tweets
        for (size_t i = 0; i < 4; i++)
            inFS.getline(line, 1000, ',');
        inFS.getline(line, 1000);
        DSString tweet(line);

        // clean and tokenize tweet
        tweet = tweet.toLower();
        tweet = tweet.removeSymbols();
        vector<DSString> tempWords = tokenize(tweet);

        // increments or decrements word's sentiment value based on tweet's sentiment value
        for (size_t i = 0; i < tempWords.size(); i++) {
            tempWords.at(i) = tempWords.at(i).stem();
            if (findWord(tempWords.at(i),stopWords) == -1) {
                if (findWord(tempWords.at(i), words) == -1) {
                    words.push_back(tempWords.at(i));
                    if (isPositive)
                        sentiments.push_back(1);
                    else
                        sentiments.push_back(-1);
                }
                else {
                    int index = findWord(tempWords.at(i), words);
                    if (isPositive)
                        (sentiments.at(index))++;
                    else
                        (sentiments.at(index))--;
                }
            }
        }
    }
    inFS.close();
    auto end = chrono::high_resolution_clock::now();
    cout << "Training execution time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds\n";
}

void Classifier::predict(char* fileName) {
    auto start = chrono::high_resolution_clock::now();
    ifstream inFS(fileName);
    char line[1000];

    if (!inFS.is_open())
        cerr << "Error reading input file\n";

    inFS.getline(line,1000); // ignore header text in data
    while(inFS.getline(line, 1000, ',')) {
        // stores testing IDs in vector
        DSString id(line);
        testingID.push_back(id);

        // stores tweet in DSString
        for (size_t i = 0; i < 3; i++)
            inFS.getline(line, 1000, ',');
        inFS.getline(line, 1000);
        DSString tweet(line);

        // cleans and tokenizes tweet
        tweet = tweet.toLower();
        tweet = tweet.removeSymbols();
        vector<DSString> tempWords = tokenize(tweet);

        // predicts sentiment values of tweet based on sentiments of words in training data
        int prediction = 0;
        for (size_t i = 0; i < tempWords.size(); i++) {
            tempWords.at(i) = tempWords.at(i).stem();
            int index = findWord(tempWords.at(i), words);
            if (index != -1) {
                if (sentiments.at(index) > 0)
                    prediction++;
                else if (sentiments.at(index) < 0)
                    prediction--;
            }
        }

        // adds prediction to vector based on total prediction number
        if (prediction >= 0)
            predictions.push_back("4");
        else if (prediction < 0)
            predictions.push_back("0");
        else
            predictions.push_back("Error");
    }

    inFS.close();
    auto end = chrono::high_resolution_clock::now();
    cout << "Classification execution time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds\n";
}

void Classifier::evaluatePredictions(char* testingSentimentFile, char* resultsFileName, char* accuracyFileName) {
    vector<DSString> sentimentsAnswers;
    double correctCount;
    size_t i = 0;

    char line[1000];
    ifstream inFS(testingSentimentFile);
    ofstream resultsFile(resultsFileName);

    if (!inFS.is_open())
        cerr << "Error reading input file\n";

    inFS.getline(line,1000); // ignore header text in data
    while(inFS.getline(line, 1000, ',')) {
        // assign first char of each line to sentimentAnswers
        DSString sentiment(line);
        sentimentsAnswers.push_back(sentiment);

        // compare predictions and sentimentAnswers
        if (sentimentsAnswers.at(i) == predictions.at(i))
            correctCount++;

        // writes to results file
        resultsFile << predictions.at(i) << "," << testingID.at(i) << "\n";

        inFS.getline(line, 1000);
        i++;
    }

    inFS.close();
    resultsFile.close();

    double accuracy = correctCount / sentimentsAnswers.size();

    // write to accuracyFile
    ofstream accuracyFile(accuracyFileName);
    accuracyFile << fixed << setprecision(3) << accuracy << "\n";
    for (size_t i = 0; i < predictions.size(); i++) {
        if (!(predictions.at(i) == sentimentsAnswers.at(i)))
            accuracyFile << predictions.at(i) << "," << sentimentsAnswers.at(i) << "," << testingID.at(i) << "\n";
    }
    accuracyFile.close();
}

vector<DSString> Classifier::tokenize(DSString& tweet) { // splits tweet into tokens and returns vector of tokens
    size_t start = 0;
    vector<DSString> words;
    DSString token;

    for (size_t i = 0; i < tweet.length(); i++) {
        if (tweet[i] == ' ' || i == tweet.length()-1) {
            token = tweet.substring(start, i-start);
            start = i+1;
            words.push_back(token);
        }
    }

    return words;
}

// searches vector for word and returns index if found or -1 if not found
int Classifier::findWord(DSString word, vector<DSString> vector) {
    for (size_t i = 0; i < vector.size(); i++) {
        if (word == vector.at(i))
            return i;
    }
    return -1;
}

void Classifier::readStopWords() { // reads txt file of stop words
    ifstream inFS("../data/stopwords.txt");
    char line[200];

    if (!inFS.is_open())
        cerr << "Error reading input file\n";
    
    while(inFS.getline(line,200)) {
        DSString temp(line);
        stopWords.push_back(temp);
    }

    inFS.close();
}