#pragma once

#ifndef CENTERDETECTION2_H
#define CENTERDETECTION2_H

#include <map>
#include "uEyeSteal.h"
#include <core/cuda.inl.hpp>

using namespace std;


class CenterDetection

{
public:

	/// Global variables für CannyFilter OpenCV
	int edgeThresh;
	int lowThreshold;
	int max_lowThreshold;
	int ratio1;
	int kernel_size;


	double threshold_value;
	double max_BINARY_value;

	cv::Mat image;
	cv::Mat image1;
	cv::Mat im_gray;
	cv::Mat im_bw;
	cv::Mat im_contour;
	cv::Mat dst;
	cv::Mat im_snap_inv;
	cv::Mat im_snapCD;
	cv::Mat im_rgb;


	cv::Mat im_floodfill;
	cv::Mat binaryMat;
	cv::Mat binaryMat_inv;
	cv::Mat image_bw;


	typedef cv::Point2f CenterLB;
	CenterLB CenterLB_Min;
	CenterLB CenterLB_Max;

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;
	//RNG rng(12345);


	char* window_name_rgb = "Laser Beam in RGB";
	char* window_name_bw = "Laser Beam in BW";
	char* window_name_contour = "Laser Beam: Contours";
	char* window_name_dst = "rausfinden dst";
	char* window_name_im_snap_inv = "Invert SNAP";
	char* window_name_from_cam = "Image from Camera";
	char* im_name = "D:/Bild5";
	char* snap = "D:/Documents/Visual Studio 2012/Projects/IMREAD/Release/snap_BGR8";
	char* im_extension = ".jpg";
	char* bw = "_bw";



	CenterDetection();
	~CenterDetection();

	cv::Mat ImageLoad(cv::Mat image1);


	//void treshold();
	cv::Point2d findCenterPointHist();
	vector<cv::Point2f> findCenterMassCenter();
	cv::Point findCenterPointContours();
	map<int, int> computeHistogram(cv::Mat& image);
	void printHistogram(const map<int, int>& histogram);

	void Filter();
};

#endif //CENTERDETECTION2_H
