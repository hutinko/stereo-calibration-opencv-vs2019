#pragma once

//#define ALL_LIB_IN_ONE

#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996) 
#pragma warning (disable: 4819) // "Save the file in Unicode format to prevent data loss"

// opencv head files - by Edward 5/21/2016
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/objdetect/objdetect.hpp"  

// 
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h" // cvtColor CV_BGR2GRAYÎ´ÉùÃ÷µÄ±êÊ¶·û


#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

// opencv libaries [5/19/2016 edward]
#ifndef ALL_LIB_IN_ONE
 #pragma comment(lib, cvLIB("core")) 
 #pragma comment(lib, cvLIB("calib3d")) 
 #pragma comment(lib, cvLIB("imgcodecs")) 
 #pragma comment(lib, cvLIB("imgproc")) 
 #pragma comment(lib, cvLIB("highgui")) 
 #pragma comment(lib, cvLIB("features2d")) 
 #pragma comment(lib, cvLIB("video")) 
 #pragma comment(lib, cvLIB("videoio")) 
 //#pragma comment(lib, cvLIB("videostab")) 
 #pragma comment(lib, cvLIB("objdetect")) 
#else
#pragma comment(lib,cvLIB("img_hash"))
#pragma comment(lib,cvLIB("world"))
#endif//ALL_LIB_IN_ONE

using namespace cv;

typedef cv::Mat_<uchar> Imgu;
typedef cv::Mat_<float> Imgf;
typedef cv::Mat_<cv::Vec3b> Img3b;

typedef Imgf Matf;
typedef cv::Mat_<bool> Matb;
