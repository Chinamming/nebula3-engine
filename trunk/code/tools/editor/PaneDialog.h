#pragma once

#include "resource.h" 
#include "models/model.h"
// CPaneDialog dialog
#define CPropertyGridDlgBase CXTPResizeDialog

class CPaneDialog : public CPropertyGridDlgBase
{
	DECLARE_DYNAMIC(CPaneDialog)

public:
	CPaneDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaneDialog();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MEDIUM };

public:
	CXTPPropertyGrid m_wndPropertyGrid;
	void SetModel(const Ptr<Models::Model>& model);
	void ProcessModelNode(const Util::String& nodeName);

	// Implementation
protected:
	HICON m_hIcon;
	Ptr<Models::Model> m_Model;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};
