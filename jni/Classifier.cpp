#include "../include/Classifier.h"
#define trainNum 70
#define testNum 1

float trainingData[trainNum][165888];
int trainingLabels[trainNum];
float testingData[testNum][165888];
int testingLabels[testNum];

Classifier::Classifier()
{
    //ctor
}

Classifier::Classifier(int x)
{
    cout<<x;
}


Classifier::~Classifier()
{
    //dtor
}

void Classifier::split_training(char* line, int line_no )
{
    char* elem = strtok(line, " ");
    trainingLabels[line_no] = atoi(elem);
    elem = strtok(NULL, " ");
    int index = 0;
    while (elem != NULL)
    {
        string temp = elem;
        temp = temp.substr(temp.find(":") + 1, temp.size());
        float val = atof(temp.data());
        trainingData[line_no][index] = val;
        elem = strtok(NULL, " ");
        index++;
    }
}

void Classifier::split_testing(char* line, int line_no )
{
    char* elem = strtok(line, " ");
    testingLabels[line_no] = atoi(elem);
    elem = strtok(NULL, " ");
    int index = 0;
    while (elem != NULL)
    {
        string temp = elem;
        temp = temp.substr(temp.find(":") + 1, temp.size());
        float val = atof(temp.data());
        testingData[line_no][index] = val;
        elem = strtok(NULL, " ");
        index++;
    }
}

char* file_reader(string file_path)
{

    ifstream input_file;
    ifstream::pos_type size_of_file;
    input_file.open(file_path.c_str(), ios::in | ios::binary | ios::ate);
    if(!input_file.is_open())
        cout << "Cannot open file" << endl;
    size_of_file = input_file.tellg();
    input_file.seekg(0, ios::beg);
    string file_data;
    file_data[size_of_file] = '\0';
    input_file.read((char*)file_data.c_str(), size_of_file);
    return (char*)file_data.c_str();
}

void Classifier::fill_data(string filename, int flag)
{
    char* data_file = file_reader(filename);
    int index = 0;
    string acc = "";
    int line_no = 0;
    while (data_file[index] != '\0')
    {
        if (data_file[index] == '\n')
        {
            if(flag == 0)
            {
                split_training(&acc[0], line_no);
            }
            else
            {
                split_testing(&acc[0], line_no);
            }
            line_no++;
            acc = "";
            data_file[index] = 0;
        }
        else
        {
            acc += data_file[index];
            data_file[index] = 0;
        }
        index++;
    }
    data_file = NULL;
}



void Classifier::train(string fileName)
{
     CvSVM SVM;
    cout<<"Reading Training File..."<<endl;
    //training
    fill_data(fileName, 0);

    CvMat trainData;
    CvMat labels;
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS, 100, 0001);
    //CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
    cout<<"Reading Training File Done!"<<endl;
    cout<<"Start Training..."<<endl;
    cvInitMatHeader(& trainData, trainNum, 165888, CV_32FC1, trainingData);
    cvInitMatHeader(& labels, 1, trainNum, CV_32SC1, trainingLabels);
    SVM.train(&trainData, &labels, 0, 0, CvSVMParams(CvSVM::C_SVC, CvSVM::POLY, 1, 20, 0, 7, 0.0, 0.0, NULL,criteria));
    cout<<"Training Finished!"<<endl;
    // SVM.train(&trainData, &labels, 0, 0, CvSVMParams());
    cout<<"Saving Training Module..."<<endl;
    SVM.save("training.train", 0);


}

void Classifier::test(string fileName)
{
     CvSVM SVM;
    SVM.load("training.train", 0);
    fill_data(fileName,1);
    CvMat testMat;
    int error =0;
    for(int i = 0 ;  i< testNum ; i++)
    {
        cvInitMatHeader(&testMat, 1, 165888, CV_32FC1, testingData[i]);

        double r = SVM.predict(& testMat);

        if(r!= testingLabels[i])
            error++;

        // cout <<i <<" prediction: " << r << endl;
    }

    cout <<" errors: " << error << endl;
}

int Classifier::predictEmotion(string input)
{
    CvSVM SVM;
    SVM.load("training.train", 0);
    split_testing((char*)input.c_str(),0);
    CvMat testMat;

    cvInitMatHeader(&testMat, 1, 165888, CV_32FC1, testingData[0]);

    double r = SVM.predict(& testMat);


    return r;
}



