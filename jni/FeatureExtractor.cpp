#include "..\include\FeatureExtractor.h"

FeatureExtractor::FeatureExtractor()
{
    //ctor
    SCALES  = 9;
    ORIENTATIIONS = 8;
    kernelWidth = 10;
    createGaborFilters(kernelWidth);
}

FeatureExtractor::~FeatureExtractor()
{
    //dtor
}



/**detect face then crop and resize it*/
Mat FeatureExtractor::faceDetection(string imgName)
{
    vector<Rect> faces;
    Mat newImg;
    Mat image= imread(imgName);
    Mat img= imread(imgName);
    Mat gray;
    cvtColor(image,gray,CV_RGB2GRAY);

    //loading face detection classifier
    CascadeClassifier cascade;
    cascade.load("haarcascade_frontalface_alt.xml");
    cascade.detectMultiScale(gray,faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(60,60) );
    //if face was detected operate on it
    if(faces.size()>=1)
    {
        Rect face = faces.at(0);
        //crop image
        newImg = Mat(img,face);
        //resize new face image
        Mat sImg = Mat(48,48, image.depth());
        resize(newImg, sImg,sImg.size());
        Mat sImgGray;
        cvtColor(sImg,sImgGray,CV_RGB2GRAY);
        return sImgGray;
    }

    return Mat(0,0,CV_8S);
}

/**detect face then crop and resize it*/
Mat FeatureExtractor::faceDetection(Mat img)
{
    vector<Rect> faces;
    Mat newImg;
    Mat image= img;
    Mat gray;
    cvtColor(image,gray,CV_RGB2GRAY);

    //loading face detection classifier
    CascadeClassifier cascade;
    cascade.load("haarcascade_frontalface_alt.xml");
    cascade.detectMultiScale(gray,faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(60,60) );
    //if face was detected operate on it
    if(faces.size()>=1)
    {
        Rect face = faces.at(0);
        //crop image
        newImg = Mat(img,face);
        //resize new face image
        Mat sImg = Mat(48,48, image.depth());
        resize(newImg, sImg,sImg.size());
        Mat sImgGray;
        cvtColor(sImg,sImgGray,CV_RGB2GRAY);
        return sImgGray;
    }

    return Mat(0,0,CV_8S);
}

void FeatureExtractor::createGaborFilters(int kernel_size)
{

    double sig = 5;
    double ps = 90;
    int hks = (kernel_size-1)/2;
    double theta;
    double psi = ps*CV_PI/180;
    double del = 2.0/(kernel_size-1);
    double lmbd ;
    double sigma = sig/kernel_size;
    double x_theta;
    double y_theta;

    for(int v = 0; v < SCALES ; v++)
    {
        lmbd = 0.5+(v*6)/100.0;
        for(int u = 0; u < ORIENTATIIONS ; u++)
        {
            theta = (20*u)*CV_PI/180;
            realFilters[v][u]= Mat::zeros(kernel_size,kernel_size, CV_32F);
            imagFilters[v][u]= Mat::zeros(kernel_size,kernel_size, CV_32F);
            for (int y=-hks; y<=hks; y++)
            {
                for (int x=-hks; x<=hks; x++)
                {
                    x_theta = x*del*cos(theta)+y*del*sin(theta);
                    y_theta = -x*del*sin(theta)+y*del*cos(theta);
                    realFilters[v][u].at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
                }
            }

        }
    }
}
void FeatureExtractor::opencvGaborFilters(int width)
{
    double sigma = 3;
    double theta;
    double lambd;
    double gamma = 1;
    double psi = M_PI;

    for(int v = 0 ; v < SCALES ; v++ )
    {
        for (int u = 0 ; u < ORIENTATIIONS ; u ++)
        {
            theta =  u * (M_PI/8);
            lambd = M_PI/pow(2,v);

            realFilters[v][u] = getGaborKernel(Size(width,width),sigma,theta,lambd,gamma,psi);
            imagFilters[v][u] = Mat::zeros(width, width , CV_32FC1);

            stringstream name;
            name<< "filters/"<<v<<"-"<<u<<".png";
            imwrite(name.str(), realFilters[v][u]);
        }

    }


}
/**create matrix of kernels of 9 scales and 8 orientations*/
void FeatureExtractor::createKernels(int width)
{
    int x;
    int y;
    double term1;
    double term2;
    double term3;
    double F = sqrt(2.0);
    double Kmax = M_PI/2;
    double K;
    double PHI;
    double SIGMA = 2* M_PI;
    Mat mag[SCALES][ORIENTATIIONS];

    for(int v = 0 ; v < SCALES ; v++ )
    {
        for (int u = 0 ; u < ORIENTATIIONS ; u ++)
        {
            for(int i = 0 ; i < width ; i++ )
            {
                for (int j = 0 ; j < width ; j ++)
                {
                    realFilters[v][u] = Mat::Mat(width, width , CV_32FC1);
                    imagFilters[v][u] = Mat::Mat(width, width , CV_32FC1);
                    mag[v][u] = Mat::Mat(width, width , CV_32FC1);
                    K = Kmax /pow(F, (double)(v/2));
                    PHI =  u * (M_PI/8);
                    x = i-(width-1)/2;
                    y = j-(width-1)/2;
                    term1 = (pow(K,2)/pow(SIGMA,2))*exp(-(pow((double)x,2)+pow((double)y,2))*pow(K,2)/(2*pow(SIGMA,2)));
                    term2 = cos(K*cos(PHI)*x + K*sin(PHI)*y) - exp(-(pow(SIGMA,2)/2));
                    term3 = sin(K*cos(PHI)*x + K*sin(PHI)*y);

                    realFilters[v][u] .at<float>(i,j) = (float) term1*term2;

                    imagFilters[v][u] .at<float>(i,j) = (float) term1*term3;

                    mag[v][u].at<float>(i,j) = (float) sqrt(pow(realFilters[v][u].at<float>(i,j),2) + pow( imagFilters[v][u].at<float>(i,j),2) );
                }
            }


            //saving filters
            stringstream name;
            name<< "filters/"<<v<<"-"<<u<<".png";
            imwrite(name.str(), mag[v][u]);
        }

    }

}

/**apply kernels on a given images and save them into output folder*/
string FeatureExtractor::applyKernels(Mat face)
{
    int width = face.rows;
    int height = face.cols;
    double mag;
    double real;
    double imag;
    Mat outReal; //real part of output image
    Mat outImag; //imaginary part of output image
    Mat outImg (width, height, CV_32FC1);
    Mat newImg (width, height, CV_32FC1);
    Mat featureVector (165888, 1, CV_32FC1);
    Mat featureNormalized (165888, 1, CV_32FC1);
    string result= "";
    stringstream  ss ;
    int attributeNum = 1;
    int featureNum = 0;
    for(int v = 0 ; v < SCALES ; v++ )
    {
        for (int u = 0 ; u < ORIENTATIIONS ; u ++)
        {
//            Mat realFilter (kernelWidth ,kernelWidth, CV_32FC1);
//            Mat imagFilter (kernelWidth ,kernelWidth, CV_32FC1);
//            flip(realFilters[v][u],realFilter , -1 );
//            flip(imagFilters[v][u],imagFilter , -1 );
            //result of applying real filter
            filter2D(face , outReal,CV_32F,realFilters[v][u] );
            //result of applying real filter
            filter2D(face , outImag,CV_32F,imagFilters[v][u] );
            //calculating magnitude of real , imag results
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    real = outReal.at<float>(i,j);
                    imag = outImag.at<float>(i,j);
                    mag = sqrt(real*real + imag*imag);
                    outImg.at<float>(i,j) = (float) mag;
                    featureVector.at<float>(featureNum,0) = (float) mag;
                    featureNum++;

                }
            }

            float elem;
            // ss<<fixed;
            //ss.precision(4);
//            normalize(outImg, outImg, 1, 0, NORM_L2, CV_32F);
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {

                    elem = outImg.at<float>(i,j);

                    ss<<" "<<attributeNum<<":"<<elem;
                    attributeNum++;

                }

//
            }
            //saving output image
            stringstream name;
            outImg /= 2.;
            outImg += 0.5;
            name<< "output/"<<v<<"-"<<u<<".png";
            imwrite(name.str(), outImg);
        }

    }

//    float elem;
//    // ss<<fixed;
//    //ss.precision(4);
//    normalize(featureVector, featureNormalized, 1, 0, NORM_L2, CV_32F);
//    featureNum = 0;
//    for (int i = 0; i < 165888; i++)
//    {
//
//        elem = featureNormalized.at<float>(i,0);
//        ss<<" "<<i+1<<":"<<elem;
//    }



    result = ss.str();
    return result;
}

string FeatureExtractor::extract(string imgName)
{

    //detecting face
    Mat face = faceDetection(imgName);
    string result= "";
    //if face was detected
    if(face.rows != 0)
    {

        //applying filters
        result = applyKernels(face);

    }
    return result;
}

string FeatureExtractor::extractFeatures(Mat img)
{

    //detecting face
    Mat face = faceDetection(img);
    string result= "";
    //if face was detected
    if(face.rows != 0)
    {
        imwrite("result.png",face);
        //applying filters
        result = applyKernels(face);
        stringstream ss;
        ss<<"?"<<result;
        result = ss.str();
    }
    return result;
}
