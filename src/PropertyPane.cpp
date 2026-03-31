// PropertyPane.cpp
#include "pch.h"
//#include "framework.h"
#include "PropertyPane.h"
#include "MainFrm.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CPropertyPane, CDockablePane)

BEGIN_MESSAGE_MAP(CPropertyPane, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
    ON_WM_SETTINGCHANGE()
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CPropertyPane::CPropertyPane()
{
}

void CPropertyPane::AdjustLayout()
{
    if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // разместить CMFCPropertyGridCtrl внутри панели
    CRect rectDummy;
    rectDummy.SetRectEmpty();

    const DWORD dwStyle = WS_CHILD | WS_VISIBLE /* | WS_TABSTOP | WS_BORDER*/;

    m_wndPropList.Create(dwStyle, rectDummy, this, 1/*(UINT)-1*/);

    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    //m_wndPropList.SetVSDotNetLook(TRUE);   // или Office 2007/2010 look
    //m_wndPropList.SetAlphabeticMode(TRUE);
    //m_wndPropList.SetGroupNameFullWidth(FALSE);


    CMFCToolBarImages images;
    images.SetImageSize(CSize(16, 15)); // Размер одной иконки
    images.Load(IDR_PROPERTIES);

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, AFX_DEFAULT_TOOLBAR_STYLE) ||
        !m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE))
    {
        TRACE0("Не удалось создать панель инструментов\n");
        return -1;      // не удалось создать
    }

    //m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    //m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
    m_wndToolBar.CleanUpLockedImages();
    //m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);
    m_wndToolBar.LoadBitmap(IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // All commands will be routed via this control , not via the parent frame:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    //EnableDocking(CBRS_ALIGN_ANY);

    //SetVSDotNetLook(TRUE);

    AdjustLayout();

    return 0;
}

void CPropertyPane::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CPropertyPane::OnExpandAllProperties()
{
    m_wndPropList.ExpandAll();
}

void CPropertyPane::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
    //pCmdUI->Enable(TRUE);
}

void CPropertyPane::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertyPane::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
    //pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

LRESULT CPropertyPane::OnPropertyChanged(WPARAM /* wp */, LPARAM lp)
{
    CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lp;
    if (!pProp)
        return 0;

    // 1. извлекаем Id, который ты хранил через SetData
    DWORD_PTR id = pProp->GetData();
    // если хочешь, можно проверить
    // if (id < 0 || id > 255) return 0;

    // 2. перенаправляем в CMainFrame
    NotifyPropertyChanged(pProp);

    return 0;
}

void CPropertyPane::NotifyPropertyChanged(CMFCPropertyGridProperty* pProp)
{
    CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
    if (pFrame)
    {
        pFrame->OnPropertyGridChanged(pProp);
    }
}

void CPropertyPane::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);
    m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertyPane::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CPropertyPane::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndPropList.SetFocus();
}
