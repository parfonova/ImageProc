//#include "C:\Users\Lena\Documents\Visual Studio 2015\Projects\ImageProc\ImageProc\Camera.h"
#include "Camera2.h"
#include <highgui/highgui.hpp>
#include "uEyeSteal.h"
#include "CenterDetection2.h"





int main()
{//Camera Camera1;
//	Camera1.InitCamera();
//	Camera1.SetAutoParameter();
//	Camera1.SetCameraParameter();
//	Camera1.GetSensorInfo();
//	Camera1.GetCameraInfo();
////Camera1.PrintCameraInfo();
//	//Camera1.SetColor();
//	Camera1.AllocImageMem();
//	Camera1.SetImageMem();
//	Camera1.GetPictureFreeze();
//	////imshow("fgf",Camera1.frame);
//	Camera1.ExitCamera();
	//
	//Steal Steal1;
	//Steal1.OpenCamera();
	//Steal1.AllocateStealMemory();
	//Steal1.OnButtonStartLive();
	////Steal1.OnButtonSteal();
	//Steal1.StealPic();
	//Steal1.OnButtonExit();
	
	CenterDetection CenterDetection_0Grad;
//	CenterDetection_0Grad.ImageLoad(image);
 	CenterDetection_0Grad.Filter();
	CenterDetection_0Grad.findCenterMassCenter();
	CenterDetection_0Grad.findCenterPointContours();
	CenterDetection_0Grad.findCenterPointHist();

}
