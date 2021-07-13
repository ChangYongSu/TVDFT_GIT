
// stdafx.cpp : source file that includes just the standard includes
// AcqVoltageSamples_IntClk.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool gbManualScopeMode = false;
int g_ManualId[8] = {4,5,6,7,8,9,12,13};


CString g_nRemoteIPAddress[6];
//UINT g_nNetPortNumber[12] = {8010, 8020, 8030, 8040, 8050, 8060, 8070, 8080, 8090, 8100};
//UINT g_nRemotePortNumber[12] = {8010, 8020, 8030, 8040, 8050, 8060, 8070, 8080, 8090, 8100};
UINT g_nReplyTimeOut = 10;
UINT g_nTransTimeOut = 10;
UINT g_nLocalPortNumber[6] = { 8001,8002,8003,8004,8005,8006 };
//UINT g_nPLCServePortNumber = 2020;
CString		g_strPLCIPAddress = _T("192.168.0.2");		// GMES Host IP Add
UINT		g_nPLCPortNumber = 2020;		// GMES Host Port


CString g_strProcID;

CString g_strWOID;
CString g_strORGID;
CString g_strLineID;
CString g_strSetID;
CString g_strModelID;
CString g_strModelName;
CString g_strModelSuffix;



int g_nTargetCnt = 10000;
int g_nOKCnt = 0;
int g_nNGCnt = 0;



DB_Set_DataT CurrentSet;

CSerialPort m_ComPortScanner;

UINT g_nPort_Scanner;
UINT g_nBaud_Scanner;
UINT g_nParity_Scanner;


//CString g_strModelName;
//int g_nModelSelNum;
//CString g_strRemoconFileName;
int g_nRemoconSelNum;//
//TestModelDataT	gTestSeqData;
TestSeqDataT	gTestSeqData;


SaveLogDataT gSaveInfo[12];

SaveLogDataT gLoadDataInfo[12];
unsigned int gDataCount = 0;


//cv::VideoCapture vcap; 
//cv::Mat gCuptureimage;
bool gbShutdown;
bool gbCapture = TRUE;

int g_CameraFound = 0;
int g_CameraCount = 0;

CableManagerT gCableManager;

CTypedPtrList<CPtrList, CModelData*>	ModelList;


CString strAlarmList[500] =
{
"NO ERROR														"
,"8010 : <MAIN>PLC Diagnostic   ERROR[SM0]						"
,"8011 : <MAIN>PLC Self - diagnosis   ERROR[SM1]				"
,"8012 : <MAIN>PLC Battery low  ERROR[SM52]						"
,"8013 : <MAIN>PLC AC / DC DOWN detection  ERROR[SM53]			"
,"8014 : <MAIN>PLC Operation   ERROR[SM56]						"
,"8015 : <MAIN>PLC Blown fuse detection  ERROR[SM60]			"
,"8016 : <MAIN>PLC I / O module verification   ERROR[SM61]		"
,"8017 : <MAIN>  ERROR[]										"
,"8018 : <MAIN>  ERROR[]										"
,"8019 : <MAIN>  ERROR[]										"
,"8020 : <MAIN>MAIN E - STOP  ERROR[X0000]						"
,"8021 : <MAIN>ROBOT E - STOP  ERROR[X000F]						"
,"8022 : <MAIN>  ERROR[]										"
,"8023 : <MAIN>  ERROR[]										"
,"8024 : <MAIN>  ERROR[]										"
,"8025 : <MAIN>  ERROR[]										"
,"8026 : <MAIN>  ERROR[]										"
,"8027 : <MAIN>  ERROR[]										"
,"8028 : <MAIN>  ERROR[]										"
,"8029 : <MAIN>  ERROR[]										"
,"8030 : <MAIN>AIR pressure  ERROR[X0600]						"
,"8031 : <MAIN>  ERROR[]										"
,"8032 : <MAIN>  ERROR[]										"
,"8033 : <MAIN>  ERROR[]										"
,"8034 : <MAIN>  ERROR[]										"
,"8035 : <MAIN>  ERROR[]										"
,"8036 : <MAIN>  ERROR[]										"
,"8037 : <MAIN>  ERROR[]										"
,"8038 : <MAIN>  ERROR[]										"
,"8039 : <MAIN>  ERROR[]										"
,"8040 : <MAIN>DFT#1 UP SENSER  ERROR[X0601]					"
,"8041 : <MAIN>  ERROR[]										"
,"8042 : <MAIN>  ERROR[]										"
,"8043 : <MAIN>  ERROR[]										"
,"8044 : <MAIN>  ERROR[]										"
,"8045 : <MAIN>  ERROR[]										"
,"8046 : <MAIN>  ERROR[]										"
,"8047 : <MAIN>  ERROR[]										"
,"8048 : <MAIN>  ERROR[]										"
,"8049 : <MAIN>  ERROR[]										"
,"8050 : <MAIN>DFT#2 UP SENSER  ERROR[X0604]					"
,"8051 : <MAIN>  ERROR[]										"
,"8052 : <MAIN>  ERROR[]										"
,"8053 : <MAIN>  ERROR[]										"
,"8054 : <MAIN>  ERROR[]										"
,"8055 : <MAIN>  ERROR[]										"
,"8056 : <MAIN>  ERROR[]										"
,"8057 : <MAIN>  ERROR[]										"
,"8058 : <MAIN>  ERROR[]										"
,"8059 : <MAIN>  ERROR[]										"
,"8060 : <MAIN>DFT#3 UP SENSER  ERROR[X0640]					"
,"8061 : <MAIN>  ERROR[]										"
,"8062 : <MAIN>  ERROR[]										"
,"8063 : <MAIN>  ERROR[]										"
,"8064 : <MAIN>  ERROR[]										"
,"8065 : <MAIN>  ERROR[]										"
,"8066 : <MAIN>  ERROR[]										"
,"8067 : <MAIN>  ERROR[]										"
,"8068 : <MAIN>  ERROR[]										"
,"8069 : <MAIN>  ERROR[]										"
,"8070 : <MAIN>DFT#4 UP SENSER  ERROR[X0643]					"
,"8071 : <MAIN>  ERROR[]										"
,"8072 : <MAIN>  ERROR[]										"
,"8073 : <MAIN>  ERROR[]										"
,"8074 : <MAIN>  ERROR[]										"
,"8075 : <MAIN>  ERROR[]										"
,"8076 : <MAIN>  ERROR[]										"
,"8077 : <MAIN>  ERROR[]										"
,"8078 : <MAIN>  ERROR[]										"
,"8079 : <MAIN>  ERROR[]										"
,"8080 : <MAIN>DFT#5 UP SENSER  ERROR[X0660]					"
,"8081 : <MAIN>  ERROR[]										"
,"8082 : <MAIN>  ERROR[]										"
,"8083 : <MAIN>  ERROR[]										"
,"8084 : <MAIN>  ERROR[]										"
,"8085 : <MAIN>  ERROR[]										"
,"8086 : <MAIN>  ERROR[]										"
,"8087 : <MAIN>  ERROR[]										"
,"8088 : <MAIN>  ERROR[]										"
,"8089 : <MAIN>  ERROR[]										"
,"8090 : <MAIN>DFT#6 UP SENSER  ERROR[X0663]					"
,"8091 : <MAIN>  ERROR[]										"
,"8092 : <MAIN>  ERROR[]										"
,"8093 : <MAIN>  ERROR[]										"
,"8094 : <MAIN>  ERROR[]										"
,"8095 : <MAIN>  ERROR[]										"
,"8096 : <MAIN>  ERROR[]										"
,"8097 : <MAIN>  ERROR[]										"
,"8098 : <MAIN>  ERROR[]										"
,"8099 : <MAIN>  ERROR[]										"
,"8100 : <MAIN> OUT C / V UP SENSER  ERROR[]					"
,"8101 : <MAIN>OUT C / V DN  SENSER  ERROR[]					"
,"8102 : <MAIN>  ERROR[]										"
,"8103 : <MAIN>  ERROR[]										"
,"8104 : <MAIN>  ERROR[]										"
,"8105 : <MAIN>  ERROR[]										"
,"8106 : <MAIN>  ERROR[]										"
,"8107 : <MAIN>  ERROR[]										"
,"8108 : <MAIN>  ERROR[]										"
,"M8109 : <MAIN>  ERROR[]										"
,"M8110 : <MAIN>  ERROR[]										"
,"M8111 : <MAIN>  ERROR[]										"
,"M8112 : <MAIN>  ERROR[]										"
,"M8113 : <MAIN>  ERROR[]										"
,"M8114 : <MAIN>  ERROR[]										"
,"M8115 : <MAIN>  ERROR[]										"
,"M8116 : <MAIN>  ERROR[]										"
,"M8117 : <MAIN>  ERROR[]										"
,"M8118 : <MAIN>  ERROR[]										"
,"M8119 : <MAIN>  ERROR[]										"
,"M8120 : <MAIN>  ERROR[]										"
,"M8121 : <MAIN>  ERROR[]										"
,"M8122 : <MAIN>  ERROR[]										"
,"M8123 : <MAIN>  ERROR[]										"
,"M8124 : <MAIN>  ERROR[]										"
,"M8125 : <MAIN>  ERROR[]										"
,"M8126 : <MAIN>  ERROR[]										"
,"M8127 : <MAIN>  ERROR[]										"
,"M8128 : <MAIN>  ERROR[]										"
,"M8129 : <MAIN>  ERROR[]										"
,"M8130 : <MAIN>  ERROR[]										"
,"M8131 : <MAIN>  ERROR[]										"
,"M8132 : <MAIN>  ERROR[]										"
,"M8133 : <MAIN>  ERROR[]										"
,"M8134 : <MAIN>  ERROR[]										"
,"M8135 : <MAIN>  ERROR[]										"
,"M8136 : <MAIN>  ERROR[]										"
,"M8137 : <MAIN>  ERROR[]										"
,"M8138 : <MAIN>  ERROR[]										"
,"M8139 : <MAIN>  ERROR[]										"
,"M8140 : <MAIN>  ERROR[]										"
,"M8141 : <MAIN>  ERROR[]										"
,"M8142 : <MAIN>  ERROR[]										"
,"M8143 : <MAIN>  ERROR[]										"
,"M8144 : <MAIN>  ERROR[]										"
,"M8145 : <MAIN>  ERROR[]										"
,"M8146 : <MAIN>  ERROR[]										"
,"M8147 : <MAIN>  ERROR[]										"
,"M8148 : <MAIN>  ERROR[]										"
,"M8149 : <MAIN>  ERROR[]										"
,"M8150 : <RB1>SYSTEM ERROR  ERROR[None]						"
,"M8151 : <RB1>Error Code No : 1 JOG OPERATION  ERROR[None]		"
,"M8152 : <RB1>Error Code No : 2 ROBOT HOME   E  ERROR[None]	"
,"M8153 : <RB1>Error Code No : 3 COMMAND CODE E  ERROR[None]	"
,"M8154 : <RB1>Error Code No : 4 HAND C - VC ON  ERROR[None]	"
,"M8155 : <RB1>Error Code No : 5 HAND C - VC OFF  ERROR[None]	"
,"M8156 : <RB1>Error Code No : 6 SPEED DATA ERR  ERROR[None]	"
,"M8157 : <RB1>Error Code No : 7 GET / PUT OFF ER  ERROR[None]	"
,"M8158 : <RB1>Error Code No : 8 HAND CYLINDER  ERROR[None]		"
,"M8159 : <RB1>Error Code No : 9 HAND CYLINDER  ERROR[None]		"
,"M8160 : <RB1>Error Code No : 10 HAND BORD VC  ERROR[None]		"
,"M8161 : <RB1>Error Code No : 11 HAND BORD VC  ERROR[None]		"
,"M8162 : <RB1>Error Code No : 12 CUTTING DOWN  ERROR[None]		"
,"M8163 : <RB1>Error Code No : 13 CUTTING UP SI  ERROR[None]	"
,"M8164 : <RB1>Error Code No : 14 BORD NONE LOA  ERROR[None]	"
,"M8165 : <RB1>Error Code No : 15 BORD HAVE UNL  ERROR[None]	"
,"M8166 : <RB1>Error Code No : 16 BUFFER CONCUR  ERROR[None]	"
,"M8167 : <RB1>Error Code No : 17 FFC JIG VAC O  ERROR[None]	"
,"M8168 : <RB1>Error Code No : 18 FFC LOADERROR  ERROR[None]	"
,"M8169 : <RB1>Error Code No : 19 FFC UNLOADERR  ERROR[None]	"
,"M8170 : <RB1>Error Code No : 20 HAND FFC CY F  ERROR[None]	"
,"M8171 : <RB1>Error Code No : 21 HAND FFC CY B  ERROR[None]	"
,"M8172 : <RB1>Error Code No : 22 PCBA FFC NOT  ERROR[None]		"
,"M8173 : <RB1>Error Code No : 23  DUMMY CUT FAIL   ERROR[None]	"
,"M8174 : <RB1>ERROR  ERROR[None]								"
,"M8175 : <RB1>ERROR  ERROR[None]								"
,"M8176 : <RB1>ERROR  ERROR[None]								"
,"M8177 : <RB1>ERROR  ERROR[None]								"
,"M8178 : <RB1>ERROR  ERROR[None]								"
,"M8179 : <RB1>ERROR  ERROR[None]								"
,"M8180 : <RB1>ERROR  ERROR[None]								"
,"M8181 : <RB1>ERROR  ERROR[None]								"
,"M8182 : <RB1>  ERROR[None]									"
,"M8183 : <RB1>  ERROR[None]									"
,"M8184 : <RB1>  ERROR[None]									"
,"M8185 : <RB1>  ERROR[None]									"
,"M8186 : <RB1>  ERROR[None]									"
,"M8187 : <RB1>  ERROR[None]									"
,"M8188 : <RB1>  ERROR[None]									"
,"M8189 : <RB1>  ERROR[None]									"
,"M8190 : <RB2>SYSTEM ERROR  ERROR[None]						"
,"M8191 : <RB2>Error Code No : 1 JOG OPERATION  ERROR[None]		"
,"M8192 : <RB2>Error Code No : 2 ROBOT HOME   E  ERROR[None]	"
,"M8193 : <RB2>Error Code No : 3 COMMAND CODE E  ERROR[None]	"
,"M8194 : <RB2>Error Code No : 4 HAND C - VC ON  ERROR[None]	"
,"M8195 : <RB2>Error Code No : 5 HAND C - VC OFF  ERROR[None]	"
,"M8196 : <RB2>Error Code No : 6 SPEED DATA ERR  ERROR[None]	"
,"M8197 : <RB2>Error Code No : 7 GET / PUT OFF ER  ERROR[None]	"
,"M8198 : <RB2>Error Code No : 8 HAND CYLINDER  ERROR[None]		"
,"M8199 : <RB2>Error Code No : 9 HAND CYLINDER  ERROR[None]		"
,"M8200 : <RB2>Error Code No : 10 HAND BORD VC  ERROR[None]		"
,"M8201 : <RB2>Error Code No : 11 HAND BORD VC  ERROR[None]		"
,"M8202 : <RB2>Error Code No : 12 CUTTING DOWN  ERROR[None]		"
,"M8203 : <RB2>Error Code No : 13 CUTTING UP SI  ERROR[None]	"
,"M8204 : <RB2>Error Code No : 14 BORD NONE LOA  ERROR[None]	"
,"M8205 : <RB2>Error Code No : 15 BORD HAVE UNL  ERROR[None]	"
,"M8206 : <RB2>Error Code No : 16 BUFFER CONCUR  ERROR[None]	"
,"M8207 : <RB2>Error Code No : 17 FFC JIG VAC O  ERROR[None]	"
,"M8208 : <RB2>Error Code No : 18 FFC LOADERROR  ERROR[None]	"
,"M8209 : <RB2>Error Code No : 19 FFC UNLOADERR  ERROR[None]	"
,"M8210 : <RB2>Error Code No : 20 HAND FFC CY F  ERROR[None]	"
,"M8211 : <RB2>Error Code No : 21 HAND FFC CY B  ERROR[None]	"
,"M8212 : <RB2>Error Code No : 22 PCBA FFC NOT  ERROR[None]		"
,"M8213 : <RB2>Error Code No : 23  DUMMY CUT FAIL   ERROR[None]	"
,"M8214 : <RB2>  ERROR[None]									"
,"M8215 : <RB2>  ERROR[None]									"
,"M8216 : <RB2>  ERROR[None]									"
,"M8217 : <RB2>  ERROR[None]									"
,"M8218 : <RB2>  ERROR[None]									"
,"M8219 : <RB2>  ERROR[None]									"
,"M8220 : <RB2>  ERROR[None]									"
,"M8221 : <RB2>  ERROR[None]									"
,"M8222 : <RB2>  ERROR[None]									"
,"M8223 : <RB2>  ERROR[None]									"
,"M8224 : <RB2>  ERROR[None]									"
,"M8225 : <RB2>  ERROR[None]									"
,"M8226 : <RB2>  ERROR[None]									"
,"M8227 : <RB2>  ERROR[None]									"
,"M8228 : <RB2>  ERROR[None]									"
,"M8229 : <RB2>  ERROR[None]									"
,"M8230 : <RB2>  ERROR[]										"
,"M8231 : <RB2>  ERROR[]										"
,"M8232 : <RB2>  ERROR[]										"
,"M8233 : <RB2>  ERROR[]										"
,"M8234 : <RB2>  ERROR[]										"
,"M8235 : <RB2>  ERROR[]										"
,"M8236 : <RB2>  ERROR[]										"
,"M8237 : <RB2>  ERROR[]										"
,"M8238 : <RB2>  ERROR[]										"
,"M8239 : <RB2>  ERROR[]										"
,"M8240 : <RB2>  ERROR[]										"
,"M8241 : <RB2>  ERROR[]										"
,"M8242 : <RB2>  ERROR[]										"
,"M8243 : <RB2>  ERROR[]										"
,"M8244 : <RB2>  ERROR[]										"
,"M8245 : <RB2>  ERROR[]										"
,"M8246 : <RB2>  ERROR[]										"
,"M8247 : ERROR[]												"
,"M8248 : ERROR[]												"
,"M8249 : ERROR[]												"
,"M8250 : DFT#1 Product Detection / DATA    ERROR[]				"
,"M8251 : DFT#1 DATA / Product Detection    ERROR[]				"
,"M8252 : DFT#2 Product Detection / DATA    ERROR[]				"
,"M8253 : DFT#2 DATA / Product Detection    ERROR[]				"
,"M8254 : DFT#3 Product Detection / DATA   ERROR[]				"
,"M8255 : DFT#3 DATA / Product Detection  ERROR[]				"
,"M8256 : DFT#4 Product Detection / DATA   ERROR[]				"
,"M8257 : DFT#4 DATA / Product Detection   ERROR[]				"
,"M8258 : DFT#5 Product Detection / DATA   ERROR[]				"
,"M8259 : DFT#5 DATA / Product Detection   ERROR[]				"
,"M8260 : DFT#6 Product Detection / DATA   ERROR[]				"
,"M8261 : DFT#6 DATA / Product Detection    ERROR[]				"
,"M8262 :   ERROR[]												"
,"M8263 : ERROR[]												"
,"M8264 : ERROR[]												"
,"M8265 : ERROR[]												"
,"M8266 : ERROR[]												"
,"M8267 : ERROR[]												"
,"M8268 : ERROR[]												"
,"M8269 : ERROR[]												",
};

