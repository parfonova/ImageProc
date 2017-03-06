#include "CenterDetection2.h"
#include <iostream>
#include <ostream>
#include "vector"
#include <opencv/cv.h>


#include "windows.h"
#include <opencv2/imgproc.hpp>
#include <chrono>
#include <numeric>
#include <map>
#include <iomanip>
#include <highgui/highgui.hpp>


using namespace std; 
using namespace cv;


int edgeThresh = 1;//ok
int lowThreshold = 15;
int max_lowThreshold = 100;
int ratio1 = 2;//war 3 ist ok oder vllt 2
int kernel_size = 3;//gut so

const int img_bpp = 8;



//const CenterDetection::CenterLB CenterDetection::CenterLB_Min = (0.0001,0.0001);
//const CenterDetection::CenterLB CenterDetection::CenterLB_Max = (0.1* Camera::pWidth, 0.9*Camera::pHeight);


Mat image1;

CenterDetection::CenterDetection()
{
}


CenterDetection::~CenterDetection()
{
}


cv::Mat CenterDetection::ImageLoad(Mat image1)
{
	image1 = cv::imread("neu7.bmp", CV_LOAD_IMAGE_COLOR);
	cvWaitKey(0);
	return image1;
}


void CenterDetection::Filter(cv::Mat pict) 
{
	// TRESHOLD BINARY INV
	//double threshold_value = 1;
	//double max_BINARY_value = 1;
	//cv::threshold ( im_snapCD, im_snap_inv, threshold_value, max_BINARY_value, cv::THRESH_BINARY_INV); // ein SWsBild, wo  Laserstrahl schwarz ist
	//cv::namedWindow (window_name_im_snap_inv, CV_WINDOW_AUTOSIZE );
	//cv::imshow ( window_name_im_snap_inv, im_snap_inv);
	cv::Mat blr;
	//image = imread("neu7.bmp", IMREAD_COLOR);
	image = pict;

//	cvtColor(image, image_bw, CV_RGB2GRAY);
	image_bw = image > 128;
	cv::namedWindow("image_bw", CV_WINDOW_AUTOSIZE);
	cv::imshow("image_bw", image_bw);
	cvWaitKey(0);
	//bitwise_not(image_bw, image_bw);

	// then adjust the threshold to actually make it binary
	
	threshold(image_bw, binaryMat, 100, 255, CV_THRESH_BINARY);

	cv::namedWindow("binary", CV_WINDOW_AUTOSIZE);
	cv::imshow("binary", binaryMat);
	cvWaitKey(0);
	//bitwise_not(binaryMat, binaryMat_inv);



	cv::blur(image_bw, blr, cv::Size(5, 5)); // Reduce noise with a kernel 3x3
	
	
	Mat im_th;
	threshold(blr, im_th, 220, 255, THRESH_BINARY_INV);
	im_floodfill = im_th.clone();
	floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));
	Mat im_floodfill_inv;
	bitwise_not(im_floodfill, im_floodfill_inv);
	cv::namedWindow("blur and Canvnz", CV_WINDOW_AUTOSIZE);
	cv::imshow("blur and Canvnz", im_floodfill_inv);
	cvWaitKey(0);

	im_floodfill_inv.copyTo(im_gray);
	
	
	
	//morphologyEx(blr, dst, 3, 1);   //2-opening, 3 closing,   Element Rect - 1: Cross - 2: Ellipse
	//cv::namedWindow("morph", CV_WINDOW_AUTOSIZE);
	//cv::imshow("morph", dst);
	//cvWaitKey(0);

	////

	////threshold_value = 2;
	////max_BINARY_value = 255;
	////cv::threshold(dst, im_snap_inv, threshold_value, max_BINARY_value, THRESH_BINARY_INV);// ein SWsBild, wo  Laserstrahl schwarz ist
	////cv::namedWindow(window_name_im_snap_inv, CV_WINDOW_AUTOSIZE);
	////cv::imshow(window_name_im_snap_inv, im_snap_inv);
	////cvWaitKey(0);
	
	
}

//RNG rng(12345);

vector<cv::Point2f> CenterDetection::findCenterMassCenter()
{
	//cv::blur(im_gray, im_contour, cv::Size(3, 3)); // Reduce noise with a kernel 3x3
	kernel_size = 3;
	cv::Canny(im_gray, im_bw , 100, 200, kernel_size);
cv::namedWindow("im_gray", CV_WINDOW_AUTOSIZE);
	cv::imshow("im_gray", im_bw);
	cvWaitKey(0);



	cv::findContours(im_bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	dst = cv::Scalar::all(0);  // Using Canny's output as a mask, we display our resultt

	vector<cv::Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<cv::Point2f> CenterLaserBeam(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		CenterLaserBeam[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//DRAWING
	//cv::Mat drawing = cv::Mat::zeros(im_bw.size(), CV_8UC3);
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	//	circle(drawing, mc[i], 4, color, -1, 8, 0);
	//	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	//	// printf("\t Info: Area and Contour Length \n");
	//}

	//for (int i = 0; i < CenterDetection::contours.size(); i++)
	//{
	//	// printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	//	circle(drawing, mc[i], 4, color, -1, 8, 0);
	//}
	cout << "Mass of Object" << CenterLaserBeam << endl;
	//  imshow( window_name_contour, drawing );
	return CenterLaserBeam;
}

cv::Point CenterDetection::findCenterPointContours()
{

	auto minX = NULL;
	auto minY = NULL;
	auto maxX = NULL;
	auto maxY = NULL;

	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			int x = contours[i][j].x;
			int y = contours[i][j].y;

			if (minX == NULL || x < minX)
			{
				minX = x;
			}
			if (maxX == NULL || x > maxX)
			{
				maxX = x;
			}
			if (minY == NULL || y < minY)
			{
				minY = y;
			}
			if (maxY == NULL || y > maxY)
			{
				maxY = y;
			}
		}
	}
	int centerX = (maxX + minX) / 2;
	int centerY = (maxY + minY) / 2;
	cout << centerX << ", " << centerY << " Center of Laser Beam" << endl;
	cvWaitKey(0);
	return cv::Point(centerX, centerY);
}

cv::Point2d CenterDetection::findCenterPointHist()

{
	double CenterX = 0;
	double CenterY = 0;
	double meanX;
	double meanY;
	double varianceX;
	double varinaceY;
 	vector<double> VectorX(image_bw.cols, 0);
	vector<double> VectorY;
	cv::namedWindow("sashok", CV_WINDOW_AUTOSIZE);
	cv::imshow("sashok", image_bw);
	cvWaitKey(0);

	for (int row = 0; row < image_bw.rows; row++)
	{
		double coloredCols = 0;
		for (int col = 0; col < image_bw.cols; col++)
		{
			int pixelValue = image_bw.at<uchar>(row, col);
			
			if (pixelValue == 255)
			{
				coloredCols += 1;
				VectorX.at(col) = VectorX.at(col) + 1;
			}
		}
		VectorY.push_back(coloredCols);
	}

	double sumVertical = accumulate(VectorX.begin(), VectorX.end(), 0);
	double sumHorizontal = accumulate(VectorY.begin(), VectorY.end(), 0);
	if (sumHorizontal != sumVertical)
	{
		cout << "Error counting pixels" << endl;
	}


	double MeanValuePixels = sumVertical / 2;
	double sumY = 0;
	double sumX = 0;
	int itY;
	int itX;


	for (itY = 0; itY < sumVertical; itY++)
	{
		sumY += VectorY.at(itY);
		CenterY = itY;
		if (sumY >= MeanValuePixels) break;
	}

	cout << CenterY << "Integrierte CenterY" << endl;


	for (itX = 0; itX < sumHorizontal; itX++)
	{
		sumX += VectorX.at(itX);
		CenterX = itX;
		if (sumX >= MeanValuePixels) break;
	}

	cout << CenterX << "Integrierte CenterX" << endl;

	//meanX = (accumulate(VectorX.begin(), VectorX.end(), 0)) / image.cols;
	//for (int row = 0; row < image.rows; row++)
	//{
	//	VectorX[row] = VectorX[row] - meanX;
	//	VectorX[row] *= (VectorX[row]);
	//}
	//varianceX = (accumulate(VectorX.begin(), VectorX.end(), 0)) ^ 2 / image.cols;
	////cout << "variance " << varianceX << endl;


	return cv::Point2d(sumVertical, sumHorizontal);
}

map<int, int> CenterDetection::computeHistogram(cv::Mat& image)
{
	map<int, int> histogram;

	for (int row = 0; row < image.rows; ++row)
	{
		for (int col = 0; col < image.cols; ++col)
		{
			++histogram[(int)image.at<uchar>(row, col)];

		}
	}

	return histogram;
}

void CenterDetection::printHistogram(const map<int, int>& histogram) //create vector  with 0 and 1 for x and y derection
{
	map<int, int>::const_iterator histogram_iter;
	cout << "\n------------------\n";
	for (histogram_iter = histogram.begin(); histogram_iter != histogram.end(); ++histogram_iter)
	{
		cout << setw(5) << histogram_iter->first << " : " << histogram_iter->second << "\n";
	}
	cout << "------------------\n";
}

//Point findCenterPointIntens(cv::Mat)
//		{  
//	vector<int> VectorX(im_snap_inv.cols,0);
//	vector<int> VectorY(im_snap_inv.rows,0);	
//	int maxX = 0;
//	int maxY = 0;
//	
//	vector<int>::iterator itX;
//	vector<int>::iterator itY;
//
//	for(int i = 0; i < im_snap_inv.cols; i++) 
//	{	for(int j = 0; j < im_snap_inv.rows; j++)
//		{	int sumX += sumX;
//		
//			VectorX.push_back(sumX) += sumX;
//			
//			//unsinnint sumX = im_snap_inv.at<uint>(i,j);
//			
//		if (sumX <= maxX) { maxX = sumX; }
//			cout << "Centre X" << maxX <<endl;
//			//do{maxX = pixel
//			
//
//		//	maxY += im_snap_inv.at<uchar>(i,j);
//			//VectorY[j] += VectorY[j];
//			/*itY = VectorY.begin();
//			itX = VectorX.insert ( itX , im_snap_inv.at<uchar>(i,j) );*/
//
//		/*	itY = VectorY.begin();
//			itY = VectorY.insert(itY, im_snap_inv.at<uchar>(i, j));*/
//		}
//	
//	}
//
//    /*	 X = max_element(begin(VectorX), end(VectorX));
//		 Y = max_element(begin(VectorY), end(VectorY));
//		
//		X + = im_snap_inv.at<uchar>(i,j);}*/
////				
////for (itX = VectorX.begin(); itX != VectorX.end(); ++itX)
////			{
////				if (maxX < *itX)
////				        maxX = *itX;
////			}
////
////for (itY = VectorY.begin(); itY != VectorY.end(); ++itY)
////			{
////				if (maxY < *itY)
////				        maxY = *itY;
////			}
//
//
//	cout << maxX << ", " << maxY <<  " Center of Laser Beam from Intensivity of BW Image " << endl;
//	return Point(maxX, maxY);
//}

//void Histogrambuild( )
//{	cv::Mat  src, dst;
//
//	
//	/// Load image
//	String imageName("D:/Bild4.jpg"); // by default
//	src = imread(imageName, IMREAD_COLOR);
//	
//
//	/// Separate the image in 3 places ( B, G and R )
//	vector<cv::Mat> bgr_planes;
//	split(src, bgr_planes);
//
//	/// Establish the number of bins
//	int histSize = 256;
//
//	/// Set the ranges ( for B,G,R) )
//	float range[] = {0, 256};
//	const float* histRange = {range};
//
//	bool uniform = true;
//	bool accumulate = false;
//
//	cv::Mat b_hist, g_hist, r_hist;
//
//	/// Compute the histograms:
//	calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
//	calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
//	calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
//
//	// Draw the histograms for B, G and R
//	int hist_w = 1024;
//	int hist_h = 800;
//	int bin_w = cvRound((double) hist_w / histSize);
//
//	cv::Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
//
//	/// Normalize the result to [ 0, histImage.rows ]
//	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, cv::Mat());
//	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, cv::Mat());
//	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, cv::Mat());
//
//	/// Draw for each channel EIGENTLICH UNNOETIG  SPATER LOESCHEN
//	for (int i = 1; i < histSize; i++)
//	{
//		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
//		     Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
//		     Scalar(255, 0, 0), 2, 8, 0);
//		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
//		     Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
//		     Scalar(0, 255, 0), 2, 8, 0);
//		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
//		     Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
//		     Scalar(0, 0, 255), 2, 8, 0);
//	}
//
//	/// Display
//	namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
//	imshow("calcHist Demo", histImage);
//
//	waitKey(0);
//}

//void createGaussian(Size& image1, cv::Mat & output, int uX, int uY, float sigmaX, float sigmaY, float amplitude = 1.0f)
//{
//	cv::Mat temp = cv::Mat(image1, CV_32F);
//	for (int row = 0; row < im_bw.rows; row++)
//	{
//		for (int col = 0; col < im_bw.cols; col++)
//		{
//			float x = (col-uX)*((float)col-uX)/(2.0f*sigmaX*sigmaX);
//			float y = (row-uY)*((float)row-uY)/(2.0f*sigmaY*sigmaY);
//			float value = amplitude*exp(-(x+y));
//			temp.at<float>(row,col) = value;
//		}
//
//	}
//	normalize(temp,temp,0.0f,1.0f,NORM_MINMAX);
//	output = temp;
//}