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
	m_ListCompProblem.InsertColumn(0,"序号",LVCF_TEXT,40,-1);
	m_ListCompProblem.InsertColumn(1,"封装",LVCF_TEXT,80,-1);
	m_ListCompProblem.InsertColumn(2,"型号",LVCF_TEXT,50,-1);
	m_ListCompProblem.InsertColumn(3,"标号",LVCF_TEXT,50,-1);
	m_ListCompProblem.InsertColumn(4,"中心坐标x(mm)",LVCFMT_LEFT,80,-1);
	m_ListCompProblem.InsertColumn(5,"中心坐标y(mm)",LVCFMT_LEFT,80,-1);
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
		//重绘原始标准图
		//		CString org_stdfilename("pic\\orgbiaozhun.bmp");//文件路径
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


//函数功能：提取有效云图
void CACDetectDlg::CutoutPic(CString filename,CString &savename)
{
	IplImage *img = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);//加载图像
// 	if (img==NULL)//如果没有图像提示
// 	{
// 		MessageBox("no image!",NULL,MB_ICONWARNING);
// 		return;
// 	}
	CvPoint first = cvPoint(0,0),last = cvPoint(0,0);//初始化第一个点和最后一个点
	CvScalar s0;//存放像素值	
	for(int i=0;i<img->height;i++)  //去掉左侧坐标轴
	{
		for(int j=0;j<50;j++)//33为大约数
		{
			cvSet2D(img,i,j,cvScalar(0,0,0));//坐标轴设置为黑色
		}
	}	

	
	BOOL is_up = false;
	//扫描整张图
	for(i=10;i<img->height;i++)
	{
		for(int j=10;j<img->width;j++)
		{
			s0 = cvGet2D(img,i,j);//获得这个点的像素值
			//背景色为黑色或者白色,判断是否是这个两种颜色之一
			BOOL br0 = (int)s0.val[0] == 0;//若为0值则为真
			BOOL bg0 = (int)s0.val[1] == 0;
			BOOL bb0 = (int)s0.val[2] == 0;
			BOOL br1 = (s0.val[0]-254>0);//若为255则为真
			BOOL bg1 = (s0.val[1]-254>0);
			BOOL bb1 = (s0.val[2]-254>0);
			
			BOOL brgb0 = br0 && bg0 &&bb0;
			BOOL brgb1 = br1 && bg1 &&bb1;
			
			if(!(brgb0 || brgb1))  //判断是不是背景  图像基本为300*300
			{
				if(!is_up)  //图像原点是左上角
				{
					first.x = i;
					first.y = j;
					is_up = true;
				}					
			    if(is_up)
				{
					if (i<=first.x)//取最小的i值即为最小坐标的x值
					{
						first.x=i;
					}
					if(j<=first.y)  //取最小的j为左上角坐标的y值
					{
						first.y = j;
					}
					if(i>=last.x) //取最大的i值，为最大坐标的x值 (修改：20110906 first.x改为last.x)
					{
						last.x = i;
					}
					if(j>=last.y) //取最大的j值，为最大坐标的y值 (修改：20110906 first.x改为last.x)
					{
						last.y = j;
					}		
				}
			}	
		}
	}

	
	
	
	//抠出图像区域进行保存
	CvSize sz;
	sz.width = last.y - first.y + 1;//新图像的宽
	sz.height = last.x - first.x + 1;//新图像的高	
	IplImage * image = cvCreateImage(sz, 8,3);//创建新图像

	cvZero(image);//清空数组
	//将原始图像的有效区域的图赋值给新图像
	CvScalar s1,s2;	
	for(i = 0;i < sz.height;i++)
	{
		for(int j = 0;j < sz.width;j++)
		{
			

			s1=cvGet2D(img,i+first.x,j+first.y);//获得有效区域像素值
			s2.val[0] = s1.val[0];//3个通道分别赋值
			s2.val[1] = s1.val[1];
			s2.val[2] = s1.val[2];
			cvSet2D(image,i,j,s2);//设置新图像的像素值
		}
	}

	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //获得当前.exe文件的路径
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	savename=sPath+"\\"+filename.Right(filename.GetLength()-filename.ReverseFind('\\')-1);
	
 	cvSaveImage(savename,image);//保存图像
	

}

//根据待测图匹配标准图
bool CACDetectDlg::MatchStd(CString TestFileName, CString &StdFileName)
{
	/*int pz = TestName.Find("正面");
	MessageBox(""+TestName.GetAt(pz));
	return true;*/
	char chExePath[128];
	memset(chExePath, 0, 128);
	GetModuleFileName(NULL, chExePath, 128);
	//MessageBox(chExePath);

	int start = TestFileName.Find("正面");
	int end = TestFileName.ReverseFind('.');
	int rstart = TestFileName.Find("反面");
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

//调整图片大小
void CACDetectDlg::RegulatetPic(CString &testPicName, CString &stdPicName)
{
	IplImage *std = cvLoadImage(stdPicName,CV_LOAD_IMAGE_COLOR);//标准板有效云图，以它的尺寸为依据
	IplImage *test = cvLoadImage(testPicName,CV_LOAD_IMAGE_COLOR); //待测板有效云图 	
    IplImage *test_new = cvCreateImage(cvGetSize(std),std->depth,std->nChannels);//创建新图像

	CvSize size_std,size_test;
	CvScalar rgb_std,rgb_test,rgb_test_new;
	CvSize temptest;
	int i = 0;
	int j = 0;

	size_std = cvGetSize(std);//标准板有效云图尺寸
	size_test = cvGetSize(test);//待测板有效云图尺寸
	temptest = size_test;
	int wdis,hdis;
	wdis = abs(size_test.height - size_std.height)/2;//待测板有效云图与标准板有效云图的高之差的一半
	hdis = abs(size_test.width - size_std.width)/2;//待测板有效云图与标准板有效云图的宽之差的一半
	//情况1，std包在test里面
	if((size_test.height >= size_std.height) && (size_test.width >= size_std.width)) 
	{
		for(i = 0; i<size_std.height; i++ )
		{
			for(j = 0; j<size_std.width; j++)
			{
				//截取待测板有效云图的中间区域赋值给新图,
				rgb_test = cvGet2D(test,i+wdis,j+hdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];				
				cvSet2D(test_new,i,j,rgb_test_new);			
			}	
		}
	}
	//情况2，std横跨test
	else if((size_test.height >= size_std.height) && (size_test.width < size_std.width))
	{
		for(i=0;i<size_std.height;i++)
		{
			//	将有效标准云图左侧wdis宽度，复制给新图的最左侧；
			for(j=0;j<wdis;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			// 	将有效标准云图右侧wdis宽度，复制给新图的最右侧；
			for(j=wdis;j<size_test.width+wdis;j++)
			{
				rgb_test = cvGet2D(test,i+hdis,j);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			// 	将有效待测云图除去上下hdis的值，复制给新图；
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
	//情况3，std树跨test
	else if((size_test.height < size_std.height) && (size_test.width >= size_std.width))
	{
		for(j=0;j<size_std.width;j++)
		{
			//	将有效标准云图上侧hdis高度，复制给新图最上侧；
			for(i=0;i<hdis;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			 //	将有效标准云图右侧hdis宽高度，复制给新图最下侧；
			for(i=hdis;i<size_test.height+hdis;i++)
			{
				rgb_test = cvGet2D(test,i,j+hdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			// 	将有效待测云图除去左右侧hdis的值，复制给新图；
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
	else  //情况4，std包含test
	{
		for(j=0;j<size_std.width;j++)
		{	
			//将有效标准云图上侧hdis高度，复制给新图最上侧；
			for(i=0;i<hdis;i++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			//将有效标准云图右侧hdis宽高度，复制给新图最下侧；
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
			//	将有效标准云图左侧wdis宽度，复制给新图的最左侧；
			for(j=0;j<wdis;j++)
			{
				rgb_std = cvGet2D(std,i,j);
				rgb_test_new.val[0] = rgb_std.val[0];
				rgb_test_new.val[1] = rgb_std.val[1];
				rgb_test_new.val[2] = rgb_std.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);	
			}
			//	将有效标准云图右侧wdis宽度，复制给新图的最右侧；
			for(j=wdis;j<wdis+size_test.width;j++)
			{
				rgb_test = cvGet2D(test,i-hdis,j-wdis);
				rgb_test_new.val[0] = rgb_test.val[0];
				rgb_test_new.val[1] = rgb_test.val[1];
				rgb_test_new.val[2] = rgb_test.val[2];
				
				cvSet2D(test_new,i,j,rgb_test_new);
			}
			//将有效待测云图，复制给新图中间部分；
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
	
	cvSaveImage(testPicName,test_new);//新图替换原来的待测云图
	
}
//作差
CString CACDetectDlg ::SubtractPic(CString TestPicPath, CString StdPicPath)
{
	IplImage *img0 = cvLoadImage(TestPicPath);//待测有效云图
	IplImage *img1 = cvLoadImage(StdPicPath);//标准有效云图
	
	int i,j;
	CvScalar s0,s1,s2;
	IplImage * img2=cvCreateImage(cvGetSize(img0),img0->depth,img0->nChannels);//创建新图	
	cvZero(img2);//新图清零	
	//此时待测云图和标准云图大小相同,扫描整个待测云图和标准云图,作差
	for(i=0;i<img0->height;i++)
	{
        for(j=0;j<img0->width;j++)
		{
			s0=cvGet2D(img0,i,j); // 获得待测云图这点的像素
			s1=cvGet2D(img1,i,j); //获得标准云图这点像素
			s2=cvGet2D(img2,i,j); //获得新图像素,必然为空(0,0,0)
			//三通道分别作差去绝对值
			s2.val[0]=fabs(s1.val[0]-s0.val[0]);
			s2.val[1]=fabs(s1.val[1]-s0.val[1]);
			s2.val[2]=fabs(s1.val[2]-s0.val[2]);  
			
			cvSet2D(img2,i,j,s2);//设置新图的像素值 
		}
	}
	ImgSize.cx=img2->width;
	ImgSize.cy=img2->height;
	CString strSubpicname;
	
	strSubpicname=StdPicPath.Left(StdPicPath.ReverseFind('\\')+1)+strSubpicname;
	
	
	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //获得当前.exe文件的路径
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	strSubpicname=sPath+"\\"+"minuspic.bmp";
	
	cvSaveImage(strSubpicname,img2);//保存图像
	
	return strSubpicname;
	
} 

int CACDetectDlg::dtoi(double x)
{
	if(x<0) x=x-0.5;//小于0 -0.5 取整
	else x=x+0.5;//大于0 +0.5 取整
	return (int)x;
}

void CACDetectDlg::GetPcbInfo(CString filename)
{

//	CList<COMP_Shape,COMP_Shape&> listComponents;//器件列表（形状）
//  CList<COMP_List,COMP_List> listDesignComp;//设计文件所有器件
	listDesignComp.RemoveAll();//清空器件形状列表
	arrayCircOutline.RemoveAll();//清空轮廓列表
 	listComponents.RemoveAll();//清空所有器件列表
	//-------------------------------------------------各种变量----------------------
	char str[50];
	double PTX=0;  //PCB基板直线X坐标  
	double PTY=0;  //PCB基板直线Y坐标  
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	double COMPX=0;//器件中心的X坐标
	double COMPY=0;//器件中心的Y坐标d
	char Angle[20];// 器件的旋转角度
	char Level[20];//器件所在的层
	double CTFX=0;//器件线段起点x
	double CTFY=0;//器件线段起点y
	double CTLX=0;//器件线段终点x
	double CTLY=0;//器件线段终点y
	double CAX=0,CAY=0;//圆弧中心坐标(x,y)
	double CAR=0;//半径R
	double CASA=0;//起始角度StartAngle(SA)
	double CAEA=0;//终止角度EndAngle(EA)

    COMP_List *Cadd;//添加器件指针
    COMP_Shape *compadd;//添加器件形状指针
	COMP_Shape comptemp;//临时器件形状
	POSITION comp_pos;//所有器件列表位置
	CT *ctadd;//线段列表	
	CA *caadd;//圆弧列表	
	int First=1;//用来判断器件是不是第一场出现
	CvPoint *PointArray;//存放轮原始廓点
	CvPoint Ptemp;//中间临时存放轮廓
    //----------------------------------------------各种变量end-----------------		
    ifstream FilePoints(filename);//读文件
    //-------------------------------------------获得轮廓点---------------------
	int i=0;//存放点的数量
    int amount=0;//存放器件数
	FilePoints.getline(str,50);//读文件一行
	if (!strcmp(str,"POINTS"))//如果读到"POINTS"说明一下点为轮廓信息
	{			
		//cout<<"轮廓点集"<<endl;
		FilePoints>>i;//"POINT"之后参数是点的数量
		PointArray=new CvPoint[i];//动态创建数组
		
		for (int j=0;j<i;j++)//根据点的数量读取点的坐标
		{
			//读取文件接下里的两个值，分别是一个点的x坐标和y坐标
			FilePoints>>PTX;
			FilePoints>>PTY;
			PointArray[j]=cvPoint(dtoi(PTX),dtoi(PTY));  //cvPoint()中参数为整数，四舍五入
		}					
	}
	//找出电路板高和宽 有了数据这个是不是可以从数据库中得到	
	int LX=PointArray[0].x;//左下角(x,y)赋初值轮廓的第一个点
	int LY=PointArray[0].y;
	int RX=0;//右上角赋初值(0,0)
	int RY=0;
	for (int j=1;j<i;j++)
	{
		if (LX>=PointArray[j].x)//找到左下角x最大
		{
			LX=PointArray[j].x;
		}
		if(LY>=PointArray[j].y)//找到左下角y最大
		{
			LY=PointArray[j].y;
		}
		if (RY<=PointArray[j].y)//找到右上角y最大
		{
			RY=PointArray[j].y;		
		}
		if(RX<=PointArray[j].x)//找到右上角x最大
		{
			RX=PointArray[j].x;
		}
	}
	int height=RY-LY;//计算出电路板的高
	int width=RX-LX;//计算出电路板的宽
	int w=ImgSize.cx;//阈值图像高
	int h=ImgSize.cy;//阈值图像宽
	for ( j=0 ; j < i ;j++)
	{
		//转换实际坐标到像素坐标
        Ptemp.x=(PointArray[j].x-LX)*w/width;
        Ptemp.y=(PointArray[j].y-LY)*h/height;
		arrayCircOutline.Add(Ptemp);//保存在数组arrayCircOutline中
	}
	delete[] PointArray;//释放动态数组的空间
	while(!FilePoints.eof())//如果没有到文件末尾
	{
		FilePoints.getline(str,50);//读取文件一行
		if (!strcmp(str,"COMP"))//是否是COMP,如果是COMP表示一下信息是器件信息
		{			
			FilePoints.getline(str,sizeof(str));
 			strcpy(Geometry,str);//获得器件封装 ,因为有可能为空,直接用>>读取有可能出错故用strcpy
			FilePoints.getline(str,sizeof(str));
 			strcpy(Symbol,str);//获得器件型号
			FilePoints.getline(str,sizeof(str));
 			strcpy(Reference,str);//获得器件标号
			FilePoints>>COMPX;//获得器件中心x坐标
			FilePoints>>COMPY;//获得器件中心y坐标
			FilePoints>>Angle;//获得器件旋转角度
			FilePoints>>Level;//获得器件所在层
			COMPX=(COMPX-LX)*w/width;//转换器件中心坐标
			COMPY=(COMPY-LY)*h/height;
			//背面左右翻转 0519
			if (!strcmp(Level,"B"))//反面
			{
				COMPX=ImgSize.cx-COMPX;//坐标左右翻转
			}
			if (!strcmp(Angle,""))//0520 Angle 有可能为空 所以把原来的double型改为了char型
			{
				strcpy(Angle,"0");
			}
			if (COMPX>=0 && COMPY>=0 && COMPX<w && COMPY<h)//0420++器件中心坐标必然大于0,小于0为错误点,没有考虑
			{
			      bool flag;
				  flag=true;

				Cadd=new COMP_List;//创建Cadd用于添加器件形状
				strcpy(Cadd->Geometry,Geometry);//将器件封装放入Cadd中
				strcpy(Cadd->Symbol,Symbol);//将器件型号放入Cadd中
				strcpy(Cadd->Reference,Reference);//将器件标号放入Cadd中
				Cadd->Angle=atof(Angle);//char->double转换
				strcpy(Cadd->Level,Level);//将器件层放入Cadd中				
				Cadd->COMPX=COMPX;//将器件中心坐标(x,y)放入Cadd中
				Cadd->COMPY=COMPY;	
				listDesignComp.AddTail(*Cadd);//将所有器件信息放入listDesignComp列表中
				delete Cadd;//释放Cadd
				
				//器件列表
				comp_pos=listComponents.GetHeadPosition();//获得器件列表头位置
				while(comp_pos!=NULL)//如果位置不为空
				{
					//如果这个型号已在器件列表中则first为0；如果是第一次出现则first为1
					comptemp=listComponents.GetNext(comp_pos);//位置到下一个,返回当前位置值
					if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference))//判断是否已经在列表中
					{
						First=0; //已经在列表中设为0
					}
					else
					{
						First=1;//第一次出现设为1
					}				
				}		   
				//--------------------------------------获取线段和圆弧-----------------------------------------
				if (First==1)//如果first为1则把此器件放入器件列表中
				{
					compadd=new COMP_Shape;//初始化动态列表
					ctadd=new CT;//添加线段
					caadd=new CA;//添加圆弧
					strcpy(compadd->Geometry,Geometry);//器件封装
					strcpy(compadd->Reference,Reference);//器件标号
					
					while(strcmp(str,"ENDCOMP"))//到了器件信息结尾(文件中ENDCOMP描述)没?
					{
						FilePoints.getline(str,20);//读取文件一行
						if (!strcmp(str,"CT"))//是否是线段坐标 线段信息以CT开头
						{							
							FilePoints>>CTFX;//线段起始坐标(x,y)
							FilePoints>>CTFY;
							FilePoints>>CTLX;//线段终点坐标(x,y)
							FilePoints>>CTLY;	
							//转换并保存
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
								compadd->comp_CT.AddTail(*ctadd);//加入comp_CT列表尾
							}
						}
						if (!strcmp(str,"CA"))//是否读到了器件圆弧信息,以CA开头的是圆弧信息
						{
							FilePoints>>CAX;//器件圆弧中心x坐标
							FilePoints>>CAY;//器件圆弧中心y坐标
							FilePoints>>CAR;//圆弧半径R
							FilePoints>>CASA;//起始角度StartAngle(SA)
							FilePoints>>CAEA;//终止角度EndAngle(EA)
							//转换并保存
							caadd->CX=CAX*w/width;
							caadd->CY=CAY*h/height;
							caadd->R=CAR*h/height;
							caadd->SA=CASA;
							caadd->EA=CAEA;
							compadd->comp_CA.AddTail(*caadd);//加入comp_CA列表尾
						}
					}
					listComponents.AddTail(*compadd);//加入器件形状listComponents列表尾
					
					delete ctadd;//
					delete caadd;//每次释放一下
					delete compadd;//new 完要释放 要不然回内存泄漏
				}
			
			}			
		}
	}
	FilePoints.close();//关闭文件
	if (!listComponents.GetCount())//如果没有器件信息提示没有器件信息
	{
		MessageBox("没有器件信息!");
		return;
	}
	if (!arrayCircOutline.GetSize())//如果没有电路板轮廓点提示没有轮廓信息
	{
		MessageBox("没有轮廓信息!");
		return;
	}
	if (!listDesignComp.GetCount())//如果没有器件,提示没有器件
	{
		MessageBox("没有器件!");
		return;
	}

}

void CACDetectDlg::DrawPcb()
{
	//-------------------------------------------------各种变量----------------------
	double PTX=0;  //PCB基板直线X坐标  
	double PTY=0;  //PCB基板直线Y坐标  
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	double COMPX=0;//器件中心的X坐标
	double COMPY=0;//器件中心的Y坐标d
	double Angle=0;// 器件的旋转角度
	char Level[20];//器件所在的层

	double CTFX=0;//器件线段起点x
	double CTFY=0;//器件线段起点y
	double CTLX=0;//器件线段终点x
	double CTLY=0;//器件线段终点y
	double CAX=0,CAY=0;//圆弧中心坐标(x,y)
	double CAR=0;//半径R
	double CASA=0;//起始角度StartAngle(SA)
	double CAEA=0;//终止角度EndAngle(EA)
	//器件临时变量
    COMP_List ctemp;
	POSITION c_pos;
    //形状临时变量
	COMP_Shape comptemp;
	POSITION comp_pos;
	//线段列表临时变量
	CT cttemp;	
	POSITION ct_pos;
	//圆弧列表临时变量
	CA catemp;
	POSITION ca_pos;
	//问题器件临时变量
	COMP_List prob;//0905 +
	POSITION prob_pos;
    CPoint COMPPt;
	CvSize ImageSize1 = cvSize(ImgSize.cx,ImgSize.cy);//图片大小
	IplImage* imgt = cvCreateImage(ImageSize1, IPL_DEPTH_8U, 3);//新建T层图片
	imgt->origin=IPL_ORIGIN_BL;//图片坐标原点在左下角,所以只能这样设

	cvZero(imgt);
	//cvZero(imgb);
	//画轮廓
	int i=arrayCircOutline.GetSize();//获得轮廓点数
	
	//获得所有器件列表头位置
	for (int j=0;j<i-1;j++)//每两点绘制一条线段，使用OpenCV中的cvLine函数
         cvLine(imgt,arrayCircOutline[j],arrayCircOutline[j+1],CV_RGB(0,255,0),3,8,0);


	c_pos=listDesignComp.GetHeadPosition();
	while(c_pos!=NULL)//如果不为空
	{		
		ctemp=listDesignComp.GetNext(c_pos);//位置到下一个,返回当前位置值
		strcpy(Geometry,ctemp.Geometry);//获得器件封装
		strcpy(Symbol,ctemp.Symbol);//型号
		strcpy(Reference,ctemp.Reference);//标号
		strcpy(Level,ctemp.Level);//所在层
		COMPX=ctemp.COMPX;//中心x坐标
		COMPY=ctemp.COMPY;//中心y坐标
		Angle=ctemp.Angle;//旋转角度
        COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));
		CvScalar color=CV_RGB(150,150,150);//器件线段圆弧颜色灰色
		CvScalar scolor=color;//器件型号字颜色
		//问题器件设为红色
		prob_pos=listPixelComp.GetHeadPosition();//问题器件列表头位置
		while(prob_pos!=NULL)
		{
			prob=listPixelComp.GetNext(prob_pos);//位置到下一个,返回当前位置值
			
			if (CPoint(dtoi(prob.COMPX),dtoi(prob.COMPY))==COMPPt)//是否是问题器件,如果是
			{
				color=CV_RGB(255,0,0);//器件线段圆弧颜色设为红色
				//scolor=CV_RGB(0,255,255);//器件型号字设为青蓝色
				//scolor=CV_RGB(255,0,0);
			}
			if (CPoint(dtoi(strtod(strXPoint,NULL)),dtoi(strtod(strYPoint,NULL)))==COMPPt)//是否被选中
			{
				color=CV_RGB(255,255,0);
			//	scolor=CV_RGB(0,255,255);
			//	scolor=CV_RGB(255,0,0);
			}		
		}						  
		comp_pos=listComponents.GetHeadPosition();//器件形状信息列表头位置
		while(comp_pos!=NULL)//如果不为空
		{
			comptemp=listComponents.GetNext(comp_pos);//位置到下一个,返回当前位置值
			//找到这个器件的形状信息
			if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
			{					
				//----------------------------------输出线段----------------------------------------
				ct_pos=comptemp.comp_CT.GetHeadPosition();//器件线段头位置
				while(ct_pos!=NULL)//如果不为空
				{
					cttemp=comptemp.comp_CT.GetNext(ct_pos);//位置到下一个,返回当前位置值
					double FX=cttemp.BX;//器件线段起点x坐标
					double FY=cttemp.BY;//器件线段起点y坐标
					double LX=cttemp.EX;//器件线段终点x坐标
					double LY=cttemp.EY;//器件线段终点y坐标
					double CTFX1=0;
					double CTFY1=0;
					double CTLX1=0;
					double CTLY1=0;
					//根据中心坐标(x,y)和旋转角度，转换线段起点坐标和终点坐标，因为中间文件中给出的线段信息是以中心坐标为原点
					CTFX1=COMPX+cos(Angle*3.1415926/180)*FX-sin(Angle*3.1415926/180)*FY;//0414
					CTFY1=COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY;
					CTLX1=COMPX+cos(Angle*3.1415926/180)*LX-sin(Angle*3.1415926/180)*LY;							
					CTLY1=COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY;                           					
					//起点和终点
					CvPoint ptf=cvPoint(dtoi(CTFX1),dtoi(CTFY1));
					CvPoint ptl=cvPoint(dtoi(CTLX1),dtoi(CTLY1));
					
				
					cvLine(imgt,ptf,ptl,color,3,8,0);//使用OpenCV中的cvLine函数
					
				}//while CTlist	
				//-------------------------------圆弧----------------------------
				ca_pos=comptemp.comp_CA.GetHeadPosition();//圆弧头位置
				while(ca_pos!=NULL)//如果不为空
				{
					catemp=comptemp.comp_CA.GetNext(ca_pos);//获得位置到下一个,返回当前位置值
					double cx=catemp.CX;//圆心x坐标
					double cy=catemp.CY;//圆心y坐标
					double r=catemp.R;//半径
					double sa=catemp.SA;//起始角度				
					double ea=catemp.EA;//终止角度
					double cx1=0,cy1=0;
					CvPoint spt,ept;
					if (sa>ea)//如果起始角度大于终止角度
					{
						ea=ea+360;//终止角度+360,保证终止角度大于起始角度
					}
					//根据中心坐标(x,y)和旋转角度，转换圆弧圆心(x,y)坐标
					cx1=COMPX+cos(Angle*3.1415926/180)*cx-sin(Angle*3.1415926/180)*cy;//0411
					cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;				
					spt=cvPoint(cx1+r*cos((sa+Angle)*3.1415926/180),cy1+r*sin((sa+Angle)*3.1415926/180));
					ept=cvPoint(cx1+r*cos((ea+Angle)*3.1415926/180),cy1+r*sin((ea+Angle)*3.1415926/180));
					//顺时针0412，因为用opencv画图，而且原点坐标在左下角，使用需要顺时针转。
					sa=360-sa+(360-Angle);
					ea=360-ea+(360-Angle);																		
					//-------------------------------------------画圆弧--------------------------------
					
						cvLine(imgt,spt,ept,color,3,8,0);//使用OpenCV中的cvLine()
						cvEllipse(imgt,cvPoint(dtoi(cx1),dtoi(cy1)),cvSize(dtoi(r),dtoi(r)),0,ea,sa,color);//使用OpenCV中的cvEllipse()
				                 							
				}//while ca							
			}//if          
		}//while comp
	}//while

	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //获得当前.exe文件的路径
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 

	pathname_pcb=sPath+"\\"+"pcb.bmp";
	//电路板仿真图保存在指定路径下


	cvSaveImage(pathname_pcb,imgt);

}

//处理得到阈值图象
CString CACDetectDlg::ThresholdPic(CString filename, int pos)
{
	int i,j;
	CvScalar s;//存放像素值
	IplImage *img=cvLoadImage(filename);//加载图像
	for(i=0;i<img->height;i++)
	{
        for(j=0;j<img->width;j++)	
		{
			s=cvGet2D(img,i,j);//获得此点像素值			
			BOOL BigY0 = s.val[0]>pos;//如果三个通道像素值大于阈值，为真
			BOOL BigY1 = s.val[1]>pos;
			BOOL BigY2 = s.val[2]>pos;
			
			BOOL MoreThanTwo = BigY0? (BigY1||BigY2):(BigY1&&BigY2);  //三个中有两个成立
			
			if(!MoreThanTwo)  //通过阈值选择显示像素点
			{
				for(int k=0;k<img->nChannels;k++) //若有两个像素分量小于阈值，则该点各个像素分量设为0	
				{
					s.val[k]=0;//不在阈值范围内的设为黑色
				}		
			}
			else
			{
				for(int k=0;k<img->nChannels;k++) //若有两个像素分量小于阈值，则该点各个像素分量设为0	
				{
					s.val[k]=255;//不在阈值范围内的设为白色
				}	
				
			}
			cvSet2D(img,i,j,s); //设置像素值
		}
	}
	
	
    //保存转换后的阈值图像
	filename.TrimRight(".bmp");
	CString spos;
	spos.Format("%d",pos);
	filename=filename+"YuVal"+spos+".bmp";

	cvSaveImage(filename,img);
	return filename;

	
	
}

//将处理后的阈值图象变成边缘提取后的图象
CString CACDetectDlg::Edgepickpic(CString filename)
{
	IplImage *image1=cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);  //彩色阈值图像
	IplImage *image2=cvCreateImage(cvGetSize(image1),IPL_DEPTH_8U,1);  //单通道的新图
	IplImage *image3=cvCreateImage(cvGetSize(image1),IPL_DEPTH_8U,1);  //单通道的新图
	//IplImage *image2,*image3,*image4;
	//cvNot(image0,image1);
    cvErode(image1,image1,NULL,1);   //腐蚀操作
	cvDilate(image1,image1,NULL,1);  //膨胀操作
	cvDilate(image1,image1,NULL,1);   //膨胀操作
	cvErode(image1,image1,NULL,1);     //腐蚀操作
	
	cvSmooth(image1,image1,CV_GAUSSIAN,3,0,0,3);  //高通滤波
	/*//边缘检测
	//	cvSobel(image1,image2);
	cvNot(image1,image2);      //二值化
	//cvSobel(image1,image2,1,1,3);
	cvCanny(image1,image2,1,3,3); 
	//cvLaplace(image1,image2,1);*/
	cvNot(image1,image2);
	cvCanny(image2,image3,1,3,3);
	//cvLaplace(image2,image3,3);

	
    
	
	CString strSubpicname;	
	strSubpicname=filename.Left(filename.ReverseFind('\\')+1)+strSubpicname;
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //获得当前.exe文件的路径
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	strSubpicname=sPath+"\\"+"guize.bmp";	
	cvSaveImage(strSubpicname,image3);//保存图像
    return strSubpicname;
	
}

void CACDetectDlg::FindRect(CString imagename)
{
	
	listRect.RemoveAll();//清空listRect列表
	IplImage* img_8uc1=cvLoadImage(imagename,CV_LOAD_IMAGE_GRAYSCALE);//单通道图
	IplImage* img_edge=cvCreateImage(cvGetSize(img_8uc1),8,1);//创建单通道图
	
    cvFlip(img_8uc1,NULL,0);//沿x-轴翻转 因为坐标系的关系
	
	img_8uc1->origin=IPL_ORIGIN_BL;//左下角为图片原点
	img_edge->origin=IPL_ORIGIN_BL;//左下角为图片原点
    //对图像进行阈值处理，该函数对灰度图像进行阈值操作得到二值图像
	
	
	
    cvThreshold(img_8uc1,img_edge,128,255,CV_THRESH_BINARY);
	
	
	
    //查找轮廓
	CvMemStorage* storage=cvCreateMemStorage();//创建内存块，找轮廓中用到的
	CvSeq* first_contour=NULL;//序列，存储轮廓的点集
	
	int Nc= cvFindContours(   //返回轮廓个数
		img_edge,
		storage,
		&first_contour,
		sizeof(CvContour),
		CV_RETR_EXTERNAL);//只找最外层轮廓
	
	/*可以的话，将其实现在工具帮助栏*/
	// 	char msg[30];
	// 	sprintf(msg,"处理前共有轮廓:%d",Nc);
	// 	MessageBox(msg);
	
	CvRect* box;//存储点集矩形
	CvSeq* c=first_contour;//序列头指针
	box=new CvRect[Nc];//动态创建轮廓数个矩形
	//---------------------------------------ok-----------------------------------
	CRect recttemp,recttemp1,recttemp2;
	POSITION rl_pos1,rl_pos2,rl_pos3,rl_pos4;
	//获得矩形并储存在矩形链表中
	for (int i=0;c!=NULL;c=c->h_next,i++)
	{		
		box[i]=cvBoundingRect(c);//计算点集的最外面矩形边界	
		recttemp.TopLeft().x=box[i].x;//赋值给CRect
		recttemp.TopLeft().y=box[i].y;
		recttemp.BottomRight().x=box[i].x+box[i].width;
		recttemp.BottomRight().y=box[i].y+box[i].height;
		listRect.AddTail(recttemp);//保存在listRect列表中
	}
	delete[] box; //new一定要释放
    //判断矩形相交否，如果相交则合并在一起
	rl_pos1=listRect.GetHeadPosition();//获得listRect头位置
	while(rl_pos1!=NULL)
	{
		int cross=0,sum=0;
        rl_pos3=rl_pos1;//保存当前位置,以便最后删除
		recttemp1=listRect.GetNext(rl_pos1);//位置到下一个,返回当前位置值
		//从尾判断与每个矩形相交相交否
		rl_pos2=listRect.GetTailPosition();//获得listRect尾位置
		while(rl_pos2!=NULL)
		{
			rl_pos4=rl_pos2;//保存当前位置,以便合并
			recttemp2=listRect.GetPrev(rl_pos2);//位置到下一个,返回当前位置值
			if (recttemp1!=recttemp2)//获得两个矩形不是同一个
			{
				cross=recttemp.IntersectRect(recttemp1,recttemp2);//cross=1，相交；否则，cross=0
				sum=sum+cross;//计算这个矩形与其他矩形相交的总数
				if (cross==1)//是否相交
				{
					recttemp.UnionRect(recttemp1,recttemp2);//相交则合并
					listRect.SetAt(rl_pos4,recttemp);		//并存储在后者的位置	
				}	
			}			
		}
		if (sum>=1)//与其他矩形相交,已经被合并到后者了,在列表中删除这个点
		{
			listRect.RemoveAt(rl_pos3);//已被合并删除这个点
		}		
	}
	
}
void CACDetectDlg::JudgeRect( )
{
//	CList<CPoint,CPoint> listRectComp;//矩形判断的问题器件 0901 change CString to CPoint
	listRectComp.RemoveAll();//清空
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	double COMPX=0;//器件中心的X坐标
	double COMPY=0;//器件中心的Y坐标d
	double Angle=0;// 器件的旋转角度
	char Level[20];//器件所在的层
    //器件临时变量
	COMP_List ctemp;
	POSITION c_pos;
	//形状临时变量
	COMP_Shape comptemp;
	POSITION comp_pos;
	//线段临时变量
	CT cttemp;	
	POSITION ct_pos;
	//圆弧临时变量
	CA catemp;
	POSITION ca_pos;
    //矩形临时变量
	POSITION pos;
	CRect RectTemp;
	CPoint COMPPt;
	c_pos=listDesignComp.GetHeadPosition();//获得器件列表头位置
	while(c_pos!=NULL)
	{
        //读器件信息
        ctemp=listDesignComp.GetNext(c_pos);//位置到下一个,返回当前位置值
		strcpy(Geometry,ctemp.Geometry);//获取器件封装
		strcpy(Symbol,ctemp.Symbol);//获取器件型号
		strcpy(Reference,ctemp.Reference);//获取器件标号
		strcpy(Level,ctemp.Level);//获取器件所在层
		COMPX=ctemp.COMPX;//获取器件中心x坐标
		COMPY=ctemp.COMPY;//获取器件中心y坐标
		Angle=ctemp.Angle;//获取器件旋转角度	
		COMPPt.x=dtoi(COMPX);
		COMPPt.y=dtoi(COMPY);
		//COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));
		//if (!strcmp(Level,layer) )//判断正反
	//	{
			
			int incomp=0;//是否已加入列表中的标志位
			//判断中心点是否在矩形中,如果在矩形中加入problemcomp中
			pos=listRect.GetHeadPosition();//获得头位置
			while(pos!=NULL)
			{
				RectTemp=listRect.GetNext(pos);//位置到下一个,返回当前位置值
				//判断中心是否在异常区域矩形中
				if (COMPX<=RectTemp.right && COMPX>=RectTemp.left && COMPY>=RectTemp.top && COMPY<=RectTemp.bottom)
				{
					listRectComp.AddTail(COMPPt);//如果是,将器件型号加入listRectComp列表中
					incomp=1;//标志位设置为1
				}
			}

			comp_pos=listComponents.GetHeadPosition();//获取器件形状列表头位置
			while(comp_pos!=NULL)
			{
				comptemp=listComponents.GetNext(comp_pos);//位置到下一个,返回当前位置值
				//找相同类型器件形状
				if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
				{	
					//获得器件线段和圆弧信息					
					ct_pos=comptemp.comp_CT.GetHeadPosition();//获得线段头位置
					while(ct_pos!=NULL)
					{
						cttemp=comptemp.comp_CT.GetNext(ct_pos);//位置到下一个,返回当前位置值
						double FX=cttemp.BX;//线段起点x坐标
						double FY=cttemp.BY;//线段终点y坐标
						double LX=cttemp.EX;//线段终点x坐标
						double LY=cttemp.EY;//线段终点y坐标
						double CTFX1=0;
						double CTFY1=0;
						double CTLX1=0;
						double CTLY1=0;
					    //根据旋转角度转换器件起点坐标和终点坐标
						CTFX1=dtoi(COMPX+cos(Angle*3.1415926/180)*FX-sin(Angle*3.1415926/180)*FY);//0414
						CTFY1=dtoi(COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY);
						CTLX1=dtoi(COMPX+cos(Angle*3.1415926/180)*LX-sin(Angle*3.1415926/180)*LY);							
						CTLY1=dtoi(COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY); 
						

                       
						//--------------判断器件线段是否在问题矩形中-------------------
						if (incomp!=1)
						{
							
							pos=listRect.GetHeadPosition();//获得矩形链表头位置
							while(pos!=NULL)
							{								
								RectTemp=listRect.GetNext(pos);                         
								CRect CTrect,rect;
								//主要是让线段到中心组成一个矩形,判断这个矩形是否与问题区域矩形相交
								CTrect.left=CTFX1;//左下角 起点
								CTrect.bottom=CTFY1;
								if (CTFY1==CTLY1)//横线段	
								{
									CTrect.right=CTLX1;//右上角
									CTrect.top=COMPY;//到中心坐标,故使用中心坐标的y作为纵坐标
								}
								else if (CTFX1==CTLX1)//垂线
								{
									CTrect.right=COMPX;//右上角,使用中心坐标x作为横坐标
									CTrect.top=CTLY1;
								}
								else 
								{
									CTrect.right=CTLX1;//右上角 终点
									CTrect.top=CTLY1;
								}
								CTrect.NormalizeRect();//规范化矩形
								
								if(rect.IntersectRect(CTrect,RectTemp))//判断是否相交
								{
									listRectComp.AddTail(COMPPt);	   //如果相交加入到listRectComp中
									incomp=1;						   //并将incomp设为1	
									break;							   //跳出while循环,判断下一个器件
								}
							}//while
						}//if																																				
					}//while CTlist	
				
					//-------------------------------圆弧----------------------------
					ca_pos=comptemp.comp_CA.GetHeadPosition();
					while(ca_pos!=NULL)
					{
						catemp=comptemp.comp_CA.GetNext(ca_pos);
						double cx=catemp.CX;//圆心x坐标
						double cy=catemp.CY;//圆心y坐标
						double r=catemp.R;//半径
						double sa=catemp.SA;//起始角度					
						double ea=catemp.EA;//终止角度
						double cx1=0,cy1=0;
						//根据器件中心坐标和旋转角度,转换圆心坐标
						cx1=COMPX+cos(Angle*3.1415926/180)*cx-sin(Angle*3.1415926/180)*cy;//0411
						cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;										
						//--------------------判断圆弧是否与矩形相交-------------------
						if (incomp!=1)
						{								
							pos=listRect.GetHeadPosition();
							while(pos!=NULL)
							{
								RectTemp=listRect.GetNext(pos);									
								//圆用外接矩形表示，判断是否与异常区域矩形相交
								CRect CTrect,rect;
								//圆的外接矩形四个顶点
								CTrect.left=dtoi(cx1-r);
								CTrect.bottom=dtoi(cy1-r);
								CTrect.right=dtoi(cx1+r);
								CTrect.top=dtoi(cy1+r);
								CTrect.NormalizeRect();
								
								if(rect.IntersectRect(CTrect,RectTemp))//如果相交
								{
									listRectComp.AddTail(COMPPt);//如果相交加入到listRectComp中
									incomp=1;	//标志位置1
									break;//跳出,判断下一个器件
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

	listPixelComp.RemoveAll();//清空listPixelComp
			
	IplImage* img=cvLoadImage(imagename,CV_LOAD_IMAGE_COLOR);//加载阈值图像
	cvFlip(img,NULL,0);//沿x-轴翻转 因为坐标系的关系	
	img->origin=IPL_ORIGIN_BL;//左下角为原点
	CvScalar Pixel;//存放像素值
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	double COMPX=0;//器件中心的X坐标
	double COMPY=0;//器件中心的Y坐标d
	double Angle=0;// 器件的旋转角度
	char Level[20];//器件所在的层
	//器件临时变量
	COMP_List ctemp;
	POSITION c_pos;
	//器件形状临时变量
	COMP_Shape comptemp;
	POSITION comp_pos;
	CT cttemp;	//线段临时变量
	POSITION ct_pos;
	CA catemp;  //圆弧临时变量
	POSITION ca_pos;    
	//CString prob;//保存器件型号
	CPoint prob;// save this comp's position
	POSITION pos;
	CPoint COMPPt;//保存点
	COMP_List addprobcomp;//附加到listpixelComp中
	c_pos=listDesignComp.GetHeadPosition();//器件列表头位置
	while(c_pos!=NULL)
	{
		int inflag=0;//标志器件型号已经加入listPixelComp列表中
		ctemp=listDesignComp.GetNext(c_pos);
		pos=listRectComp.GetHeadPosition();	//异常区域矩形判断出的器件列表listRectComp头位置
		while(pos!=NULL)
		{
			prob=listRectComp.GetNext(pos);

			COMPX=ctemp.COMPX;//器件中心x坐标
	    	COMPY=ctemp.COMPY;//器件中心y坐标
			COMPPt=CPoint(dtoi(COMPX),dtoi(COMPY));

			//if (!strcmp(Symbol,prob))
			if(prob==COMPPt)//0902
			{
				//-获得信息
				strcpy(Geometry,ctemp.Geometry);//器件封装
				strcpy(Symbol,ctemp.Symbol);//获得器件型号
				strcpy(Reference,ctemp.Reference);//器件标号
				strcpy(Level,ctemp.Level);//器件所在层				
				Angle=ctemp.Angle;//器件旋转角度

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
				//------------------------判断中心像素是否在问题区域------------------------
				Pixel=cvGet2D(img,COMPY,COMPX);//获得器件中心坐标在阈值图像上点的像素
				if (inflag!=1)//还没有加入疑似故障器件列表中
				{
					if (Pixel.val[0]>=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)//判断像素是否为白色
					{
						inflag=1;//如果是,标志位置1
						listPixelComp.AddTail(addprobcomp);//器件型号加入疑似故障器件列表
					}
				}																
				comp_pos=listComponents.GetHeadPosition();//器件头位置
				while(comp_pos!=NULL)
				{
					comptemp=listComponents.GetNext(comp_pos);
					//找相同类型器件形状
					if (!strcmp(comptemp.Geometry,Geometry)&&!strcmp(comptemp.Reference,Reference)&&comptemp.comp_CT.IsEmpty()==0)
					{		
						//------------------------线段信息---------------------
						ct_pos=comptemp.comp_CT.GetHeadPosition();
						while(ct_pos!=NULL)
						{
							cttemp=comptemp.comp_CT.GetNext(ct_pos);
							double FX=cttemp.BX;//线段起点x坐标
							double FY=cttemp.BY;//线段起点y坐标
							double LX=cttemp.EX;//线段终点x坐标
							double LY=cttemp.EY;//线段终点y坐标
 							int CTFX1=0;
 							int CTFY1=0;
 							int CTLX1=0;
 							int CTLY1=0;
							
							//根据中心标志和旋转角度转换线段起点和终点坐标
							CTFX1=dtoi(COMPX+cos(Angle*3.1415926/180)*FX+sin(Angle*3.1415926/180)*FY);//0411
							CTFY1=dtoi(COMPY+sin(Angle*3.1415926/180)*FX+cos(Angle*3.1415926/180)*FY);
							CTLX1=dtoi(COMPX+cos(Angle*3.1415926/180)*LX+sin(Angle*3.1415926/180)*LY);							
							CTLY1=dtoi(COMPY+sin(Angle*3.1415926/180)*LX+cos(Angle*3.1415926/180)*LY);
							//用像素判断器件线段
							int i=0,j=0,nStart=0,nEnd=0,nBegin=0,nFinal=0;
							if (inflag!=1)
							{
								//像素扫描也是扫描线段与中心坐标构成的矩形的区域
								if (CTFX1==CTLX1)//垂线
								{ 
									//判断起点终点,较小的作为像素扫描的起点
									
									if (CTFX1<COMPX){nStart=CTFX1;nEnd=COMPX;}
									else{nStart=COMPX;nEnd=CTFX1;}
									if (CTLY1>CTFY1){nBegin=CTFY1;nFinal=CTLY1;}
									else {nBegin=CTLY1;nFinal=CTFY1;}
								}									
								else if (CTFY1==CTLY1)//横行
								{
									//判断起点终点,较小的作为像素扫描的起点
									if (CTLX1>CTFX1){nStart=CTFX1;nEnd=CTLX1;}
									else {nStart=CTLX1;nEnd=CTFX1;}
									if (CTFY1<COMPY){nBegin=CTFY1;nFinal=COMPY;}
									else{nBegin=COMPY;nFinal=CTFY1;}
									
								}
								else
								{	//斜边	0408 0425-修改
									//判断起点终点,较小的作为像素扫描的起点
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
										//像素三个分量大于200,判断为白色
										if (Pixel.val[0]>=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)
										{
											inflag=1;//标志位置1
											listPixelComp.AddTail(addprobcomp);//加入listPixelComp中
											//cvLine(img,cvPoint(i,j),cvPoint(i,j),CV_RGB(255,0,0),2);
											break;//跳出for循环
										}											
									}
									if (inflag==1)break;//如果标志位为1,跳出for循环
								}					
							}//if

						}//while CTlist	
						//-------------------圆弧信息-------------------------
						ca_pos=comptemp.comp_CA.GetHeadPosition();
						while(ca_pos!=NULL)
						{
							catemp=comptemp.comp_CA.GetNext(ca_pos);
							double cx=catemp.CX;//圆心x坐标
							double cy=catemp.CY;//圆心y坐标
							double r=catemp.R;//半径
							double sa=catemp.SA;//起始角度				
							double ea=catemp.EA;//终止角度		
							double cx1=0,cy1=0;
							if (sa>ea)//使终止角度大于起始角度
							{
								ea=ea+360;
							}	
							//根据中心标志和旋转角度转换圆弧圆心坐标,和角度
							cx1=COMPX+cos(Angle*3.1415926/180)*cx+sin(Angle*3.1415926/180)*cy;//0411
							cy1=COMPY+sin(Angle*3.1415926/180)*cx+cos(Angle*3.1415926/180)*cy;
							sa=sa+Angle;
							ea=ea+Angle;
							//以起点横坐标为起始点扫描整个圆弧像素
							if (inflag!=1)
							{ 
								//弧 0411 ok
								double xt=0,yt=0;
								int i=0,j=0,n=0;
								double xSt=cx1+r*cos(sa*3.1415926/180);//起点横坐标
								for (i=sa;i<=ea;i=i+3)//每次角度增3(这个数值可以更小)
								{
									xt=cx1+r*cos(i*3.1415926/180);//旋转了角度x坐标
									yt=cy1+r*sin(i*3.1415926/180);//旋转了角度y坐标
									if (xt<xSt){j=xt;n=xSt;}//初始化循环起点
									else{j=xSt;n=xt;}
				                    for(;j<=n;j++)
									{
										Pixel=cvGet2D(img,yt,j);//获得像素
										//判断像素是否为白色
										if (Pixel.val[0] >=200 && Pixel.val[1]>=200 && Pixel.val[2]>=200)
										{
											inflag=1;//如果是,标志位置1
											listPixelComp.AddTail(addprobcomp);//加入加入listPixelComp中
											break;//跳出内层for循环
										}
									
									}
									if(inflag==1)break;//跳出外层for循环,判断下一个器件
								}
							}							
						}						
					}//if 相同器件				
				}//while器件como_pos      
			}//if
		}//while
	}//while
	//销毁图像
	//cvReleaseImage(&img);


}

void CACDetectDlg::AlphaMerge(CString PCBImage, CString Imagename)
{
	IplImage *src1, *src2;
	src1=cvLoadImage(PCBImage,1);//加载仿真电路图像
	src2=cvLoadImage(Imagename,1);//加载彩色阈值图像
	
	//alpha和beta是融合强度，alpha是src1,beta是src2,且有关系beta=1-alpha
	double alpha = 0.4;
	double beta =  0.6;
    //---------------OpenCV中融合函数------------------
	cvAddWeighted(src1,alpha,src2,beta,0.0,src1);
	//保存融合后的图像
	
	CString sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   //获得当前.exe文件的路径
	sPath.ReleaseBuffer();   
	int nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos); 
	
	pathname_merge=sPath+"\\"+"merge.bmp";
	//电路板仿真图保存在指定路径下
	
	
	cvSaveImage(pathname_merge,src1);
}

void CACDetectDlg::ListProbComp()
{
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	CString COMPX;//器件中心的X坐标
	CString COMPY;//器件中心的Y坐标d
	COMP_List prob;//save this comp's info
	POSITION ppos;	
	CString ID;//器件序号
	long count=listPixelComp.GetCount();//器件总数
	
	ppos=listPixelComp.GetHeadPosition();//疑似故障器件列表头位置
	while(ppos!=NULL)
	{
		prob=listPixelComp.GetNext(ppos);
		
		//-获得信息
		strcpy(Geometry,prob.Geometry);//器件封装
		strcpy(Symbol,prob.Symbol);//获得疑似故障器件型号
		strcpy(Reference,prob.Reference);//器件标号
		COMPX.Format("%0.3f",prob.COMPX);//器件中心x坐标
		COMPY.Format("%0.3f",prob.COMPY);//器件中心y坐标
		ID.Format("%d",count);//器件序号
		count--;//每次都插入第一个行,故从总数开始依次减
		m_ListCompProblem.InsertItem(0,ID,1);//插入首行,序号
		m_ListCompProblem.SetItemText(0,1,Geometry);//第二列封装
		m_ListCompProblem.SetItemText(0,2,Symbol);//第三列型号
		m_ListCompProblem.SetItemText(0,3,Reference);//第四列标号
		m_ListCompProblem.SetItemText(0,4,COMPX);//器件中心x坐标
		m_ListCompProblem.SetItemText(0,5,COMPY);//器件中心y坐标		
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
		MessageBox("待测云图未选中，请再次打开");
		return;
	}

	GetDlgItem(IDC_TEST_PATH)->SetWindowText(pathname_test);

    bool pipei= MatchStd(pathname_test , pathname_std);

	//length_std=pathname_std.GetLength();
	

	if(pathname_std.GetLength()==0)
	{
		
			MessageBox("没有与之对应的标准云图");
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

		MessageBox("设计文件未选中，请再次打开");
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
			MessageBox("请先加载待测云图和设计文件");
			return;
		}
		if(FlagDesign)
		{
			MessageBox("请先加载待测云图");
			return;
		}
	}
	
	if(FlagTest)
	{
		if(!FlagDesign)
		{
		//	MessageBox("请先加载设计文件");
			MessageBox("请先加载设计文件!","系统提示",MB_OK|MB_ICONWARNING);
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
