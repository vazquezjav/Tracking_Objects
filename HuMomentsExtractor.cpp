
#include "HuMomentsExtractor.hpp"

int cf;

HuMomentsExtractor::HuMomentsExtractor(string outDir) {
    this->outDir = outDir;
    this->operationMode = 2;
    this->hMax = 180;
    this->sMax = 255;
    this->vMax = 255;
    this->modo = 0;
}


vector<double> HuMomentsExtractor::extractHuMoments(Mat imageO) {

    Mat image;
    Mat hsv;
    Moments _moments;
    double huMoments[7];
    vector<double> huMomentsV;

    cvtColor(imageO, hsv, COLOR_BGR2HSV);
    //hsv = this->filterClahe(hsv);

    imshow("hsv", hsv);

    

    inRange(hsv, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), imageThreshold);
    imshow("threshold", imageThreshold);

    _moments = moments(imageThreshold, true);
    HuMoments(_moments, huMoments);

    for (int i = 0; i < 7; i++) {
        huMomentsV.push_back(0);
        huMomentsV[i] = huMoments[i];
        cout << huMoments[i] << ",";
    }
    cout << endl;

    return huMomentsV;
}

vector<double> HuMomentsExtractor::extractHuMoments(Mat imageO, int hmin, int smin, int vmin, int hmax, int smax, int vmax) {

    Mat hsv;
    Moments _moments;
    double huMoments[7];
    vector<double> huMomentsV;

    cvtColor(imageO, hsv, COLOR_BGR2HSV);

    //imshow("hsv", hsv);

    //filtro clahe 
    hsv = this->filterClahe(hsv);

    inRange(hsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), imageThreshold);
    imshow("threshold", imageThreshold);

    _moments = moments(imageThreshold, true);
    HuMoments(_moments, huMoments);

    for (int i = 0; i < 7; i++) {
        huMomentsV.push_back(0);
        huMomentsV[i] = huMoments[i];
        cout << huMoments[i] << "@";
    }
    huMomentsV.push_back(_moments.m10 / _moments.m00);
    huMomentsV.push_back(_moments.m01 / _moments.m00);
    cout << endl;

    return huMomentsV;
}

void HuMomentsExtractor::huFunc(int v, void* p) {
    HuMomentsExtractor* hu = reinterpret_cast<HuMomentsExtractor*> (p);
    hu->refreshImg();
}

void HuMomentsExtractor::refreshImg() {
    imshow("threshold", imageThreshold);
    printHSV();
}

void HuMomentsExtractor::printHSV() {
    cout << "hsv-min (" << hMin << "," << sMin << "," << vMin << ")" << "hsv-max (" << hMax << "," << sMax << "," << vMax << ")" << endl;
}

int HuMomentsExtractor::euclideanDistance(vector<double> moms, int i) {
    double d = 0.0;
    int index = -1;

    double thresholdm = 0.2;

    for (int j = 0; j < 7; j++) {
        //cout << "m: " << moms[j] << " humoments: " << basehumoments[i][j] << ",";
        d += ((moms[j] - basehumoments[i][j]) * (moms[j] - basehumoments[i][j]));
    }
    cout << endl;
    d = sqrt(d);

    //cout << "Distance: " << " i: " << i << " :: " << d << endl;
    cout << "distancia" << d << endl;
    if (i == 0)
        thresholdm = 0.20;
        cout << "ROJOOO " << d << endl;
    if (i == 1)
        thresholdm = 0.33;
        cout << "AZULLL " << d << endl;
    if (i == 2)
        thresholdm = 0.25;
        cout << "VERDEEEEEE " << d << endl;


    if (d < thresholdm)
        return i;

    return -1;
}

void HuMomentsExtractor::setOperationMode(int m) {
    this->operationMode = m;
}

Mat HuMomentsExtractor::filterClahe(Mat image) {

    //aplicando filtro clahe, imagen hsv, trabajando unicamente con el canal 2 ya que es similar al canal gris
    Mat hsv = image.clone();

    vector<Mat> canales;
    split(hsv, canales);

    //equalizeHist(canales[2], canales[2]);

    Ptr<CLAHE> clahe = createCLAHE();

    clahe->setClipLimit(3);

    clahe->setTilesGridSize(Size(3,3));

    Mat dst;
    clahe->apply(canales[2], dst);

    dst.copyTo(canales[2]);

    merge(canales, hsv);

    return hsv;
}

Mat HuMomentsExtractor::filterClaheGris(Mat image) {
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4);
    Mat dst;
    clahe->apply(image, dst);

    return dst;
}
Mat HuMomentsExtractor::suavizado(Mat image) {

    int k = 3;
    medianBlur(image, image, k);
    GaussianBlur(image, image, Size(k,k), 1);

    return image;
}


void HuMomentsExtractor::capture(Mat image) {
    

        Mat frame;
        Mat hsv;

        frame = image.clone();
        resize(frame, frame, Size(800, 600));
        namedWindow("video", WINDOW_AUTOSIZE);
        //namedWindow("hsv", WINDOW_AUTOSIZE);
        namedWindow("threshold", WINDOW_AUTOSIZE);


        vector<double> huMoments,huMoments2, huMoments3;
        int indexRed = -1;
        int indexBlue = -1;
        int indexGreen = -1;


            frame = this->suavizado(frame);

            if (this->operationMode == 1) { 
                createTrackbar("HMin", "video", &hMin, 180, HuMomentsExtractor::huFunc, static_cast<void*>(this));
                createTrackbar("SMin", "video", &sMin, 255, HuMomentsExtractor::huFunc, static_cast<void*>(this));
                createTrackbar("VMin", "video", &vMin, 255, HuMomentsExtractor::huFunc, static_cast<void*>(this));

                createTrackbar("HMax", "video", &hMax, 180, HuMomentsExtractor::huFunc, static_cast<void*>(this));
                createTrackbar("SMax", "video", &sMax, 255, HuMomentsExtractor::huFunc, static_cast<void*>(this));
                createTrackbar("VMax", "video", &vMax, 255, HuMomentsExtractor::huFunc, static_cast<void*>(this));
                huMoments = this->extractHuMoments(frame);
            }
            else if (this->operationMode == 2) {
                
                huMoments = this->extractHuMoments(frame,75,101,71,180,255,255);
                indexRed = this->euclideanDistance(huMoments, 0);

                huMoments2 = this->extractHuMoments(frame, 84,55,48,162,255,255);
                indexBlue = this->euclideanDistance(huMoments2, 1);

                huMoments3 = this->extractHuMoments(frame, 38,71,17,64,214,240);
                indexGreen = this->euclideanDistance(huMoments3, 2);

                if (indexRed != -1) {
                    cout << "Red object: " << indexRed << endl;
                    putText(frame, "Object 1 Red", Point(huMoments[7], huMoments[8]), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 10, 143), 2);
                }
                if (indexBlue != -1) {
                    cout << "Blue object: " << indexBlue << endl;
                    putText(frame, "Object 2 Blue", Point(huMoments2[7], huMoments2[8]), FONT_HERSHEY_DUPLEX, 1, Scalar(143, 10, 0), 2);
                }
                if (indexGreen != -1) {
                    cout << "Object 3 Detected - Green: " << indexGreen << endl;
                    putText(frame, "Object 3 Green: ", Point(huMoments3[7], huMoments3[8]), FONT_HERSHEY_DUPLEX, 1, Scalar(10, 143, 3), 2);
                }
                imshow("video", frame);
            }
            //imshow("video", frame);
    
}

void HuMomentsExtractor::captureSurf(Mat image, Mat objeto, Mat objeto_gris, Mat objeto2, Mat objeto_gris2,
    Mat objeto3, Mat objeto_gris3, Ptr<BRISK>surf, vector<KeyPoint> key_points_1, vector<KeyPoint> key_points_3, vector<KeyPoint> key_points_4,
    Mat descriptors_1, Mat descriptors_3, Mat descriptors_4 ) {

    //VideoCapture video(1);
    resize(image, image, Size(600, 600));
            Mat frame = image.clone();
            Mat scena_gris;

            //video >> frame;
            //flip(frame, frame, 1);
            frame = this->suavizado(frame);
            cvtColor(frame, scena_gris, COLOR_BGR2GRAY);
            scena_gris = this->filterClaheGris(scena_gris);

            //detector frame
            vector<KeyPoint> key_points_2; // Puntos clave que se detectan con la matriz Hessiana
            Mat descriptors_2;
            surf->detect(scena_gris, key_points_2);
            surf->compute(scena_gris, key_points_2, descriptors_2);

            BFMatcher matcher;
            Mat img_matches;

            vector<vector<DMatch> > matches;

            matcher.knnMatch(descriptors_1, descriptors_2, matches, 2);

            vector<DMatch> matchesFiltrados;

            float ratio = 0.7f;
            for (int i = 0; i < matches.size(); i++) {
                if (matches[i][0].distance < ratio * matches[i][1].distance)
                    matchesFiltrados.push_back(matches[i][0]);
            }

            drawMatches(objeto, key_points_1, frame, key_points_2, matchesFiltrados, img_matches, Scalar::all(1),
                Scalar::all(1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

            //localizar objeto

            if (matchesFiltrados.size() > 2) {

                try
                {
                    destroyWindow("Objeto2");
                    vector<Point2f> obj, scene;

                    for (int i = 0; i < matchesFiltrados.size(); i++) {
                        obj.push_back(key_points_1[matchesFiltrados[i].queryIdx].pt);
                        scene.push_back(key_points_2[matchesFiltrados[i].trainIdx].pt);
                    }
                    //esquinas del objeto del objeto que fue detectdo
                    Mat h = findHomography(obj, scene, RANSAC, 5.0);

                    vector<Point2f> obj_corners(4);
                    obj_corners[0] = Point2f(0, 0);
                    obj_corners[3] = Point2f((float)objeto_gris.cols, 0);
                    obj_corners[2] = Point2f((float)objeto_gris.cols, (float)objeto_gris.rows);
                    obj_corners[1] = Point2f(0, (float)objeto_gris.rows);

                    vector<Point2f> scene_corners(4);
                    cout << "Objeto 1" << endl;
                    Mat dst;
                    perspectiveTransform(obj_corners, scene_corners, h);

                    //lineas, mapeando del objeto en la scena
                    line(img_matches, scene_corners[0] + Point2f((float)objeto_gris.cols, 0),
                        scene_corners[1] + Point2f((float)objeto_gris.cols, 0), Scalar(255, 0, 0), 2);

                    line(img_matches, scene_corners[1] + Point2f((float)objeto_gris.cols, 0),
                        scene_corners[2] + Point2f((float)objeto_gris.cols, 0), Scalar(255, 0, 0), 2);
                    line(img_matches, scene_corners[2] + Point2f((float)objeto_gris.cols, 0),
                        scene_corners[3] + Point2f((float)objeto_gris.cols, 0), Scalar(255, 0, 0), 2);
                    line(img_matches, scene_corners[3] + Point2f((float)objeto_gris.cols, 0),
                        scene_corners[0] + Point2f((float)objeto_gris.cols, 0), Scalar(255, 0, 0), 2);

                    imshow("Objeto1", img_matches);
                }
                catch (const std::exception&)
                {

                }

                
            }
            else {
                destroyWindow("Objeto1");
                //objeto 2
                vector<vector<DMatch>> matches_objeto_2;
                vector<DMatch> matchesFiltrados2;
                BFMatcher matcher_objeto2;
                Mat img_matches2;

                matcher_objeto2.knnMatch(descriptors_3, descriptors_2, matches_objeto_2, 2);
                float ratio = 0.65f;
                for (int i = 0; i < matches_objeto_2.size(); i++) {
                    if (matches_objeto_2[i][0].distance < ratio * matches_objeto_2[i][1].distance)
                        matchesFiltrados2.push_back(matches_objeto_2[i][0]);
                }
                //objeto
                drawMatches(objeto2, key_points_3, frame, key_points_2, matchesFiltrados2, img_matches2, Scalar::all(-1),
                    Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


                if (matchesFiltrados2.size() > 2) {
                    try
                    {
                        vector<Point2f> obj2, scene2;
                        for (int i = 0; i < matchesFiltrados2.size(); i++) {
                            obj2.push_back(key_points_3[matchesFiltrados2[i].queryIdx].pt);
                            scene2.push_back(key_points_2[matchesFiltrados2[i].trainIdx].pt);
                        }

                        Mat h2 = findHomography(obj2, scene2, RANSAC, 5.0);

                        vector<Point2f> obj_corners2(4);
                        obj_corners2[0] = Point2f(0, 0);
                        obj_corners2[3] = Point2f((float)objeto_gris2.cols, 0);
                        obj_corners2[2] = Point2f((float)objeto_gris2.cols, (float)objeto_gris2.rows);
                        obj_corners2[1] = Point2f(0, (float)objeto_gris2.rows);

                        vector<Point2f> scene_corners2(4);
                        Mat dst;
                        perspectiveTransform(obj_corners2, scene_corners2, h2);

                        //lineas, mapeando del objeto en la scena
                        line(img_matches2, scene_corners2[0] + Point2f((float)objeto_gris2.cols, 0),
                            scene_corners2[1] + Point2f((float)objeto_gris2.cols, 0), Scalar(255, 0, 0), 2);

                        line(img_matches2, scene_corners2[1] + Point2f((float)objeto_gris2.cols, 0),
                            scene_corners2[2] + Point2f((float)objeto_gris2.cols, 0), Scalar(255, 0, 0), 2);
                        line(img_matches2, scene_corners2[2] + Point2f((float)objeto_gris2.cols, 0),
                            scene_corners2[3] + Point2f((float)objeto_gris2.cols, 0), Scalar(255, 0, 0), 2);
                        line(img_matches2, scene_corners2[3] + Point2f((float)objeto_gris2.cols, 0),
                            scene_corners2[0] + Point2f((float)objeto_gris2.cols, 0), Scalar(255, 0, 0), 2);
                        imshow("Objeto2", img_matches2);
                    }
                    catch (const std::exception&)
                    {

                    }
                    
                }
                else {
                    destroyWindow("Objeto1");
                    destroyWindow("Objeto2");
                    vector<vector<DMatch>> matches_objeto_3;
                    vector<DMatch> matchesFiltrados3;
                    BFMatcher matcher_objeto3;
                    Mat img_matches3;

                    matcher_objeto3.knnMatch(descriptors_4, descriptors_2, matches_objeto_3, 2);
                    float ratio = 0.65f;
                    for (int i = 0; i < matches_objeto_3.size(); i++) {
                        if (matches_objeto_3[i][0].distance < ratio * matches_objeto_3[i][1].distance)
                            matchesFiltrados3.push_back(matches_objeto_3[i][0]);
                    }
                    //objeto
                    drawMatches(objeto3, key_points_4, frame, key_points_2, matchesFiltrados3, img_matches3, Scalar::all(1),
                        Scalar::all(1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                    Mat points = objeto3.clone();
                    drawKeypoints(points, key_points_1, points);

                    if (matchesFiltrados3.size() > 2) {
                        try
                        {
                            vector<Point2f> obj3, scene3;
                            for (int i = 0; i < matchesFiltrados3.size(); i++) {
                                obj3.push_back(key_points_4[matchesFiltrados3[i].queryIdx].pt);
                                scene3.push_back(key_points_2[matchesFiltrados3[i].trainIdx].pt);
                            }

                            Mat h3 = findHomography(obj3, scene3, RANSAC, 5.0);

                            vector<Point2f> obj_corners3(4);
                            obj_corners3[0] = Point2f(0, 0);
                            obj_corners3[3] = Point2f((float)objeto_gris3.cols, 0);
                            obj_corners3[2] = Point2f((float)objeto_gris3.cols, (float)objeto_gris3.rows);
                            obj_corners3[1] = Point2f(0, (float)objeto_gris3.rows);

                            vector<Point2f> scene_corners3(4);
                            perspectiveTransform(obj_corners3, scene_corners3, h3);

                            //lineas, mapeando del objeto en la scena
                            line(img_matches3, scene_corners3[0] + Point2f((float)objeto_gris3.cols, 0),
                                scene_corners3[1] + Point2f((float)objeto_gris3.cols, 0), Scalar(0, 0, 255), 2);

                            line(img_matches3, scene_corners3[1] + Point2f((float)objeto_gris3.cols, 0),
                                scene_corners3[2] + Point2f((float)objeto_gris3.cols, 0), Scalar(0, 0, 255), 2);
                            line(img_matches3, scene_corners3[2] + Point2f((float)objeto_gris3.cols, 0),
                                scene_corners3[3] + Point2f((float)objeto_gris3.cols, 0), Scalar(0, 0, 255), 2);
                            line(img_matches3, scene_corners3[3] + Point2f((float)objeto_gris3.cols, 0),
                                scene_corners3[0] + Point2f((float)objeto_gris3.cols, 0), Scalar(0, 0, 255), 2);
                            imshow("Objeto3", img_matches3);
                        }
                        catch (Exception e) {}

                    }
                    else {
                        destroyWindow("Objeto1");
                        destroyWindow("Objeto2");
                        destroyWindow("Objeto3");

                    }

                }
            }
            matches.clear();
            matchesFiltrados.clear();
        
}

void HuMomentsExtractor::funcionTrackbar(int v, void* p) {
    cout << "modo " << v << endl;
    cf = v;  
    
}

void  HuMomentsExtractor::trackbar() {
    HuMomentsExtractor* hu = new HuMomentsExtractor();
    VideoCapture video(1);

    Mat objeto = imread("libro.jpg");
    Mat objeto_gris = imread("libro.jpg", IMREAD_GRAYSCALE);

    Mat objeto2 = imread("cien.jpg");
    Mat objeto_gris2 = imread("cien.jpg", IMREAD_GRAYSCALE);

    Mat objeto3 = imread("libro2.jpg");
    Mat objeto_gris3 = imread("libro2.jpg", IMREAD_GRAYSCALE);


    Ptr<BRISK> surf = BRISK::create();
    vector<KeyPoint> key_points_1, key_points_3, key_points_4;
    surf->detect(objeto_gris, key_points_1);
    surf->detect(objeto_gris2, key_points_3);
    surf->detect(objeto_gris3, key_points_4);

    Mat descriptors_1, descriptors_3, descriptors_4;
    surf->compute(objeto_gris, key_points_1, descriptors_1);

    surf->compute(objeto_gris2, key_points_3, descriptors_3);

    surf->compute(objeto_gris3, key_points_4, descriptors_4);

    if (video.isOpened()) {
        video.set(CAP_PROP_FRAME_WIDTH, 800);
        video.set(CAP_PROP_FRAME_HEIGHT, 600);

        namedWindow("Frame", WINDOW_AUTOSIZE);
        createTrackbar("Modo", "Frame", &modo, 2, HuMomentsExtractor::funcionTrackbar, static_cast<void*>(this));
        Mat frame;
        while (1 == 1) {
            
            video >> frame;
            if (cf == 1) {
               
                flip(frame, frame, 1);
                hu->capture(frame);
            }
            if (cf == 2) {
                destroyWindow("video");
                destroyWindow("threshold");
                hu->captureSurf(frame,objeto,objeto_gris, objeto2, objeto_gris2, objeto3, objeto_gris3, surf, key_points_1, key_points_3, key_points_4,
                    descriptors_1, descriptors_3, descriptors_4);
                
            }
            imshow("Frame", frame);

            if (waitKey(23) == 27)
                break;
        }
    }
    
}