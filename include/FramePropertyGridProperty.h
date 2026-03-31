#pragma once

#include "PluginGUI\include\Frame.h"

using namespace PluginGUI;

class CFramePropertyGridProperty : public CMFCPropertyGridProperty
{
    DECLARE_DYNAMIC(CFramePropertyGridProperty)

public:
    // В конструкторе создаём только пустое свойство, под‑свойства позже
    CFramePropertyGridProperty(
        const CString& strName,
        const Frame& frame);

    // Возвращает строку "left;top;right;bottom"
    virtual CString FormatProperty() override;

    // Обновить под‑свойства из Frame
    void UpdateFromFrame(const Frame& frame);

    // Обновить Frame из под‑свойств
    void UpdateFrame(Frame& frame) const;

    // Инициализация под‑свойств; вызывать ПОСЛЕ AddProperty
    void InitSubItems();

    virtual ~CFramePropertyGridProperty() = default;

private:
    enum SubItemIndex
    {
        kIdx_Left = 0,
        kIdx_Top = 1,
        kIdx_Right = 2,
        kIdx_Bottom = 3
    };

    bool m_bSubItemsInitialized = false;
private:
    Frame m_frameCache;
};