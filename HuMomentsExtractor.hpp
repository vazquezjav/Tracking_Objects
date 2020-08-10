#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>
#include <fstream>

#include <dirent.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/core.hpp>

#include <opencv2/xfeatures2d/nonfree.hpp>

#include <math.h>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

class HuMomentsExtractor {

private:
    string outDir;
    int hMin, sMin, vMin;
    int hMax, sMax, vMax;
    int operationMode;

    int modo;

    Mat imageThreshold;
    Mat origen;

    double basehumoments[3][7] = { {0.319094,0.0100832,0.0314054,0.0205816,0.00044178,0.00194764,0.000280423},
                {0.230433,0.0100892,0.000826096,0.000456105,2.79865e-07,4.07163e-05,7.70697e-09},
                {0.230889, 0.0005549, 0.000228693, 0.000167363, 3.17442e-08, 3.90972e-06, -8.02465e-09  } };

   
    static void huFunc(int, void*);
    void printHSV();


    int euclideanDistance(vector<double>, int);

    void refreshImg();

    Mat filterClahe(Mat);
    Mat filterClaheGris(Mat);
    Mat suavizado(Mat);

    static void funcionTrackbar(int, void*);

    /*
    static void hMax(int,void*);
    static void sMax(int,void*);
    static void vMax(int,void*);
    static void hMin(int,void*);
    static void sMin(int,void*);
    static void vMin(int,void*);
    */

public:
    HuMomentsExtractor(string = "fichero.txt");
    vector<double> extractHuMoments(Mat);
    vector<double> extractHuMoments(Mat, int, int, int, int, int, int);
    void capture(Mat);
    void captureSurf(Mat,Mat,Mat,Mat,Mat,Mat,Mat, Ptr<BRISK>, vector<KeyPoint>, vector<KeyPoint>, vector<KeyPoint>,
            Mat, Mat,Mat);

    void setOperationMode(int);

    void trackbar();
    
    
};
