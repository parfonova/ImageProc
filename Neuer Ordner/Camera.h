#pragma once

#ifndef CAMERA_H
#define CAMERA_H


#include <ueye.h>
#include <uEye_tools.h>
#include <ueye_deprecated.h>
//#include <uEyeCaptureInterface.h>
#include <version.h>
#include <opencv2/core/mat.hpp>


using namespace std;
//using namespace cv;


class Camera
{

	Camera();
public:
	
	static const int img_width;
	static const int img_height;
	static const int img_bpp;

	//Variables for Camera Settings
	static HIDS hCam;
#define CAPTURE_WIDTH  768
#define CAPTURE_HEIGHT 576
	static const SENSORINFO sensor_info;
	static const CAMINFO camera_info;

	static char* m_pcImageMemory;
	static int m_lMemoryId;
	static const int BITS_PER_PIXEL;
	static const int pWidth;
	static const int pHeight;
	static int pXPos, pYPos;

	static double FPS, NEWFPS;
	static double Exposure; // Belichtungszeit (in ms)
	static double disable;
	static double gval, pval;
	
	
	static int dummy;
	static char *pMem, *pLast;
	static char key;
	static cv::Mat im_snap;

	//just for test
	static short stop;
	
	//Variables for Image Proc
	



	
	~Camera();
	static void SetAutoParameter();
	static void SetCameraParameter();
	static void InitCamera();
	//static void GetSensorInfo();
	//static void GetCameraInfo();
	//static void PrintCameraInfo();
	static void SetColor();
	static void SetImageMem();
	static void AllocImageMem();
	static cv::Mat GetPictureFreeze();
	static void ExitCamera();
};


#endif //CAMERA_H