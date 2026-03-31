#pragma once

class CSimpleToolTipCtrl : public CMFCToolTipCtrl
{
    DECLARE_DYNCREATE(CSimpleToolTipCtrl)

public:
    virtual void OnDrawBorder(CDC* pDC, CRect rect, COLORREF clrLine) override
    {
        COLORREF clrBorder = m_Params.m_clrBorder;
        if (clrBorder == (COLORREF)-1) clrBorder = RGB(118, 118, 118); // Дефолт VS

        CBrush brush(clrBorder);
        pDC->FrameRect(rect, &brush);
        //pDC->Draw3dRect(rect, clrBorder, clrBorder);
    }
};