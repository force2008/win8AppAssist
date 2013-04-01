
// Win8APPAssist.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

const CString native = L"/native/src/native";
const CString json = L"/native/src/json";
const CString jsonxmlconverter = L"/native/src/JsonXmlConverter";
const CString notifications = L"/native/src/notifications";
const CString sqlite3 = L"/native/src/sqlite3";
const CString wbxml = L"/native/src/wbxml";
// CWin8APPAssistApp:
// See Win8APPAssist.cpp for the implementation of this class
//

class CWin8APPAssistApp : public CWinApp
{
public:
	CWin8APPAssistApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWin8APPAssistApp theApp;