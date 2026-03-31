// IndentedPropertyGridCtrl.h
#pragma once

class CIndentedPropertyGridCtrl : public CMFCPropertyGridCtrl
{
    DECLARE_DYNAMIC(CIndentedPropertyGridCtrl)

public:
    CIndentedPropertyGridCtrl();
    virtual ~CIndentedPropertyGridCtrl();

protected:
    virtual void OnDrawProperty(CDC* pDC, CMFCPropertyGridProperty* pProp,
        CRect& rect, BOOL bIsSelected, BOOL bIsDroppedDown, BOOL bIsGrouped);

    DECLARE_MESSAGE_MAP()
};
