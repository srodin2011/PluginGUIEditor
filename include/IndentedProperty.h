// IndentedProperty.h
#pragma once

class CIndentedProperty : public CMFCPropertyGridProperty
{
    DECLARE_DYNAMIC(CIndentedProperty)

public:
    CIndentedProperty(const CString& strName,
        const COleVariant& varValue,
        DWORD_PTR dwData = 0);

protected:
    virtual void OnDrawName(CDC* pDC, CRect rect) override;

    //DECLARE_MESSAGE_MAP()
};
