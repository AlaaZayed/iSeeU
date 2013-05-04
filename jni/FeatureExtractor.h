#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <iostream>
#include <vector>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <math.h>
#include <complex>
#include <sstream>
#include <string>

#define _USE_MATH_DEFINES

using namespace cv;
using namespace std;
using namespace gpu;



class FeatureExtractor
{
public:
    FeatureExtractor();
    virtual ~FeatureExtractor();
    string extract(string imgName);
    string extractFeatures(Mat img);
    Mat faceDetection(string imgName);
    Mat faceDetection(Mat img);
protected:
private:
    Mat realFilters[9][8];
    Mat imagFilters[9][8];
    int SCALES;
    int ORIENTATIIONS;
    int kernelWidth;
    void createKernels(int width);
    void createGaborFilters(int width);
    void opencvGaborFilters(int width);
    string applyKernels(Mat face);
};

#endif // FEATUREEXTRACTOR_H
