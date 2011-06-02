// ExplorerView.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2011 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdneb.h"
#include "ExplorerView.h"

using namespace Models;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorerView

CExplorerView::CExplorerView()
{
	//m_nToolBarID = IDR_EXPLORER_BAR;
}

CExplorerView::~CExplorerView()
{
}


BEGIN_MESSAGE_MAP(CExplorerView, CPaneView)
	//{{AFX_MSG_MAP(CExplorerView)
	//ON_COMMAND_RANGE(ID_EXPLORER_PARENT, ID_EXPLORER_SEARCH, OnExplorerCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExplorerView message handlers

CWnd* CExplorerView::OnCreateView()
{
	if (!m_wndExplorer.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, 0xFF ))
	{
		TRACE0( "Unable to create tree control.\n" );
		return NULL;
	}
	//m_wndExplorer.ModifyStyleEx(0, WS_EX_STATICEDGE);


	// add the parent item, make it bold
	HTREEITEM htiParent = m_wndExplorer.InsertItem(_T("Codejock Software"));
	m_wndExplorer.SetItemState (htiParent, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM htiChild; // child item

	htiChild = m_wndExplorer.InsertItem (_T("Library"), 1, 1, htiParent);

	// add the children of the parent item
	HTREEITEM hti = m_wndExplorer.InsertItem(_T("Icons"), 2, 3, htiChild);
	m_wndExplorer.InsertItem(_T("Temporary Files"), 4, 4, htiChild);
	m_wndExplorer.InsertItem(_T("New Files"), 4, 4, htiChild);
	m_wndExplorer.Expand(hti, TVE_EXPAND);

	hti = m_wndExplorer.InsertItem(_T("Deleted Items"), 2, 3, htiParent);

	m_wndExplorer.Expand(htiParent, TVE_EXPAND);
	m_wndExplorer.Expand(htiChild, TVE_EXPAND);


	return &m_wndExplorer;
}

void CExplorerView::OnExpandModel(const Ptr<Model>& model)
{
	m_wndExplorer.DeleteAllItems();
	const Ptr<ModelNode>& modelNode = model->GetRootNode();
	Util::String nodeName = modelNode->GetName().AsString();
	HTREEITEM htiRoot = m_wndExplorer.InsertItem(nodeName.AsCharPtr());
	m_wndExplorer.SetItemState (htiRoot, TVIS_BOLD, TVIS_BOLD);
	m_wndExplorer.AddNode(nodeName,nodeName);
	const Util::Array<Ptr<ModelNode> >&childs = modelNode->GetChildren();
	for (IndexT index=0;index < childs.Size();index++)
	{
		const Ptr<Models::ModelNode>& childNode = childs[index];
		n_assert( childNode.isvalid());
		this->OnExpandModelNode(childNode,htiRoot,nodeName);
	}
	m_wndExplorer.Expand(htiRoot, TVE_EXPAND);
}

void CExplorerView::OnExpandModelNode(const Ptr<Models::ModelNode>& modelNode,HTREEITEM& htiParent,const Util::String& parentPath)
{
	Util::String nodeName = modelNode->GetName().AsString();
	Util::String nodePath = parentPath;
	nodePath			 += "/";
	nodePath			 += nodeName;
	HTREEITEM htiRoot  = m_wndExplorer.InsertItem(nodeName.AsCharPtr(),1,1,htiParent);
	m_wndExplorer.AddNode(nodeName,nodePath);

	const Util::Array<Ptr<ModelNode> >&childs = modelNode->GetChildren();
	for (IndexT index=0;index < childs.Size();index++)
	{
		const Ptr<Models::ModelNode>& childNode = childs[index];
		n_assert( childNode.isvalid());
		this->OnExpandModelNode(childNode,htiRoot,nodePath);
	}
	m_wndExplorer.Expand(htiRoot, TVE_EXPAND);
}

void CExplorerView::OnExplorerCommand(UINT)
{
	// TODO: Add your command handler code here

}

