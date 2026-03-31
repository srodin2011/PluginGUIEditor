// IndentedPropertyGridCtrl.cpp
#include "pch.h"
#include "IndentedPropertyGridCtrl.h"

IMPLEMENT_DYNAMIC(CIndentedPropertyGridCtrl, CMFCPropertyGridCtrl)

CIndentedPropertyGridCtrl::CIndentedPropertyGridCtrl()
{
}

CIndentedPropertyGridCtrl::~CIndentedPropertyGridCtrl()
{
}

BEGIN_MESSAGE_MAP(CIndentedPropertyGridCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()

void CIndentedPropertyGridCtrl::OnDrawProperty(CDC* pDC,
    CMFCPropertyGridProperty* pProp, CRect& rect,
    BOOL bIsSelected, BOOL bIsDroppedDown, BOOL bIsGrouped)
{
    const int nIndentWidth = 16;

    // 1. Серая полоса слева от Name
    CRect rectIndent = rect;
    rectIndent.right = rectIndent.left + nIndentWidth;
    CRect rectName = rect;
    rectName.left = rectIndent.right;

    CBrush brush(GetColor(COLOR_GROUP_BACKGROUND));
    pDC->FillRect(rectIndent, &brush);   // индентация слева от Name

    // 2. Передаём остальное стандартному OnDrawProperty
    CMFCPropertyGridCtrl::OnDrawProperty(pDC, pProp, rectName, bIsSelected, bIsDroppedDown, bIsGrouped);
}
