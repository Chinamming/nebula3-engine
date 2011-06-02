// PaneDialog.cpp : implementation file
//

#include "stdneb.h"
#include "PaneDialog.h"
#include "models/nodes/statenode.h"

using namespace Models;
// CPaneDialog dialog

IMPLEMENT_DYNAMIC(CPaneDialog, CPropertyGridDlgBase)

CPaneDialog::CPaneDialog(CWnd* pParent /*=NULL*/)
	: CPropertyGridDlgBase(CPaneDialog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPaneDialog::~CPaneDialog()
{	
}

void CPaneDialog::DoDataExchange(CDataExchange* pDX)
{
	CPropertyGridDlgBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaneDialog, CPropertyGridDlgBase)
	//ON_WM_PAINT()
END_MESSAGE_MAP()

void CPaneDialog::SetModel(const Ptr<Models::Model>& model)
{
	this->m_Model = model;
}

void CPaneDialog::ProcessModelNode(const Util::String& nodeName)
{
	m_wndPropertyGrid.ResetContent();
	//this->m_Model->LookupNode()
	const Ptr<ModelNode>& modelNode = this->m_Model->LookupNode(nodeName);
	if( modelNode.isvalid() )
	{
		if(modelNode->IsA(Models::StateNode::RTTI) )
		{
			/// 
			const Ptr<StateNode>& stateNode = modelNode.downcast<StateNode>();
			m_wndPropertyGrid.SetVariableItemsHeight(TRUE);
			LOGFONT lf;
			GetFont()->GetLogFont( &lf );
			// create document settings category.
			CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("StateNode"));
			m_wndPropertyGrid.SetTheme(xtpGridThemeNativeWinXP);
			pSettings->SetTooltip(_T("Matrial Settings Category"));
			pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Shader"),stateNode->GetShader().AsString().AsCharPtr()));

			CXTPPropertyGridItem* pParameters        = m_wndPropertyGrid.AddCategory(_T("Parameters"));
			const Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> >&shaderParams = stateNode->GetShaderParameter();
			for (IndexT index=0;index < shaderParams.Size();index++ )
			{
				if( shaderParams[index].Value().GetType() == Util::Variant::Int )
				{
					pParameters->AddChildItem(
						new CXTPPropertyGridItemNumber(shaderParams[index].Key().AsString().AsCharPtr(),shaderParams[index].Value().GetInt())
						);
				}
				else if( shaderParams[index].Value().GetType() == Util::Variant::Float )
				{
					pParameters->AddChildItem(
						new CXTPPropertyGridItemDouble(shaderParams[index].Key().AsString().AsCharPtr(),shaderParams[index].Value().GetFloat(),"%f")
						);
				}
				else if( shaderParams[index].Value().GetType() == Util::Variant::String )
				{
					pParameters->AddChildItem(
						new CXTPPropertyGridItem(shaderParams[index].Key().AsString().AsCharPtr(),shaderParams[index].Value().GetString().AsCharPtr())
						);
				}
			}///end for
			pSettings->Expand();
			pParameters->Expand();
		}
	}

	
}


// CPaneDialog message handlers
BOOL CPaneDialog::OnInitDialog()
{
	CPropertyGridDlgBase::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	CRect rcWorkspace;//GetDlgItem(IDC_STATIC)
	this->GetWindowRect(rcWorkspace);
	ScreenToClient(rcWorkspace);

	if ( m_wndPropertyGrid.Create( rcWorkspace, this, IDC_PROPERTY_GRID ) )
	{
		m_wndPropertyGrid.SetVariableItemsHeight(TRUE);

		LOGFONT lf;
		GetFont()->GetLogFont( &lf );

		// create document settings category.
		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("Document Settings"));
		m_wndPropertyGrid.SetTheme(xtpGridThemeNativeWinXP);

		pSettings->SetTooltip(_T("Document Settings Category"));

		// add child items to category.
		CXTPPropertyGridItem* pItemSaveOnClose = pSettings->AddChildItem(new CXTPPropertyGridItemBool(_T("SaveOnClose"), TRUE));
		pSettings->AddChildItem(new CXTPPropertyGridItemFont(_T("WindowFont"), lf));
		pSettings->AddChildItem(new CXTPPropertyGridItemSize(_T("WindowSize"), CSize(100, 100)));

		pSettings->Expand();
		pItemSaveOnClose->Select();

		// create global settings category.
		CXTPPropertyGridItem* pGlobals      = m_wndPropertyGrid.AddCategory(_T("Global Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemGreeting = pGlobals->AddChildItem(new CXTPPropertyGridItem(_T("Greeting Text"), _T("Welcome to your application!")));
		pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("ItemsInMRUList"), 4));
		CXTPPropertyGridItem* pItemRate     = pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("MaxRepeatRate"), 10));
		pGlobals->AddChildItem(new CXTPPropertyGridItemColor(_T("ToolbarColor"), RGB(255, 192,128)));

		pItemGreeting->SetReadOnly(TRUE);
		pItemRate->SetDescription(_T("The rate in milliseconds that the text will repeat."));
	}

	// Set control resizing.
	SetResize(IDC_PROPERTY_GRID, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}