
#pragma once

#include "PluginGUI\include\PluginView.h"
#include "PluginGUI\include\Control.h"
#include "PluginGUI\include\ButtonControl.h"
#include "PluginGUI\include\LableControl.h"
#include "PluginGUI\include\CheckBoxControl.h"
#include "PluginGUI\include\KnobControl.h"

using namespace PluginGUI;

class PluginViewWnd : public PluginView
{
	DECLARE_DYNAMIC(PluginViewWnd)
public:
	PluginViewWnd(Gdiplus::Color backColor);
	virtual ~PluginViewWnd();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	CString m_strCursorPos;

	afx_msg void OnMouseMove(UINT nFlags, CPoint p);
	afx_msg void OnInsertKnob();
	afx_msg void OnInsertSlider();
	afx_msg void OnInsertPad();
	afx_msg void OnInsertButton();
	afx_msg void OnInsertEnvelop();
	afx_msg void OnEditMode();
	afx_msg void OnUpdateInsertControlCommand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCursorPos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorEditMode(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	ButtonControl* pButtonControl1 = nullptr;
	void ChangeButtonText(float);

	LableControl* pLableControl1 = nullptr;
	void ChangeLableText(float);

	CheckBoxControl* pCheckBox1 = nullptr;
	void ChangeObserver2(bool);

	KnobControl* pCircleControl3 = nullptr;
	Event<float>::CallbackPtr observer2 = nullptr;
};
