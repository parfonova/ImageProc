#pragma once

#ifndef CAMERA2_H
#define CAMERA2_H


#include <ueye.h>
#include <opencv2/core/mat.hpp>


using namespace std;
//#define CAPTURE_WIDTH  752
//#define CAPTURE_HEIGHT 480
//using namespace cv;


class Camera
{

public:
	Camera()
	{
	}


	~Camera()
	{
	}

	 const int img_width = 752;
	 const int img_height = 480;
	 const int img_bpp = 8;

	//Variables for Camera Settings
	HIDS hCam = 0;

	SENSORINFO sensor_info;
	CAMINFO camera_info;

	char* m_pcImageMemory;
	int m_lMemoryId;
	int BITS_PER_PIXEL;
	int pWidth;
	int pHeight;
	int pXPos, pYPos;

	double FPS, NEWFPS;
	double Exposure; // Belichtungszeit (in ms)
	double disable;
	double gval, pval;


	int dummy;
	char *pMem, *pLast;
	char key;
	cv::Mat im_snap;
	cv::Mat frame;


	void SetAutoParameter();
	void SetCameraParameter();
	void InitCamera();
	void GetSensorInfo();
	void GetCameraInfo();
	void PrintCameraInfo();
	void SetColor();
	void SetImageMem();
	void AllocImageMem();

	void GetPictureFreeze();
	void ExitCamera();
};


#endif //CAMERA2_H