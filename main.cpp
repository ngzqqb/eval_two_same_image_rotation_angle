#include "MainWindow.hpp"

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <iostream>

double evalAngle(  const cv::Mat & img_object,
                   const cv::Mat & img_scene ){
    using namespace cv;
    using namespace cv::xfeatures2d;

    Mat descriptors_object;
    Mat descriptors_scene;

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 666;
    auto detector =  SURF::create( minHessian );

    std::vector<KeyPoint> keypoints_object;
    detector->detectAndCompute( img_object, noArray(), keypoints_object, descriptors_object );

    std::vector<KeyPoint> keypoints_scene;
    detector->detectAndCompute( img_scene, noArray(), keypoints_scene, descriptors_scene );

    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    auto matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );

    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.923f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++){
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance){
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    for( size_t i = 0; i < good_matches.size(); i++ ){
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, RANSAC ,1.1 );
    return std::atan2( H.at<double>(0,1),H.at<double>(0,0) ) / 3.141592654 * 180 ;

}


void test(){

    // cv::features2d ;


    qDebug() <<  evalAngle(  cv::imread(THE_DEBUG_PATH"/1.jpg",cv::IMREAD_GRAYSCALE) ,
                             cv::imread(THE_DEBUG_PATH"/2.jpg",cv::IMREAD_GRAYSCALE)     );

    /*
https://blog.csdn.net/fengyeer20120/article/details/87798638
*/
    //cv::findHomography(    cv::RANSAC ,1. );

}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    test();

    MainWindow w;
    w.show();
    return a.exec();
}





