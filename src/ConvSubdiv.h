#if !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
#define AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConvSubdiv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CConvSubdiv ダイアログ

class CConvSubdiv : public CDialog
{
// コンストラクション
public:
	CConvSubdiv(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConvSubdiv)
	enum { IDD = IDD_CONVERT_SUBDIV };
	CEdit	m_simp_l2norm;
	CEdit	m_cur_l2norm;
	CEdit	m_simp_nvertex;
	CEdit	m_cur_nvertex;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConvSubdiv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

 public:
	virtual BOOL Create( void );
	
// インプリメンテーション
 protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConvSubdiv)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnL2NormOk();
	afx_msg void OnL2NormCancel();
	afx_msg void OnConvReset();
	afx_msg void OnRadioDis();
	afx_msg void OnRadioNumvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONVSUBDIV_H__1950D551_5A59_4AFB_A577_53F4E33DAF96__INCLUDED_)
