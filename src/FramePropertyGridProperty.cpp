#include "pch.h"       // или свой stdafx.h / common.h
#include "FramePropertyGridProperty.h"

IMPLEMENT_DYNAMIC(CFramePropertyGridProperty, CMFCPropertyGridProperty)


//CFramePropertyGridProperty::CFramePropertyGridProperty(
//    const CString& strName,
//    const Frame& frame)
//    : CMFCPropertyGridProperty(strName, COleVariant(L""))
//    , m_bSubItemsInitialized(false)
//    , m_frameCache(frame)
//{
//}

CFramePropertyGridProperty::CFramePropertyGridProperty(
    const CString& strName,
    const Frame& frame)
    : CMFCPropertyGridProperty(strName, _variant_t(L""), L"") // Пустое значение родителя
    , m_frameCache(frame)
{

    auto* pTest = new CMFCPropertyGridProperty(L"Test", _variant_t(0L), L"");
    if (pTest)
    {
        this->AddSubItem(pTest);
    }

    auto* pPropLeft = new CMFCPropertyGridProperty(L"Left", COleVariant((long)m_frameCache.left));
    auto* pPropTop = new CMFCPropertyGridProperty(L"Top", COleVariant((long)m_frameCache.top));
    auto* pPropRight = new CMFCPropertyGridProperty(L"Right", COleVariant((long)m_frameCache.right));
    auto* pPropBottom = new CMFCPropertyGridProperty(L"Bottom", COleVariant((long)m_frameCache.bottom));

    AddSubItem(pPropLeft);
    AddSubItem(pPropTop);
    AddSubItem(pPropRight);
    AddSubItem(pPropBottom);

    m_bSubItemsInitialized = true;
}

CString CFramePropertyGridProperty::FormatProperty() 
{
    CString strFmt;
    strFmt.Format(L"%d;%d;%d;%d",
        static_cast<int>(m_frameCache.left),
        static_cast<int>(m_frameCache.top),
        static_cast<int>(m_frameCache.right),
        static_cast<int>(m_frameCache.bottom));
    return strFmt;
}

void CFramePropertyGridProperty::UpdateFromFrame(const Frame& frame)
{
    m_frameCache = frame;
}

void CFramePropertyGridProperty::UpdateFrame(Frame& frame) const
{
    frame = m_frameCache;
}

void CFramePropertyGridProperty::InitSubItems()
{
    if (m_bSubItemsInitialized)
        return;

    AddSubItem(new CMFCPropertyGridProperty(
        L"Left",
        COleVariant(0L)
    ));
    AddSubItem(new CMFCPropertyGridProperty(
        L"Top",
        COleVariant(0L)
    ));
    AddSubItem(new CMFCPropertyGridProperty(
        L"Right",
        COleVariant(0L)
    ));
    AddSubItem(new CMFCPropertyGridProperty(
        L"Bottom",
        COleVariant(0L)
    ));

    m_bSubItemsInitialized = true;
}

//CString CFramePropertyGridProperty::FormatProperty() 
//{
//    if (!m_bSubItemsInitialized)
//    {
//        // Внутри const‑метода придётся const_cast
//        const_cast<CFramePropertyGridProperty*>(this)->InitSubItems();
//    }
//
//    const CMFCPropertyGridProperty* pLeft =
//        GetSubItem(static_cast<int>(kIdx_Left));
//    const CMFCPropertyGridProperty* pTop =
//        GetSubItem(static_cast<int>(kIdx_Top));
//    const CMFCPropertyGridProperty* pRight =
//        GetSubItem(static_cast<int>(kIdx_Right));
//    const CMFCPropertyGridProperty* pBottom =
//        GetSubItem(static_cast<int>(kIdx_Bottom));
//
//    if (!pLeft || !pTop || !pRight || !pBottom)
//        return L"";
//
//    long left = pLeft->GetValue().lVal;
//    long top = pTop->GetValue().lVal;
//    long right = pRight->GetValue().lVal;
//    long bottom = pBottom->GetValue().lVal;
//
//    CString strFmt;
//    strFmt.Format(L"%d;%d;%d;%d", left, top, right, bottom);
//    return strFmt;
//}
//
//void CFramePropertyGridProperty::UpdateFromFrame(const Frame& frame)
//{
//    if (!m_bSubItemsInitialized)
//        return;
//
//    CMFCPropertyGridProperty* pLeft = GetSubItem(kIdx_Left);
//    CMFCPropertyGridProperty* pTop = GetSubItem(kIdx_Top);
//    CMFCPropertyGridProperty* pRight = GetSubItem(kIdx_Right);
//    CMFCPropertyGridProperty* pBottom = GetSubItem(kIdx_Bottom);
//
//    pLeft->SetValue(COleVariant(frame.left));
//    pTop->SetValue(COleVariant(frame.top));
//    pRight->SetValue(COleVariant(frame.right));
//    pBottom->SetValue(COleVariant(frame.bottom));
//}
//
//void CFramePropertyGridProperty::UpdateFrame(Frame& frame) const
//{
//    if (!m_bSubItemsInitialized)
//        return;
//
//    const CMFCPropertyGridProperty* pLeft = GetSubItem(kIdx_Left);
//    const CMFCPropertyGridProperty* pTop = GetSubItem(kIdx_Top);
//    const CMFCPropertyGridProperty* pRight = GetSubItem(kIdx_Right);
//    const CMFCPropertyGridProperty* pBottom = GetSubItem(kIdx_Bottom);
//
//    if (!pLeft || !pTop || !pRight || !pBottom)
//        return;
//
//    frame.left = static_cast<long>(pLeft->GetValue().lVal);
//    frame.top = static_cast<long>(pTop->GetValue().lVal);
//    frame.right = static_cast<long>(pRight->GetValue().lVal);
//    frame.bottom = static_cast<long>(pBottom->GetValue().lVal);
//}
