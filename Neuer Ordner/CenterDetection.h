#pragma once

#ifndef CENTERDETECTION_H
#define CENTERDETECTION_H

#include <map>
#include <opencv2/core/mat.hpp>

using namespace std;


class CenterDetection
{
	
	CenterDetection();
	~CenterDetection();
public:

	/// Global variables für CannyFilter OpenCV
	static const int edgeThresh;
	static const int lowThreshold;
	static const int max_lowThreshold;
	static const int ratio1;
	static const int kernel_size;


	static const double threshold_value;
	static const double max_BINARY_value;

	static cv::Mat image;
	//image Camera::GetPictureFreeze();
	static const cv::Mat im_gray;
	static cv::Mat im_bw;
	static cv::Mat im_contour;
	static cv::Mat dst;
	static cv::Mat im_snap_inv;
	static cv::Mat im_rgb;
	//static cv::Mat image;
	


	typedef cv::Point2f CenterLB;
	static const CenterLB CenterLB_Min;
	static const CenterLB CenterLB_Max;

	static vector<vector<cv::Point> > contours;
	static vector<cv::Vec4i> hierarchy;
	//RNG rng(12345);


	string window_name_rgb = "Laser Beam in RGB";
	string window_name_bw = "Laser Beam in BW";
	string window_name_contour = "Laser Beam: Contours";
	string window_name_dst = "rausfinden dst";
	string window_name_im_snap_inv = "Invert SNAP";
	string window_name_from_cam = "Image from Camera";
	string im_name = "D:/Bild5";
	string snap = "D:/Documents/Visual Studio 2012/Projects/IMREAD/Release/snap_BGR8";
	string im_extension = ".jpg";
	string bw = "_bw";

	//void treshold();
	static cv::Point2d findCenterPointHist(cv::Mat image);
	static vector<cv::Point2f> findCenterMassCenter();
	static cv::Point findCenterPointContours(vector<vector<cv::Point> > contours);
	static map<int, int> computeHistogram(cv::Mat& image);
	static void printHistogram(const map<int, int>& histogram);

	const int img_bpp = 8;


	void Filter() const;




};

#endif //CENTERDETECTION_H
