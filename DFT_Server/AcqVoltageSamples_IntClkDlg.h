
// CAcqVoltageSamples_IntClkDlg : header file
//

#pragma once
#include "afxwin.h"
#include "textbox_okng.h"
#include "afxext.h"
#include "label_text.h"
//#include "afxcmn.h"

#include "DlgResult.h"
//#include "DlgBarcode.h"
#include "DlgMessage.h"
#include "DlgSimpleInform.h"

//#include <opencv2\opencv.hpp>
//#include <allheaders.h>
//#include <baseapi.h>
#include "afxcmn.h"

#include "UDpAsySk.h"

#include "MySocket.h"
#include "MyEchoSocket.h"


#pragma comment(lib, "liblept168")
#pragma comment(lib, "libtesseract302")

using namespace std;
//using namespace cv;
//using namespace tesseract;

//#define TESTMODE_LEVEL_CHANGE

#define TESTMODE_NOISE_HIGHSAMPLERATE

#ifdef TESTMODE_NOISE_HIGHSAMPLERATE
#define NI_SAMPLE_RATE		200000.0
#define NI_SAMPLE_COUNT		6000
#define NI_CHECK_COUNT		4000
#else
#define NI_SAMPLE_RATE		100000.0
#define NI_SAMPLE_COUNT		3000
#define NI_CHECK_COUNT		2000

#endif
//m_SelectDFT = DFT_SELECT_ALL;
#define DFT_SELECT_ALL 0
#define DFT_SELECT_123 1
#define DFT_SELECT_456 2


// CAcqVoltageSamples_IntClkDlg dialog
class CAcqVoltageSamples_IntClkDlg : public CDialog
{
// Construction
public:
    // Standard constructor
    CAcqVoltageSamples_IntClkDlg(CWnd* pParent = NULL);
	virtual ~CAcqVoltageSamples_IntClkDlg();
    // Dialog data
    enum { IDD = IDD_ACQVOLTAGE_INTCLKDIGREF_DIALOG };

protected:
    // DDX/DDV support 
    virtual void DoDataExchange(CDataExchange* pDX);

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    DECLARE_MESSAGE_MAP()
public:

	int		m_RunCounter;
	int		m_TaskRun;
	int		m_nCurrentNumber;
	int		m_nCurrentProcess;
	int		m_nCurrentStep;
	int		m_WaitCnt ;

	UINT m_OKcnt;


	

	int m_MainPAGE_COM_PROCESS;//m_TESTPAGE_COM_PROCESS;
	int  m_ComAck;
	//	 m_WaitCnt = 0;
	int  m_ResendCnt;

	int m_PCB_COM_PROCESS ;
	int m_PCBComAck ;
	int m_PCBWaitCnt ;
	int m_PCBResendCnt;

		int m_WaitPCB_Ack_Limit;

		
	int m_LED_COM_PROCESS;
	int m_LEDComAck;
	int m_LEDWaitCnt;
	int m_LEDResendCnt;


	
	BYTE m_RecvBuf[4096];
	int  m_RecvCnt;

	BYTE m_RecvBufPCB[4096];
	int  m_RecvCntPCB;

	BYTE m_RecvBufLED[4096];
	int  m_RecvCntLED;

	BYTE m_RecvBufSCAN[4096];
	int  m_RecvCntSCAN;


	BYTE m_SendBuf[4096];
	int  m_SendCnt;	
	
	BYTE m_SendBufPCB[4096];
	int  m_SendCntPCB;

	BYTE m_SendBufLED[4096];
	int  m_SendCntLED;

	//void	SendData(BYTE *lbuf, int lcnt);
	int m_ComWaitCnt;

	
	
	
	Recomon_ModelT m_RemoconModel;

	
	void MessageDisplay(int lError, CString sMessage);


	//void MesStart();
	//int  MesTestReport();

	double m_avg_PPVoltage[8][3];
	double m_avg_Freg[8][3];

	double m_PPVoltage[8];
	double m_Frequency[8];

	double m_NoiseFlag[8];


	double m_RefVoltageMin[8];
	double m_RefVoltageMax[8];
	double m_FrequencyMin[8];
	double m_FrequencyMax[8];
	//double m_VoltageBalnce[8];//m_TmpVoltageBalnce
	

	double m_CheckVoltage[8];
	double m_CheckFrequency[8];

	int m_ChannelOK[8];
	int m_ChannelOKCNT[8];
	int m_VoltageOK[8];
	int m_FrequencyOK[8];
	int m_BalanceOK[8];
	int m_NG_OK_Num;

	bool m_LevelSkip;

	//int m_TEST_1_OK;
	//int m_TEST_2_OK;
	int m_TEST_Number;
	bool m_NewSystemCheck;

	int  m_TEST_Result;

	int m_TotalOK;
	bool m_bStartMES;


	//CNiReal64Matrix Check_data[16];

	int  m_CheckID[8];
	int  m_CheckEnable[8];
	CString  m_CheckName[8];

	int m_DigitalPort;
	int m_DigitalPort_Save;

	int m_InitAiChannel;
	int m_DOportOpen;
	int m_DIportOpen;

	unsigned long m_DO_Writedata;
	
	//CString m_BarcodeString;
	//CString m_WipID;
	int m_GMES_Skipped;


	void SetScroll(int  lIndex, int lScroll);
	
	void SetTimeCheck(int  lIndex);

	unsigned long GetTimeCheck(int  lIndex);
	void  DisplayTimeLap();


	unsigned long mstartT_1;
	unsigned long mstartT_2;
	unsigned long mstartT_3;
	unsigned long mstartT_4;
	unsigned long mstartT_5;
	unsigned long mstartT_6;

	int		m_PreRunCounter;
	int		m_PreTaskRun;
	int		m_nPreCurrentNumber;
	int		m_nPreCurrentStep;
	int		m_nPreCurrentProcess;


	int CheckTimeOn;
	int CycleTimeOn;

	int m_LAN_CheckOK;
	int m_USB1_CheckOK;
	int m_USB2_CheckOK;
	int m_PCB_CheckOK;
	int m_LED_CheckOK;

	CFont* font;
	
	void LoadTestData(CString lstrModelname);//LoadTestData(CString lstrModelname)
	void DisplayTestData();
	//void SetListControl();
	//void SetSubListControl(int lEQP_index, SaveLogDataT lSaveReceivedData);
	void SetOQAListControl();
	void SetSubDetailDisplay(int lEQP_index);
	void SetSubComDetailDisplay(int lEQP_index);

//	void SetSubListControlScan(SaveLogDataT lSaveReceivedData);
	BOOL CheckOKNG(CString l_BarcodeString);


	/*void SetFunction();
	void RunFunction();
	void SetChannelDataSeq(int lTestChannel, int lSource);
	void SetChannelDataRangeSeq(int lTestChannel, int l_RefVoltageMin,int l_RefVoltageMax,int l_FrequencyMin,int l_FrequencyMax);
	void SetChannelDataRangeSeq(int lTestChannel, int l_RefVoltageMin,int l_RefVoltageMax,int l_FrequencyMin,int l_FrequencyMax, int l_TmpVoltageBalnce);

	void CAcqVoltageSamples_IntClkDlg::SetChannelDataManual(int lTestNumber);

	void PreSetFunction();
	void PreRunFunction();

	void SetFreqSend(UINT lPort, UINT lFreq );
*/

	int mCheckAll;

	CString m_FunctionName;
	CString m_FunctionParameter;
	int			m_FunctionType;
	int			m_CheckDelayTime;
	CString		m_strMP_Key;
	int			m_RepeatCount;

	
	CString		m_PreFunctionName;
	CString		m_PreFunctionParameter;
	int			m_PreFunctionType;
	int			m_PreCheckDelayTime;
	CString		m_PrestrMP_Key;



	//TestPort_T m_PortData;
	int			m_nChannel;
	int			m_nSource;

	int m_TmpVoltageMin;
	int m_TmpVoltageMax;
	int m_TmpFrequencyMin;
	int m_TmpFrequencyMax;
	int m_TmpVoltageBalnce;

	int m_nBalanceEnable[8];
	int m_nBalanceChannel[8];
	int m_nBalanceValue[8];


	CString		m_strCameraCheckIMGfile;
	int			m_Match_Percent;
	CString		m_strCameraCheckChar;

	unsigned char m_UartMcuType;
	unsigned char m_UartCommand[50];
	int m_UartCommandCnt;
	unsigned char m_UartOkAckData[50];
	unsigned char m_UartREADData[50];
	int m_UartOkAckDataCnt;

	unsigned char m_LEDOkAckData[4];
	unsigned char m_LEDReadData[4];
	int m_MIC_Status;




//////////////////////////////////////////////////////////////

	double tres1;
	double tres2;
	
	int nErode;
	int nDilate;
	int mTres,mTresMax;//	
	int mX,mY;	
	int mXW,mYH;	
	int mR,mG,mB;

	int m_ocrX,m_ocrY;	
	int m_ocrXW,m_ocrYH;	



	CRect rect;
	CDC memDC;
	CBitmap Bitmap,*pOldBitmap;
	CDC *m_pDC;

	//void ResizeImage(IplImage* src_img, IplImage** dst_img, CRect* rect);
	//void DisplayImage(IplImage* img);
	//void DisplayBitmap(CDC* pDC, CRect rect, IplImage* img);


	//int m_Camera_CheckOK;
	//int m_Camera_Capture_Flag;
	//CWinThread *pThreadCap;

	int m_EnableCount;
	int m_RoopCnt;

	int m_BtnClearCnt;


	CMySocket m_sListenSocket[6];
	CMySocket m_pUDPSocketServer[6];
	unsigned int m_SockCount;
	MyEchoSocket m_pPLCSocketClient;
	BOOL CAcqVoltageSamples_IntClkDlg::CreateEthernetSocketPLC(CString sIpAdd, int nPort);

	TCHAR m_szError[255];
	int m_EthernetMode;
	UINT m_EthernetWaitCnt;
	UINT m_EthernetAck;
	UINT m_EthernetResendCnt;
	UINT m_GMES_Alive_WaitCnt[6];

	UINT m_DFT_Online[6];
	UINT m_GMES_CommError;
	UINT m_GMES_NGCNT;
	
	CString m_strEthernetReceive[6];
	CString m_strMesSend;
	CString m_GMES_ErrorName;//
	CString m_GMES_ErrorValue;

	CString m_Deisplay_ErrorName;//
	CString m_Deisplay_ErrorDetail;

	int m_PCB_IN_Error[6];
	//int m_OK_Count;
	//int m_NG_Count;


	//void CloseEthernetScocket() ;
	//void CreateEthernetSocket() ;
	long OnParseDataEthernet(int lIndex);
	BOOL CreateEthernetSocket() ;
	BOOL ConnectSocket();

	void OnShockAccept(int lIndex);
	void OnShockConnect(int lIndex);
	void OnShockSend(int lIndex);
	void OnShockReceive(int lIndex);
	void OnShockClose(int lIndex);

	BOOL OnCmdGmesSend(int lIndex);
	int OnCmdPLCSend(int lIndex, const void* lpBuf, int nBufLen);
	BOOL CAcqVoltageSamples_IntClkDlg::OnPLCSocketReceive();
	BOOL CAcqVoltageSamples_IntClkDlg::PLC_AlarmCheck();
	void CAcqVoltageSamples_IntClkDlg::SavePLCDataLog();

	int CAcqVoltageSamples_IntClkDlg::SavePLCAlarm(int lAlarmNumber, int lON);
	void CAcqVoltageSamples_IntClkDlg::SavePLCAlarm();
	int CAcqVoltageSamples_IntClkDlg::SavePLC_UPH();
	int m_AlarmCount;


	//char m_sReadBuffer[4096];

	char	m_sPLCReadBuffer[8192];
	int     m_nPLCReadSize;

	int		m_bPLC_Online;
	int		m_bPLC_WaitCount;
	int		m_PLC_AlarmOn;
	int		m_PLC_AlarmNumber[5];
	int		m_PLC_TotalQty;
	int		m_PLC_OK_Qty;
	int		m_PLC_NG_Qty;
	int		m_PLC_OK_QtyDFT[6];
	int		m_PLC_NG_QtyDFT[6];


	BOOL CreateEthernetSocketPLC();
	BOOL	CreateEthernetSocketSvr(int lIndex) ;
	BOOL	OnCmdGmesSendSvr(int lIndex);
	void	CloseEthernetScocketSvr(int lIndex) ;
	
	int			m_AlarmNumberOn[12];
	int			m_AlarmCode[100];
	CString		m_AlarmName[100];
	CString		m_AlarmMessage[100];


	int		m_ScanEnable;


	void GMESLinkTestResponse() ;
	void GMES_TestCtrl(int  lEQP_Index, CString   lstrCommand) ;
	void GMES_TestCtrl(int  lEQP_Index, CString   lstrCommand, CString   lstrData) ;
	

	CString CAcqVoltageSamples_IntClkDlg::GetMyIPAddress();

	int CAcqVoltageSamples_IntClkDlg::WriteCommand(UINT Address, char* Data, int lDataLen);
	int CAcqVoltageSamples_IntClkDlg::ReadCommand(UINT Address, int lDataLen);

	CDlgResult	*mpDlgResult;
//	CDlgBarcode	*mpDlgBarcode;

	//CDlgSimpleInform 	*mpDlgSimpleInform;

	void NG_Display(int l_ID, CString lstr = "");
	void OK_Display(int l_ID);
	void CLOSE_Result_Display();

	void BarcodeErrorDisplay();
	int m_NG_Retry_Count;
	int m_NG_WaitFlag;

	CString m_SuffixTmpString;
	//void SetGuiPos(int Mode);

	void AddGmesLog(CString LogStr, int Recv = 0);
	void AddPLCLog(CString LogStr, int Recv = 0);
	//void OnBnClickedButtonFreqSet();

	void ChangeModel(CString lModel);
	int MessageWindow(CString str);
	CString GetModelList(CString NewName);

	int m_SelectedMonitoringEquipID;
//	void CustomdrawListOqaMonitor(NMLVCUSTOMDRAW* pLVCD, LRESULT *pResult, int lIndex);
	void CustomdrawListOqaMonitor(int nItem, int nlSubItem, COLORREF *pcrText,COLORREF *pcrBkgnd, int lIndex);

	void SetRobotStatusDisplay(int lEQP_index, CString RcvStr);
	void SetPLC_DataDisplay(int lEQP_index, CString RcvStr);
	void SetSeq_DataDisplay(int lEQP_index, CString RcvStr);

	void CommandGetDFT_ModelList(int lIndexd);
	void CommandDFT_ModelChange(int lIndexd, CString strModelInfo);
	void CommandStart(int lIndexd);
	void CommandPause(int lIndexd);
	void CommandStop(int lIndexd);
	void CommandGetToolOption(int lIndexd, CString strSuffix);
	void CommandGetToolOptionClose(int lIndexd);


	void CommandJigUp(int lIndexd);
	void CommandJigDown(int lIndexd);
	void CommandLoad(int lIndexd);
	void CommandTestPass(int lIndexd);
	void CommandTestNG(int lIndexd);
	void CommandClear(int lIndexd);
	void CommandRead(int lIndexd);
	void CommandACPowerOn(int lIndexd);
	void CommandACPowerOff(int lIndexd);
	   	 
	void OpenComportSCAN();

	unsigned int m_ModelListCount;
	//CString  m_strModelList[200];
	int m_DFT_CommandCheck;

	void DisplayCountData();

	int m_SelectDFT;

public:
     afx_msg void OnBnClickedStart();
	 afx_msg long OnReceiveDataSCAN(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedStopButton();
//	CString m_ChnnelName[8];

	//CLabel_text m_BarCode;
	CLabel_text m_BarCodeMark;
	

	//CLabel_text m_FreqLabel[8];
	//CLabel_text m_VoltLabel[8];
	//CLabel_text m_NameLabel[8];



	CLabel_text m_MessageDisplay;

	//CLabel_text m_CheckTimeLabel;
	//CLabel_text m_CycleTimeLabel;
	//CLabel_text m_CheckTimeLabelName;
	//CLabel_text m_CycleTimeLabelName;

	CLabel_text m_OKCntLabel;
	CLabel_text m_NGCntLabel;
	CLabel_text m_OKCntLabelName;
	CLabel_text m_NGCntLabelName;//IDC_LABEL_OK_CNT_NAME
	
	afx_msg void OnBnClickedSetupButton();
	afx_msg void OnBnClickedTestButton();

	CBitmapButton m_cBtnStart;
	CBitmapButton m_cSetupButton;
	CBitmapButton m_cStopButton;
	CBitmapButton m_cTestButton;
	CBitmapButton m_cExitButton;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	DECLARE_EVENTSINK_MAP()
	//void MouseDownTextboxModel(short Button, short Shift, float X, float Y);
	//void MouseUpTextboxModel(short Button, short Shift, float X, float Y);

	CLabel_text m_cLbModel;
	CLabel_text m_cLbOkNg;
	CLabel_text m_cLb_Test1;
	CLabel_text m_cLb_Test2;
	CLabel_text m_cLb_Test3;
	//CLabel_text m_clbHz;

	CLabel_text m_cLb_Lan;
	CLabel_text m_cLb_Usb1;
	CLabel_text m_cLb_Usb2;

	CLabel_text m_LanLabelName;
	CLabel_text m_Usb1LabelName;
	CLabel_text m_Usb2LabelName;

//	CListCtrl m_CtrlListMainProcess;
//	CListCtrl m_CtrlListMainProcessScan;
	CLabel_text m_CameraLabelName;
	CLabel_text m_cLb_Camera;
//	afx_msg void OnHdnItemclickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMClickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMCustomdrawListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_pic;
	//afx_msg void OnStnDblclickStaticPic();
	//afx_msg void OnStnClickedStaticPic();
	DECLARE_EVENTSINK_MAP()
//	void ClickLabelCamreaName();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
//	void DblClickLabel(LPDISPATCH Cancel);
//	void DblClickLabelOK1(LPDISPATCH Cancel);
//	void DblClickLabelOK2(LPDISPATCH Cancel);
//	CComboBox m_cComboName[8];
	//afx_msg void OnCbnSelchangeComboName1();
	//CLabel_text m_cLb_ComMain;
	//CLabel_text m_cLb_ComSet;
	//CLabel_text m_cLb_ComLed;
	//CLabel_text m_cLb_ComMainGmes;
//	void DblClickLabelComGmes(LPDISPATCH Cancel);
//	void DblClickLabelComMain(LPDISPATCH Cancel);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//void ClickLabelOK1();
//	void ClickLabelOK2();
////	void DblClickLabelCheckTime(LPDISPATCH Cancel);
//	void DblClickLabelNgCnt(LPDISPATCH Cancel);
//	CLabel_text m_cLabelCableManager;
//	CLabel_text m_cLabelCableMessage;
	//void DblClickLabelManager(LPDISPATCH Cancel);
	void ClickLabelServer();

	CLabel_text m_cLabelResult;
//	CLabel_text m_Barcode;

	//afx_msg void OnBnClickedButtonExit();


	CLabel_text m_cLB_ModelName;//IDC_LABEL_MODEL_NAME_VALUE	
	CLabel_text m_cLB_Suffix;//IDC_LABEL_SUFFIX_VALUE
	CLabel_text m_cLB_BtnStartAll;//IDC_LABEL_BTN_START_ALL
	CLabel_text m_cLB_BtnPauseAll;//IDC_LABEL_BTN_PAUSE_ALL
	CLabel_text m_cLB_BtnStopAll;//IDC_LABEL_BTN_STOP_ALL

	CLabel_text m_cLB_BtnStartSelect;//IDC_LABEL_BTN_START_SELECT
	CLabel_text m_cLB_BtnPauseSelect;//IDC_LABEL_BTN_PAUSE_SELECT
	CLabel_text m_cLB_BtnStopSelect;//IDC_LABEL_BTN_STOP_SELECT



	CLabel_text m_cLB_BtnChangeModel;//IDC_LABEL_BTN_CHANGE_MODEL
	CLabel_text m_cLB_BtnScanOn;//IDC_LABEL_BTN_SCAN_ON
	CLabel_text m_cLB_BtnGetToolOption;//IDC_LABEL_BTN_GET_TOOL_OPTION
	CLabel_text m_cLB_BtnGetToolOptionClose;//IDC_LABEL_BTN_GET_TOOL_OPTION
	CLabel_text m_cLB_BtnSelectDFT;//IDC_LABEL_BTN_GET_TOOL_OPTION
	/*str_head[0] = "#Auto//";
	str_head[1] = "#Manual//";
	str_head[2] = "#Start//";
	str_head[3] = "#Pause//";
	str_head[4] = "#Stop//";
	str_head[5] = "#Warn//";
	str_head[6] = "#Alarm//";
	str_head[7] = "#SkipFFC//";
	str_head[8] = "#PCBIn//";
	str_head[9] = "#JigUp//";
	str_head[10] = "#JigDown//";*/
	CLabel_text m_cLB_PLC_Data_Auto;//IDC_LABEL_PLC_DATA_AUTO
	CLabel_text m_cLB_PLC_Data_Manual;//IDC_LABEL_PLC_DATA_MAN
	CLabel_text m_cLB_PLC_Data_Start;//IDC_LABEL_PLC_DATA_START
	CLabel_text m_cLB_PLC_Data_Pause;//IDC_LABEL_PLC_DATA_PAUSE
	CLabel_text m_cLB_PLC_Data_Stop;//IDC_LABEL_PLC_DATA_STOP
	CLabel_text m_cLB_PLC_Data_Warn;//IDC_LABEL_PLC_DATA_WARN
	CLabel_text m_cLB_PLC_Data_Alarm;//IDC_LABEL_PLC_DATA_ALARM
	CLabel_text m_cLB_PLC_Data_SkipFFC;//IDC_LABEL_PLC_DATA_SKIP_FFC
	CLabel_text m_cLB_PLC_Data_PcbSensor;//IDC_LABEL_PLC_DATA_PCB_SENSOR
	CLabel_text m_cLB_PLC_Data_JigUp;//IDC_LABEL_PLC_DATA_JIG_UP
	CLabel_text m_cLB_PLC_Data_JigDown;//IDC_LABEL_PLC_DATA_JIG_DOWN

	CLabel_text m_cLB_PLC_Ctrl_JigUp;
	CLabel_text m_cLB_PLC_Ctrl_JigDown;//
	CLabel_text m_cLB_PLC_Ctrl_Load;//
	CLabel_text m_cLB_PLC_Ctrl_TestPass;//
	CLabel_text m_cLB_PLC_Ctrl_TestNG;//
	CLabel_text m_cLB_PLC_Ctrl_Clear;//
	CLabel_text m_cLB_PLC_Ctrl_Read;//
	CLabel_text m_cLB_PLC_Ctrl_AC_ONOFF;//

//	CButton m_cBtnExit;
	void ClickLabelEquip1();
	void ClickLabelEquip2();
	void ClickLabelEquip3();
	void ClickLabelEquip4();
	void ClickLabelEquip5();
	void ClickLabelEquip6();


	CLabel_text m_cLbEquipSelect;
	CLabel_text m_cLbEquip[6];
	//CLabel_text m_cLb_ComMain;
	//CLabel_text m_cLb_ComMainGmes;
	//CLabel_text m_cLb_ComMainMonitor;
	afx_msg void OnNMCustomdrawListOqaMonitorSelect(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOqaMonitor6(NMHDR *pNMHDR, LRESULT *pResult);


	//void ClickLabelSelect1();	
	//void ClickLabelSelect2();	
	//void ClickLabelSelect3();	
	//void ClickLabelSelect4();	
	//void ClickLabelSelect5();	
	//void ClickLabelSelect6();	


	CLabel_text m_cLbServerLabel;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ClickLabelConfigSet();
	void ClickLabelExit();
	CLabel_text m_cLabelPCBASN[6];
	CLabel_text m_cLabelCtrlStatus[6];//IDC_LABEL_PLC_ROBOT_STATUS_DFT1
	CLabel_text m_cLabelPcbStatus[6];//IDC_LABEL_PCB_IN_STATUS_DFT1
	CLabel_text m_cLabelProcessStatus[6];//IDC_LABEL_PROCESS_STATUS_DFT1
	CLabel_text m_cLabelBtnStart[6];//IDC_LABEL_BTN_START_DFT1
	CLabel_text m_cLabelBtnPause[6];//IDC_LABEL_BTN_PAUSE_DFT1	
	CLabel_text m_cLabelBtnStop[6];//IDC_LABEL_BTN_STOP_DFT1

	void ClickLabelBtnStartAll();
	void ClickLabelBtnPauseAll();
	void ClickLabelBtnStopAll();

	void ClickLabelBtnStartDft1();
	void ClickLabelBtnStartDft2();
	void ClickLabelBtnStartDft3();
	void ClickLabelBtnStartDft4();
	void ClickLabelBtnStartDft5();
	void ClickLabelBtnStartDft6();

	void ClickLabelBtnPauseDft1();
	void ClickLabelBtnPauseDft2();
	void ClickLabelBtnPauseDft3();
	void ClickLabelBtnPauseDft4();
	void ClickLabelBtnPauseDft5();
	void ClickLabelBtnPauseDft6();

	void ClickLabelBtnStopDft1();
	void ClickLabelBtnStopDft2();
	void ClickLabelBtnStopDft3();
	void ClickLabelBtnStopDft4();
	void ClickLabelBtnStopDft5();
	void ClickLabelBtnStopDft6();


	void ClickLabelBtnChangeModel();
	void ClickLabelBtnScanOn();
	void ClickLabelBtnGetToolOption();
	void ClickLabelBtnStartSelect();
	void ClickLabelBtnPauseSelect();
	void ClickLabelBtnStopSelect();

	CListCtrl m_cListCtrl_OQA_Monitor[6];
	CListCtrl m_cListCtrl_OQA_Monitor_Select;
	//CListCtrl m_cListCtrlMainLog;
	CListBox m_cListMainProcessLog;
	void ClickLabelBtnGetToolClose();
	CLabel_text m_cLabelComScan;
	CLabel_text m_cLabelCountDisplay;
	void ClickLabelCountSet();
	void ClickLabelCountReset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ClickLabelBtnCtrlJigup();
	void ClickLabelBtnCtrlJigDown();
	void ClickLabelBtnCtrlLoad();
	void ClickLabelBtnCtrlTestPass();
	void ClickLabelBtnCtrlTestNg();
	void ClickLabelBtnCtrlClear();
	void ClickLabelBtnCtrlRead();
	void ClickLabelBtnCtrlAcOnOff();
	void ClickLabelBtnSelectDft();
	void ClickLabelComMonitorScan();
};
