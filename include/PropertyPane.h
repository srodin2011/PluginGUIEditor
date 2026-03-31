#pragma once

class CPropertiesToolBar : public CMFCToolBar
{
public:
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
    {
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertyPane : public CDockablePane
{
    DECLARE_DYNAMIC(CPropertyPane)

    CFont m_fntPropList;
    CMFCPropertyGridCtrl m_wndPropList;
    CPropertiesToolBar   m_wndToolBar;

public:
    CPropertyPane();
    virtual ~CPropertyPane() {};

    CMFCPropertyGridCtrl* GetGrid() { return &m_wndPropList; }

protected:

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnPropertyChanged(WPARAM wp, LPARAM lp);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnExpandAllProperties();
    afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
    afx_msg void OnSortProperties();
    afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
    afx_msg void OnSetFocus(CWnd* pOldWnd);

    DECLARE_MESSAGE_MAP()

    void NotifyPropertyChanged(CMFCPropertyGridProperty* pProp);
    void SetPropListFont();
    void AdjustLayout();
    void SetVSDotNetLook(BOOL bSet)
    {
        m_wndPropList.SetVSDotNetLook(bSet);
        m_wndPropList.SetGroupNameFullWidth(bSet);
    }
};