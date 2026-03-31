
// MainFrm.cpp: реализация класса CMainFrame
//

#include "pch.h"
//#include "framework.h"
#include "TestMFCSimpleApp.h"
//#include "FramePropertyGridProperty.h"
#include "PluginGUI\include\PropertyRegistry.h"
#include "PluginGUI\include\Control.h"

#include "MainFrm.h"
#include "IndentedProperty.h"
#include "resource.h"	
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PluginGUI;
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSOR_POS, OnUpdateCursorPos)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	0,
	ID_INDICATOR_EDIT_MODE,
	ID_INDICATOR_CURSOR_POS
};
//static UINT indicators[] =
//{
//	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
//};

// Создание или уничтожение CMainFrame

CMainFrame::CMainFrame() noexcept:
	m_wndView(Gdiplus::Color(63, 72, 77/*200, 200, 200*/))
{
	m_wndView.OnSelectionChanged.Subscribe([this](Control* pCtrl) { OnSelectionChanged(pCtrl); });
}

void CMainFrame::OnSelectionChanged(Control* pCtrl)
{
	m_pSelectedControl = pCtrl;
	UpdatePropertyGrid(pCtrl);
}


CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.SetShowAllCommands(TRUE);

	// создать представление для размещения рабочей области рамки
	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Не удалось создать окно представлений\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	// 2. Подготавливаем ленту PNG
	//CMFCToolBarImages images;
	m_layoutToolBarImages.SetImageSize(CSize(16, 16)); // Размер одной иконки
	if (!m_layoutToolBarImages.Load(IDR_LAYOUT))
	{
		TRACE0("Failed to load images from IDR_LAYOUT\\n");
		return -1;
	}

	if (!m_layoutToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_ALIGN_ANY)  ||
		!m_layoutToolBar.LoadToolBar(IDR_LAYOUT, 0, 0, TRUE))
	{
		TRACE0("Не удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	//CSize tbSize = m_layoutToolBar.GetImageSize();
	//m_layoutToolBar.SetUserImages(&m_layoutToolBarImages);
	//m_layoutToolBar.AdjustSizeImmediate();

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Не удалось создать строку состояния\n");
		return -1;      // не удалось создать
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//m_wndStatusBar.SetPaneInfo(0, 0, SBPS_NOBORDERS | SBPS_STRETCH, 0); 
	//m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_EDIT_MODE, SBPS_NORMAL, 50);
	//m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_CURSOR_POS, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneText(1, L"Edit: Off");
	//CImage img;
	//img.LoadFromResource(AfxGetResourceHandle(), IDI_CURSOR_POSITION);
	//HICON hIcon = img.ExtractIcon(0); 
	HICON hCursorPosIcon = (HICON)::LoadImage(
		AfxGetResourceHandle(),           // Получаем хендл ресурсов приложения
		MAKEINTRESOURCE(IDI_CURSOR_POSITION),     // Превращаем числовой ID в ресурсный путь
		IMAGE_ICON,                       // Указываем, что загружаем именно иконку
		16, 16,                           // ЖЕСТКО задаем размер (важно для статус-бара)
		LR_DEFAULTCOLOR                   // Флаг загрузки (обычные цвета)
	);
	m_wndStatusBar.SetPaneIcon(2, hCursorPosIcon);

	HICON hEditDocumentIcon = (HICON)::LoadImage(
		AfxGetResourceHandle(),           // Получаем хендл ресурсов приложения
		MAKEINTRESOURCE(IDI_EDIT_DOCUMENT),     // Превращаем числовой ID в ресурсный путь
		IMAGE_ICON,                       // Указываем, что загружаем именно иконку
		16, 16,                           // ЖЕСТКО задаем размер (важно для статус-бара)
		LR_DEFAULTCOLOR                   // Флаг загрузки (обычные цвета)
	);
	m_wndStatusBar.SetPaneIcon(1, hEditDocumentIcon);

	m_wndStatusBar.SetPaneWidth(1, 200);
	m_wndStatusBar.SetPaneWidth(2, 200);

	// Для стиля VS 2022 уберите рамки у всех панелей:
	m_wndStatusBar.SetPaneStyle(0, SBPS_STRETCH | SBPS_NORMAL);
	m_wndStatusBar.SetPaneStyle(1, SBPS_NORMAL);
	m_wndStatusBar.SetPaneStyle(2, SBPS_NORMAL);

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_layoutToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	//EnableLoadDockState(FALSE);

	DockPane(&m_wndMenuBar);
	DockPane(&m_layoutToolBar);
	DockPaneLeftOf(&m_wndToolBar, &m_layoutToolBar);
	//DockPane(&m_layoutToolBar);

	//EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);
	//EnableDocking(CBRS_ALIGN_LEFT);
	//EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	if (!CreateDockingWindows())
		return -1;

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	//CList<UINT, UINT> lstBasicCommands;

	//lstBasicCommands.AddTail(ID_APP_EXIT);
	//lstBasicCommands.AddTail(ID_EDIT_CUT);
	//lstBasicCommands.AddTail(ID_EDIT_PASTE);
	//lstBasicCommands.AddTail(ID_EDIT_UNDO);
	//lstBasicCommands.AddTail(ID_APP_ABOUT);
	//lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	//lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// Диагностика CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// Обработчики сообщений CMainFrame

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// передача фокуса окну представления
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// разрешить ошибки в представлении при выполнении команды
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// в противном случае выполняется обработка по умолчанию
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnUpdateCursorPos(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);  // Предотвращает сброс текста
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

// MainFrame.cpp
BOOL CMainFrame::CreateDockingWindows()
{
	if (!m_wndPropertyBar.Create(_T("Свойства"),
		this,
		CRect(0, 0, 200, 400),
		TRUE,                  // bHasGripper
		1,                     // nID
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT,
		AFX_CBRS_RESIZE | AFX_CBRS_CLOSE))
	{
		TRACE0("Не удалось создать панель свойств\n");
		return FALSE;
	}

	m_wndPropertyBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPropertyBar);

	///////////////////////////////////////////////////////////////////////////////
	//CMFCPropertyGridCtrl* pGrid = m_wndPropertyBar.GetGrid();
	//if (!pGrid)
	//	return false;

	//if (pGrid->GetSafeHwnd() == NULL)
	//{
	//	// Если зашли сюда — грид еще не создан как окно!
	//	return false;
	//}

	//pGrid->SetRedraw(FALSE);

	//// 1. Очистить всё
	//pGrid->RemoveAll();

	//_variant_t v1(L"");
	//_variant_t v2(10L);

	//auto* pGroup = new CMFCPropertyGridProperty(L"Тестовая Группа", v1, L"Описание");

	////pGrid->AddProperty(pGroup);

	//// Создаем ребенка
	//auto* pChild = new CMFCPropertyGridProperty(L"Поле 1", v2, L"");

	//// Добавляем ребенка в группу ДО добавления группы в грид
	//pGroup->AddSubItem(pChild);

	//pGrid->SetRedraw(TRUE);  // Разрешаем отрисовку
	//pGrid->AdjustLayout();   // Принудительно пересчитываем геометрию
	//pGrid->Invalidate();

	return TRUE;
}

void CMainFrame::OnPropertyGridChanged(CMFCPropertyGridProperty* pProp)
{
	if (!m_pSelectedControl || !pProp)
		return;

	// Извлечь Id
	int id = static_cast<int>(pProp->GetData());

	if (pProp->GetParent() != nullptr && id == -1)
	{
		pProp = pProp->GetParent();
		id = static_cast<int>(pProp->GetData());   // id из головного свойства
	}

	// Получить тип свойства
	const PropertyInfo* pPropInfo = m_pSelectedControl->FindProperty(id);
	if (!pPropInfo)
		return;

	// COleVariant из PropertyGrid
	COleVariant var = pProp->GetValue();
	//if (var.vt == VT_EMPTY)
	//	return;

	// Создать Value как Variant, соответствующий pPropInfo->m_type
	Variant newValue;
	switch (pPropInfo->m_type)
	{
		case TypeId::Bool:
			newValue = static_cast<bool>(V_BOOL(&var) != 0);
			break;
		case TypeId::Color:
		{
			DWORD rgb = V_UI4(&var);
			Gdiplus::Color c(
				GetRValue(rgb),
				GetGValue(rgb),
				GetBValue(rgb)
			);
			newValue = c;
		}
		break;
		case TypeId::WString:
			newValue = std::wstring(OLE2CW(var.bstrVal));
			break;
		case TypeId::Int:
			newValue = static_cast<int>(V_I4(&var));
			break;
		case TypeId::Float:
			newValue = static_cast<float>(V_R8(&var));
			break;
		case TypeId::Frame:
			{
				// Получить актуальный Frame из Control
				Variant v = m_pSelectedControl->GetPropertyValue(id);
				Frame frame = std::get<Frame>(v);

				// Для Frame обновляем из подсвойств Left/Top/Right/Bottom
				for (int i = 0; i < pProp->GetSubItemsCount(); ++i)
				{
					CMFCPropertyGridProperty* pSub = pProp->GetSubItem(i);
					CString name = pSub->GetName();

					COleVariant subVar = pSub->GetValue();
					if (subVar.vt == VT_EMPTY)
						continue;

					double val = (subVar.vt == VT_R8) ? subVar.dblVal :
						(subVar.vt == VT_I4) ? subVar.lVal : 0.0;

					if (name == _T("Left"))
						frame.left = static_cast<int>(val);
					else if (name == _T("Top"))
						frame.top = static_cast<int>(val);
					else if (name == _T("Right"))
						frame.right = static_cast<int>(val);
					else if (name == _T("Bottom"))
						frame.bottom = static_cast<int>(val);
				}

				newValue = frame;
			}			
			break;
		default:
			// fallback
			return;
	}

	// 2. Установить в Control
	bool bOk = m_pSelectedControl->SetPropertyValue(id, newValue);

	if (!bOk)
		pProp->ResetOriginalValue();   // отменить изменение в UI
}

std::string GetShortClassName(const char* typeName)
{
	std::string s(typeName);
	std::regex re("::([[:alnum:]_]+)$");     // ищем последний :: за которым идёт имя
	std::smatch match;

	if (std::regex_search(s, match, re) && match.size() > 1)
		return match[1].str();               // уже std::string

	// fallback
	return s;
}

void CMainFrame::UpdatePropertyGrid(Control* pCtrl)
{
	USES_CONVERSION;

	CMFCPropertyGridCtrl* pGrid = m_wndPropertyBar.GetGrid();
	if (!pGrid || !pCtrl)
		return;

	if (pGrid->GetSafeHwnd() == NULL)
	{
		// Если зашли сюда — грид еще не создан как окно!
		return;
	}

	// 1. Очистить всё
	pGrid->RemoveAll();

	// 2. Карта: className → группа‑свойствойств
	std::unordered_map<std::string, CMFCPropertyGridProperty*> groupMap;

	// 2. Пройти по всем Id, которые есть в этом типе контрола
	for (int id = 0; id < pCtrl->GetPropertyCount(); ++id)  // ph: id < MaxPropertiesGuess
	{
		const PropertyInfo* pPropInfo = pCtrl->FindProperty(id);
		if (!pPropInfo)
			continue;

		const std::type_info& owner = pCtrl->GetPropertyOwnerType(id);
		const char* classNameRaw = owner.name();
		std::string className = classNameRaw ? GetShortClassName(classNameRaw) : "Unknown";

		// 3. Получить значение из Control
		Variant value = pCtrl->GetPropertyValue(id);

		// 4. Convert to COleVariant
		COleVariant var = ConvertToVariant(value);

		// 5. Создать свойство в PropertyGrid
		CMFCPropertyGridProperty* pProp = nullptr;

		auto opt_meta = PropertyRegistry::Instance().Get(id);

		CString propName = A2T(pPropInfo->m_uiName.c_str());

		switch (pPropInfo->m_type)
		{
			case TypeId::Bool:
				pProp = new CMFCPropertyGridProperty(propName, var);
				break;

			case TypeId::Color:
				{
					Gdiplus::Color c = std::get<Gdiplus::Color>(value);
					DWORD rgb = RGB(c.GetR(), c.GetG(), c.GetB());
					CMFCPropertyGridColorProperty* pColorProp =
						new CMFCPropertyGridColorProperty(propName, rgb, FALSE);
					pProp = pColorProp;
					pColorProp->EnableOtherButton(L"&Choose Color...", TRUE);
				}
				break;

			case TypeId::WString:
				pProp = new CMFCPropertyGridProperty(propName, var);
				break;

			case TypeId::Int:
				pProp = new CMFCPropertyGridProperty(propName, var);
				break;

			case TypeId::Float:
				pProp = new CMFCPropertyGridProperty(propName, var);
				break;

			case TypeId::Frame:
				{
					Frame frame = std::get<Frame>(value);
					CRect rect;
					m_wndView.GetClientRect(&rect);

					CMFCPropertyGridProperty* pBorder = new CMFCPropertyGridProperty(propName, 0, TRUE);

					pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t)frame.left, _T(""), -1);
					pProp->EnableSpinControl(TRUE, 0, rect.Width());
					pBorder->AddSubItem(pProp);

					pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t)frame.top, _T(""), -1);
					pProp->EnableSpinControl(TRUE, 0, rect.Height());
					pBorder->AddSubItem(pProp);

					pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t)frame.right, _T(""), -1);
					pProp->EnableSpinControl(TRUE, 0, rect.Width());
					pBorder->AddSubItem(pProp);

					pProp = new CMFCPropertyGridProperty(_T("Bottom"), (_variant_t)frame.bottom, _T(""), -1);
					pProp->EnableSpinControl(TRUE, 0, rect.Height());
					pBorder->AddSubItem(pProp);

					pProp = pBorder;
				}
				break;

			default:
				pProp = new CMFCPropertyGridProperty(propName, var);
				break;
		}

		if (pProp)
		{
			if (opt_meta.has_value())
			{
				const PropertyRegistry::PropertyMetadata& meta = *opt_meta;

				if (!meta.description.empty())
				{
					pProp->SetDescription(meta.description.c_str());
				}

				if (meta.readonly)
				{
					pProp->Enable(FALSE);
				}
			}

			pProp->SetData(id);   // хранить Id в Data

			// --- ГРУППИРОВКА по className ---

			CMFCPropertyGridProperty* pGroup = nullptr;

			auto it = groupMap.find(className);
			if (it == groupMap.end())
			{
				pGroup = new CMFCPropertyGridProperty(CString(className.c_str()));
				pGroup->Enable(FALSE);
				groupMap[className] = pGroup;
			}
			else
			{
				pGroup = it->second;
			}

			pGroup->AddSubItem(pProp);

			//pGrid->AddProperty(pProp);
		}
	}

	// 4. Добавить группы в грид
	for (auto& kv : groupMap)
	{
		pGrid->AddProperty(kv.second);
	}

	pGrid->AdjustLayout();
}

COleVariant CMainFrame::ConvertToVariant(const Variant& v)
{
	return std::visit([](const auto& arg) -> COleVariant
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, bool>)
				return COleVariant(static_cast<LONG>(arg));
			else if constexpr (std::is_same_v<T, Gdiplus::Color>)
				return COleVariant(static_cast<LONG>(
					RGB(arg.GetR(), arg.GetG(), arg.GetB())));
			else if constexpr (std::is_same_v<T, std::wstring>)
				return COleVariant(arg.c_str());
			else if constexpr (std::is_same_v<T, int>)
				return COleVariant(static_cast<LONG>(arg));
			else if constexpr (std::is_same_v<T, float>)
				return COleVariant(static_cast<DOUBLE>(arg));
			else if constexpr (std::is_same_v<T, Frame>)
				return COleVariant(L"<Frame>");   // заглушка
			else
				static_assert(!sizeof(T), "Not implemented");
		}, v);
}

