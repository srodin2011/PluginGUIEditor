// IndentedProperty.cpp
#include "pch.h"
#include "IndentedProperty.h"

IMPLEMENT_DYNAMIC(CIndentedProperty, CMFCPropertyGridProperty)

CIndentedProperty::CIndentedProperty(
    const CString& strName,
    const COleVariant& varValue,
    DWORD_PTR dwData)
    : CMFCPropertyGridProperty(strName, varValue, NULL, dwData)
{
}

//BEGIN_MESSAGE_MAP(CIndentedProperty, CMFCPropertyGridProperty)
//END_MESSAGE_MAP()

void CIndentedProperty::OnDrawName(CDC* pDC, CRect rect)
{
    const int nIndentWidth = 16;

    CRect rectIndent = rect;
    rectIndent.right = rectIndent.left + nIndentWidth;

    CRect rectName = rect;
    rectName.left = rectIndent.right;

    // 1. Серая полоса слева от Name
    CBrush brush(RGB(242, 242, 242));
    pDC->FillRect(rectIndent, &brush);

    // 2. Стандартная отрисовка имени
    CMFCPropertyGridProperty::OnDrawName(pDC, rectName);
}