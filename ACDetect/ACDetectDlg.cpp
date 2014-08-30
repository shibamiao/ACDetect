// ACDetectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACDetect.h"
#include "ACDetectDlg.h"

#include "fstream.h"
#include "highgui.h"

#include "cv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACDetectDlg dialog

CACDetectDlg::CACDetectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACDetectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACDetectDlg)
	m_thresold = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CACDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACDetectDlg)
	DDX_Control(pDX, IDC_LIST, m_ListCompProblem);
	DDX_Control(pDX, IDC_MERGE, m_merge_pic);
	DDX_Control(pDX, IDC_MINUS, m_minus_pic);
	DDX_Control(pDX, IDC_TEST_COUT, m_test_cout_pic);
	DDX_Control(pDX, IDC_TEST, m_test_pic);
	DDX_Control(pDX, IDC_STD_COUT, m_std_cout_pic);
	DDX_Control(pDX, IDC_STD, m_std_pic);
	DDX_Text(pDX, IDC_EDIT_THRESOLD, m_thresold);
	DDV_MinMaxUInt(pDX, m_thresold, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACDetectDlg, CDialog)
	//{{AFX_MSG_MAP(CACDetectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_TEST, OnBtnOpenTest)
	ON_BN_CLICKED(IDC_BTN_OPEN_DESIGNE, OnBtnOpenDesigne)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACDetectDlg message handlers

BOOL CACDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, TRUE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ListCompProblem.InsertColumn(0,"���",LVCF_TEXT,40,-1);
	m_ListCompProblem.InsertColumn(1,"��װ",LVCF_TEXT,80,-1);
	m_ListCompProblem.InsertColumn(2,"�ͺ�",LVCF_TEXT,50,-1);
	m_ListCompProblem.InsertColumn(3,"���",LVCF_TEXT,50,-1);
	m_ListCompProblem.InsertColumn(4,"��������x(mm)",LVCFMT_LEFT,80,-1);
	m_ListCompProblem.InsertColumn(5,"��������y(mm)",LVCFMT_LEFT,80,-1);
	m_ListCompProblem.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


	FlagTest=false;
	FlagDesign=false;
	FlagShow=false;

	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CACDetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CACDetectDlg::OnPaint() 
{
	/*if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}*/
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(FlagShow)
	{
		//�ػ�ԭʼ��׼ͼ
		//		CString org_stdfilename("pic\\orgbiaozhun.bmp");//�ļ�·��
		CRect org_stdrect;
		GetDlgItem(IDC_MERGE)->GetWindowRect(&org_stdrect);
		ScreenToClient(org_stdrect);
		DisplayPic(pathname_merge,org_stdrect);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CACDetectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//�������ܣ���ȡ��Ч��ͼ
void CACDetectDlg::CutoutPic(CString filename,CString &savename)
{
	IplImage *img = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);//����ͼ��
// 	if (img==NULL)//���û��ͼ����ʾ
// 	{
// 		MessageBox("no image!",NULL,MB_ICONWARNING);
// 		return;
// 	}
	CvPoint first = cvPoint(0,0),last = cvPoint(0,0);//��ʼ����һ��������һ����
	CvScalar s0;//�������ֵ	
	for(int i=0;i<img->height;i++)  //ȥ�����������
	{
		for(int j=0;j<50;j++)//33Ϊ��Լ��
		{
			cvSet2D(img,i,j,cvScalar(0,0,0));//����������Ϊ��ɫ
		}
	}	

	
	BOOL is_up = false;
	//ɨ������ͼ
	for(i=10;i<img->height;i++)
	{
		for(int j=10;j<img->width;j++)
		{
			s0 = cvGet2D(img,i,j);//�������������ֵ
			//����ɫΪ��ɫ���߰�ɫ,�ж��Ƿ������������ɫ֮һ
			BOOL br0 = (int)s0.val[0] == 0;//��Ϊ0ֵ��Ϊ��
			BOOL bg0 = (int)s0.val[1] == 0;
			BOOL bb0 = (int)s0.val[2] == 0;
			BOOL br1 = (s0.val[0]-254>0);//��Ϊ255��Ϊ��
			BOOL bg1 = (s0.val[1]-254>0);
			BOOL bb1 = (s0.val[2]-254>0);
			
			BOOL brgb0 = br0 && bg0 &&bb0;
			BOOL brgb1 = br1 && bg1 &&bb1;
			
			if(!(brgb0 || brgb1))  //�ж��ǲ��Ǳ���  ͼ�����Ϊ300*300
			{
				if(!is_up)  //ͼ��ԭ�������Ͻ�
				{
					first.x = i;
					first.y = j;
					is_up = true;
				}					
			    if(is_up)
				{
					if (i<=first.x)//ȡ��С��iֵ��Ϊ��С�����xֵ
					{
						first.x=i;
					}
					if(j<=first.y)  //ȡ��С��jΪ���Ͻ������yֵ
					{
						first.y = j;
					}
					if(i>=last.x) //ȡ����iֵ��Ϊ��������xֵ (�޸ģ�20110906 first.x��Ϊlast.x)
					{
						last.x = i;
					}
					if(j>=last.y) //ȡ����jֵ��Ϊ��������yֵ (�޸ģ�20110906 first.x��Ϊlast.x)
					{
						last.y = j;
					}		
				}
			}	
		}
	}

	
	
	
	//�ٳ�ͼ��������б���
	CvSize sz;
	sz.width = last.y - first.y + 1;//��ͼ��Ŀ�
	sz.height = last.x - first.x + 1;//��ͼ��ĸ�	
	IplImage * image = cvCreateImage(sz, 8,3);//������ͼ��

	cvZero(image);//�������
	//��ԭʼͼ�����Ч�����ͼ��ֵ����ͼ��
	CvScalar s1,s2;	
	for(i = 0;i < sz.height;i++)
	{
		for(int j = 0;j < sz.width;j++)
		{
			

			s1=cvGet2D(img,i+first.x,j+first.y);//�����Ч��������ֵ
			s2.val[0] = s1.val[0];//3��ͨ���ֱ�ֵ
			s2.val[1] = s1.val[1];
			s2.val[2] = s1.val[2];
			cvSet2D(image,i,j,s2);//������ͼ�������ֵ
		}
	}

	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //��õ�ǰ.exe�ļ���·��
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	savename=sPath+"\\"+filename.Right(filename.GetLength()-filename.ReverseFind('\\')-1);
	
 	cvSaveImage(savename,image);//����ͼ��
	

}

//���ݴ���ͼƥ���׼ͼ
bool CACDetectDlg::MatchStd(CString TestFileName, CString &StdFileName)
{
	/*int pz = TestName.Find("����");
	MessageBox(""+TestName.GetAt(pz));
	return true;*/
	char chExePath[128];
	memset(chExePath, 0, 128);
	GetModuleFileName(NULL, chExePath, 128);
	//MessageBox(chExePath);

	int start = TestFileName.Find("����");
	int end = TestFileName.ReverseFind('.');
	int rstart = TestFileName.Find("����");
	int rend = TestFileName.ReverseFind('.');

	CString ExePath(chExePath);
	ExePath	= ExePath.Left(ExePath.ReverseFind('\\')+1);
	
	CString pattern("");
	if(start!=-1)
		pattern = TestFileName.Mid(start, end-start);
	if(rstart != -1)
		pattern = TestFileName.Mid(rstart, rend-rstart);
	
	CFileFind FileFinder;
	ExePath += "standard\\";
	BOOL HasNextFile = FileFinder.FindFile(ExePath+"*.*");
	
	while(HasNextFile)
	{
		HasNextFile = FileFinder.FindNextFile();
		StdFileName = FileFinder.GetFileName();
		if(StdFileName.Find(pattern)!=-1)
		{
			StdFileName =ExePath+StdFileName;
			return true;
		}
	}
	return false;
}

//����ͼƬ��С
void CACDetectDlg::RegulatetPic(CString &testPicName, CString &stdPicName)
{
	IplImage *std = cvLoadImage(stdPicName,CV_LOAD_IMAGE_COLOR);//��׼����Ч��ͼ�������ĳߴ�Ϊ����
	IplImage *test = cvLoadImage(testPicName,CV_LOAD_IMAGE_COLOR); //�������Ч��ͼ 	
    IplImage *test_new = cvCreateImage(cvGetSize(std),std->depth,std->nChannels);//������ͼ��

	CvSize size_std,size_test;
	CvScalar rgb_std,rgb_test,rgb_test_new;
	CvSize temptest;
	int i = 0;
	int j = 0;

	size_std = cvGetSize(std);//��׼����Ч��ͼ�ߴ�
	size_test = cvGetSize(test);//�������Ч��ͼ�ߴ�
	temptest = size_test;
	int wdis,hdis;
	wdis = abs(size_test.height - size_std.height)/2;//�������Ч��ͼ���׼����Ч��ͼ�ĸ�֮���һ��
	hdis = abs(size_test.width - size_std.width)/2;//�������Ч��ͼ���׼����Ч��ͼ�Ŀ�֮���һ��
	//���1��std����test����
	if((size_test.height >= size_std.height) && (size_test.width >= size_std.width)) 
	{
		for(i = 0; i<size_std.height; i++ )
		{
			for(j = 0; j<size_std.width; j++)
			{
				//��ȡ�������Ч��ͼ���м�����ֵ����ͼ,
				rgb_test = cvGet2D(test,i+wdis,j+hdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];				
				cvSet2D(test_new,i,j,rgb_test_new);			
			}	
		}
	}
	//���2��std���test
	else if((size_test.height >= size_std.height) && (size_test.width < size_std.width))
	{
		for(i=0;i<size_std.height;i++)
		{
			//	����Ч��׼��ͼ���wdis��ȣ����Ƹ���ͼ������ࣻ
			for(j=0;j<wdis;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			// 	����Ч��׼��ͼ�Ҳ�wdis��ȣ����Ƹ���ͼ�����Ҳࣻ
			for(j=wdis;j<size_test.width+wdis;j++)
			{
				rgb_test = cvGet2D(test,i+hdis,j);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			// 	����Ч������ͼ��ȥ����hdis��ֵ�����Ƹ���ͼ��
			for(j=size_test.width+wdis;j<size_std.width;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
		}
	}
	//���3��std����test
	else if((size_test.height < size_std.height) && (size_test.width >= size_std.width))
	{
		for(j=0;j<size_std.width;j++)
		{
			//	����Ч��׼��ͼ�ϲ�hdis�߶ȣ����Ƹ���ͼ���ϲࣻ
			for(i=0;i<hdis;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			 //	����Ч��׼��ͼ�Ҳ�hdis��߶ȣ����Ƹ���ͼ���²ࣻ
			for(i=hdis;i<size_test.height+hdis;i++)
			{
				rgb_test = cvGet2D(test,i,j+hdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			// 	����Ч������ͼ��ȥ���Ҳ�hdis��ֵ�����Ƹ���ͼ��
			for(i=size_test.height+hdis;i<size_std.height;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
		}
	}
	else  //���4��std����test
	{
		for(j=0;j<size_std.width;j++)
		{	
			//����Ч��׼��ͼ�ϲ�hdis�߶ȣ����Ƹ���ͼ���ϲࣻ
			for(i=0;i<hdis;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			//����Ч��׼��ͼ�Ҳ�hdis��߶ȣ����Ƹ���ͼ���²ࣻ
			for(i=size_test.height+hdis;i<size_std.height;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
		}
		for(i=hdis;i<size_test.height+hdis;i++)
		{
			//	����Ч��׼��ͼ���wdis��ȣ����Ƹ���ͼ������ࣻ
			for(j=0;j<wdis;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			//	����Ч��׼��ͼ�Ҳ�wdis��ȣ����Ƹ���ͼ�����Ҳࣻ
			for(j=wdis;j<wdis+size_test.width;j++)
			{
				rgb_test = cvGet2D(test,i-hdis,j-wdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			//����Ч������ͼ�����Ƹ���ͼ�м䲿�֣�
			for(j=wdis+size_test.width;j<size_std.width;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
		}
	}
	
	cvSaveImage(testPicName,test_new);//��ͼ�滻ԭ���Ĵ�����ͼ
	
}
//����
CString CACDetectDlg ::SubtractPic(CString TestPicPath, CString StdPicPath)
{
	IplImage *img0 = cvLoadImage(TestPicPath);//������Ч��ͼ
	IplImage *img1 = cvLoadImage(StdPicPath);//��׼��Ч��ͼ
	
	int i,j;
	CvScalar s0,s1,s2;
	IplImage * img2=cvCreateImage(cvGetSize(img0),img0->depth,img0->nChannels);//������ͼ	
	cvZero(img2);//��ͼ����	
	//��ʱ������ͼ�ͱ�׼��ͼ��С��ͬ,ɨ������������ͼ�ͱ�׼��ͼ,����
	for(i=0;i<img0->height;i++)
	{
        for(j=0;j<img0->width;j++)
		{
			s0=cvGet2D(img0,i,j); // ��ô�����ͼ��������
			s1=cvGet2D(img1,i,j); //��ñ�׼��ͼ�������
			s2=cvGet2D(img2,i,j); //�����ͼ����,��ȻΪ��(0,0,0)
			//��ͨ���ֱ�����ȥ����ֵ
			s2.val[0]=fabs(s1.val[0]-s0.val[0]);
			s2.val[1]=fabs(s1.val[1]-s0.val[1]);
			s2.val[2]=fabs(s1.val[2]-s0.val[2]);  
			
			cvSet2D(img2,i,j,s2);//������ͼ������ֵ 
		}
	}
	ImgSize.cx=img2->width;
	ImgSize.cy=img2->height;
	CString strSubpicname;
	
	strSubpicname=StdPicPath.Left(StdPicPath.ReverseFind('\\')+1)+strSubpicname;
	
	
	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //��õ�ǰ.exe�ļ���·��
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	strSubpicname=sPath+"\\"+"minuspic.bmp";
	
	cvSaveImage(strSubpicname,img2);//����ͼ��
	
	return strSubpicname;
	
} 

int CACDetectDlg::dtoi(double x)
{
	if(x<0) x=x-0.5;//С��0 -0.5 ȡ��
	else x=x+0.5;//����0 +0.5 ȡ��
	return (int)x;
}

void CACDetectDlg::GetPcbInfo(CString filename)
{

//	CList<COMP_Shape,COMP_Shape&> listComponents;//�����б���״��
//  CList<COMP_List,COMP_List> listDesignComp;//����ļ���������
	listDesignComp.RemoveAll();//���������״�б�
	arrayCircOutline.RemoveAll();//��������б�
 	listComponents.RemoveAll();//������������б�
	//-------------------------------------------------���ֱ���----------------------
	char str[50];
	double PTX=0;  //PCB����ֱ��X����  
	double PTY=0;  //PCB����ֱ��Y����  
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	double COMPX=0;//�������ĵ�X����
	double COMPY=0;//�������ĵ�Y����d
	char Angle[20];// ��������ת�Ƕ�
	char Level[20];//�������ڵĲ�
	double CTFX=0;//�����߶����x
	double CTFY=0;//�����߶����y
	double CTLX=0;//�����߶��յ�x
	double CTLY=0;//�����߶��յ�y
	double CAX=0,CAY=0;//Բ����������(x,y)
	double CAR=0;//�뾶R
	double CASA=0;//��ʼ�Ƕ�StartAngle(SA)
	double CAEA=0;//��ֹ�Ƕ�EndAngle(EA)

    COMP_List *Cadd;//�������ָ��
    COMP_Shape *compadd;//���������״ָ��
	COMP_Shape comptemp;//��ʱ������״
	POSITION comp_pos;//���������б�λ��
	CT *ctadd;//�߶��б�	
	CA *caadd;//Բ���б�	
	int First=1;//�����ж������ǲ��ǵ�һ������
	CvPoint *PointArray;//�����ԭʼ����
	CvPoint Ptemp;//�м���ʱ�������
    //----------------------------------------------���ֱ���end-----------------		
    ifstream FilePoints(filename);//���ļ�
    //-------------------------------------------���������---------------------
	int i=0;//��ŵ������
    int amount=0;//���������
	FilePoints.getline(str,50);//���ļ�һ��
	if (!strcmp(str,"POINTS"))//�������"POINTS"˵��һ�µ�Ϊ������Ϣ
	{			
		//cout<<"�����㼯"<<endl;
		FilePoints>>i;//"POINT"֮������ǵ������
		PointArray=new CvPoint[i];//��̬��������
		
		for (int j=0;j<i;j++)//���ݵ��������ȡ�������
		{
			//��ȡ�ļ������������ֵ���ֱ���һ�����x�����y����
			FilePoints>>PTX;
			FilePoints>>PTY;
			PointArray[j]=cvPoint(dtoi(PTX),dtoi(PTY));  //cvPoint()�в���Ϊ��������������
		}					
	}
	//�ҳ���·��ߺͿ� ������������ǲ��ǿ��Դ����ݿ��еõ�	
	int LX=PointArray[0].x;//���½�(x,y)����ֵ�����ĵ�һ����
	int LY=PointArray[0].y;
	int RX=0;//���ϽǸ���ֵ(0,0)
	int RY=0;
	for (int j=1;j<i;j++)
	{
		if (LX>=PointArray[j].x)//�ҵ����½�x���
		{
			LX=PointArray[j].x;
		}
		if(LY>=PointArray[j].y)//�ҵ����½�y���
		{
			LY=PointArray[j].y;
		}
		if (RY<=PointArray[j].y)//�ҵ����Ͻ�y���
		{
			RY=PointArray[j].y;		
		}
		if(RX<=PointArray[j].x)//�ҵ����Ͻ�x���
		{
			RX=PointArray[j].x;
		}
	}
	int height=RY-LY;//�������·��ĸ�
	int width=RX-LX;//�������·��Ŀ�
	int w=ImgSize.cx;//��ֵͼ���
	int h=ImgSize.cy;//��ֵͼ���
	for ( j=0 ; j < i ;j++)
	{
		//ת��ʵ�����굽��������
        Ptemp.x=(PointArray[j].x-LX)*w/width;
        Ptemp.y=(PointArray[j].y-LY)*h/height;
		arrayCircOutline.Add(Ptemp);//����������arrayCircOutline��
	}
	delete[] PointArray;//�ͷŶ�̬����Ŀռ�
	while(!FilePoints.eof())//���û�е��ļ�ĩβ
	{
		FilePoints.getline(str,50);//��ȡ�ļ�һ��
		if (!strcmp(str,"COMP"))//�Ƿ���COMP,�����COMP��ʾһ����Ϣ��������Ϣ
		{			
			FilePoints.getline(str,sizeof(str));
 			strcpy(Geometry,str);//���������װ ,��Ϊ�п���Ϊ��,ֱ����>>��ȡ�п��ܳ������strcpy
			FilePoints.getline(str,sizeof(str));
 			strcpy(Symbol,str);//��������ͺ�
			FilePoints.getline(str,sizeof(str));
 			strcpy(Reference,str);//����������
			FilePoints>>COMPX;//�����������x����
			FilePoints>>COMPY;//�����������y����
			FilePoints>>Angle;//���������ת�Ƕ�
			FilePoints>>Level;//����������ڲ�
			COMPX=(COMPX-LX)*w/width;//ת��������������
			COMPY=(COMPY-LY)*h/height;
			//�������ҷ�ת 0519
			if (!strcmp(Level,"B"))//����
			{
				COMPX=ImgSize.cx-COMPX;//�������ҷ�ת
			}
			if (!strcmp(Angle,""))//0520 Angle �п���Ϊ�� ���԰�ԭ����double�͸�Ϊ��char��
			{
				strcpy(Angle,"0");
			}
			if (COMPX>=0 && COMPY>=0 && COMPX<w && COMPY<h)//0420++�������������Ȼ����0,С��0Ϊ�����,û�п���
			{
			      bool flag;
				  flag=true;

				Cadd=new COMP_List;//����Cadd�������������״
				strcpy(Cadd->Geometry,Geometry);//��������װ����Cadd��
				strcpy(Cadd->Symbol,Symbol);//�������ͺŷ���Cadd��
				strcpy(Cadd->Reference,Reference);//��������ŷ���Cadd��
				Cadd->Angle=atof(Angle);//char->doubleת��
				strcpy(Cadd->Level,Level);//�����������Cadd��				
				Cadd->COMPX=COMPX;//��������������(x,y)����Cadd��
				Cadd->COMPY=COMPY;	
				listDesignComp.AddTail(*Cadd);//������������Ϣ����listDesignComp�б���
				delete Cadd;//�ͷ�Cadd
				
				//�����б�
				comp_pos=listComponents.GetHeadPosition();//��������б�ͷλ��
				while(comp_pos!=NULL)//���λ�ò�Ϊ��
				{
					//�������ͺ����������б�����firstΪ0������ǵ�һ�γ�����firstΪ1
					comptemp=listComponents.GetNext(comp_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
					if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference))//�ж��Ƿ��Ѿ����б���
					{
						First=0; //�Ѿ����б�����Ϊ0
					}
					else
					{
						First=1;//��һ�γ�����Ϊ1
					}				
				}		   
				//--------------------------------------��ȡ�߶κ�Բ��-----------------------------------------
				if (First==1)//���firstΪ1��Ѵ��������������б���
				{
					compadd=new COMP_Shape;//��ʼ����̬�б�
					ctadd=new CT;//����߶�
					caadd=new CA;//���Բ��
					strcpy(compadd->Geometry,Geometry);//������װ
					strcpy(compadd->Reference,Reference);//�������
					
					while(strcmp(str,"ENDCOMP"))//����������Ϣ��β(�ļ���ENDCOMP����)û?
					{
						FilePoints.getline(str,20);//��ȡ�ļ�һ��
						if (!strcmp(str,"CT"))//�Ƿ����߶����� �߶���Ϣ��CT��ͷ
						{							
							FilePoints>>CTFX;//�߶���ʼ����(x,y)
							FilePoints>>CTFY;
							FilePoints>>CTLX;//�߶��յ�����(x,y)
							FilePoints>>CTLY;	
							//ת��������
							ctadd->BX=CTFX*w/width;
							ctadd->BY=CTFY*h/height;
							ctadd->EX=CTLX*w/width;
							ctadd->EY=CTLY*h/height;
						
							double x1,y1,x2,y2;
							x1=COMPX+cos(Cadd->Angle*3.1415926/180)*ctadd->BX-sin(Cadd->Angle*3.1415926/180)*ctadd->BY;//0414
							y1=COMPY+sin(Cadd->Angle*3.1415926/180)*ctadd->BX+cos(Cadd->Angle*3.1415926/180)*ctadd->BY;
							x2=COMPX+cos(Cadd->Angle*3.1415926/180)*ctadd->EX-sin(Cadd->Angle*3.1415926/180)*ctadd->EY;							
				        	y2=COMPY+sin(Cadd->Angle*3.1415926/180)*ctadd->EX+cos(Cadd->Angle*3.1415926/180)*ctadd->EY;
						
							if(! ( ((x1>0) && (x1<w))  && ((x2>0) && (x2<w) ))&&( ((y1>0) && (y1<h)) &&( (y2>0) && (y2<h))))
							{
								flag=false;
							}

							if(flag)

							{
								compadd->comp_CT.AddTail(*ctadd);//����comp_CT�б�β
							}
						}
						if (!strcmp(str,"CA"))//�Ƿ����������Բ����Ϣ,��CA��ͷ����Բ����Ϣ
						{
							FilePoints>>CAX;//����Բ������x����
							FilePoints>>CAY;//����Բ������y����
							FilePoints>>CAR;//Բ���뾶R
							FilePoints>>CASA;//��ʼ�Ƕ�StartAngle(SA)
							FilePoints>>CAEA;//��ֹ�Ƕ�EndAngle(EA)
							//ת��������
							caadd->CX=CAX*w/width;
							caadd->CY=CAY*h/height;
							caadd->R=CAR*h/height;
							caadd->SA=CASA;
							caadd->EA=CAEA;
							compadd->comp_CA.AddTail(*caadd);//����comp_CA�б�β
						}
					}
					listComponents.AddTail(*compadd);//����������״listComponents�б�β
					
					delete ctadd;//
					delete caadd;//ÿ���ͷ�һ��
					delete compadd;//new ��Ҫ�ͷ� Ҫ��Ȼ���ڴ�й©
				}
			
			}			
		}
	}
	FilePoints.close();//�ر��ļ�
	if (!listComponents.GetCount())//���û��������Ϣ��ʾû��������Ϣ
	{
		MessageBox("û��������Ϣ!");
		return;
	}
	if (!arrayCircOutline.GetSize())//���û�е�·����������ʾû��������Ϣ
	{
		MessageBox("û��������Ϣ!");
		return;
	}
	if (!listDesignComp.GetCount())//���û������,��ʾû������
	{
		MessageBox("û������!");
		return;
	}

}

void CACDetectDlg::DrawPcb()
{
	//-------------------------------------------------���ֱ���----------------------
	double PTX=0;  //PCB����ֱ��X����  
	double PTY=0;  //PCB����ֱ��Y����  
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	double COMPX=0;//�������ĵ�X����
	double COMPY=0;//�������ĵ�Y����d
	double Angle=0;// ��������ת�Ƕ�
	char Level[20];//�������ڵĲ�

	double CTFX=0;//�����߶����x
	double CTFY=0;//�����߶����y
	double CTLX=0;//�����߶��յ�x
	double CTLY=0;//�����߶��յ�y
	double CAX=0,CAY=0;//Բ����������(x,y)
	double CAR=0;//�뾶R
	double CASA=0;//��ʼ�Ƕ�StartAngle(SA)
	double CAEA=0;//��ֹ�Ƕ�EndAngle(EA)
	//������ʱ����
    COMP_List ctemp;
	POSITION c_pos;
    //��״��ʱ����
	COMP_Shape comptemp;
	POSITION comp_pos;
	//�߶��б���ʱ����
	CT cttemp;	
	POSITION ct_pos;
	//Բ���б���ʱ����
	CA catemp;
	POSITION ca_pos;
	//����������ʱ����
	COMP_List prob;//0905 +
	POSITION prob_pos;
    CPoint COMPPt;
	CvSize ImageSize1 = cvSize(ImgSize.cx,ImgSize.cy);//ͼƬ��С
	IplImage* imgt = cvCreateImage(ImageSize1, IPL_DEPTH_8U, 3);//�½�T��ͼƬ
	imgt->origin=IPL_ORIGIN_BL;//ͼƬ����ԭ�������½�,����ֻ��������

	cvZero(imgt);
	//cvZero(imgb);
	//������
	int i=arrayCircOutline.GetSize();//�����������
	
	//������������б�ͷλ��
	for (int j=0;j<i-1;j++)//ÿ�������һ���߶Σ�ʹ��OpenCV�е�cvLine����
         cvLine(imgt,arrayCircOutline[j],arrayCircOutline[j+1],CV_RGB(0,255,0),3,8,0);


	c_pos=listDesignComp.GetHeadPosition();
	while(c_pos!=NULL)//�����Ϊ��
	{		
		ctemp=listDesignComp.GetNext(c_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
		strcpy(Geometry,ctemp.Geometry);//���������װ
		strcpy(Symbol,ctemp.Symbol);//�ͺ�
		strcpy(Reference,ctemp.Reference);//���
		strcpy(Level,ctemp.Level);//���ڲ�
		COMPX=ctemp.COMPX;//����x����
		COMPY=ctemp.COMPY;//����y����
		Angle=ctemp.Angle;//��ת�Ƕ�
        COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));
		CvScalar color=CV_RGB(150,150,150);//�����߶�Բ����ɫ��ɫ
		CvScalar scolor=color;//�����ͺ�����ɫ
		//����������Ϊ��ɫ
		prob_pos=listPixelComp.GetHeadPosition();//���������б�ͷλ��
		while(prob_pos!=NULL)
		{
			prob=listPixelComp.GetNext(prob_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
			
			if (CPoint(dtoi(prob.COMPX),dtoi(prob.COMPY))==COMPPt)//�Ƿ�����������,�����
			{
				color=CV_RGB(255,0,0);//�����߶�Բ����ɫ��Ϊ��ɫ
				//scolor=CV_RGB(0,255,255);//�����ͺ�����Ϊ����ɫ
				//scolor=CV_RGB(255,0,0);
			}
			if (CPoint(dtoi(strtod(strXPoint,NULL)),dtoi(strtod(strYPoint,NULL)))==COMPPt)//�Ƿ�ѡ��
			{
				color=CV_RGB(255,255,0);
			//	scolor=CV_RGB(0,255,255);
			//	scolor=CV_RGB(255,0,0);
			}		
		}						  
		comp_pos=listComponents.GetHeadPosition();//������״��Ϣ�б�ͷλ��
		while(comp_pos!=NULL)//�����Ϊ��
		{
			comptemp=listComponents.GetNext(comp_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
			//�ҵ������������״��Ϣ
			if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
			{					
				//----------------------------------����߶�----------------------------------------
				ct_pos=comptemp.comp_CT.GetHeadPosition();//�����߶�ͷλ��
				while(ct_pos!=NULL)//�����Ϊ��
				{
					cttemp=comptemp.comp_CT.GetNext(ct_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
					double FX=cttemp.BX;//�����߶����x����
					double FY=cttemp.BY;//�����߶����y����
					double LX=cttemp.EX;//�����߶��յ�x����
					double LY=cttemp.EY;//�����߶��յ�y����
					double CTFX1=0;
					double CTFY1=0;
					double CTLX1=0;
					double CTLY1=0;
					//������������(x,y)����ת�Ƕȣ�ת���߶����������յ����꣬��Ϊ�м��ļ��и������߶���Ϣ������������Ϊԭ��
					CTFX1=COMPX+cos(Angle*3.1415926/180)*FX-sin(Angle*3.1415926/180)*FY;//0414
					CTFY1=COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY;
					CTLX1=COMPX+cos(Angle*3.1415926/180)*LX-sin(Angle*3.1415926/180)*LY;							
					CTLY1=COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY;                           					
					//�����յ�
					CvPoint ptf=cvPoint(dtoi(CTFX1),dtoi(CTFY1));
					CvPoint ptl=cvPoint(dtoi(CTLX1),dtoi(CTLY1));
					
				
					cvLine(imgt,ptf,ptl,color,3,8,0);//ʹ��OpenCV�е�cvLine����
					
				}//while CTlist	
				//-------------------------------Բ��----------------------------
				ca_pos=comptemp.comp_CA.GetHeadPosition();//Բ��ͷλ��
				while(ca_pos!=NULL)//�����Ϊ��
				{
					catemp=comptemp.comp_CA.GetNext(ca_pos);//���λ�õ���һ��,���ص�ǰλ��ֵ
					double cx=catemp.CX;//Բ��x����
					double cy=catemp.CY;//Բ��y����
					double r=catemp.R;//�뾶
					double sa=catemp.SA;//��ʼ�Ƕ�				
					double ea=catemp.EA;//��ֹ�Ƕ�
					double cx1=0,cy1=0;
					CvPoint spt,ept;
					if (sa>ea)//�����ʼ�Ƕȴ�����ֹ�Ƕ�
					{
						ea=ea+360;//��ֹ�Ƕ�+360,��֤��ֹ�Ƕȴ�����ʼ�Ƕ�
					}
					//������������(x,y)����ת�Ƕȣ�ת��Բ��Բ��(x,y)����
					cx1=COMPX+cos(Angle*3.1415926/180)*cx-sin(Angle*3.1415926/180)*cy;//0411
					cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;				
					spt=cvPoint(cx1+r*cos((sa+Angle)*3.1415926/180),cy1+r*sin((sa+Angle)*3.1415926/180));
					ept=cvPoint(cx1+r*cos((ea+Angle)*3.1415926/180),cy1+r*sin((ea+Angle)*3.1415926/180));
					//˳ʱ��0412����Ϊ��opencv��ͼ������ԭ�����������½ǣ�ʹ����Ҫ˳ʱ��ת��
					sa=360-sa+(360-Angle);
					ea=360-ea+(360-Angle);																		
					//-------------------------------------------��Բ��--------------------------------
					
						cvLine(imgt,spt,ept,color,3,8,0);//ʹ��OpenCV�е�cvLine()
						cvEllipse(imgt,cvPoint(dtoi(cx1),dtoi(cy1)),cvSize(dtoi(r),dtoi(r)),0,ea,sa,color);//ʹ��OpenCV�е�cvEllipse()
				                 							
				}//while ca							
			}//if          
		}//while comp
	}//while

	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //��õ�ǰ.exe�ļ���·��
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 

	pathname_pcb=sPath+"\\"+"pcb.bmp";
	//��·�����ͼ������ָ��·����


	cvSaveImage(pathname_pcb,imgt);

}

//����õ���ֵͼ��
CString CACDetectDlg::ThresholdPic(CString filename, int pos)
{
	int i,j;
	CvScalar s;//�������ֵ
	IplImage *img=cvLoadImage(filename);//����ͼ��
	for(i=0;i<img->height;i++)
	{
        for(j=0;j<img->width;j++)	
		{
			s=cvGet2D(img,i,j);//��ô˵�����ֵ			
			BOOL BigY0 = s.val[0]>pos;//�������ͨ������ֵ������ֵ��Ϊ��
			BOOL BigY1 = s.val[1]>pos;
			BOOL BigY2 = s.val[2]>pos;
			
			BOOL MoreThanTwo = BigY0? (BigY1||BigY2):(BigY1&&BigY2);  //����������������
			
			if(!MoreThanTwo)  //ͨ����ֵѡ����ʾ���ص�
			{
				for(int k=0;k<img->nChannels;k++) //�����������ط���С����ֵ����õ�������ط�����Ϊ0	
				{
					s.val[k]=0;//������ֵ��Χ�ڵ���Ϊ��ɫ
				}		
			}
			else
			{
				for(int k=0;k<img->nChannels;k++) //�����������ط���С����ֵ����õ�������ط�����Ϊ0	
				{
					s.val[k]=255;//������ֵ��Χ�ڵ���Ϊ��ɫ
				}	
				
			}
			cvSet2D(img,i,j,s); //��������ֵ
		}
	}
	
	
    //����ת�������ֵͼ��
	filename.TrimRight(".bmp");
	CString spos;
	spos.Format("%d",pos);
	filename=filename+"YuVal"+spos+".bmp";

	cvSaveImage(filename,img);
	return filename;

	
	
}

//����������ֵͼ���ɱ�Ե��ȡ���ͼ��
CString CACDetectDlg::Edgepickpic(CString filename)
{
	IplImage *image1=cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);  //��ɫ��ֵͼ��
	IplImage *image2=cvCreateImage(cvGetSize(image1),IPL_DEPTH_8U,1);  //��ͨ������ͼ
	IplImage *image3=cvCreateImage(cvGetSize(image1),IPL_DEPTH_8U,1);  //��ͨ������ͼ
	//IplImage *image2,*image3,*image4;
	//cvNot(image0,image1);
    cvErode(image1,image1,NULL,1);   //��ʴ����
	cvDilate(image1,image1,NULL,1);  //���Ͳ���
	cvDilate(image1,image1,NULL,1);   //���Ͳ���
	cvErode(image1,image1,NULL,1);     //��ʴ����
	
	cvSmooth(image1,image1,CV_GAUSSIAN,3,0,0,3);  //��ͨ�˲�
	/*//��Ե���
	//	cvSobel(image1,image2);
	cvNot(image1,image2);      //��ֵ��
	//cvSobel(image1,image2,1,1,3);
	cvCanny(image1,image2,1,3,3); 
	//cvLaplace(image1,image2,1);*/
	cvNot(image1,image2);
	cvCanny(image2,image3,1,3,3);
	//cvLaplace(image2,image3,3);

	
    
	
	CString strSubpicname;	
	strSubpicname=filename.Left(filename.ReverseFind('\\')+1)+strSubpicname;
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //��õ�ǰ.exe�ļ���·��
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	strSubpicname=sPath+"\\"+"guize.bmp";	
	cvSaveImage(strSubpicname,image3);//����ͼ��
    return strSubpicname;
	
}

void CACDetectDlg::FindRect(CString imagename)
{
	
	listRect.RemoveAll();//���listRect�б�
	IplImage* img_8uc1=cvLoadImage(imagename,CV_LOAD_IMAGE_GRAYSCALE);//��ͨ��ͼ
	IplImage* img_edge=cvCreateImage(cvGetSize(img_8uc1),8,1);//������ͨ��ͼ
	
    cvFlip(img_8uc1,NULL,0);//��x-�ᷭת ��Ϊ����ϵ�Ĺ�ϵ
	
	img_8uc1->origin=IPL_ORIGIN_BL;//���½�ΪͼƬԭ��
	img_edge->origin=IPL_ORIGIN_BL;//���½�ΪͼƬԭ��
    //��ͼ�������ֵ�����ú����ԻҶ�ͼ�������ֵ�����õ���ֵͼ��
	
	
	
    cvThreshold(img_8uc1,img_edge,128,255,CV_THRESH_BINARY);
	
	
	
    //��������
	CvMemStorage* storage=cvCreateMemStorage();//�����ڴ�飬���������õ���
	CvSeq* first_contour=NULL;//���У��洢�����ĵ㼯
	
	int Nc= cvFindContours(   //������������
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_EXTERNAL);//ֻ�����������
	
	/*���ԵĻ�������ʵ���ڹ��߰�����*/
	// 	char msg[30];
	// 	sprintf(msg,"����ǰ��������:%d",Nc);
	// 	MessageBox(msg);
	
	CvRect* box;//�洢�㼯����
	CvSeq* c=first_contour;//����ͷָ��
	box=new CvRect[Nc];//��̬����������������
	//---------------------------------------ok-----------------------------------
	CRect recttemp,recttemp1,recttemp2;
	POSITION rl_pos1,rl_pos2,rl_pos3,rl_pos4;
	//��þ��β������ھ���������
	for (int i=0;c!=NULL;c=c->h_next,i++)
	{		
		box[i]=cvBoundingRect(c);//����㼯����������α߽�	
		recttemp.TopLeft().x=box[i].x;//��ֵ��CRect
		recttemp.TopLeft().y=box[i].y;
		recttemp.BottomRight().x=box[i].x+box[i].width;
		recttemp.BottomRight().y=box[i].y+box[i].height;
		listRect.AddTail(recttemp);//������listRect�б���
	}
	delete[] box; //newһ��Ҫ�ͷ�
    //�жϾ����ཻ������ཻ��ϲ���һ��
	rl_pos1=listRect.GetHeadPosition();//���listRectͷλ��
	while(rl_pos1!=NULL)
	{
		int cross=0,sum=0;
        rl_pos3=rl_pos1;//���浱ǰλ��,�Ա����ɾ��
		recttemp1=listRect.GetNext(rl_pos1);//λ�õ���һ��,���ص�ǰλ��ֵ
		//��β�ж���ÿ�������ཻ�ཻ��
		rl_pos2=listRect.GetTailPosition();//���listRectβλ��
		while(rl_pos2!=NULL)
		{
			rl_pos4=rl_pos2;//���浱ǰλ��,�Ա�ϲ�
			recttemp2=listRect.GetPrev(rl_pos2);//λ�õ���һ��,���ص�ǰλ��ֵ
			if (recttemp1!=recttemp2)//����������β���ͬһ��
			{
				cross=recttemp.IntersectRect(recttemp1,recttemp2);//cross=1���ཻ������cross=0
				sum=sum+cross;//����������������������ཻ������
				if (cross==1)//�Ƿ��ཻ
				{
					recttemp.UnionRect(recttemp1,recttemp2);//�ཻ��ϲ�
					listRect.SetAt(rl_pos4,recttemp);		//���洢�ں��ߵ�λ��	
				}	
			}			
		}
		if (sum>=1)//�����������ཻ,�Ѿ����ϲ���������,���б���ɾ�������
		{
			listRect.RemoveAt(rl_pos3);//�ѱ��ϲ�ɾ�������
		}		
	}
	
}
void CACDetectDlg::JudgeRect( )
{
//	CList<CPoint,CPoint> listRectComp;//�����жϵ��������� 0901 change CString to CPoint
	listRectComp.RemoveAll();//���
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	double COMPX=0;//�������ĵ�X����
	double COMPY=0;//�������ĵ�Y����d
	double Angle=0;// ��������ת�Ƕ�
	char Level[20];//�������ڵĲ�
    //������ʱ����
	COMP_List ctemp;
	POSITION c_pos;
	//��״��ʱ����
	COMP_Shape comptemp;
	POSITION comp_pos;
	//�߶���ʱ����
	CT cttemp;	
	POSITION ct_pos;
	//Բ����ʱ����
	CA catemp;
	POSITION ca_pos;
    //������ʱ����
	POSITION pos;
	CRect RectTemp;
	CPoint COMPPt;
	c_pos=listDesignComp.GetHeadPosition();//��������б�ͷλ��
	while(c_pos!=NULL)
	{
        //��������Ϣ
        ctemp=listDesignComp.GetNext(c_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
		strcpy(Geometry,ctemp.Geometry);//��ȡ������װ
		strcpy(Symbol,ctemp.Symbol);//��ȡ�����ͺ�
		strcpy(Reference,ctemp.Reference);//��ȡ�������
		strcpy(Level,ctemp.Level);//��ȡ�������ڲ�
		COMPX=ctemp.COMPX;//��ȡ��������x����
		COMPY=ctemp.COMPY;//��ȡ��������y����
		Angle=ctemp.Angle;//��ȡ������ת�Ƕ�	
		COMPPt.x=dtoi(COMPX);
		COMPPt.y=dtoi(COMPY);
		//COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));
		//if (!strcmp(Level,layer) )//�ж�����
	//	{
			
			int incomp=0;//�Ƿ��Ѽ����б��еı�־λ
			//�ж����ĵ��Ƿ��ھ�����,����ھ����м���problemcomp��
			pos=listRect.GetHeadPosition();//���ͷλ��
			while(pos!=NULL)
			{
				RectTemp=listRect.GetNext(pos);//λ�õ���һ��,���ص�ǰλ��ֵ
				//�ж������Ƿ����쳣���������
				if (COMPX<=RectTemp.right && COMPX>=RectTemp.left && COMPY>=RectTemp.top && COMPY<=RectTemp.bottom)
				{
					listRectComp.AddTail(COMPPt);//�����,�������ͺż���listRectComp�б���
					incomp=1;//��־λ����Ϊ1
				}
			}

			comp_pos=listComponents.GetHeadPosition();//��ȡ������״�б�ͷλ��
			while(comp_pos!=NULL)
			{
				comptemp=listComponents.GetNext(comp_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
				//����ͬ����������״
				if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
				{	
					//��������߶κ�Բ����Ϣ					
					ct_pos=comptemp.comp_CT.GetHeadPosition();//����߶�ͷλ��
					while(ct_pos!=NULL)
					{
						cttemp=comptemp.comp_CT.GetNext(ct_pos);//λ�õ���һ��,���ص�ǰλ��ֵ
						double FX=cttemp.BX;//�߶����x����
						double FY=cttemp.BY;//�߶��յ�y����
						double LX=cttemp.EX;//�߶��յ�x����
						double LY=cttemp.EY;//�߶��յ�y����
						double CTFX1=0;
						double CTFY1=0;
						double CTLX1=0;
						double CTLY1=0;
					    //������ת�Ƕ�ת���������������յ�����
						CTFX1=dtoi(COMPX+cos(Angle*3.1415926/180)*FX-sin(Angle*3.1415926/180)*FY);//0414
						CTFY1=dtoi(COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY);
						CTLX1=dtoi(COMPX+cos(Angle*3.1415926/180)*LX-sin(Angle*3.1415926/180)*LY);							
						CTLY1=dtoi(COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY); 
						

                       
						//--------------�ж������߶��Ƿ������������-------------------
						if (incomp!=1)
						{
							
							pos=listRect.GetHeadPosition();//��þ�������ͷλ��
							while(pos!=NULL)
							{								
								RectTemp=listRect.GetNext(pos);                         
								CRect CTrect,rect;
								//��Ҫ�����߶ε��������һ������,�ж���������Ƿ���������������ཻ
								CTrect.left=CTFX1;//���½� ���
								CTrect.bottom=CTFY1;
								if (CTFY1==CTLY1)//���߶�	
								{
									CTrect.right=CTLX1;//���Ͻ�
									CTrect.top=COMPY;//����������,��ʹ�����������y��Ϊ������
								}
								else if (CTFX1==CTLX1)//����
								{
									CTrect.right=COMPX;//���Ͻ�,ʹ����������x��Ϊ������
									CTrect.top=CTLY1;
								}
								else 
								{
									CTrect.right=CTLX1;//���Ͻ� �յ�
									CTrect.top=CTLY1;
								}
								CTrect.NormalizeRect();//�淶������
								
								if(rect.IntersectRect(CTrect,RectTemp))//�ж��Ƿ��ཻ
								{
									listRectComp.AddTail(COMPPt);	   //����ཻ���뵽listRectComp��
									incomp=1;						   //����incomp��Ϊ1	
									break;							   //����whileѭ��,�ж���һ������
								}
							}//while
						}//if																																				
					}//while CTlist	
				
					//-------------------------------Բ��----------------------------
					ca_pos=comptemp.comp_CA.GetHeadPosition();
					while(ca_pos!=NULL)
					{
						catemp=comptemp.comp_CA.GetNext(ca_pos);
						double cx=catemp.CX;//Բ��x����
						double cy=catemp.CY;//Բ��y����
						double r=catemp.R;//�뾶
						double sa=catemp.SA;//��ʼ�Ƕ�					
						double ea=catemp.EA;//��ֹ�Ƕ�
						double cx1=0,cy1=0;
						//�������������������ת�Ƕ�,ת��Բ������
						cx1=COMPX+cos(Angle*3.1415926/180)*cx-sin(Angle*3.1415926/180)*cy;//0411
						cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;										
						//--------------------�ж�Բ���Ƿ�������ཻ-------------------
						if (incomp!=1)
						{								
							pos=listRect.GetHeadPosition();
							while(pos!=NULL)
							{
								RectTemp=listRect.GetNext(pos);									
								//Բ����Ӿ��α�ʾ���ж��Ƿ����쳣��������ཻ
								CRect CTrect,rect;
								//Բ����Ӿ����ĸ�����
								CTrect.left=dtoi(cx1-r);
								CTrect.bottom=dtoi(cy1-r);
								CTrect.right=dtoi(cx1+r);
								CTrect.top=dtoi(cy1+r);
								CTrect.NormalizeRect();
								
								if(rect.IntersectRect(CTrect,RectTemp))//����ཻ
								{
									listRectComp.AddTail(COMPPt);//����ཻ���뵽listRectComp��
									incomp=1;	//��־λ��1
									break;//����,�ж���һ������
								}								
							}
						}						
					}//while-ca
				}//if
			}//while			
		//}//if-layer
	}//while
}

void CACDetectDlg::JudgePixel(CString imagename)
{

	listPixelComp.RemoveAll();//���listPixelComp
			
	IplImage* img=cvLoadImage(imagename,CV_LOAD_IMAGE_COLOR);//������ֵͼ��
	cvFlip(img,NULL,0);//��x-�ᷭת ��Ϊ����ϵ�Ĺ�ϵ	
	img->origin=IPL_ORIGIN_BL;//���½�Ϊԭ��
	CvScalar Pixel;//�������ֵ
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	double COMPX=0;//�������ĵ�X����
	double COMPY=0;//�������ĵ�Y����d
	double Angle=0;// ��������ת�Ƕ�
	char Level[20];//�������ڵĲ�
	//������ʱ����
	COMP_List ctemp;
	POSITION c_pos;
	//������״��ʱ����
	COMP_Shape comptemp;
	POSITION comp_pos;
	CT cttemp;	//�߶���ʱ����
	POSITION ct_pos;
	CA catemp;  //Բ����ʱ����
	POSITION ca_pos;    
	//CString prob;//���������ͺ�
	CPoint prob;// save this comp's position
	POSITION pos;
	CPoint COMPPt;//�����
	COMP_List addprobcomp;//���ӵ�listpixelComp��
	c_pos=listDesignComp.GetHeadPosition();//�����б�ͷλ��
	while(c_pos!=NULL)
	{
		int inflag=0;//��־�����ͺ��Ѿ�����listPixelComp�б���
		ctemp=listDesignComp.GetNext(c_pos);
		pos=listRectComp.GetHeadPosition();	//�쳣��������жϳ��������б�listRectCompͷλ��
		while(pos!=NULL)
		{
			prob=listRectComp.GetNext(pos);

			COMPX=ctemp.COMPX;//��������x����
	    	COMPY=ctemp.COMPY;//��������y����
			COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));

			//if (!strcmp(Symbol,prob))
			if(prob==COMPPt)//0902
			{
				//-�����Ϣ
				strcpy(Geometry,ctemp.Geometry);//������װ
				strcpy(Symbol,ctemp.Symbol);//��������ͺ�
				strcpy(Reference,ctemp.Reference);//�������
				strcpy(Level,ctemp.Level);//�������ڲ�				
				Angle=ctemp.Angle;//������ת�Ƕ�

                strcpy(addprobcomp.Geometry,Geometry);
				strcpy(addprobcomp.Symbol,Symbol);
				strcpy(addprobcomp.Reference,Reference);
				addprobcomp.COMPX=COMPX;
				addprobcomp.COMPY=COMPY;
				addprobcomp.Angle=Angle;
				strcpy(addprobcomp.Level,Level);
				//
// 				post=listPixelComp.GetTailPosition();
// 				while (post!=NULL)
// 				{
// 					past=listPixelComp.GetPrev(post);
// 					if (past==prob)
// 					{
// 						inflag=1;
// 					}
// 				}
				//------------------------�ж����������Ƿ�����������------------------------
				Pixel=cvGet2D(img,COMPY,COMPX);//�������������������ֵͼ���ϵ������
				if (inflag!=1)//��û�м������ƹ��������б���
				{
					if (Pixel.val[0]>=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)//�ж������Ƿ�Ϊ��ɫ
					{
						inflag=1;//�����,��־λ��1
						listPixelComp.AddTail(addprobcomp);//�����ͺż������ƹ��������б�
					}
				}																
				comp_pos=listComponents.GetHeadPosition();//����ͷλ��
				while(comp_pos!=NULL)
				{
					comptemp=listComponents.GetNext(comp_pos);
					//����ͬ����������״
					if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
					{		
						//------------------------�߶���Ϣ---------------------
						ct_pos=comptemp.comp_CT.GetHeadPosition();
						while(ct_pos!=NULL)
						{
							cttemp=comptemp.comp_CT.GetNext(ct_pos);
							double FX=cttemp.BX;//�߶����x����
							double FY=cttemp.BY;//�߶����y����
							double LX=cttemp.EX;//�߶��յ�x����
							double LY=cttemp.EY;//�߶��յ�y����
 							int CTFX1=0;
 							int CTFY1=0;
 							int CTLX1=0;
 							int CTLY1=0;
							
							//�������ı�־����ת�Ƕ�ת���߶������յ�����
							CTFX1=dtoi(COMPX+cos(Angle*3.1415926/180)*FX+sin(Angle*3.1415926/180)*FY);//0411
							CTFY1=dtoi(COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY);
							CTLX1=dtoi(COMPX+cos(Angle*3.1415926/180)*LX+sin(Angle*3.1415926/180)*LY);							
							CTLY1=dtoi(COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY);
							//�������ж������߶�
							int i=0,j=0,nStart=0,nEnd=0,nBegin=0,nFinal=0;
							if (inflag!=1)
							{
								//����ɨ��Ҳ��ɨ���߶����������깹�ɵľ��ε�����
								if (CTFX1==CTLX1)//����
								{ 
									//�ж�����յ�,��С����Ϊ����ɨ������
									
									if (CTFX1<COMPX){nStart=CTFX1;nEnd=COMPX;}
									else{nStart=COMPX;nEnd=CTFX1;}
									if (CTLY1>CTFY1){nBegin=CTFY1;nFinal=CTLY1;}
									else {nBegin=CTLY1;nFinal=CTFY1;}
								}									
								else if (CTFY1==CTLY1)//����
								{
									//�ж�����յ�,��С����Ϊ����ɨ������
									if (CTLX1>CTFX1){nStart=CTFX1;nEnd=CTLX1;}
									else {nStart=CTLX1;nEnd=CTFX1;}
									if (CTFY1<COMPY){nBegin=CTFY1;nFinal=COMPY;}
									else{nBegin=COMPY;nFinal=CTFY1;}
									
								}
								else
								{	//б��	0408 0425-�޸�
									//�ж�����յ�,��С����Ϊ����ɨ������
									if (CTFX1<CTLX1){nStart=CTFX1;nEnd=CTLX1;}			
									else{nStart=CTLX1;nEnd=CTFX1;}
									if (CTFY1<CTLY1){nBegin=CTFY1;nFinal=CTLY1;}
									else{nBegin=CTLY1;nFinal=CTFY1;}
									
								}//else
								for(j=nBegin;j<=nFinal;j++)
								{
									for(i=nStart;i<=nEnd;i++)
									{
										Pixel=cvGet2D(img,j,i);
										//����������������200,�ж�Ϊ��ɫ
										if (Pixel.val[0]>=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)
										{
											inflag=1;//��־λ��1
											listPixelComp.AddTail(addprobcomp);//����listPixelComp��
											//cvLine(img,cvPoint(i,j),cvPoint(i,j),CV_RGB(255,0,0),2);
											break;//����forѭ��
										}											
									}
									if (inflag==1)break;//�����־λΪ1,����forѭ��
								}					
							}//if

						}//while CTlist	
						//-------------------Բ����Ϣ-------------------------
						ca_pos=comptemp.comp_CA.GetHeadPosition();
						while(ca_pos!=NULL)
						{
							catemp=comptemp.comp_CA.GetNext(ca_pos);
							double cx=catemp.CX;//Բ��x����
							double cy=catemp.CY;//Բ��y����
							double r=catemp.R;//�뾶
							double sa=catemp.SA;//��ʼ�Ƕ�				
							double ea=catemp.EA;//��ֹ�Ƕ�		
							double cx1=0,cy1=0;
							if (sa>ea)//ʹ��ֹ�Ƕȴ�����ʼ�Ƕ�
							{
								ea=ea+360;
							}	
							//�������ı�־����ת�Ƕ�ת��Բ��Բ������,�ͽǶ�
							cx1=COMPX+cos(Angle*3.1415926/180)*cx+sin(Angle*3.1415926/180)*cy;//0411
							cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;
							sa=sa+Angle;
							ea=ea+Angle;
							//����������Ϊ��ʼ��ɨ������Բ������
							if (inflag!=1)
							{ 
								//�� 0411 ok
								double xt=0,yt=0;
								int i=0,j=0,n=0;
								double xSt=cx1+r*cos(sa*3.1415926/180);//��������
								for (i=sa;i<=ea;i=i+3)//ÿ�νǶ���3(�����ֵ���Ը�С)
								{
									xt=cx1+r*cos(i*3.1415926/180);//��ת�˽Ƕ�x����
									yt=cy1+r*sin(i*3.1415926/180);//��ת�˽Ƕ�y����
									if (xt<xSt){j=xt;n=xSt;}//��ʼ��ѭ�����
									else{j=xSt;n=xt;}
				                    for(;j<=n;j++)
									{
										Pixel=cvGet2D(img,yt,j);//�������
										//�ж������Ƿ�Ϊ��ɫ
										if (Pixel.val[0] >=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)
										{
											inflag=1;//�����,��־λ��1
											listPixelComp.AddTail(addprobcomp);//�������listPixelComp��
											break;//�����ڲ�forѭ��
										}
									
									}
									if(inflag==1)break;//�������forѭ��,�ж���һ������
								}
							}							
						}						
					}//if ��ͬ����				
				}//while����como_pos      
			}//if
		}//while
	}//while
	//����ͼ��
	//cvReleaseImage(&img);


}

void CACDetectDlg::AlphaMerge(CString PCBImage, CString Imagename)
{
	IplImage *src1, *src2;
	src1=cvLoadImage(PCBImage,1);//���ط����·ͼ��
	src2=cvLoadImage(Imagename,1);//���ز�ɫ��ֵͼ��
	
	//alpha��beta���ں�ǿ�ȣ�alpha��src1,beta��src2,���й�ϵbeta=1-alpha
	double alpha = 0.4;
	double beta =  0.6;
    //---------------OpenCV���ںϺ���------------------
	cvAddWeighted(src1,alpha,src2,beta,0.0,src1);
	//�����ںϺ��ͼ��
	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //��õ�ǰ.exe�ļ���·��
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	
	pathname_merge=sPath+"\\"+"merge.bmp";
	//��·�����ͼ������ָ��·����
	
	
	cvSaveImage(pathname_merge,src1);
}

void CACDetectDlg::ListProbComp()
{
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	CString COMPX;//�������ĵ�X����
	CString COMPY;//�������ĵ�Y����d
	COMP_List prob;//save this comp's info
	POSITION ppos;	
	CString ID;//�������
	long count=listPixelComp.GetCount();//��������
	
	ppos=listPixelComp.GetHeadPosition();//���ƹ��������б�ͷλ��
	while(ppos!=NULL)
	{
		prob=listPixelComp.GetNext(ppos);
		
		//-�����Ϣ
		strcpy(Geometry,prob.Geometry);//������װ
		strcpy(Symbol,prob.Symbol);//������ƹ��������ͺ�
		strcpy(Reference,prob.Reference);//�������
		COMPX.Format("%0.3f",prob.COMPX);//��������x����
		COMPY.Format("%0.3f",prob.COMPY);//��������y����
		ID.Format("%d",count);//�������
		count--;//ÿ�ζ������һ����,�ʴ�������ʼ���μ�
		m_ListCompProblem.InsertItem(0,ID,1);//��������,���
		m_ListCompProblem.SetItemText(0,1,Geometry);//�ڶ��з�װ
		m_ListCompProblem.SetItemText(0,2,Symbol);//�������ͺ�
		m_ListCompProblem.SetItemText(0,3,Reference);//�����б��
		m_ListCompProblem.SetItemText(0,4,COMPX);//��������x����
		m_ListCompProblem.SetItemText(0,5,COMPY);//��������y����		
	}
	
}
void CACDetectDlg::DisplayPic(CString m_filename, CRect rect)
{
	CDC *dc=GetDC();
	Graphics graphics(dc->m_hDC);
	Image image(m_filename.AllocSysString(),FALSE);
	graphics.DrawImage(&image,rect.left,rect.top,rect.Width(),rect.Height());
	FlagShow=true;
}

void CACDetectDlg::OnBtnOpenTest() 
{
	// TODO: Add your control notification handler code here
	//int length_std;
	//int length_test;

	CFileDialog dlg(true);
	dlg.DoModal();
	pathname_test = dlg.GetPathName();

	if(pathname_test.GetLength()==0)
	{
		MessageBox("������ͼδѡ�У����ٴδ�");
		return;
	}

	GetDlgItem(IDC_TEST_PATH)->SetWindowText(pathname_test);

    bool pipei= MatchStd(pathname_test , pathname_std);

	//length_std=pathname_std.GetLength();
	

	if(pathname_std.GetLength()==0)
	{
		
			MessageBox("û����֮��Ӧ�ı�׼��ͼ");
			return;	
	}

	GetDlgItem(IDC_STD_PATH)->SetWindowText(pathname_std);

  //show the original standard picture	
	CRect r0;
	GetDlgItem(IDC_STD)->GetClientRect(&r0);
	HBITMAP hbm0=(HBITMAP)LoadImage((HINSTANCE)IDC_STD,
		pathname_std,
		IMAGE_BITMAP,
		r0.Width(),
		r0.Height(),
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	m_std_pic.SetBitmap(hbm0);
	
	
	CutoutPic(pathname_std, pathname_std_cout);
	

	//show the cutout standard picture
	CRect r3;
	GetDlgItem(IDC_STD_COUT)->GetClientRect(&r3);
	HBITMAP hbm3=(HBITMAP)LoadImage((HINSTANCE)IDC_STD_COUT,
		pathname_std_cout,
		IMAGE_BITMAP,
		r3.Width(),
		r3.Height(),
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	m_std_cout_pic.SetBitmap(hbm3);
		
	//show the original testing picture
	
	CRect r;
	GetDlgItem(IDC_TEST)->GetClientRect(&r);
	HBITMAP hbm=(HBITMAP)LoadImage((HINSTANCE)IDC_TEST,
		pathname_test,
		IMAGE_BITMAP,
		r.Width(),
		r.Height(),
        LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	m_test_pic.SetBitmap(hbm);
	
    CutoutPic(pathname_test, pathname_test_cout);
	
	//show the cutout testing picture
	CRect r1;
	GetDlgItem(IDC_TEST_COUT)->GetClientRect(&r1);
	HBITMAP hbm1=(HBITMAP)LoadImage((HINSTANCE)IDC_TEST_COUT,
		pathname_test_cout,
		IMAGE_BITMAP,
		r1.Width(),
		r1.Height(),
        LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	m_test_cout_pic.SetBitmap(hbm1);
    
	
	RegulatetPic(pathname_test_cout, pathname_std_cout);
    
	pathname_minus=SubtractPic(pathname_test_cout,pathname_std_cout);
	
	CRect r2;
	GetDlgItem(IDC_MINUS)->GetClientRect(&r2);
	HBITMAP hbm2=(HBITMAP)LoadImage((HINSTANCE)IDC_MINUS,
		pathname_minus,
		IMAGE_BITMAP,
		r2.Width(),
		r2.Height(),
        LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	m_minus_pic.SetBitmap(hbm2);

	FlagTest=true;
	
}



void CACDetectDlg::OnBtnOpenDesigne() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true);
	dlg.DoModal();
	pathname_designfile = dlg.GetPathName();

	if(pathname_designfile.GetLength()==0)
	{

		MessageBox("����ļ�δѡ�У����ٴδ�");
		return;
		
	}
	FlagDesign=true;
	GetDlgItem(IDC_DESIGNE_PATH)->SetWindowText(pathname_designfile);
	
	//FlagDesign=true;
	
}

void CACDetectDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	if(!FlagTest)
	{
		if(!FlagDesign)
		{
			MessageBox("���ȼ��ش�����ͼ������ļ�");
			return;
		}
		if(FlagDesign)
		{
			MessageBox("���ȼ��ش�����ͼ");
			return;
		}
	}
	
	if(FlagTest)
	{
		if(!FlagDesign)
		{
		//	MessageBox("���ȼ�������ļ�");
			MessageBox("���ȼ�������ļ�!","ϵͳ��ʾ",MB_OK|MB_ICONWARNING);
			return;
		}
	}


	GetPcbInfo(pathname_designfile);
	UpdateData(true);
    thresold=m_thresold;
	pathname_threshold=ThresholdPic(pathname_minus, thresold);
	
	pathname_edgepick=Edgepickpic(pathname_threshold);
	//MessageBox("GUIZE");
   // FindRect(pathname_edgepick);
	FindRect(pathname_threshold);
	
    JudgeRect();
    JudgePixel(pathname_threshold);
	//DrawPcb();
	DrawPcb();
	
    AlphaMerge(pathname_pcb, pathname_threshold);
	
	CRect org_stdrect;
	GetDlgItem(IDC_MERGE)->GetWindowRect(&org_stdrect);
	ScreenToClient(org_stdrect);
	DisplayPic(pathname_merge,org_stdrect);
	
    ListProbComp();

	
}
