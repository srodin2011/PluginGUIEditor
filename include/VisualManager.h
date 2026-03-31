#pragma once

class CRodinVisualManagerVS2022 : public CMFCVisualManagerVS2008
{
    using Base = CMFCVisualManagerVS2008;
    DECLARE_DYNCREATE(CRodinVisualManagerVS2022)
public:
    CRodinVisualManagerVS2022();
    virtual ~CRodinVisualManagerVS2022();

    // Цвета VS 2022 Dark
    const COLORREF clrBackground = RGB(204, 213, 240);    // Темный фон
    const COLORREF clrMenuLight = RGB(45, 45, 48);    // Фон выпадающего меню
    const COLORREF clrAccent = RGB(236, 244, 255);   // Синий акцент (hover)
    const COLORREF clrTextColor = RGB(30, 30, 30); // Цвет текста меню

    // Отрисовка фона меню и панелей
    virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea) override
    {
        pDC->FillSolidRect(rectClient, clrBackground);
    }

    // Подсветка кнопок и пунктов меню при наведении
    //virtual void OnDrawButtonBorder(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) override
    //{
    //    if (state == ButtonsIsHighlighted || state == ButtonsIsPressed)
    //    {
    //        //pDC->Draw3dRect(rect, clrAccent, clrAccent);
    //        pDC->FillSolidRect(rect, clrAccent);
    //    }
    //}

    virtual void OnHighlightMenuItem(CDC* pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText) override
    {
        // Задаем цвет фона при наведении (Синий акцент VS 2022)
        pDC->FillSolidRect(rect, clrAccent);

        // ВАЖНО: Устанавливаем цвет текста, который MFC использует при рисовании этого пункта
        clrText = clrTextColor; // Белый текст на синем фоне
    }

    virtual void OnFillHighlightedArea(CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton) override
    {
        // Проверяем, что это именно кнопка меню (в выпадающем списке)
        if (pButton != NULL && pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)))
        {
            pDC->FillSolidRect(rect, clrAccent);
        }
        else
        {
            // Для обычных кнопок на тулбаре оставляем стандарт или свой цвет
            pDC->FillSolidRect(rect, clrAccent);
        }
    }

    // 1. Подсветка фона верхних пунктов (MenuBar)
    virtual void OnFillButtonInterior(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) override
    {
        // Проверяем, что кнопка подсвечена или нажата
        if (state == ButtonsIsHighlighted || state == ButtonsIsPressed)
        {
            // В VS 2022 верхнее меню при наведении подсвечивается чуть светлее основного фона
            // или синим акцентом, если меню открыто
            COLORREF clrHover = (state == ButtonsIsPressed) ? clrAccent : clrAccent;
            pDC->FillSolidRect(rect, clrHover);
        }
    }

    // 2. Цвет текста для верхних пунктов
    virtual COLORREF GetToolbarButtonTextColor(CMFCToolBarButton* pButton, CMFCVisualManager::AFX_BUTTON_STATE state) override
    {
        // Если это кнопка в строке меню
        if (pButton->IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton)))
        {
            if (state == ButtonsIsHighlighted || state == ButtonsIsPressed)
            {
                return clrTextColor; // Белый при наведении
            }
            return clrTextColor; // Обычный цвет текста (светло-серый)
        }

        // Для остальных кнопок тулбара вызываем базу или свой цвет
        return Base::GetToolbarButtonTextColor(pButton, state);
    }

    virtual BOOL GetToolTipInfo(CMFCToolTipInfo& params, UINT nType) override
    {
        BOOL b= Base::GetToolTipInfo(params, nType);
        params.m_bVislManagerTheme = TRUE;
        params.m_bBoldLabel = FALSE;
        params.m_bDrawDescription = FALSE;
        params.m_bDrawIcon = FALSE;
        params.m_bDrawSeparator = FALSE;
        params.m_clrBorder = RGB(118, 118, 118);
        return b;
    }
    //virtual void OnFillButtonInterior(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state) override
    //{
    //    if (state == ButtonsIsHighlighted || state == ButtonsIsPressed)
    //    {
    //        pDC->FillSolidRect(rect, clrAccent);
    //    }
    //}

    //// Цвет фона выпадающего списка
    //virtual void OnFillHighlightedArea(CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton) override
    //{
    //    pDC->FillSolidRect(rect, clrAccent);
    //}

    //virtual COLORREF OnFillCommandsListBackground(CDC* pDC, CRect rect, BOOL bIsSelected) override
    //{
    //    // Закрашиваем всё меню темным цветом VS 2022
    //    pDC->FillSolidRect(rect, RGB(204, 213, 240));
    //    return RGB(241, 241, 241); // Возвращаем цвет текста по умолчанию
    //}
};
