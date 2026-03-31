
// MainFrm.h: интерфейс класса CMainFrame
//

#pragma once
#include "PluginView.h"
#include "PropertyPane.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Реализация
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnPropertyGridChanged(CMFCPropertyGridProperty* pProp);

protected:  // встроенные члены панели элементов управления
	CMFCMenuBar       m_wndMenuBar;

	CMFCToolBar       m_wndToolBar;
	CMFCToolBar		  m_layoutToolBar;
	CMFCStatusBar     m_wndStatusBar;
	PluginViewWnd     m_wndView;
	CMFCToolBarImages m_UserImages;
	CPropertyPane	  m_wndPropertyBar;
	CMFCToolBarImages m_layoutToolBarImages;

// Созданные функции схемы сообщений
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnUpdateCursorPos(CCmdUI* pCmdUI);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	virtual void GetMessageString(UINT nID, CString& rMessage) const override
	{
		// 1. Загружаем полную строку из ресурсов по ID кнопки
		CString strFull;
		if (strFull.LoadString(nID))
		{
			// 2. Ищем разделитель \n
			int nPos = strFull.Find(_T('\n'));
			if (nPos != -1)
			{
				// 3. Берем текст ПОСЛЕ \n и отдаем его статусбару
				rMessage = strFull.Mid(nPos + 1);
				return;
			}
			else
			{
				// Если \n нет, отдаем всю строку как есть
				rMessage = strFull;
				return;
			}
		}

		// Если строка не найдена, вызываем стандартную обработку (для "Ready" и меню)
		CFrameWndEx::GetMessageString(nID, rMessage);
	}

	BOOL CreateDockingWindows();
	void UpdatePropertyGrid(Control* pCtrl);
	void OnSelectionChanged(Control* pCtrl);
	Control* m_pSelectedControl = nullptr;
	COleVariant ConvertToVariant(const Variant& v);
};


