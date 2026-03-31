
// ChildView.cpp: реализация класса CChildView
//

#include "pch.h"
//#include "framework.h"
#include "PluginView.h"
#include "resource.h"

#include "PluginGUI\include\KnobControl.h"
#include "PluginGUI\include\CheckBoxControl.h"
#include "PluginGUI\include\ButtonControl.h"
#include "PluginGUI\include\LableControl.h"
#include "PluginGUI\include\SliderControl.h"
#include "PluginGUI\include\PadControl.h"
#include "PluginGUI\include\EnvelopeGraphControl.h"
#include "PluginGUI\include\ButtonImage.h"
#include "PluginGUI\include\ButtonRadioImage.h"
#include "PluginGUI\include\Panel.h"
#include "PluginGUI\include\commands.h"
#include <string>

using namespace Gdiplus;
using namespace PluginGUI;

IMPLEMENT_DYNAMIC(PluginViewWnd, PluginGUI::PluginView)

PluginViewWnd::PluginViewWnd(Gdiplus::Color backColor):
	PluginView(backColor)
{
	KnobControl* circleControl = new KnobControl(CPoint(100, 100), 50);
	circleControl->SetMinVal(0);
	circleControl->SetMaxVal(10);
	circleControl->SetValue(0);
	circleControl->DefaultValue = 5.f;
	circleControl->SetFullRuler(true);
	//circleControl.SetBaseColor(Color(255, 190, 190, 190));
	ARGB aggb = Color::MakeARGB(255, 58, 72, 82);
	Color circleControlColor = Color::MakeARGB(255, 58, 72, 82);
	ARGB aaa = circleControlColor.GetValue();
	circleControl->CapColor = circleControlColor;
	circleControl->RingColor = Color(255, 52, 59, 66);//Color(255, 105, 63, 35);
	circleControl->ScaleColor = Color(255, 0, 0, 0);//Color(255, 134, 47, 45);
	circleControl->IndicatorColor = Color(255, 255, 255, 255);
	circleControl->CapPercent = 80.;
	circleControl->GearPercent = 5.;
	circleControl->Scale = false;
	circleControl->IndicatorOnGear = true;
	circleControl->Name = L"circleControl";
	//circleControl->SetTooltipText(L"A Level: 75% [-5.0 dB]");

	KnobControl* circleControl2 = new KnobControl(CPoint(1200, 100), 400);
	circleControl2->SetMinVal(0);
	circleControl2->SetMaxVal(10);
	circleControl2->SetValue(10);
	circleControl2->NotchCount = 10; // 20;
	circleControl2->CapColor = Color(255, 105, 35, 60);
	circleControl2->ScaleBigSteps = 2;
	circleControl2->ScaleSmallSteps = 6;
	circleControl2->IndicatorColor = Color(255, 190, 0, 0);
	//circleControl2->Sensitivity = 0.05f;
	circleControl2->IndicatorColor = Color::MakeARGB(255, 255, 255, 255);
	circleControl2->CapPercent = 80.;
	circleControl2->GearPercent = 5.;
	circleControl2->Scale = false;
	//pGainKnob->CapPercent = 60.;
	//pGainKnob->GearPercent = 20.;
	circleControl2->CapReductionPercent = 5.;
	circleControl2->SetTooltipText(L"A Level: 75% [-5.0 dB]");
	circleControl2->Name = L"circleControl2";



	//circleControl2.SetBaseColor(Color(255, 255, 30, 30));
	//circleControl2.SetBaseColor(Color(255, 64, 160, 53));

	SliderControl* sliderControl = new SliderControl(CRect(550, 50, 650, 550));
	sliderControl->SetMinVal(0);
	sliderControl->SetMaxVal(10);
	sliderControl->SetValue(0);
	sliderControl->ScaleColor = Color(255, 0, 0, 190);
	sliderControl->Name = L"sliderControl";


	CheckBoxControl& checkBox1 = *new CheckBoxControl(CRect(100, 500, 300, 600));
	checkBox1.SetOnColor(Color(255, 235, 42, 37));
	checkBox1.SetOffColor(Color(255, 58, 72, 82));
	checkBox1.SetOn(false);
	std::wstring wstr = L"Change observer";
	std::wstring str(wstr.begin(), wstr.end());
	checkBox1.Text = str;
	pCheckBox1 = &checkBox1;
	pCheckBox1->Name = L"pCheckBox1";
	checkBox1.ValueChanged.Subscribe(
		[&](bool val)
		{
			ChangeObserver2(val);
		}
	);

	ButtonControl& buttonControl1 = *new ButtonControl(CRect(100, 700, 300, 800));
	pButtonControl1 = &buttonControl1;

	PadControl* pad1 = new PadControl(CRect(50, 150, 150, 150));
	//PadControl* pad1 = new PadControl(CRect(50, 50, 450, 450));
	pad1->SetBaseColor(Color(255, 39, 44, 50));
	//pad1->SetBaseColor(Color(255, 203, 206, 187));

	//Add(pad1);
	//Add(sliderControl);
	//Add(circleControl);
	//Add(circleControl2);
	Add(&checkBox1);
	Add(&buttonControl1);

	pCircleControl3 = dynamic_cast<KnobControl*>( PluginControlFactory::Instance().Create(PluginGUI::KnobControl::pluginClassId));
	pCircleControl3->CapColor = Color::MakeARGB(255, 190, 0, 0);
	pCircleControl3->Border = CRect(300, 150, 500, 350);
	pCircleControl3->SetMinVal(0);
	pCircleControl3->SetMaxVal(10);
	pCircleControl3->SetValue(0);
	pCircleControl3->IndicatorColor = Color(255, 190, 0, 0);
	pCircleControl3->DefaultValue = 5.f;
	pCircleControl3->IndicatorOnGear = false;
	pCircleControl3->IndicatorColor = Color::MakeARGB(255, 255, 255, 255);
	pCircleControl3->Name = L"pCircleControl3";
	pCircleControl3->ValueChanged.Subscribe([&](float val)
		{
			PluginViewWnd::ChangeButtonText(val);
		}
	);
	//Add(pCircleControl3);

	pLableControl1 = new LableControl(CRect(800, 50, 900, 70), std::to_wstring(pCircleControl3->GetValue()));
	pLableControl1->TextAlignmentH = LableControl::TextAlignmentStyle::Center;
	pLableControl1->Name = L"pLableControl1";

	//Add(pLableControl1);

	buttonControl1.SetText(std::to_wstring(pCircleControl3->GetValue()));

	EnvelopeGraphControl* envelop = new EnvelopeGraphControl(CRect(800, 500, 1300, 700));
	envelop->Curve = EnvelopeGraphControl::CurveType::Bezier;
	envelop->Name = L"envelop";
	Add(envelop);

	ButtonImage* btn = new ButtonImage(CRect(10, 10, 50, 50));
	btn->ImagePath = L"C:\\temp\\lock.png";
	btn->HoverBgColor = Color(255, 39, 45, 52); // Розовый hover
	btn->Name = L"btn";
	Add(btn);

	ButtonImage* btn2 = new ButtonImage(CRect(400, 100, 400+44, 100+36));
	btn2->ImagePath = L"C:\\temp\\lock2.png";
	btn2->HoverBgColor = Color(255, 47, 46, 45); 
	btn2->PressedColor = Color(255, 113, 170, 255);
	btn2->CornerStyle = CornerMask::TopLeft;
	btn2->Name = L"btn2";
	Add(btn2);

	ButtonRadioImage* radio1 = new ButtonRadioImage(CPoint(100, 200));
	radio1->ImagePath = L"C:\\temp\\radio2.png";  // Загрузится автоматически
	radio1->ButtonCount = 3;
	radio1->ButtonW = 44;
	radio1->ButtonH = 36;
	radio1->Spacing = 0;
	radio1->OrientationVertical = false;
	radio1->SelectedIndex = 1;
	radio1->HoverBgColor = Color(255, 47, 46, 45);
	radio1->PressedColor = Color(255, 113, 170, 255);
	radio1->SelectedIndex = 1;  // Вторая кнопка выбрана
	radio1->CrStyle = ButtonRadioImage::СornerStyle::RoundedEdges;
	radio1->Name = L"radio1";
	Add(radio1);

	LableControl* lable1 = new LableControl(CRect(300, 50, 400, 100), L"SEMI\n+0");
	//LableControl* lable1 = new LableControl(CRect(300, 50, 400, 100), L"HARMONIC\nHARMONIC");
	lable1->TextAlignmentH = LableControl::TextAlignmentStyle::Far;
	lable1->TextAlignmentV = LableControl::TextAlignmentStyle::Center;
	lable1->TextColor = Color(255, 113, 170, 255);
	lable1->FontSize = 12;
	lable1->FontName = L"Bahnschrift";
	lable1->CornerStyle = CornerMask::All;
	lable1->BackColor = Color::MakeARGB(255, 23, 26, 30);
	lable1->Name = L"lable1";
	Add(lable1);

	Panel* panel = new Panel(CRect(600, 100, 1300, 400));
	panel->Name = L"panel";

	ButtonImage* btn3 = new ButtonImage(CRect(10, 10, 10 + 44, 10 + 36));
	btn3->ImagePath = L"C:\\temp\\lock2.png";
	btn3->HoverBgColor = Color(255, 47, 46, 45);
	btn3->PressedColor = Color(255, 113, 170, 255);
	btn3->CornerStyle = CornerMask::TopLeft;
	btn3->CornerStyle = CornerMask::TopLeft;
	btn3->Name = L"btn3";

	KnobControl* circleControl4 = new KnobControl(CPoint(50, 50), 100);
	circleControl4->SetMinVal(0);
	circleControl4->SetMaxVal(10);
	circleControl4->SetValue(10);
	circleControl4->NotchCount = 10; // 20;
	circleControl4->CapColor = Color(255, 105, 35, 60);
	circleControl4->ScaleBigSteps = 2;
	circleControl4->ScaleSmallSteps = 6;
	circleControl4->IndicatorColor = Color(255, 190, 0, 0);
	//circleControl4->Sensitivity = 0.05f;
	circleControl4->IndicatorColor = Color::MakeARGB(255, 255, 255, 255);
	circleControl4->CapPercent = 80.;
	circleControl4->GearPercent = 5.;
	circleControl4->Scale = false;
	//pGainKnob->CapPercent = 60.;
	//pGainKnob->GearPercent = 20.;
	circleControl4->CapReductionPercent = 5.;
	circleControl4->SetTooltipText(L"A Level: 75% [-5.0 dB]");
	circleControl4->Name = L"circleControl4";

	panel->Add(btn3);
	panel->Add(circleControl4);

	Add(panel);
}

/// <summary>
/// Вывести значение в виде текста кнопки pButtonControl1
/// </summary>
/// <param name="val"></param>
void PluginViewWnd::ChangeButtonText(float val)
{
	if (pButtonControl1)
	{
		pButtonControl1->SetText(std::to_wstring(val));
	}
}

/// <summary>
/// Вывести значение val в виде текста pLableControl1
/// </summary>
/// <param name="val"></param>
void PluginViewWnd::ChangeLableText(float val)
{
	if (pLableControl1)
	{
		pLableControl1->SetText(std::to_wstring(val));
		pCircleControl3->SetTooltipText(std::to_wstring(val));

	}
}

/// <summary>
/// Добавить/убрать наблюдателя ChangeLableText при изменении флага pCheckBox1
/// </summary>
/// <param name="val"></param>
void PluginViewWnd::ChangeObserver2(bool val)
{
	if (pCircleControl3)
	{
		if (val)
		{
			//observer2 = pCircleControl3->ValueChanged.Subscribe(std::bind(&PluginViewWnd::ChangeLableText, this, std::placeholders::_1));
			observer2 = pCircleControl3->ValueChanged.Subscribe(
				[&](float val)
				{
					ChangeLableText(val);
				}
			);
		}
		else if(observer2 != nullptr)
		{
			pCircleControl3->ValueChanged.Unsubscribe(observer2);
			observer2 = nullptr;
		}
	}
}

PluginViewWnd::~PluginViewWnd()
{
}

BEGIN_MESSAGE_MAP(PluginViewWnd, PluginView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_INSERT_KNOB, OnInsertKnob)
	ON_COMMAND(ID_EDITMODE, OnEditMode)
	ON_COMMAND(ID_INSERT_PAD, OnInsertPad)
	ON_COMMAND(ID_INSERT_BUTTON, OnInsertButton)
	ON_COMMAND(ID_INSERT_ENVELOP, OnInsertEnvelop)
	ON_UPDATE_COMMAND_UI_RANGE(ID_INSERT_KNOB, ID_INSERT_ENVELOP, OnUpdateInsertControlCommand)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSOR_POS, OnUpdateCursorPos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_EDIT_MODE, OnUpdateIndicatorEditMode)
END_MESSAGE_MAP()

// Обработчики сообщений CChildView

BOOL PluginViewWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPCHILDREN;

	if (!PluginView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), CreateSolidBrush(RGB(200,200,200)), nullptr);

	return TRUE;
}

void PluginViewWnd::OnInsertKnob()
{
	POINT pt;
	GetCursorPos(&pt);
	//ScreenToClient(&pt);

	auto control = dynamic_cast<KnobControl*>(PluginControlFactory::Instance().Create(PluginGUI::KnobControl::pluginClassId));
	control->Border = CRect( pt, SIZE(40, 40));
	control->SetMinVal(-1.f);
	control->SetMaxVal(1.f);
	control->DefaultValue = 0.f;
	control->SetFullRuler(true);
	Color circleControlColor = Color::MakeARGB(255, 58, 72, 82);
	control->CapColor = circleControlColor;
	control->BaseColor = circleControlColor;
	control->RingColor = Color(255, 52, 59, 66);//Color(255, 105, 63, 35);
	control->ScaleColor = Color(255, 0, 0, 0);//Color(255, 134, 47, 45);
	control->IndicatorColor = Color(255, 255, 0, 0);
	control->CapPercent = 60.;
	control->GearPercent = 20.;
	control->CapReductionPercent = 5.;
	control->Scale = false;
	control->IndicatorOnGear = false;
	control->IndicatorColor = Color(255, 255, 255, 255);
	//control->SetMinVal(0);
	//control->SetMaxVal(10);
	//control->SetValue(0);
	//control->DrawBorder = true;
	this->Add(control);
	control->Invalidate();
}

void PluginViewWnd::OnInsertSlider()
{
	POINT pt;
	GetCursorPos(&pt);
	//ScreenToClient(&pt);

	auto control = dynamic_cast<SliderControl*>(PluginControlFactory::Instance().Create(PluginGUI::SliderControl::pluginClassId));
	control->Border = CRect(pt, SIZE(50, 300));
	control->SetMinVal(0);
	control->SetMaxVal(10);
	control->SetValue(0);
	this->Add(control);
	control->Invalidate();
}

void PluginViewWnd::OnInsertPad()
{
	POINT pt;
	GetCursorPos(&pt);
	//ScreenToClient(&pt);

	auto control = dynamic_cast<PadControl*>(PluginControlFactory::Instance().Create(PluginGUI::PadControl::pluginClassId));
	control->Border = CRect(pt, SIZE(200, 200));
	control->SetBaseColor(Color(255, 39, 44, 50));
	this->Add(control);
	control->Invalidate();
}

void PluginViewWnd::OnInsertButton()
{
	POINT pt;
	GetCursorPos(&pt);
	//ScreenToClient(&pt);

	auto control = dynamic_cast<ButtonControl*>(PluginControlFactory::Instance().Create(PluginGUI::ButtonControl::pluginClassId));
	control->Border = CRect(pt, SIZE(100, 30));
	control->SetBaseColor(Color(255, 39, 44, 50));
	this->Add(control);
	control->Invalidate();
}

void PluginViewWnd::OnInsertEnvelop()
{
	POINT pt;
	GetCursorPos(&pt);
	//ScreenToClient(&pt);

	auto control = dynamic_cast<EnvelopeGraphControl*>(PluginControlFactory::Instance().Create(PluginGUI::EnvelopeGraphControl::pluginClassId));
	control->SetPoints(
		{
			EnvelopeGraphControl::Point(0.0f, 0.0f),
			EnvelopeGraphControl::Point(0.25f, 0.5f, 0.0f, 0.0f, EnvelopeGraphControl::CurveType::Power),
			EnvelopeGraphControl::Point(0.5f, 1.0f),
			EnvelopeGraphControl::Point(0.75f, 0.5f, 0.0f, 0.0f, EnvelopeGraphControl::CurveType::Power),
			EnvelopeGraphControl::Point(1.0f, 0.0f)
		});
	control->Border = CRect(pt, SIZE(400, 200));
	this->Add(control);
	control->Invalidate();
}

void PluginViewWnd::OnUpdateInsertControlCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditMode());
}

void PluginViewWnd::OnMouseMove(UINT nFlags, CPoint p)
{
	m_strCursorPos.Format(_T("x: %d, y: %d"), p.x, p.y);
	PluginView::OnMouseMove(nFlags, p);
}

void PluginViewWnd::OnEditMode()
{
	//CFrameWnd* pFrame = (CFrameWnd*)GetParentFrame();
	//CMFCStatusBar* pStatus = (CMFCStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

	//TRACE(_T("OnChangeEditMode()\n"));
	SetEditMode(!GetEditMode());
	//int nIndex = pStatus->CommandToIndex(ID_INDICATOR_EDIT_MODE);
	//if (nIndex >= 0)
	//{
	//	CString str(GetEditMode() ? L"Edit: On" : L"Edit: Off");
	//	pStatus->SetPaneText(nIndex, str, TRUE);
	//}
}

void PluginViewWnd::OnUpdateCursorPos(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetText(m_strCursorPos);
}

void PluginViewWnd::OnUpdateIndicatorEditMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetText(GetEditMode() ? L"Edit: On" : L"Edit: Off");
}





