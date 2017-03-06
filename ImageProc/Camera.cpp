#include "Camera2.h"
#include <iostream>
#include <C:\Users\Lena\Documents\Visual Studio 2015\Projects\ImageProc\ImageProc\uEye.h>
//#include <uEyeCaptureInterface.h>
#include <uEye_tools.h>
#include <ueye_deprecated.h>
#include <opencv2/core/cuda.inl.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <atlconv.h>

using namespace cv;

//frame(CAPTURE_HEIGHT, CAPTURE_WIDTH, CV_8UC3);


void Camera::SetAutoParameter()
{
	disable = 0;
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN_SHUTTER, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_SENSOR_FRAMERATE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_REFERENCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_ANTI_FLICKER_MODE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_SENS_AUTO_BACKLIGHT_COMP, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_SENS_AUTO_CONTRAST_CORRECTION, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_SENS_AUTO_SHUTTER_PHOTOM, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_SKIPFRAMES, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_SKIPFRAMES, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_SENS_AUTO_GAIN_PHOTOM, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_HYSTERESIS, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_REFERENCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_GAIN_MAX, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_SHUTTER_MAX, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_SPEED, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_OFFSET, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_GAIN_RANGE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_SPEED, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_WB_ONCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_BRIGHTNESS_ONCE, &disable, nullptr);
	is_SetAutoParameter(hCam, IS_SET_AUTO_HYSTERESIS, &disable, nullptr);
	double pval = 1;
	double gval = 1;
	int whiteb = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &pval, nullptr);
	int gains = is_SetAutoParameter(hCam, IS_SET_ENABLE_AUTO_GAIN, &gval, nullptr);
}

void Camera::SetCameraParameter()
{
	FPS = 3;
	is_SetFrameRate(hCam, FPS, &NEWFPS);
	Exposure = 300;
	is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&Exposure, sizeof(Exposure));
	is_SetGamma(hCam, 100); // Default = 100, corresponds to a gamma value of 1.0
	is_Focus(hCam, FOC_CMD_SET_DISABLE_AUTOFOCUS, nullptr, 0);
	is_SetHardwareGain(hCam, 1, 0, 0, 0);
	is_SetDisplayMode(hCam, IS_SET_DM_DIB); //Bitmap-Modus
	is_SetColorMode(hCam, IS_CM_RGB8_PACKED);
	//is_SetColorMode (hCam, IS_CM_MONO8);              GROESSTER FRAGE
	is_SetImageSize(hCam, pWidth, pHeight);
}

void Camera::InitCamera()
{
	if (hCam != 0) {

		is_FreeImageMem(hCam, m_pcImageMemory, m_lMemoryId);
		is_ExitCamera(hCam);
	}


	int initcamera = is_InitCamera(&hCam, NULL);
	if (initcamera != IS_SUCCESS)
	{
		std::cout << std::endl << "Initialisierung der Camera ist möglich!" << std::endl;
		exit(-1);
	}
}

void Camera::GetSensorInfo()
{
	int sensorinfo = is_GetSensorInfo(hCam, &sensor_info);
	 pXPos = (sensor_info.nMaxWidth);
	 pYPos = (sensor_info.nMaxHeight);
	if (sensorinfo != IS_SUCCESS)
	{
		cout<<"Impossibile acquisire le informazioni del sensore"<<endl;
		exit(-1);
	}
}

void Camera::GetCameraInfo()
{
	int camerainfo = is_GetCameraInfo(hCam, &camera_info);
	if (camerainfo != IS_SUCCESS)
	{
		cout << "Impossibile acquisire le informazioni della telecamera" << endl;
		exit(-1);
	}
}

void Camera::PrintCameraInfo()
{
	cout << std::endl << "<<< CARATTERISTICHE DELLA TELECAMERA COLLEGATA >>>" << std::endl;
	cout << "Numero seriale: " << camera_info.SerNo << std::endl;
	cout << "Produttore: " << camera_info.ID << std::endl;
	cout << "Modello: " << sensor_info.strSensorName << std::endl;
	cout << "Dimensioni massime per l'immagine: " << sensor_info.nMaxWidth << "x" << sensor_info.nMaxHeight << std::endl;
}

void Camera::SetColor()
{
	int colormode = is_SetColorMode(hCam, 0);
	//int colormode = is_SetColorMode(hCam, IS_CM_BGR8_PACKED);
	//int colormode = is_SetColorMode(hCam, IS_SET_CM_RGB24);
	if (colormode != IS_SUCCESS)
	{
		printf("Impossibile impostare il modo di colore");
		exit(-1);
	}
}

void Camera::SetImageMem()
{
	int rat = is_SetImageMem(hCam,m_pcImageMemory,m_lMemoryId);
	if (rat != IS_SUCCESS)
	{
		std::cout << std::endl << "IMPOSSIBILE ATTIVARE LA MEMORIA" << std::endl;
		system("PAUSE");
		exit(-1);
	}
	std::cout << std::endl << "Memoria Attivata" << std::endl;
}

void Camera::AllocImageMem()
{


	int rit = is_AllocImageMem(hCam,img_width, img_height, img_bpp, &m_pcImageMemory, &m_lMemoryId);
	if (rit != IS_SUCCESS)
	{
		std::cout << std::endl << "Es ist nicht möglich Speicher zu initialisieren" << std::endl;
		system("PAUSE");
		exit(-1);
	}
	std::cout << std::endl << "Memoria inizializzata" << std::endl;
}

void Camera::GetPictureFreeze()
{
	
	//int freeze = is_FreezeVideo(hCam, IS_WAIT);
	//if (freeze != IS_SUCCESS)
	//{
	//	std::cout << std::endl << "Es ist nicht möglich Video zu freezen" << std::endl;
	//	system("PAUSE");
	//	exit(-1);
	//}

	if ((hCam != 0) && (m_lMemoryId != 0))
	{
		INT wait = IS_DONT_WAIT;
	//	is_DirectRenderer(hCam, DR_STEAL_NEXT_FRAME, (void*)&wait, sizeof(wait));

		INT nColorMode;
		//is_DirectRenderer(hCam, DR_GET_STEAL_FORMAT,			(void*)&nColorMode, sizeof(nColorMode));

		nColorMode = IS_CM_MONO8;
		//is_DirectRenderer(hCam, DR_SET_STEAL_FORMAT,			(void*)&nColorMode, sizeof(nColorMode));

		//Bild kopieren, Funktion kehrt sofort zurück
		INT nwait = IS_DONT_WAIT;
		//is_DirectRenderer(hCam, DR_STEAL_NEXT_FRAME,			(void*)&wait, sizeof(wait));
	}





	IMAGE_FILE_PARAMS ImageFileParams;
	USES_CONVERSION;
	//ImageFileParams.pwchFileName = T2W(m_SaveFileName.GetBuffer());
	ImageFileParams.pwchFileName = L"D:\\neuu.bmp";
	ImageFileParams.pnImageID = (UINT*)m_lMemoryId;      // valid ID
															//ImageFileParams.pnImageID = NULL;
															//ImageFileParams.ppcImageMem = NULL;
	ImageFileParams.ppcImageMem = &m_pcImageMemory;         // valid buffer 
	ImageFileParams.nFileType = IS_IMG_BMP;
	//ImageFileParams.nQuality = 0;

	int Speichern = is_ImageFile(hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
	std::cout << Speichern;

	//if (freeze == IS_SUCCESS)
	//{
	//	int m_Ret = is_GetActiveImageMem(hCam, &pLast, &dummy);
	//	int n_Ret = is_GetImageMem(hCam, (void**)&pLast);
	//	void *pMemVoid;
	//	is_GetImageMem(hCam, &pMemVoid);
	//	IplImage* tmpImg = cvCreateImageHeader(cvSize(img_width, img_height), 8, 1);
	//	tmpImg->nSize = sizeof(IplImage);
	//	tmpImg->ID = 0;
	//	tmpImg->nChannels = 1;
	//	tmpImg->alphaChannel = 0;
	//	tmpImg->depth = 8;
	//	tmpImg->dataOrder = 0;
	//	tmpImg->origin = 0;
	//	tmpImg->align = 4;	// egal
	//	tmpImg->width = img_width;
	//	tmpImg->height = img_height;
	//	tmpImg->roi = NULL;
	//	tmpImg->maskROI = NULL;
	//	tmpImg->imageId = NULL;
	//	tmpImg->tileInfo = NULL;
	//	tmpImg->imageSize = img_width*img_height;
	//	tmpImg->imageData = (char*)pMemVoid;  //the pointer to imagaData
	//	tmpImg->widthStep = img_width;
	//	tmpImg->imageDataOrigin = (char*)pMemVoid; //and again
	//	tmpImg->imageData = m_pcImageMemory;
	//	Mat picframe(img_height, img_width, CV_8UC3);
	//	frame = picframe;
	//	frame = cv::cvarrToMat(tmpImg);
	//	imwrite("D:/kohk.bmp", frame);
	//	imshow("PROVA", frame);
	//	waitKey(0);
	
//return frame;
	}



void Camera::ExitCamera()
{
	int en = is_ExitCamera(hCam);
	if (en == IS_SUCCESS)
	{
		cout << endl << "Camera chiusa correttamente" << endl;
	}
}
