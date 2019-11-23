#include <Common/cv/cvHead.h>
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>

using namespace std;
using namespace cv;

static  vector< vector< Point3f > > object_points;
static  vector< vector< Point2f > > image_points;
static  vector< Point2f > corners;
static  vector< vector< Point2f > > left_img_points;

static  Mat img, gray;
static  Size im_size;

bool doesExist(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void setup_calibration(int board_width, int board_height, int num_imgs,
	float square_size, const char* imgs_directory, const char* imgs_filename,
	const char* extension) {
	Size board_size = Size(board_width, board_height);
	int board_n = board_width * board_height;

	for (int k = 1; k <= num_imgs; k++) {
		char img_file[100];
		sprintf(img_file, "%s%s%d.%s", imgs_directory, imgs_filename, k, extension);
		if (!doesExist(img_file))
			continue;
		img = imread(img_file, IMREAD_UNCHANGED);
		cv::cvtColor(img, gray, CV_BGR2GRAY);

		bool found = false;
		found = cv::findChessboardCorners(img, board_size, corners,
			CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS);
		if (found)
		{
			cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
				TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(img, board_size, corners, found);
		}

		vector< Point3f > obj;
		for (int i = 0; i < board_height; i++)
			for (int j = 0; j < board_width; j++)
				obj.push_back(Point3f((float)j * square_size, (float)i * square_size, 0));

		if (found) {
			cout << k << ". Found corners!" << endl;
			image_points.push_back(corners);
			object_points.push_back(obj);
		}
	}
}

double computeReprojectionErrors(const vector< vector< Point3f > >& objectPoints,
	const vector< vector< Point2f > >& imagePoints,
	const vector< Mat >& rvecs, const vector< Mat >& tvecs,
	const Mat& cameraMatrix, const Mat& distCoeffs) {
	vector< Point2f > imagePoints2;
	int i, totalPoints = 0;
	double totalErr = 0, err;
	vector< float > perViewErrors;
	perViewErrors.resize(objectPoints.size());

	for (i = 0; i < (int)objectPoints.size(); ++i) {
		projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
			distCoeffs, imagePoints2);
		err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);
		int n = (int)objectPoints[i].size();
		perViewErrors[i] = (float)std::sqrt(err * err / n);
		totalErr += err * err;
		totalPoints += n;
	}
	return std::sqrt(totalErr / totalPoints);
}

int calib_intrinsic_main(String imgs_directory, String imgs_filename, String out_file)
{
	//-w [board_width] -h [board_height] -n [num_imgs] -s [square_size] 
	//-d [imgs_directory] -i [imgs_filename] -o [file_extension] -e [output_filename]
	//-w 9 -h 6 -n 27 -s 0.02423 -d "../calib_imgs/1/" -i "left" -o "cam_left.yml" -e "jpg"
	int board_width = 9;
	int board_height = 6;
	int num_imgs = 27;
	float square_size = 0.02423;
	//const char* imgs_directory = "./calib_imgs/1/";
	//const char* imgs_filename = "left";
	//const char* out_file = "cam_left.yml";
	const char* extension = "jpg";

	setup_calibration(board_width, board_height, num_imgs, square_size,
		imgs_directory.c_str(), imgs_filename.c_str(), extension);

	printf("Starting Calibration\n");
	Mat K; // 内参
	Mat D; // 畸变系数
	vector< Mat > rvecs, tvecs; // 外参 - 旋转、 偏移
	int flag = 0;
	flag |= CALIB_FIX_K4;
	flag |= CALIB_FIX_K5;
	calibrateCamera(object_points, image_points, img.size(), K, D, rvecs, tvecs, flag);

	cout << "Calibration error: " << computeReprojectionErrors(object_points, image_points, rvecs, tvecs, K, D) << endl;

	FileStorage fs(out_file, FileStorage::WRITE);
	fs << "K" << K;
	fs << "D" << D;
	fs << "board_width" << board_width;
	fs << "board_height" << board_height;
	fs << "square_size" << square_size;
	printf("Done Calibration\n");

	return 0;
}
