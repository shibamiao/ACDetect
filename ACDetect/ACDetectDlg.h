// ACDetectDlg.h : header file
//

#if !defined(AFX_ACDETECTDLG_H__4F70D411_FE9F_4641_8B9F_196BE787E269__INCLUDED_)
#define AFX_ACDETECTDLG_H__4F70D411_FE9F_4641_8B9F_196BE787E269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "highgui.h"
#include "COMP_Shape.h"
#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CACDetectDlg dialog

class CACDetectDlg : public CDialog
{
// Construction
public:
	CACDetectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CACDetectDlg)
	enum { IDD = IDD_ACDETECT_DIALOG };
	CListCtrl	m_ListCompProblem;
	CStatic	m_merge_pic;
	CStatic	m_minus_pic;
	CStatic	m_test_cout_pic;
	CStatic	m_test_pic;
	CStatic	m_std_cout_pic;
	CStatic	m_std_pic;
	UINT	m_thresold;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACDetectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
    void CACDetectDlg::CutoutPic(CString filename,CString &savename);
	bool CACDetectDlg::MatchStd(CString TestFileName, CString &StdFileName);
	void CACDetectDlg::RegulatetPic(CString &testPicName, CString &stdPicName);
	CString CACDetectDlg::SubtractPic(CString TestPicPath, CString StdPicPath);
	void CACDetectDlg::GetPcbInfo(CString filename);
	int CACDetectDlg::dtoi(double x);
	void CACDetectDlg::DrawPcb();
	CString CACDetectDlg::ThresholdPic(CString filename, int pos);
	CString CACDetectDlg::Edgepickpic(CString filename);
	void CACDetectDlg::FindRect(CString imagename);
	void CACDetectDlg::JudgeRect( );
	void CACDetectDlg::JudgePixel(CString imagename);
	void CACDetectDlg::AlphaMerge(CString PCBImage, CString Imagename);
	void CACDetectDlg::ListProbComp();
	void CACDetectDlg::DisplayPic(CString m_filename, CRect rect);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACDetectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOpenTest();
	afx_msg void OnBtnOpenDesigne();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString pathname_edgepick;
	CString pathname_threshold;
	CString pathname_pcb;
	CString pathname_designfile;
	int thresold;
	CSize ImgSize;
	CString strYPoint;
	CString strXPoint;
	CString pathname_std;
	CString pathname_test;
	CString pathname_std_cout;
	CString pathname_test_cout;
	CString pathname_minus;
	CString pathname_merge;

	CList<CRect,CRect> listRect;
	CList<CPoint,CPoint> listRectComp;//矩形判断的问题器件 0901 change CString to CPoint
	CList<COMP_List,COMP_List> listPixelComp;//像素判断的问题器件 0901 change CString to CPoint
	CList<COMP_Shape,COMP_Shape&> listComponents;//器件列表（形状）
	CList<COMP_List,COMP_List> listDesignComp;//设计文件所有器件
	CArray<CvPoint,CvPoint> arrayCircOutline;// 电路轮廓

	bool FlagTest;
	bool FlagDesign;
	bool FlagShow;


  public:
	GdiplusStartupInput m_gdiplusStartupInput;
    ULONG_PTR m_pGdiToken;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACDETECTDLG_H__4F70D411_FE9F_4641_8B9F_196BE787E269__INCLUDED_)
