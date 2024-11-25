// DlgModelSetup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "FileControl.h"
#include <direct.h>




CFileControl::CFileControl()	
{
//	m_AlarmCount = 0;
}

CFileControl::~CFileControl()
{
}


int CFileControl::OpenModel(CString lFilename)
{
	int lFileError = 0;
	
	

	//FILE *m_fp;
	//CString str;
	//CString str1;
	//CString str2 ;
	//int lpos= 0; 
	//CString Filename;
	//CString SeqFilename;
	//char lbuf[1024];
	////char lTemp_Company[64];
	/////char lTemp_Code[64];
	////char lTemp_Repeat[64];
	/////char lTemp_Name[64];
	////char lTemp_Discription[129];Filename
	//int l_nDataReadStep  = 0;
	//int lnProcessSeqNum = 0;
//	int l_nEnable, l_nPort, l_nSel_Input,lMinFreq, lMaxFreq,lMinVolt,lMaxVolt;
	
//	UINT lDefaultFreq[10] = {100, 1000, 600, 800, 400, 1000, 400, 1000, 400, 1000};
//	for(int i = 0; i < 10; i++)
//	{
//		//if((gTestSeqData.Frequency[i] < 100)||(gTestSeqData.Frequency[i] > 5000))
//		//{
//			gTestSeqData.Frequency[i] = lDefaultFreq[i];
////		}
//	}
//
//	 g_nRemotePortType = 0;
//
//
//	Filename = PATH_BASE_DIR;
//	Filename += _T("\\Model\\");
//	Filename += lFilename;
//	
//	Filename +=  _T(".mdl");
//
//	SeqFilename = lFilename;
//	SeqFilename +=  _T(".seq");
//
//	gTestSeqData.strSeqFileName = SeqFilename;
//
//	fopen_s(&m_fp, (LPCSTR)Filename, "r");
//	if(m_fp)
//	{
//
//
//		while(fgets(lbuf, 1024, m_fp) != NULL) 
//		{
//			for(int i = 0; i < strlen(lbuf); i++)
//			{
//				if((lbuf[i] == '\r')||(lbuf[i] == '\n'))
//				{
//					lbuf[i] = 0;
//					break;
//				}
//			}
//
//#ifdef _CHANGE_MODEL_DEBUG__
//		AfxMessageBox(lbuf);
//#endif
//			if(lbuf[0] == '[')
//			{
//				
//			
//				if(strlen(lbuf) < 5)
//				{
//					//MessageBox("Test Model File Read Error !!");
//					lFileError = 1;
//					break;
//				}
//				if(strncmp(&lbuf[1], "PROCESS", 7) == 0)
//				{
//					l_nDataReadStep = FILE_STEP_PROCESS;
//					lnProcessSeqNum = 0;
//				}
//				
//				else if(strncmp(&lbuf[1], "REMOCON", 7) == 0)
//				{
//					l_nDataReadStep = FILE_STEP_REMOCON;
//				}
//				else if(strncmp(&lbuf[1], "MES", 3) == 0)
//				{
//					l_nDataReadStep = FILE_STEP_MES;
//				}			
//				else if(strncmp(&lbuf[1], "CAMERA", 6) == 0)
//				{
//					l_nDataReadStep = FILE_STEP_CAMERA;
//				}
//				continue;
//			}
//
//			switch(l_nDataReadStep)
//			{
//			case FILE_STEP_PROCESS:
//			
//				if(strncmp(lbuf, "FILE=", 5) == 0)
//				{
//					//gTestSeqData.strSeqFileName.Format(_T("%s"), &lbuf[5]);
//					gTestSeqData.strSeqFileName = SeqFilename;
//				}			
//				else if(strncmp(lbuf, "CHECK SUM=", strlen("CHECK SUM=")) == 0)
//				{
//					
//					str1 = &lbuf[strlen("CHECK SUM=")];
//					str2 = str1;
//
//					str2.MakeUpper();
//			
//					lpos = str2.Find("0X");
//					if(lpos  >= 0)
//					{						
//						gTestSeqData.CheckSum[0] = ReadHex(str2);
//						str2 = str2.Mid(lpos+4);					
//					}	
//					lpos = str2.Find("0X");
//					if(lpos  >= 0)
//					{						
//						gTestSeqData.CheckSum[1] = ReadHex(str2);
//						str2 = str2.Mid(lpos+4);					
//					}
//						
//						
//				}
//				else if(strncmp(lbuf, "FREQUENCY=", strlen("FREQUENCY=")) == 0)
//				{
//				
//					//str1 = &lbuf[strlen("FREQUENCY=")];
//					//str2 = str1;
//					sscanf_s(&lbuf[strlen("FREQUENCY=")], "%d %d %d %d %d %d %d %d %d %d", 
//						&gTestSeqData.Frequency[0],&gTestSeqData.Frequency[1],&gTestSeqData.Frequency[2],&gTestSeqData.Frequency[3],
//						&gTestSeqData.Frequency[4],&gTestSeqData.Frequency[5],&gTestSeqData.Frequency[6],&gTestSeqData.Frequency[7],
//						&gTestSeqData.Frequency[8],&gTestSeqData.Frequency[9]);	
//						
//				
//				}	
//
//				break;	
//
//			case FILE_STEP_REMOCON:
//				if(strncmp(lbuf, "TYPE=", 5) == 0)
//				{
//					if(strncmp(&lbuf[5], "CA", 2) == 0)
//					{
//						gTestSeqData.nRemoconSigType = IR_REMOTE_CA;
//					}
//					else
//					{
//						gTestSeqData.nRemoconSigType = IR_REMOTE_RF;
//					}
//				}
//				else if(strncmp(lbuf, "FILE=", 5) == 0)
//				{
//					gTestSeqData.strRemoconFileName.Format(_T("%s"), &lbuf[5]);
//
//				}
//				break;
//
//			case FILE_STEP_MES:
//				if(strncmp(lbuf, "ENABLE=1", 8) == 0)
//				{
//					gTestSeqData.bMesEnable = 1;
//				}
//				else if(strncmp(lbuf, "ENABLE=0", 8) == 0)
//				{
//					gTestSeqData.bMesEnable = 0;
//				}
//				//else if(strncmp(lbuf, "IP=", strlen("IP=")) == 0)
//				//{
//				//	lbuf[strlen(lbuf)] = 0;
//
//				//	g_nRemoteIPAddress = &lbuf[strlen("IP=")];
//				//}
//				//else if(strncmp(lbuf, "PORT NUMBER=", strlen("PORT NUMBER=")) == 0)
//				//{
//				//	lbuf[strlen(lbuf)] = 0;
//				//	g_nRemotePortNumber = atoi(&lbuf[strlen("PORT NUMBER=")]);
//				//}
//				else if(strncmp(lbuf, "PORT TYPE=", strlen("PORT TYPE=")) == 0)//	sprintf(lbuf,"PORT TYPE=%d\n",g_nRemotePortType);//
//				{
//					lbuf[strlen(lbuf)] = 0;
//					g_nRemotePortType = atoi(&lbuf[strlen("PORT TYPE=")]);
//
//					
//				}
//				else if(strncmp(lbuf, "LOCAL NUMBER=", strlen("LOCAL NUMBER=")) == 0)
//				{
//					lbuf[strlen(lbuf)] = 0;
//					g_nLocalPortNumber = atoi(&lbuf[strlen("LOCAL NUMBER=")]);
//				}
//				
//				
//				break;
//
//
//		
//
//		case FILE_STEP_CAMERA:
//				if(strncmp(lbuf, "IP=", 3) == 0)
//				{
//					gTestSeqData.strCameraIPAddress.Format(_T("%s"), &lbuf[3]);
//				}
//				
//				break;
//
//
//			default:
//				break;
//			}				
//		}
//		
//		gTestSeqData.strModelName = lFilename;
//
//		fclose(m_fp);	
//		//SetDlgItemText(IDC_STATIC_RMT_FILE, Filename);
//
//	} 
//	else
//	{
//		lFileError = 5;
//	}
//
//	//UINT lDefaultFreq[10] = {100, 1000, 600, 800, 400, 1000, 400, 1000, 400, 1000};
//	if(g_nRemotePortType == 1)
//		g_nRemotePortNumber = g_nRemotePortNumberCM89;
//	else
//		g_nRemotePortNumber = g_nRemotePortNumberCM45;
//
//
//	for(int i = 0; i < 10; i++)
//	{
//		if((gTestSeqData.Frequency[i] < 20)||(gTestSeqData.Frequency[i] > 5000))
//		{
//			gTestSeqData.Frequency[i] = lDefaultFreq[i];
//		}
//	}

	return lFileError;
}

int CFileControl::OpenModelSeq(CString lFilename)
{
	int lFileError = 0;
	int  lpos;
	
	gTestSeqData.TestCnt = 0;

	FILE *m_fp;
	CString str;
	CString strTemp;
	CString Filename;
	char lbuf[1024];
	//char lTemp_Company[64];
	///char lTemp_Code[64];
	//char lTemp_Repeat[64];
	///char lTemp_Name[64];
	//char lTemp_Discription[129];Filename
	int l_nDataReadStep  = 0;
	int lnProcessSeqNum = 0;
	int l_nEnable, l_nPort, l_nSel_Input,lMinFreq, lMaxFreq,lMinVolt,lMaxVolt;
	
	int l_nStepCount  = 0;
	int l_nPreStepCount  = 0;
	int lFoundContents = 0;
	int lStart = 0;
	int l_nFunctionCount = 0;


	Filename = PATH_BASE_DIR;
	Filename += _T("\\Model\\");
	Filename += lFilename;

	//	int lpos = gTestSeqData.strSeqFileName.Find(".seq");
	//if(gTestSeqData.strModelName.Compare(gTestSeqData.strSeqFileName.Left(lpos)) != 0)
	//{
	//	CopyFile(
	//	//CFile LCtrfile(
	//	//CFile::Rename(pOldName, pNewName);
	//	//	CFile::Duplicate();
	//}
//	Filename +=  _T(".seq");

	gTestSeqData.TestCnt = 0;
	gTestSeqData.PreTestCnt = 0;

	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if(m_fp)
	{
		while(fgets(lbuf, 1024, m_fp) != NULL) 
		{
			lStart = 0;
			for(int i = 0; i < strlen(lbuf); i++)
			{
				if((lbuf[i] != ' ')&&(lbuf[i] != '\t'))
				{
					lStart = i;
					break;
				}
			}

			for(int i = 0; i < strlen(lbuf); i++)
			{
				if((lbuf[i] == '\r')||(lbuf[i] == '\n'))
				{
					lbuf[i] = 0;
					break;
				}
			}

			strTemp = _T(&lbuf[lStart]);
			lpos = strTemp.Find("//");
			if(lpos >= 0)
			{
				strTemp = strTemp.Left(lpos);
			}
			if(strTemp.GetLength() == 0)
				continue;
			//gTestSeqData

			lFoundContents = 0;
			l_nFunctionCount = 0;
			lpos = strTemp.Find("prestep");

			if(lpos >= 0)
			{
					
				gTestSeqData.PreStepList[l_nPreStepCount].strStepName = FindStepName(strTemp);
				gTestSeqData.PreStepList[l_nPreStepCount].strStepWork = FindStepWork(strTemp);
				
				//LOAD Step Sequence
				while(fgets(lbuf, 1024, m_fp) != NULL) 
				{
					lStart = 0;
					for(int i = 0; i < strlen(lbuf); i++)
					{
						if((lbuf[i] != ' ')&&(lbuf[i] != '\t'))
						{
							lStart = i;
							break;
						}
					}
					for(int i = 0; i < strlen(lbuf); i++)
					{
						if((lbuf[i] == '\r')||(lbuf[i] == '\n'))
						{
							lbuf[i] = 0;
							break;
						}
					}

					strTemp = _T(&lbuf[lStart]);

					lpos = strTemp.Find("//");
					if(lpos >= 0)
					{
						strTemp = strTemp.Left(lpos);
					}

					int len = strTemp.GetLength();
					char lchar =  strTemp.GetAt(0);
					if(len < 1)
						continue;


					if(lFoundContents == 0)
					{						
						lpos = strTemp.Find("{");
						if(lpos == 0)
						{
							lFoundContents = 1;
							//break;
						}
						else
						{
							lFileError = 1;//MessageBox("");
							
						}						
					}
					else if(lFoundContents == 1)
					{
						lpos = strTemp.Find("}");
						if(lpos == 0)
						{
							lFoundContents = 2;
							gTestSeqData.PreStepList[l_nPreStepCount].nCount =  l_nFunctionCount;
							break;
						}
						else
						{
							gTestSeqData.PreStepList[l_nPreStepCount].FunctionCommand[l_nFunctionCount] =  strTemp;
							l_nFunctionCount++;	
							if(l_nFunctionCount >= 100)
							{
								lFileError = 2;
								break;
							}
						}					
					}
					

					if(lFileError)
						break;
				}

				if(lFileError)
					break;
					
				l_nPreStepCount++;
				gTestSeqData.PreTestCnt = l_nPreStepCount;
			}
			else
			{			
				lFoundContents = 0;
				l_nFunctionCount = 0;
				lpos = strTemp.Find("step");

				if(lpos >= 0)
				{
					gTestSeqData.TestStepList[l_nStepCount].strStepName = FindStepName(strTemp);
					gTestSeqData.TestStepList[l_nStepCount].strStepWork = FindStepWork(strTemp);
				
					//LOAD Step Sequence
					while(fgets(lbuf, 1024, m_fp) != NULL) 
					{
						lStart = 0;
						for(int i = 0; i < strlen(lbuf); i++)
						{
							if((lbuf[i] != ' ')&&(lbuf[i] != '\t'))
							{
								lStart = i;
								break;
							}
						}
						for(int i = 0; i < strlen(lbuf); i++)
						{
							if((lbuf[i] == '\r')||(lbuf[i] == '\n'))
							{
								lbuf[i] = 0;
								break;
							}
						}

						strTemp = _T(&lbuf[lStart]);

						lpos = strTemp.Find("//");
						if(lpos >= 0)
						{
							strTemp = strTemp.Left(lpos);
						}

						int len = strTemp.GetLength();
						char lchar =  strTemp.GetAt(0);
						if(len < 1)
							continue;


						if(lFoundContents == 0)
						{						
							lpos = strTemp.Find("{");
							if(lpos == 0)
							{
								lFoundContents = 1;
								//break;
							}
							else
							{
								lFileError = 1;//MessageBox("");
							
							}						
						}
						else if(lFoundContents == 1)
						{
							lpos = strTemp.Find("}");
							if(lpos == 0)
							{
								lFoundContents = 2;
								gTestSeqData.TestStepList[l_nStepCount].nCount =  l_nFunctionCount;
								break;
							}
							else
							{
								gTestSeqData.TestStepList[l_nStepCount].FunctionCommand[l_nFunctionCount] =  strTemp;
								l_nFunctionCount++;	
								if(l_nFunctionCount >= 100)
								{

									lFileError = 2;
									break;
								}
							}					
						}					

						if(lFileError)
							break;
					}

					if(lFileError)
						break;

					l_nStepCount++;
					gTestSeqData.TestCnt = l_nStepCount;
				}				
			}
		}
		fclose(m_fp);	
		//SetDlgItemText(IDC_STATIC_RMT_FILE, Filename);

	} 
	else
	{
		lFileError = 5;
	}
	return lFileError;
}

CString CFileControl::FindStepName(CString StringLine)
{
	int strtPos = StringLine.Find("\"");
	CString strTEmp = StringLine.Mid(strtPos+1);
	int endPos = strTEmp.Find("\"");
	strTEmp = strTEmp.Left(endPos);

	return strTEmp;
}


CString CFileControl::FindStepWork(CString StringLine)
{
//step(8, "Portable Front_Test"):check("DTV", audio_test, TRUE, , ,"")
	int strtPos = StringLine.Find(":");
	CString strTEmp = StringLine.Mid(strtPos+1);
	//int endPos = strTEmp.Find("\"");
	//strTEmp = strTEmp.Left(endPos);

	return strTEmp;


}


int CFileControl::RemoveModel(CString lFilename)//gTestSeqData
{
	int lFileError = 0;
	//FILE *m_fp;
	CString str;
	CString Filename;
	//char lbuf[1024];
	
	CFile l_FILE;
	
	Filename = PATH_BASE_DIR;
	Filename += _T("\\Model\\");
	Filename += lFilename;
	Filename +=  _T(".mdl");

	l_FILE.Remove(Filename);

		
	return 0;
}
		

int CFileControl::SaveModel(CString lFilename)//gTestSeqData
{
	int lFileError = 0;
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];
	//char lTemp_Company[64];
	///char lTemp_Code[64];
	//char lTemp_Repeat[64];
	///char lTemp_Name[64];
	//char lTemp_Discription[129];Filename
	//int l_nDataReadStep  = 0;
	//int lnProcessSeqNum = 0;
	//int l_nEnable, l_nPort, l_nSel_Input,lMinFreq, lMaxFreq,lMinVolt,lMaxVolt;
	//
	//Filename = PATH_BASE_DIR;
	//Filename += _T("\\Model\\");
	//Filename += lFilename;
	//Filename +=  _T(".mdl");

	//fopen_s(&m_fp, (LPCSTR)Filename, "w+");
	//if(m_fp)
	//{
	//	fseek(m_fp, 0, 0);
	//
	//	sprintf(lbuf,"[PROCESS]\n");
	//	fputs(lbuf,m_fp);
	//
	//	sprintf(lbuf,"FILE=%s\n",gTestSeqData.strSeqFileName.GetBuffer());
	//	fputs(lbuf,m_fp);
	//	
	//	sprintf(lbuf,"CHECK SUM= 0x%02X 0x%02X\n", gTestSeqData.CheckSum[0] ,gTestSeqData.CheckSum[1]   );
	//	fputs(lbuf,m_fp);

	//	sprintf(lbuf,"FREQUENCY=%d %d %d %d %d %d %d %d %d %d \n", 
	//					gTestSeqData.Frequency[0], gTestSeqData.Frequency[1], gTestSeqData.Frequency[2],  gTestSeqData.Frequency[3],
	//					gTestSeqData.Frequency[4], gTestSeqData.Frequency[5], gTestSeqData.Frequency[6],  gTestSeqData.Frequency[7],
	//					gTestSeqData.Frequency[8], gTestSeqData.Frequency[9]);	
	//	fputs(lbuf,m_fp);
	//
	//	sprintf(lbuf,"[REMOCON]\n");
	//	fputs(lbuf,m_fp);
	//	if(gTestSeqData.nRemoconSigType == IR_REMOTE_CA)
	//	{
	//		sprintf(lbuf,"TYPE=CA\n");
	//	}
	//	else
	//	{
	//		sprintf(lbuf,"TYPE=RF\n");
	//	}
	//	fputs(lbuf,m_fp);

	//	sprintf(lbuf,"FILE=%s\n",gTestSeqData.strRemoconFileName.GetBuffer());
	//	fputs(lbuf,m_fp);

	//	sprintf(lbuf,"[MES]\n");
	//	fputs(lbuf,m_fp);
	//	if(gTestSeqData.bMesEnable == 1)
	//	{
	//		sprintf(lbuf,"ENABLE=1\n");
	//	}
	//	else
	//	{
	//		sprintf(lbuf,"ENABLE=0\n");
	//	}
	//	fputs(lbuf,m_fp);		

	//	sprintf(lbuf,"IP=%s\n",g_nRemoteIPAddress.GetBuffer());
	//	fputs(lbuf,m_fp);	

	//	sprintf(lbuf,"PORT NUMBER=%d\n",g_nRemotePortNumber);//g_nRemotePortType
	//	fputs(lbuf,m_fp);	
	//	sprintf(lbuf,"PORT TYPE=%d\n",g_nRemotePortType);//
	//	fputs(lbuf,m_fp);	
	//	sprintf(lbuf,"LOCAL NUMBER=%d\n",g_nLocalPortNumber);
	//	fputs(lbuf,m_fp);	

	//
	//	sprintf(lbuf,"[CAMERA]\n");
	//	fputs(lbuf,m_fp);
	//
	//	sprintf(lbuf,"IP=%s\n",gTestSeqData.strCameraIPAddress.GetBuffer());//gTestSeqData.strCameraIPAddress
	//	fputs(lbuf,m_fp);
	//		

	//	fclose(m_fp);	
	//	

	//}
	//else
	//{
	//	lFileError = 5;
	//}

	return lFileError;
}



int CFileControl::Read_IniModel()
{
	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	

	Filename = PATH_BASE_DIR;
	Filename += _T("\\start.ini");


	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if(m_fp)
	{
		if(fgets(lbuf, 1024, m_fp) != NULL) 
		{
			for(int i = 0; i < strlen(lbuf); i++)
			{
				if((lbuf[i] == '\r')||(lbuf[i] == '\n'))
				{
					lbuf[i] = 0;
					break;
				}
			}	
		
		
			gTestSeqData.strModelName = gTestSeqData.strModelName = lbuf;

		
		}	

		if(fgets(lbuf, 1024, m_fp) != NULL) 
		{
		
			sscanf(lbuf, "%d %d %d %d %d %d %d %d",
				&g_ManualId[0], &g_ManualId[1], &g_ManualId[2], &g_ManualId[3], 
				&g_ManualId[4], &g_ManualId[5], &g_ManualId[6], &g_ManualId[7]);
		}
		fclose(m_fp);		

	} 
	else
	{
		fopen_s(&m_fp, (LPCSTR)Filename, "w+");
		if(m_fp)
		{
			sprintf(lbuf,"LG_BDPLAER");
			fputs(lbuf,  m_fp);
			fclose(m_fp);
		}
		else
		{
			lFileError = 5;
		}
	}
	return lFileError;
}


int CFileControl::SaveINIModel()//gTestSeqData
{

	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	

	Filename = PATH_BASE_DIR;
	Filename += _T("\\start.ini");


	fopen_s(&m_fp, (LPCSTR)Filename, "w+");
	if(m_fp)
	{
		sprintf(lbuf,"%s\r\n",gTestSeqData.strModelName.GetBuffer());
		fputs(lbuf,  m_fp);
		
		sprintf(lbuf,"%d %d %d %d %d %d %d %d\r\n",
				g_ManualId[0], g_ManualId[1], g_ManualId[2], g_ManualId[3], 
				g_ManualId[4], g_ManualId[5], g_ManualId[6], g_ManualId[7]);
		fputs(lbuf,  m_fp);
			
	
		fclose(m_fp);		

	} 
	else
	{
		lFileError = 5;
	}
	

	
	return lFileError;
}


/*
 �� �� �� : strDsn		=> MES DB DSN		default : DAVMDEV
			strUserID	=> MES DB UserID	�׽�Ʈ�뼭��ID : oraspc
			strPassWD	=> MES DB PassWD	�׽�Ʈ�뼭��Pass : spcora00
*/


//
//int  CFileControl::Read_ConfigData()//SaveConfigData()
//{
//	int lFileError = 0;
//	
//	FILE *m_fp;
//	CString str;
//	CString Filename;
//	char lbuf[1024];	
//
//	Filename = PATH_BASE_DIR;
//	Filename += _T("\\config.cfg");
//
//
//	fopen_s(&m_fp, (LPCSTR)Filename, "r");
//	if(m_fp)
//	{		
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			sscanf_s(lbuf, "%d %d %d", &g_nPort_Set, &g_nBaud_Set, &g_nParity_Set);
//		}
//		else
//		{
//			g_nPort_Set = 1;
//			g_nBaud_Set = 19200;
//			g_nParity_Set = 0;
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strPassword = lbuf;
//		}
//		else
//		{
//			g_strPassword = "1111";	
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesTns = lbuf;
//		}
//		else
//		{
//			g_strMesTns = _T("DAVMDEV");
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)//AVN
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesAvn = lbuf;
//		}
//		else
//		{
//			
//		g_strMesAvn = _T("ORADAVMES");
//		
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesUserID = lbuf;
//		}
//		else
//		{
//			g_strMesUserID = _T("oraspc");		
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesPassword = lbuf;
//		}
//		else
//		{
//			g_strMesPassword = _T("spcora00");
//			lFileError++;
//		}
//				
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesLine = lbuf;
//		}
//		else
//		{
//			g_strMesLine = _T("S1");
//			lFileError++;
//		}
//					
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			int lsize = strlen(lbuf);
//			lbuf[lsize-1] = '\0';
//			g_strMesInspStep = lbuf;
//		}
//		else
//		{
//			g_strMesInspStep = _T("TEST1");
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			sscanf_s(lbuf, "%d %d %d", &g_nPort_Pcb, &g_nBaud_Pcb, &g_nParity_Pcb);
//		}
//		else
//		{
//			g_nPort_Pcb = 2;
//			g_nBaud_Pcb = 19200;
//			g_nParity_Pcb = 0;
//			lFileError++;
//		}
//
//		if(fgets(lbuf, 1024, m_fp) != NULL)
//		{
//			sscanf_s(lbuf, "%d %d %d", &g_nPort_LED, &g_nBaud_LED, &g_nParity_LED);
//		}
//		else
//		{
//			g_nPort_LED = 3;
//			g_nBaud_LED = 19200;
//			g_nParity_LED = 0;
//			lFileError++;
//		}
//	/*			fprintf(m_fp, "%d %d %d\n",  g_nPort_Pcb, g_nBaud_Pcb, g_nParity_Pcb );
//		fprintf(m_fp, "%d %d %d\n",  g_nPort_LED, g_nBaud_LED, g_nParity_LED );
//	*/	
//		fclose(m_fp);		
//
//	} 
//	else
//	{
//		g_nPort_Set = 1;
//		g_nBaud_Set = 19200;
//		g_nParity_Set = 0;
//		g_strPassword = "1111";	
//		g_strMesTns = _T("DAVMDEV");
//		g_strMesAvn = _T("ORADAVMES");
//		g_strMesUserID = _T("oraspc");
//		g_strMesPassword = _T("spcora00");
//		g_strMesLine = _T("S1");
//		g_strMesInspStep = _T("TEST1");
//
//		g_nPort_Pcb = 2;
//		g_nBaud_Pcb = 19200;
//		g_nParity_Pcb = 0;	
//		
//		g_nPort_LED = 3;
//		g_nBaud_LED = 115200;
//		g_nParity_LED = 1;
//		lFileError++;
//		
//	}
//	return lFileError;
//}



int  CFileControl::Read_ConfigSeqData()//SaveConfigData()
{
	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	

	Filename = PATH_BASE_DIR;
	Filename += _T("\\configseq.cfg");

	UINT l_nLocalPortNumber[6] = { 0,0,0,0,0,0 };
	
	g_nReplyTimeOut = 10;
	g_nTransTimeOut = 10;

	
	fopen_s(&m_fp, (LPCSTR)Filename, "r");

	if(m_fp)
	{		
		while(fgets(lbuf, 1024, m_fp) != NULL)
		{		
			if(strncmp(lbuf,"[PORT NUMBER]",strlen("[PORT NUMBER]")) == 0)
			{
				int lsize = strlen(lbuf);
				lbuf[lsize-1] = '\0';
				sscanf_s(&lbuf[strlen("[PORT NUMBER]")], "%d %d %d %d %d %d", 
					&g_nLocalPortNumber[0],
					&g_nLocalPortNumber[1],
					&g_nLocalPortNumber[2],
					&g_nLocalPortNumber[3],
					&g_nLocalPortNumber[4],
					&g_nLocalPortNumber[5]);
			}		
			else if (strncmp(lbuf, "[SCAN COM]", strlen("[SCAN COM]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[SCAN COM]")], "%d %d %d", &g_nPort_Scanner, &g_nBaud_Scanner, &g_nParity_Scanner);
			}
			else if (strncmp(lbuf, "[PLC IP ADDRESS]", strlen("[PLC IP ADDRESS]")) == 0)
			{
				int lsize = strlen(lbuf);
				lbuf[lsize - 1] = '\0';
				CString strTmp = &lbuf[strlen("[PLC IP ADDRESS]")];
				g_strPLCIPAddress = strTmp;
				//sscanf_s(&lbuf[strlen("[PLC IP ADDRESS]")], "%d %d %d", &g_nPort_Scanner, &g_nBaud_Scanner, &g_nParity_Scanner);
			}
			else if (strncmp(lbuf, "[PLC PORT]", strlen("[PLC PORT]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[PLC PORT]")], "%d", &g_nPLCPortNumber);
			}

			//fprintf(m_fp, "[PLC IP ADDRESS]%Ts\n", g_strPLCIPAddress);
			//fprintf(m_fp, "[PLC PORT]%d\n", g_nPLCPortNumber);
			
		}	
		fclose(m_fp);		

	/*	if ((   l_nLocalPortNumber[0] > 0)
			&& (l_nLocalPortNumber[1] > 0)
			&& (l_nLocalPortNumber[2] > 0)
			&& (l_nLocalPortNumber[3] > 0)
			&& (l_nLocalPortNumber[4] > 0)
			&& (l_nLocalPortNumber[5] > 0))
		{

			g_nLocalPortNumber[0] = l_nLocalPortNumber[0];
			g_nLocalPortNumber[1] = l_nLocalPortNumber[1];
			g_nLocalPortNumber[2] = l_nLocalPortNumber[2];
			g_nLocalPortNumber[3] = l_nLocalPortNumber[3];
			g_nLocalPortNumber[4] = l_nLocalPortNumber[4];
			g_nLocalPortNumber[5] = l_nLocalPortNumber[5];
		}*/
	} 
	else
	{
		g_nPort_Scanner = 1;
		g_nBaud_Scanner = 9600;
		g_nParity_Scanner = 0;
			
		lFileError++;
		
	}
	return lFileError;
}





int CFileControl::SaveConfigSeqData()//
{

	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	

	Filename = PATH_BASE_DIR;
	Filename += _T("\\configseq.cfg");
	

	fopen_s(&m_fp, (LPCSTR)Filename, "w+");
	if(m_fp)
	{
		fprintf(m_fp, "[PORT NUMBER]%d %d %d %d %d %d %d\n", 
			g_nLocalPortNumber[0], 
			g_nLocalPortNumber[1], 
			g_nLocalPortNumber[2], 
			g_nLocalPortNumber[3], 
			g_nLocalPortNumber[4],
			g_nLocalPortNumber[5]);

		fprintf(m_fp, "[SCAN COM]%d %d %d\n", g_nPort_Scanner, g_nBaud_Scanner, g_nParity_Scanner);
		fprintf(m_fp, "[PLC IP ADDRESS]%Ts\n", g_strPLCIPAddress);
		fprintf(m_fp, "[PLC PORT]%d\n", g_nPLCPortNumber);
	
		fclose(m_fp);	
	} 
	else
	{
	

		lFileError = 5;
	}
	
	return lFileError;
}

int CFileControl::LoadData()
{
	char lmonbuf[12][4]={"JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC"};
//	CFile logFILE;
	
	int buflen = 0;
	char pbufwrite[4096];
//	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		
	char sDate[1024];

	

	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\Log\\");

	//sprintf(sDate, "\\[20%02d-%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1);
	strDir += _T(sDate);
	//strDir += _T(sDate);
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	int lpos = 0;
	int i = 0;
	CString strLine;
//	CString strPart;
	CString strData;
	SaveLogDataT lSaveData;
	

	sprintf(sDate, "%02d%02d%02d", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	
	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".csv";


	CString fnameScaned = strDir;
	fnameScaned += _T("\\");
	fnameScaned += _T(sDate);
	fnameScaned += _T("SCAN");
	fnameScaned += ".csv";
	
	FILE *m_fp;
	char lbuf[1024];

//	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	fopen_s(&m_fp, (LPCSTR)fname.GetBuffer(), "r");
	gDataCount = 0;
	if(m_fp)
	{	
		if(fgets(lbuf, 1024, m_fp) != NULL)
		{
		}
		//while(fgets(lbuf, 1024, m_fp) != NULL)//gLoadDataInfo
		//{
		//	lSaveData.bOutScaned = 0;

		//	strLine = lbuf;

		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strWipID_SetSN = strData; //lSaveData.strWipID_SetSN.GetBuffer(), 

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strTestResult = strData; ////lSaveData.strTestResult.GetBuffer(), 	

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strErrorDiscription = strData; //lSaveData.strErrorDiscription.GetBuffer(), 	

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strEQPID = strData; //lSaveData.strEQPID,

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strProcID = strData; //lSaveData.strProcID,

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strTestDateTime = strData; //lSaveData.strTestDateTime,		

		//
		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strTestDiscription = strData; //lSaveData.strTestDiscription.GetBuffer());	

		//	if(gDataCount < 100000)
		//	{
		//		gLoadDataInfo[gDataCount] = lSaveData;
		//		gLoadDataInfo[gDataCount].uSaveNo = gDataCount;
		//		gDataCount++;
		//	}
		//}
		fclose(m_fp);
	}

	
	fopen_s(&m_fp, (LPCSTR)fnameScaned.GetBuffer(), "r");
	
	if(m_fp)
	{		
		//while(fgets(lbuf, 1024, m_fp) != NULL)//gLoadDataInfo
		//{
		//	strLine = lbuf;

		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.uSaveNo = _ttoi(strData); //lSaveData.strWipID_SetSN.GetBuffer(), 

		//	lpos = strLine.Find(",");
		//	strData = strLine.Left(lpos);
		//	strLine = strLine.Mid(lpos+1);
		//	lSaveData.strWipID_SetSN = strData; //lSaveData.strWipID_SetSN.GetBuffer(), 
		//
		//	if(lSaveData.uSaveNo < gDataCount)
		//	{
		//		if(gLoadDataInfo[lSaveData.uSaveNo].strWipID_SetSN.Compare(lSaveData.strWipID_SetSN) == 0)
		//		{
		//			gLoadDataInfo[lSaveData.uSaveNo].bOutScaned = 1;
		//		}				
		//	}
		//}
		fclose(m_fp);
	}




	return 1;
}


int CFileControl::SaveLog(SaveLogDataT lSaveData )
{
	char lmonbuf[12][4]={"JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC"};
	CFile logFILE;
	
	int buflen = 0;
	char pbufwrite[4096];
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		
	char sDate[1024];

	

	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\Log\\");

	//sprintf(sDate, "\\[20%02d-%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1);
	strDir += _T(sDate);
	//strDir += _T(sDate);
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	int lpos = 0;
	int i = 0;

	

	sprintf(sDate, "%02d%02d%02d", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	
	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".csv";



	if(!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if(!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
		//buflen = sprintf_s(pbufwrite,"OK/NG, SET_SN, MES, CheckTime, CycleTime, Date, Discription \r\n" );	
		//logFILE.Write(pbufwrite, buflen);

		buflen = sprintf_s(pbufwrite,"SET_SN, OK/NG, Reason ,EQUIP, JIG,  Date, Discription \r\n" );	
		logFILE.Write(pbufwrite, buflen);

	}
	
	//sprintf(sDate, "%4d/%02d/%02d %02dh %02dm %02ds",
	//t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	
	//buflen = sprintf_s(pbufwrite,"%s,%s,'%d, %.1f, %.1f, %s, %s \r\n", 
	//	gSaveInfo.strTestResult.GetBuffer(), 
	//	gSaveInfo.strWipID_SetSN.GetBuffer(), 
	//	gSaveInfo.bMesSts,
	//	gSaveInfo.CheckTime,
	//	gSaveInfo.CycleTime, 
	//	sDate,
	//	gSaveInfo.strTestDiscription.GetBuffer());	
	//logFILE.SeekToEnd();
	//buflen = sprintf_s(pbufwrite,"%s,%s,%s,%s,%s,%s,%s\r\n", 
	//	lSaveData.strWipID_SetSN.GetBuffer(), 
	//	lSaveData.strTestResult.GetBuffer(), 		
	//	lSaveData.strErrorDiscription.GetBuffer(), 	
	//	lSaveData.strEQPID,
	//	lSaveData.strProcID,
	//	lSaveData.strTestDateTime,			
	//	lSaveData.strTestDiscription.GetBuffer());	
	//logFILE.SeekToEnd();
	//logFILE.Write(pbufwrite, buflen);
	
	logFILE.Close();
	
	//if(gDataCount < 100000)
	//{
	///*	gLoadDataInfo[gDataCount] = lSaveData;
	//	gLoadDataInfo[gDataCount].uSaveNo = gDataCount;
	//	gDataCount++;*/
	//}
	return 1;
}

int CFileControl::SaveLogScan(SaveLogDataT lSaveData )
{
	char lmonbuf[12][4]={"JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC"};
	CFile logFILE;
	
	int buflen = 0;
	char pbufwrite[4096];
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		
	char sDate[1024];

	

	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\Log\\");

	//sprintf(sDate, "\\[20%02d-%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1);
	strDir += _T(sDate);
	//strDir += _T(sDate);
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	int lpos = 0;
	int i = 0;

	

	sprintf(sDate, "%02d%02d%02d", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	
	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += _T("SCAN");
	fname += ".csv";



	if(!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if(!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
		//buflen = sprintf_s(pbufwrite,"OK/NG, SET_SN, MES, CheckTime, CycleTime, Date, Discription \r\n" );	
		//logFILE.Write(pbufwrite, buflen);

		buflen = sprintf_s(pbufwrite,"No, SET_SN, OK/NG, Reason ,EQUIP, JIG,  Date, Discription \r\n" );	
		logFILE.Write(pbufwrite, buflen);

	}
	
	//sprintf(sDate, "%4d/%02d/%02d %02dh %02dm %02ds",
	//t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	
	//buflen = sprintf_s(pbufwrite,"%s,%s,'%d, %.1f, %.1f, %s, %s \r\n", 
	//	gSaveInfo.strTestResult.GetBuffer(), 
	//	gSaveInfo.strWipID_SetSN.GetBuffer(), 
	//	gSaveInfo.bMesSts,
	//	gSaveInfo.CheckTime,
	//	gSaveInfo.CycleTime, 
	//	sDate,
	//	gSaveInfo.strTestDiscription.GetBuffer());	
	//logFILE.SeekToEnd();
	//buflen = sprintf_s(pbufwrite,"%d,%s,%s,%s,%s,%s,%s,%s \r\n", 
	//	lSaveData.uSaveNo, 
	//	lSaveData.strWipID_SetSN.GetBuffer(), 
	//	lSaveData.strTestResult.GetBuffer(), 		
	//	lSaveData.strErrorDiscription.GetBuffer(), 	
	//	lSaveData.strEQPID,
	//	lSaveData.strProcID,
	//	lSaveData.strTestDateTime,			
	//	lSaveData.strTestDiscription.GetBuffer());	
	//logFILE.SeekToEnd();
	//logFILE.Write(pbufwrite, buflen);
	
	logFILE.Close();
	

	return 1;
}


int CFileControl::SaveGmesLOG(CString LogStr, int lRev)
{
	char lmonbuf[12][4] = { "JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC" };
	CFile logFILE;

	int buflen = 0;
	char pbufwrite[4096];
	CString strBufWrite;
	CString StrTemp;
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);
	char sDate[1024];


	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\GmesLog\\");

	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year - 100, t->tm_mon + 1);
	strDir += _T(sDate);
	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//int lpos = 0;
	//int i = 0;

	//gTestSeqData.strModelName.MakeUpper();
	//for( i = 0; i < gTestSeqData.strModelName.GetLength(); i++)
	//{
	//	if(((gTestSeqData.strModelName.GetAt(i) >= '0') && (gTestSeqData.strModelName.GetAt(i) <= '9'))
	//		||((gTestSeqData.strModelName.GetAt(i) >= 'A') && (gTestSeqData.strModelName.GetAt(i) <= 'Z')))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		break;

	//	}
	//}

	//lpos = i;
	//
	//sTempDir = gTestSeqData.strModelName.Left(lpos);
	//strDir += sTempDir;
	//strDir += "\\";
	//if(_mkdir(strDir.GetBuffer())==0)
	//{
	//}

	//strDir += gTestSeqData.strModelName;
	//strDir += "\\";
	//if(_mkdir(strDir.GetBuffer())==0)
	//{
	//}

	//sprintf(sDate, "%02d%02d%02d_%s_GMES_log", t->tm_year-100, t->tm_mon+1, t->tm_mday, gTestSeqData.strModelName.GetBuffer());
	sprintf(sDate, "%02d%02d%02d_GMES_log", t->tm_year - 100, t->tm_mon + 1, t->tm_mday);

	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".txt";



	if (!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if (!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
	}

	if (lRev == 1)
	{
		strBufWrite = _T("[RECV(GMES)] : \r\n");
	}
	else
	{
		strBufWrite = _T("[SEND(GMES)] : \r\n");

	}
	StrTemp.Format(_T("Time:%4d/%02d/%02d %02dh %02dm %02ds\r\n"), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	strBufWrite += StrTemp;
	strBufWrite += LogStr;
	strBufWrite += _T("\r\n\r\n");

	logFILE.SeekToEnd();
	logFILE.Write(strBufWrite, strBufWrite.GetLength());

	logFILE.Close();


	return 1;
}

int CFileControl::SavePLCLOG(CString LogStr, int lRev)
{
	char lmonbuf[12][4] = { "JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC" };
	CFile logFILE;

	int buflen = 0;
	char pbufwrite[4096];
	CString strBufWrite;
	CString StrTemp;
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);
	char sDate[1024];


	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\PLCDebugLog\\");

	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year - 100, t->tm_mon + 1);
	strDir += _T(sDate);
	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//int lpos = 0;
	//int i = 0;

	//gTestSeqData.strModelName.MakeUpper();
	//for( i = 0; i < gTestSeqData.strModelName.GetLength(); i++)
	//{
	//	if(((gTestSeqData.strModelName.GetAt(i) >= '0') && (gTestSeqData.strModelName.GetAt(i) <= '9'))
	//		||((gTestSeqData.strModelName.GetAt(i) >= 'A') && (gTestSeqData.strModelName.GetAt(i) <= 'Z')))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		break;

	//	}
	//}

	//lpos = i;
	//
	//sTempDir = gTestSeqData.strModelName.Left(lpos);
	//strDir += sTempDir;
	//strDir += "\\";
	//if(_mkdir(strDir.GetBuffer())==0)
	//{
	//}

	//strDir += gTestSeqData.strModelName;
	//strDir += "\\";
	//if(_mkdir(strDir.GetBuffer())==0)
	//{
	//}

	//sprintf(sDate, "%02d%02d%02d_%s_GMES_log", t->tm_year-100, t->tm_mon+1, t->tm_mday, gTestSeqData.strModelName.GetBuffer());
	sprintf(sDate, "%02d%02d%02d_PLC_log", t->tm_year - 100, t->tm_mon + 1, t->tm_mday);

	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".txt";



	if (!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if (!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
	}

	if (lRev == 1)
	{
		strBufWrite = _T("[RECV from(PLC)] : \r\n");
	}
	else
	{
		strBufWrite = _T("[SEND to(PLC)] : \r\n");

	}
	StrTemp.Format(_T("Time:%4d/%02d/%02d %02dh %02dm %02ds\r\n"), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	strBufWrite += StrTemp;
	strBufWrite += LogStr;
	strBufWrite += _T("\r\n\r\n");

	logFILE.SeekToEnd();
	logFILE.Write(strBufWrite, strBufWrite.GetLength());

	logFILE.Close();


	return 1;
}


int CFileControl::SaveCountData()//
{
	char lmonbuf[12][4]={"JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC"};
	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
//	char lbuf[1024];	
	
	int buflen = 0;
//	char pbufwrite[1024];


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		
	char sDate[1024];

	

	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\Count");
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	/*sprintf_s(sDate, "\\20%02d_%s", t->tm_year-100, lmonbuf[t->tm_mon]);
	strDir += _T(sDate);
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}*/

	//sprintf_s(sDate, "COUNT_%s_%02d%02d%02d", gLoadDataInfo.strModelName.GetBuffer(), t->tm_year-100, t->tm_mon+1, t->tm_mday);//sprintf(sDate, "COUNT_%02d%02d%02d", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	
	Filename = strDir;
	Filename += _T("\\");
	Filename += "Count";// _T(sDate);
	Filename += ".txt";




	fopen_s(&m_fp, (LPCSTR)Filename, "w+");
	if(m_fp)
	{
		fprintf(m_fp, "%d %d %d\n", g_nOKCnt, g_nNGCnt, g_nTargetCnt);
		
		fclose(m_fp);	
	} 
	else
	{
		lFileError = 5;
	}
	
	return lFileError;
}

int  CFileControl::ReadCountData()//
{
	int lFileError = 0;
	
	char lmonbuf[12][4]={"JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC"};
	
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	
	
	int buflen = 0;
//	char pbufwrite[1024];


	time_t ti;
	time(&ti);
	struct tm *t = 	localtime( &ti);		
	//localtime_s(t, &ti);		
	char sDate[1024];

	

	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\Count");
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	/*sprintf_s(sDate, "\\20%02d_%s", t->tm_year-100, lmonbuf[t->tm_mon]);
	strDir += _T(sDate);
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	sprintf_s(sDate, "COUNT_%s_%02d%02d%02d", gTestSeqData.strModelName.GetBuffer(), t->tm_year-100, t->tm_mon+1, t->tm_mday);*/
	
	Filename = strDir;
	Filename += _T("\\");
	Filename += "Count";// _T(sDate);
	Filename += ".txt";

	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if(m_fp)
	{		
		if(fgets(lbuf, 1024, m_fp) != NULL)
		{
			sscanf_s(lbuf, "%d %d %d", &g_nOKCnt, &g_nNGCnt, &g_nTargetCnt);
		}
		fclose(m_fp);		

	} 
	else
	{
		g_nOKCnt = 0;
		g_nNGCnt = 0;
		g_nTargetCnt = 10000;
		SaveCountData();
		
	}
	return lFileError;
}


int CFileControl::SaveCableCountData()//
{
	
	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
//	char lbuf[1024];	
	
	int buflen = 0;
//	char pbufwrite[1024];


	//time_t ti;
	//time(&ti);
	//struct tm *t = localtime(&ti);		
	//char sDate[1024];

	

	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\CableManager");
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	Filename = strDir;
	Filename += _T("\\cablecount.txt");





	fopen_s(&m_fp, (LPCSTR)Filename, "w+");
	if(m_fp)
	{
		fprintf(m_fp, "[USB]%d %d\n", gCableManager.nUSB_CableCountMax, gCableManager.nUSB_CableCount);
		fprintf(m_fp, "[PORT]%d %d\n", gCableManager.nPort_CableCountMax, gCableManager.nPort_CableCount);
		fprintf(m_fp, "[CM4CHECKER]%d %d\n", gCableManager.nCM4CheckerCountMax, gCableManager.nCM4CheckerCount);
		fprintf(m_fp, "[CM5CHECKER]%d %d\n", gCableManager.nCM5CheckerCountMax, gCableManager.nCM5CheckerCount);
		fprintf(m_fp, "[MIC]%d %d\n", gCableManager.nMIC_CableCountMax, gCableManager.nMIC_CableCount);
		
		fclose(m_fp);	
	} 
	else
	{
		lFileError = 5;
	}
	
	return lFileError;
}

int  CFileControl::ReadCableCountData()//
{
	
	
	int lFileError = 0;
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];	
	
	int buflen = 0;
//	char pbufwrite[1024];
	
	
	gCableManager.nUSB_CableCountMax = 500;
	gCableManager.nUSB_CableCount = 0;
	gCableManager.nPort_CableCountMax = 1000;
	gCableManager.nPort_CableCount=0;
	gCableManager.nCM4CheckerCountMax=10000;
	gCableManager.nCM4CheckerCount=0;
	gCableManager.nCM5CheckerCountMax=10000;
	gCableManager.nCM5CheckerCount=0;
	gCableManager.nMIC_CableCountMax=1000;
	gCableManager.nMIC_CableCount=0;

	

	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\CableManager");
	if(_mkdir(strDir.GetBuffer())==0)
	{
	}

	Filename = strDir;
	Filename += _T("\\cablecount.txt");
	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if(m_fp)
	{
		while(fgets(lbuf, 1024, m_fp) != NULL)
		{
			if(strncmp(lbuf,"[USB]",strlen("[USB]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[USB]")], "%d %d ", &gCableManager.nUSB_CableCountMax, &gCableManager.nUSB_CableCount);
			}
			else if(strncmp(lbuf,"[PORT]",strlen("[PORT]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[PORT]")], "%d %d ", &gCableManager.nPort_CableCountMax, &gCableManager.nPort_CableCount);
			}
			else if(strncmp(lbuf,"[CM4CHECKER]",strlen("[CM4CHECKER]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[CM4CHECKER]")], "%d %d ", &gCableManager.nCM4CheckerCountMax, &gCableManager.nCM4CheckerCount);
			}
			else if(strncmp(lbuf,"[CM5CHECKER]",strlen("[CM5CHECKER]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[CM5CHECKER]")], "%d %d ", &gCableManager.nCM5CheckerCountMax, &gCableManager.nCM5CheckerCount);
			}
			else if(strncmp(lbuf,"[MIC]",strlen("[MIC]")) == 0)
			{
				sscanf_s(&lbuf[strlen("[MIC]")], "%d %d ", &gCableManager.nMIC_CableCountMax, &gCableManager.nMIC_CableCount);
			}
		}

		fclose(m_fp);

	} 
	else
	{
		lFileError = 5;
	}
	
	
	return lFileError;
}

unsigned char  CFileControl::ReadHex(CString lstrASCII)//
{
	int lFileError = 0;
	
	lstrASCII.MakeUpper();
	unsigned char lchar = 0;
	unsigned char lRtnHex = 0;

	int lpos = lstrASCII.Find("0X");
	lstrASCII = lstrASCII.Mid(lpos+2);

	lchar = lstrASCII.GetAt(0);
	if(lchar > 0x39)
	{
		lchar = lchar - 'A' + 0x0A;
	}
	else
	{
		lchar = lchar - 0x30;
	}
	lRtnHex = lchar * 0x10;

	lchar = lstrASCII.GetAt(1);
	if(lchar > 0x39)
	{
		lchar = lchar - 'A' + 0x0A;
	}
	else
	{
		lchar = lchar - 0x30;
	}
	lRtnHex = lRtnHex + lchar ;

	return lRtnHex;
}

int  CFileControl::ReadAlarmCount(CString Filename)//
{	
	int lFileError = 0;

	FILE *m_fp;
	CString str;
//	CString Filename;
	char lbuf[1024];

	int buflen = 0;
	
	int lCnt = 0;
	

	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if (m_fp)
	{
		if (strncmp(lbuf, "NO", strlen("NO")) == 0)
		{		
			while (fgets(lbuf, 1024, m_fp) != NULL)
			{				
				sscanf_s(lbuf, "%d  ", &lCnt);				
			}
		}
		fclose(m_fp);
	}
	


	return -1;
}


//int CFileControl::SavePLCAlarm(CString LogStr)//m_AlarmCount
//{
//	char lmonbuf[12][4] = { "JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC" };
//	CFile logFILE;
//
//	int buflen = 0;
//	char pbufwrite[4096];
//	CString strBufWrite;
//	CString StrTemp;
//	CFileException ex;
//
//
//	time_t ti;
//	time(&ti);
//	struct tm *t = localtime(&ti);
//	char sDate[1024];
//
//
//	CString sTempDir;
//	CString strDir;
//	strDir = _T(PATH_BASE_DIR);
//	strDir += _T("\\PLC_AlarmLog\\");
//
//	if (_mkdir(strDir.GetBuffer()) == 0)
//	{
//	}
//
//	//sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
//	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year - 100, t->tm_mon + 1);
//	strDir += _T(sDate);
//	if (_mkdir(strDir.GetBuffer()) == 0)
//	{
//	}
//	
//	//sprintf(sDate, "%02d%02d%02d_%s_GMES_log", t->tm_year-100, t->tm_mon+1, t->tm_mday, gTestSeqData.strModelName.GetBuffer());
//	sprintf(sDate, "%02d%02d%02d_ALARM_log", t->tm_year - 100, t->tm_mon + 1, t->tm_mday);
//
//	CString fname = strDir;
//	fname += _T("\\");
//	fname += _T(sDate);
//	fname += ".csv";
//
//
//
//	if (!logFILE.Open(fname, CFile::modeReadWrite, &ex))
//	{
//		if (!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
//		{
//			//MessageBox("Fail to Save Log !!!");
//			return 0;
//		}
//	//	NO	DATE	ACK	COMMANT	REV
//
//		StrTemp.Format(_T("NO,	DATE,	ACK,	COMMANT,	REV\r\n") );
//		strBufWrite = StrTemp;
//		logFILE.SeekToEnd();
//		logFILE.Write(strBufWrite, strBufWrite.GetLength());
//		m_AlarmCount = 0;
//	}
//	else
//	{
//		
//		if (m_AlarmCount == 0)
//		{
//			m_AlarmCount = ReadAlarmCount(fname);
//		}
//	}
//	m_AlarmCount++;
//	StrTemp.Format(_T("%4d, %02d/%02d %02dh %02dm %02ds\r\n"), m_AlarmCount, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
//	strBufWrite += StrTemp;
//	strBufWrite += LogStr;
//	strBufWrite += _T("\r\n\r\n");
//
//	logFILE.SeekToEnd();
//	logFILE.Write(strBufWrite, strBufWrite.GetLength());
//
//	logFILE.Close();
//
//
//	return 1;
//}
