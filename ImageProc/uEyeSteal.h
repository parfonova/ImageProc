#if !defined(AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_)
#define AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"
#include "atlstr.h"
#include "CenterDetection2.h"
#include <core/cuda.inl.hpp>


class Steal 
{

public:
	Steal()
	{
	}


	~Steal()
	{
		
	}
	

	// camera variables
	HIDS	m_hCam;			    // handle to camera
	HWND	m_hWndLive;	        // handle to live diplay window
	HWND	m_hWndSteal;	    // handle to steal image window
	INT		m_nColorMode;	    // Y8/RGB16/RGB24/RGB32
	INT		m_nBitsPerPixel;    // number of bits needed store one pixel
	INT     m_nSizeX;           // image size
	INT     m_nSizeY;           // image size
	INT     m_nStealFormat;     // Pixel format for the steal operation 

								// steal memory buffers
	INT		m_lStealMemId;	    // camera memory - buffer ID
	char*	m_pcStealImgMem;    // camera memory - pointer to buffer
	CString m_SaveFileName;
	BOOL    m_bFirstTime;
	cv::Mat pic;



	// camera functions
	INT InitCamera(HIDS *hCam, HWND hWnd);
	bool OpenCamera();
	void AllocateStealMemory();
	void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	//// message counters
	//unsigned int m_nCntTransFail;
	//unsigned int m_nCntFrame;
	//unsigned int m_nCntSteal;

	//enum { IDD = IDD_UEYESTEAL_DIALOG };

	//virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HICON m_hIcon;

	//virtual BOOL OnInitDialog();
	// void OnSysCommand(UINT nID, LPARAM lParam);
	// void OnPaint();
	// HCURSOR OnQueryDragIcon();
	 void OnButtonExit();
	 cv::Mat StealPic();
	 void OnButtonStartLive();
	//oid OnButtonStopLive();
	 void OnButtonSteal();
	 void OnButtonLoadParam();
	// LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);
	 void Close();
};

#endif // !defined(AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_)
