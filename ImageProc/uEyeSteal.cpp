#include "stdafx.h"
#include "uEyeSteal.h"
//#include ".\ueyestealdlg.h"
#include "version.h"
#include <iostream>
#include <stdio.h>
#include <ueye.h>
#include <atlconv.h>
#include <C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\atlmfc\include\atlsimpstr.h>
#include <locale>
#include <core/cvstd.hpp>
#include <core/mat.hpp>
#include <core/cuda.inl.hpp>
//#include <imgcodecs/imgcodecs_c.h>

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>      // redefines the new() operator 
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// METHOD CUEyeStealDlg::OpenCamera() 
// DESCRIPTION: Opens a handle to a connected camera
bool Steal::OpenCamera()
{
	INT nRet = 0;
	// close any opened cameras
	if (m_hCam != 0)
	{
		is_ExitCamera(m_hCam);
	}

	// init memory variables
	m_pcStealImgMem = NULL;
	m_lStealMemId = NULL;

	// init camera
	m_hCam = (HIDS)0;  // open next camera
	auto Init = is_InitCamera(&m_hCam, m_hWndSteal);
	if (Init == IS_SUCCESS)
	{
		//Check if DirectX is available on the local machine
		//if (is_DirectRenderer(m_hCam, DR_CHECK_COMPATIBILITY, NULL, NULL) != IS_SUCCESS)
		//{
		//	return false;
		//}

		// enable the dialog based error report
		nRet = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
		if (nRet != IS_SUCCESS)
		{
			return false;
		}
		// get sensor info
		SENSORINFO SensorInfo;
		is_GetSensorInfo(m_hCam, &SensorInfo);

		// init image size to sensor size by default
		GetMaxImageSize(&m_nSizeX, &m_nSizeY);

		IS_SIZE_2D imageSize;
		imageSize.s32Width = m_nSizeX;
		imageSize.s32Height = m_nSizeY;

		is_SetColorMode(m_hCam, IS_CM_MONO8); //IS_CM_BGRA8_PACKED
		is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

		// Scale image
		is_DirectRenderer(m_hCam, DR_ENABLE_SCALING, NULL, NULL);

		// setup the color mode for steal
		if (SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME)
		{	// monochrome camera
			m_nColorMode = IS_CM_MONO8;
			m_nBitsPerPixel = 8;
			m_nStealFormat = 3;
		}
		else
		{	// color camera
			m_nColorMode = IS_CM_BGR8_PACKED;
			m_nBitsPerPixel = 24;
			m_nStealFormat = 1;
		}

		//	UpdateData(FALSE);

		INT nMode = m_nColorMode;
	//	is_DirectRenderer(m_hCam, DR_SET_STEAL_FORMAT, (void*)&nMode, sizeof(nMode));
	}
	else
	{
		std::cout << "ERROR: Cannot open uEye camera!" << std::endl;

	}
	return true;
}

void Steal::AllocateStealMemory()
{
	if (m_hCam != 0)
	{
		// memory free?
		if (m_pcStealImgMem != NULL)
		{
			is_FreeImageMem(m_hCam, m_pcStealImgMem, m_lStealMemId);
		}

		// allocate memory for steal
		if (is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcStealImgMem, &m_lStealMemId) != IS_SUCCESS)
		{
			std::cout << "ERROR: cannot allocate buffer memory for steal" << std::endl;
		}

		// activate image memory
		is_SetImageMem(m_hCam, m_pcStealImgMem, m_lStealMemId);
	}
}


void Steal::OnButtonStartLive()
{
	if (m_hCam == 0)
	{
		OpenCamera();
	}

	// uEye
	//is_CaptureVideo(m_hCam, IS_WAIT);
	is_FreezeVideo(m_hCam, IS_WAIT);
}


void Steal::OnButtonExit()
{
	// memory and events are automatically released
	is_ExitCamera(m_hCam);
}

void Steal::OnButtonSteal()
{
	if ((m_hCam != 0) && (m_lStealMemId != 0))
	{
		INT wait = IS_DONT_WAIT;
		//is_DirectRenderer(m_hCam, DR_STEAL_NEXT_FRAME, (void*)&wait, sizeof(wait));

		INT nColorMode;
		//is_DirectRenderer(m_hCam, DR_GET_STEAL_FORMAT, (void*)&nColorMode, sizeof(nColorMode));


		nColorMode = IS_CM_MONO8;
		//is_DirectRenderer(m_hCam, DR_SET_STEAL_FORMAT, (void*)&nColorMode, sizeof(nColorMode));

		//Bild kopieren, Funktion kehrt sofort zurück
		INT nwait = IS_DONT_WAIT;
		//is_DirectRenderer(m_hCam, DR_STEAL_NEXT_FRAME, (void*)&wait, sizeof(wait));
	}
}


cv::Mat Steal::StealPic()
{
	if (m_hCam != 0)
	{
		m_bFirstTime = TRUE;
		m_SaveFileName = "-1";

		// save
		if (m_bFirstTime)
		{
			m_bFirstTime = FALSE;
			m_SaveFileName = "C:\\Users\\Lena\\Pictures\\jhkj.bmp";
		}
		if (m_SaveFileName != "-1")
		{
			// Save image as bitmap
			//ImageFileParams.pwchFileName = T2W(m_SaveFileName.GetBuffer());
			//ImageFileParams.pwchFileName = m_SaveFileName;
			IMAGE_FILE_PARAMS ImageFileParams;
			USES_CONVERSION;
			ImageFileParams.pwchFileName = T2W(m_SaveFileName.GetBuffer());
			ImageFileParams.pnImageID = (UINT*)&m_lStealMemId;      // valid ID
			ImageFileParams.ppcImageMem = &m_pcStealImgMem;         // valid buffer 
			ImageFileParams.nFileType = IS_IMG_BMP;
			//ImageFileParams.nQuality = 0;

			int Speichern = is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
			 pic = cv::Mat (480, 752, CV_8UC1, m_pcStealImgMem);
			
						
			
			if (Speichern == 0)
			{ 
				std::cout << " Das Bild wurde  Erfolgreich gespeichert!" << std::endl;
			}
		}
	}
	return pic;
}

void Steal::OnButtonLoadParam()
{
	// load parameters from file
	if (m_hCam == 0)
	{
		OpenCamera();
	}

	is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL);

	// realloc memory
	IS_SIZE_2D imageSize;
	is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

	m_nSizeX = imageSize.s32Width;
	m_nSizeY = imageSize.s32Height;

	m_nColorMode = is_SetColorMode(m_hCam, IS_GET_COLOR_MODE);
	switch (m_nColorMode)
	{
	case IS_CM_MONO8:

		is_SetDisplayMode(m_hCam, IS_SET_DM_DIRECT3D | IS_SET_DM_MONO);
		break;

	case IS_CM_SENSOR_RAW8:

		is_SetDisplayMode(m_hCam, IS_SET_DM_DIRECT3D | IS_SET_DM_BAYER);
		break;
	}
}



void Steal::Close()
{
	// memory and events are automatically released
	is_ExitCamera(m_hCam);
}


INT Steal::InitCamera(HIDS *hCam, HWND hWnd)
{
	INT nRet = is_InitCamera(hCam, hWnd);
	/************************************************************************************************/
	/*                                                                                              */
	/*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
	/*  is necessary. This upload can take several seconds. We recommend to check the required      */
	/*  time with the function is_GetDuration().                                                    */
	/*                                                                                              */
	/*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
	/*  is "OR"-ed to the deviceID. This flag allows an automatic upload of the firmware.           */
	/*                                                                                              */
	/************************************************************************************************/
	if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
	{
		// Time for the firmware upload = 25 seconds by default
		INT nUploadTime = 25000;
		is_GetDuration(*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);


		// Try again to open the camera. This time we allow the automatic upload of the firmware by
		// specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
		*hCam = (HIDS)(((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
		nRet = is_InitCamera(hCam, hWnd);
	}

	return nRet;
}


void Steal::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
	// Check if the camera supports an arbitrary AOI
	// Only the ueye xs does not support an arbitrary AOI
	INT nAOISupported = 0;

	BOOL bAOISupported = TRUE;

	if (is_ImageFormat(m_hCam,
		IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
		(void*)&nAOISupported,
		sizeof(nAOISupported)) == IS_SUCCESS)
	{
		bAOISupported = (nAOISupported != 0);
	}

	if (bAOISupported)
	{
		// All other sensors
		// Get maximum image size
		SENSORINFO sInfo;
		is_GetSensorInfo(m_hCam, &sInfo);
		*pnSizeX = sInfo.nMaxWidth;
		*pnSizeY = sInfo.nMaxHeight;
	}
	else
	{
		// Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		*pnSizeX = imageSize.s32Width;
		*pnSizeY = imageSize.s32Height;
	}
}

