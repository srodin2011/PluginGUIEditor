
// TestMFCSimpleApp.h: основной файл заголовка для приложения TestMFCSimpleApp
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CTestMFCSimpleAppApp:
// Сведения о реализации этого класса: TestMFCSimpleApp.cpp
//

class PluginGUIEditorApp : public CWinAppEx
{
public:
	PluginGUIEditorApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern PluginGUIEditorApp theApp;
