#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include<string>




using namespace std;
using namespace cv;

class Classifier
{
public:
    Classifier();
    Classifier(int x);
    virtual ~Classifier();
    void classify(string modelFileName, string testFileName);
    void train(string trainFileName);
    void test(string testFileName);
    int predictEmotion(string fileName);

protected:
private:
    void split_training(char* line, int line_no );
    void split_testing(char* line, int line_no );
    void fill_data(string filename, int flag) ;

};

#endif // CLASSIFIER_H
