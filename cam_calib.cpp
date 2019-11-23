// cam_calib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "calib_intrinsic.h"
#include "calib_stereo.h"
#include <Common/cv/cvHead.h>
using namespace std;

int main()
{
    std::cout << "Hello World!\n";

	// left
	String imgs_directory = "./calib_imgs/1/";
	String imgs_filename = "left";
	String out_file = "cam_left.yml";
	calib_intrinsic_main(imgs_directory, imgs_filename, out_file);

	// right
	imgs_filename = "right";
	out_file = "cam_right.yml";
	calib_intrinsic_main(imgs_directory, imgs_filename, out_file);

	// stereo calib
	String stereo_out_file = "cam_stereo.yml";
	calib_stereo_main(imgs_directory, stereo_out_file);
}
