#include "StdAfx.h"
#include "DataConvert.h"
UCI_TrafficData g_UCIData;
CRITICAL_SECTION g_csUCI;
CRITICAL_SECTION g_csTread;
CRITICAL_SECTION g_csSend;
CRITICAL_SECTION g_csMysql;
DataConvert::DataConvert(CConfig &config)
{  
	m_strDBServer = config.ReadConfig("Center","DBServer","127.0.0.1");
	m_strDBName =  config.ReadConfig("Center","DBName","gdw_parking_db");
	m_strDBUser =  config.ReadConfig("Center","DBUser","root");
	m_strDBPwd  =  config.ReadConfig("Center","DBPassword","sa");
	m_strDBPort =  config.ReadConfig("Center","DBPort","3306");    
   
   m_bConnDB = FALSE;
    
    m_pTrafficData = NULL;
    m_pUCIData = &g_UCIData;
    m_tranid =0;
 //   FILE file;
	//file = fopen("C:/1234.txt","w+");
	//fwrite(file,
    InitializeCriticalSection(&m_cs);
}

DataConvert::~DataConvert(void)
{
    CString strTmp;
    if(m_bConnDB)
    {
        m_opMysql.CloseConn();
        m_bConnDB = FALSE;
    }
    if(m_pTrafficData)
    {     
      
        if(m_pTrafficData->pnPlateJpegData)
        {
            delete[] m_pTrafficData->pnPlateJpegData;
            m_pTrafficData->pnPlateJpegData = NULL;
        }
        
       
        if(m_pTrafficData->pnCarJpegData)
        {
            delete[] m_pTrafficData->pnCarJpegData;
            m_pTrafficData->pnCarJpegData = NULL;
        }
        if(m_pTrafficData->pnCarJpeg1Data)
        {
            delete[] m_pTrafficData->pnCarJpeg1Data;
            m_pTrafficData->pnCarJpeg1Data = NULL;
        }
        if(m_pTrafficData)
        {
            delete m_pTrafficData;
            m_pTrafficData = NULL;
        }
    }
    DeleteCriticalSection(&m_cs);
}

BOOL DataConvert::Init(CConfig &config)
{   
    m_strDBServer = config.ReadConfig("Center","DBServer","127.0.0.1");
    m_strDBName =  config.ReadConfig("Center","DBName","gdw_parking_db");
    m_strDBUser =  config.ReadConfig("Center","DBUser","root");
    m_strDBPwd  =  config.ReadConfig("Center","DBPassword","sa");
    m_strDBPort =  config.ReadConfig("Center","DBPort","3306");
    
    m_bConnDB = m_opMysql.OpenConn(m_strDBServer,m_strDBUser,m_strDBPwd,m_strDBName,m_strDBPort);
    
    return m_bConnDB;
}


CMR_TrafficData* DataConvert::GetTrafficData()
{
     return m_pTrafficData;
}

UCI_TrafficData *DataConvert::GetUCIData()
{
	return m_pUCIData;
}

// 从数据库中获取一行数据

BOOL DataConvert::GetDBData(CStringArray &arr,BOOL bRealTime)
{
    CString strTmp;
    CString strSql =_T("");
    
    if(bRealTime)
    {
        strSql.Format("select Tranid,T.LaneNo,T.PlateNo,PlateConfidence,RecognitionCharacterNo,RecognitionCharacterConfiendce,T.VehicleColor,ColorDepth,\
                  PassTime,T.Direction,T.VehicleType,parkingType,T.CardNo,T.IoNo,TranMeans,IsMonttor,T.PlateColor,ImagePath,plateImagePath,VideoPath,IsUpload from \
                  TranVehicleInfo T left join VehicleInfo V on V.PlateNo = T.PlateNo where IsUpload = 0 and Tranmeans<>5 order by TranId desc limit 0,1");// limit 0,1只取1条记录,降序排列，实时数据优先上传
    
    }
    else
    {
        strSql.Format("select Tranid,T.LaneNo,T.PlateNo,PlateConfidence,RecognitionCharacterNo,RecognitionCharacterConfiendce,T.VehicleColor,ColorDepth,\
                  PassTime,T.Direction,T.VehicleType,parkingType,T.CardNo,T.IoNo,TranMeans,IsMonttor,T.PlateColor,ImagePath,plateImagePath,VideoPath,IsUpload from \
                  TranVehicleInfo T left join VehicleInfo V on V.PlateNo = T.PlateNo where IsUpload = 0 and Tranmeans<>5 order by TranId asc limit 0,1");// limit 0,1只取1条记录,升序排列，历史数据优先上传
    
    }
 
	EnterCriticalSection(&g_csTread);
	EnterCriticalSection(&g_csMysql);
    m_pRes = m_opMysql.OpenRecordset(strSql);
	LeaveCriticalSection(&g_csMysql);
    
    if(m_pRes == NULL)
    {
        strTmp.Format("skid OpenRecordset fail\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    //清空CStringArray
    if(!arr.IsEmpty())
    {
        arr.RemoveAll();
    }
    
    MYSQL_ROW row;
    int nFieldNum = 0;
    
    CString strData = _T("");

    while(row = mysql_fetch_row(m_pRes))
    { 
        nFieldNum = mysql_num_fields(m_pRes); // 获取字段个数
        for(int i =0;i<nFieldNum;i++)
        {

            strData.Format("%s",row[i]);
            strData.Trim();
            arr.Add(strData);      
        }				
    }
    
   
    
    m_opMysql.CloseRecordset(m_pRes);
    m_pRes = NULL;
    LeaveCriticalSection(&g_csTread);
    if(arr.IsEmpty())
    {
        strTmp.Format("目前没有需要上传的过车记录\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    return TRUE;

  
}

 
BOOL DataConvert::UpdateTrafficID(UINT64 nTrafficID, UINT16 nStatus) // 更新过车记录
{
    
    CString strSql,strTmp;
    BOOL bRet = FALSE;
	//int tmp = 1116;
	//char msg[255]={0};
	//sprintf(msg, "update tranvehicleinfo set isupload=%d where tranid=%d", nstatus, tmp);
	//strsql = msg;
	//strSql.Format("%d,%d",1, 2);
    strSql.Format("update tranvehicleinfo set IsUpload=%d where TranId=%d",nStatus,nTrafficID);
    if(m_bConnDB)
    {
        EnterCriticalSection(&g_csMysql);  
		bRet = m_opMysql.ExecuteSQL(strSql);
		LeaveCriticalSection(&g_csMysql);
          return bRet;
    }
    
    strTmp.Format("skid------数据库连接失败!\n");
    CPowerInfo::WLog(strTmp);
    return bRet;    
    
}

// 将GetDBData得到的数组，验证其正确性，并转换成协议类型数据
BOOL DataConvert::ConvertData(CStringArray &arr)
{
    CString strTmp;
    
    if(m_pTrafficData)
    {
        if(m_pTrafficData->pnPlateJpegData)
        {
            delete [] m_pTrafficData->pnPlateJpegData;
            m_pTrafficData->pnPlateJpegData = NULL;            
        }
        if(m_pTrafficData->pnCarJpegData)
        {
            delete [] m_pTrafficData->pnCarJpegData;
            m_pTrafficData->pnCarJpegData = NULL;
        }
        if(m_pTrafficData->pnCarJpeg1Data)
        {
            delete[] m_pTrafficData->pnCarJpeg1Data;
            m_pTrafficData->pnCarJpeg1Data = NULL;
        }
        delete m_pTrafficData;
        m_pTrafficData = NULL; 
    }
    
    if(m_pTrafficData == NULL)
    {
        m_pTrafficData = new CMR_TrafficData();
    }
    
    if(m_pTrafficData == NULL)
    {
        strTmp.Format("skid----new CMR_TrafficData() fail\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    
    }
    
    CStringArray arrField;
    //arrVal.SetSize(20);
    arrField.SetSize(20);
    
    
    for(int i = 0;i<arr.GetCount();i++)
    {
       if(!PutData(arr,i,arrField))
       {
            strTmp.Format("Put Filed:%s fail\n",arrField[i]);
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            return FALSE;
       }
    }
    
    return TRUE;
    
}

BOOL DataConvert::PutData(CStringArray &arr,int nIndex,CStringArray & arrField)
{
    CString strTmp;
   
    if(m_pTrafficData == NULL)
    {
        strTmp.Format("m_pTrafficData NULL\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    CString strVal;
    
    switch(nIndex)
    {
        case 0:
        {
            arrField[nIndex] = "Tranid"; // 过车序号
            strVal = arr[nIndex];
            m_pTrafficData->nVehicleNo = atoi(strVal)%65535;
            m_tranid = atoi(strVal); // 用以更新记录集
        }
        break;
        case 1:
        {
            arrField[nIndex] = "LaneNo"; // 车道号
            strVal = arr[nIndex];
            m_pTrafficData->nLaneId = atoi(strVal)&0xffff;
        }
        break;
        case 2:
        {
            arrField[nIndex] = "PlateNo";  // 车牌
            strVal = arr[nIndex];
            sprintf_s(m_pTrafficData->chPlateNo,MAX_PLATE_LEN,"%s",strVal);
        }
        break;
        case 3:
        {
            arrField[nIndex] = "PlateConfidence";
            strVal = arr[nIndex];
            m_pTrafficData->nPlateConfidence = atoi(strVal)&0xffff;
        
        }
        break;
        case 4:
        {
            arrField[nIndex] = "RecognitionCharacterNo";
            strVal = arr[nIndex];
            m_pTrafficData->nPlateConfidence = atoi(strVal) & 0xffff;
        }
        break;
        case 5:
        {
            arrField[nIndex] = "RecognitionCharacterConfiendce";
            // 各个字符的置性度预留
        }
        break;
        case 6:
        {
            arrField[nIndex] = "VehicleColor";  // 车身颜色
            strVal = arr[nIndex];
            
            if(strVal == "白色")
            {
                m_pTrafficData->nVehicleColor = 1;
            } 
            else if(strVal == "灰(银)")
            {
                m_pTrafficData->nVehicleColor = 2;
            }
            else if(strVal == "银(灰)")
            {
                m_pTrafficData->nVehicleColor = 3;
            }
            else if (strVal == "黑色")
            {
                m_pTrafficData->nVehicleColor = 4;
            }
            else if(strVal == "红色")
            {
                m_pTrafficData->nVehicleColor = 5;
            }
            else if(strVal == "深蓝")
            {
                m_pTrafficData->nVehicleColor = 6;
            }
            else if(strVal == "蓝色")
            {
                m_pTrafficData->nVehicleColor = 7;
            }
            else if(strVal == "黄色")
            {
                m_pTrafficData->nVehicleColor = 8;
            }
            else if(strVal == "绿色")
            {
                m_pTrafficData->nVehicleColor = 9;
            }
            else if(strVal == "棕色")
            {
                m_pTrafficData->nVehicleColor = 10;
            }
            else if(strVal == "粉色")
            {
                m_pTrafficData->nVehicleColor = 11;
            }
            else if(strVal == "紫色")
            {
                m_pTrafficData->nVehicleColor = 12;
            }
            else
            {
                m_pTrafficData->nVehicleColor = 0; //其它 和其它颜色均作为其他情况处理
            }
           
            
        }
        break;
        case 7:
        {
            arrField[nIndex] = "ColorDepth";  // 车身颜色深浅
            strVal = arr[nIndex];
            if(strVal == "浅色")
            {
                m_pTrafficData->nVehicleColorDeep = 1;
            }
            else if(strVal =="深色")
            {
                m_pTrafficData->nVehicleColorDeep = 0;
            }
            else
            {
                m_pTrafficData->nVehicleColorDeep = 1; // 其他情况作为浅色处理
            }
        
        }
        break;
        case 8:
        {
             char chPassTime[20]={0};
            arrField[nIndex] = "PassTime";   // 过车时间
            strVal = arr[nIndex];
            strcpy_s(chPassTime,20,strVal.GetBuffer(0));
            SYSTEMTIME st;
           
            sscanf_s(chPassTime,"%d-%d-%d %d:%d:%d.%d",&st.wYear,&st.wMonth,&st.wDay,&st.wHour,&st.wMinute,&st.wSecond,&st.wMilliseconds);   //这里保存了毫秒数
            
            st.wYear -= 1970;
            
            BYTE tm[8] ={0};
            tm[0] = (BYTE)st.wYear &0xff;
            tm[1] = (BYTE)st.wMonth &0xff;
            tm[2] = (BYTE)st.wDay &0xff;
            tm[3] = (BYTE)st.wHour & 0xff;
            tm[4] = (BYTE)st.wMinute & 0xff;
            tm[5] = (BYTE)st.wSecond & 0xff;
            memcpy_s(&tm[6],2,&st.wMilliseconds,2);
            
            memcpy_s(m_pTrafficData->nPassTime,8,&tm,8);
            
        
        }
        break;
        case 9:
        {
            arrField[nIndex] = "Direction"; // 出入类型
            strVal = arr[nIndex];
            
            if(strVal == "入口")
            {
                m_pTrafficData->nTypeIO = 0;
            }
            else if(strVal == "出口")
            {
                m_pTrafficData->nTypeIO = 1;
            } 
            else 
            {
                m_pTrafficData->nTypeIO = 2; //非法数据
            }        
        }
        break;
        case 10:
        {
            arrField[nIndex] = "VehicleType"; // 车辆类型
            strVal = arr[nIndex];
            
            if(strVal == "军警汽车")
            {
                m_pTrafficData->nTypeVehicle = 1;
            }
            else if(strVal == "大型汽车")
            {
                m_pTrafficData->nTypeVehicle = 2;
            }
            else if (strVal == "小型汽车")
            {
                m_pTrafficData->nTypeVehicle = 3;
            }
            else if(strVal == "外籍汽车")
            {
                m_pTrafficData->nTypeVehicle = 4;
            }
            else if(strVal == "使馆汽车")
            {
                m_pTrafficData->nTypeVehicle = 5;
            }
            else if(strVal == "领馆汽车")
            {
                m_pTrafficData->nTypeVehicle = 6;
            }
            else   //其他车辆类型均作为其他车辆
            {
                m_pTrafficData->nTypeVehicle = 0;
            }
        }
        break;
        case 11:
        {
            arrField[nIndex] = "parkingType"; // 停车类型
            strVal = arr[nIndex];
            
            if(strVal == "固定车")
            {
                m_pTrafficData->nTypePark = 0;
            }
            else if(strVal == "预定车")
            {
                m_pTrafficData->nTypePark = 2;
            }
            else if(strVal == "联检车辆")
            {
            	m_pTrafficData->nTypePark = 3;
            }
            else  //其它均为临时车辆
            {
                m_pTrafficData->nTypePark = 1;
            }
        
        }
        break;
        case 12:
        {
            arrField[nIndex] = "CardNo"; // 卡号
            strVal = arr[nIndex];
            sprintf_s(m_pTrafficData->chCardNo,MAX_CARD_LEN,"%s",strVal);
        }
        break;
        case 13:
        {
            arrField[nIndex] = "IoNo"; //出入口序号
            strVal = arr[nIndex];
            m_pTrafficData->nDoorIndex = atoi(strVal);
        }
        break;
        case 14:
        {
            arrField[nIndex] = "TranMeans"; // 放行方式
            strVal = arr[nIndex];
            m_pTrafficData->nOutMode = atoi(strVal);
        }
        break;
        case 15:
        {
            arrField[nIndex] = "IsMontter";  //报警方式
            strVal = arr[nIndex];
            m_pTrafficData->nTypeAlarm = atoi(strVal);
            
        }
        break;
        case 16:
        {
            arrField[nIndex] = "PlateColor"; //车牌颜色
            strVal = arr[nIndex];
            strVal = strVal.Mid(0,2);
            
            strVal.AppendFormat("%s",m_pTrafficData->chPlateNo);
            
            sprintf_s(m_pTrafficData->chPlateNo,MAX_PLATE_LEN,"%s",strVal);
        }
        break;
        case 17: 
        {
            arrField[nIndex] = "ImagePath"; // 近景车辆图片路径
            strVal = arr[nIndex];
            
            if(!ReadJpegFile(strVal,m_pTrafficData->nCarJpegSize,2))
            {
                strTmp.Format("读取车辆图片出错,with error code:%d",GetLastError());
                OutputDebugString(strTmp);
                CPowerInfo::WLog(strTmp);
                return FALSE;
            }
            
        }
        break;
        case 18:
        {
            arrField[nIndex] = "plateImagePath";
            strVal = arr[nIndex];
            if(!ReadJpegFile(strVal,m_pTrafficData->nPlateJpegSize,1))
            {
               strTmp.Format("读取车牌图片出错,with error code：%d",GetLastError());
               OutputDebugString(strTmp);
               CPowerInfo::WLog(strTmp);
               return FALSE;
            }
        }
    }
    
    return TRUE;
    
}

BOOL DataConvert::ReadJpegFile(CString strFile,UINT32 &nFileSize,int nType)
{
    CString strTmp; 
    
    try 
    {
        UINT32 nReadBytes = 0;
       
        EnterCriticalSection(&m_cs);
        CFile file;
        
        if(!file.Open(strFile.GetBuffer(0),CFile::modeRead|CFile::shareDenyWrite)) //以共享读方式打开文件
        {
            strTmp.Format("打开文件失败,with error code:%d",GetLastError());
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            LeaveCriticalSection(&m_cs);
            return FALSE;          
        }
        nFileSize = (UINT32)file.GetLength();
        
      
        if(nType == 1) // 车牌图片 
        {
             if(m_pTrafficData)
            {
                  if(m_pTrafficData->pnPlateJpegData)
                  {
                    delete [] m_pTrafficData->pnPlateJpegData;
                    m_pTrafficData->pnPlateJpegData = NULL;
                  }
                  if(m_pTrafficData->pnPlateJpegData == NULL)
                  {
                      m_pTrafficData->pnPlateJpegData = new UINT8[nFileSize];
                     
                     if(m_pTrafficData->pnPlateJpegData == NULL)
                     {
                        strTmp.Format("new plateJpeg fail\n");
                        OutputDebugString(strTmp);
                        CPowerInfo::WLog(strTmp);
                        LeaveCriticalSection(&m_cs);
                        return FALSE;
                     }
                  }
                  nReadBytes =file.Read(m_pTrafficData->pnPlateJpegData,nFileSize);
                  
                  if(nReadBytes != nFileSize)
                  {
                      strTmp.Format("ReadPlateFile fail,with error code:%d",GetLastError());
                      OutputDebugString(strTmp);
                      CPowerInfo::WLog(strTmp);
                      nFileSize = 0;
                      LeaveCriticalSection(&m_cs);
                      return FALSE;
                  }
                  
                  LeaveCriticalSection(&m_cs);
                  return TRUE;
    
            }
            
            LeaveCriticalSection(&m_cs);
            nFileSize = 0;
            return FALSE;
        
        }
        
        else if(nType == 2) // 车辆图片
        {
            if(m_pTrafficData)
            {
                if(m_pTrafficData->pnCarJpegData)
                {
                    delete [] m_pTrafficData->pnCarJpegData;
                    m_pTrafficData->pnCarJpegData = NULL;
                }
                if(m_pTrafficData->pnCarJpegData == NULL)
                {
                    m_pTrafficData->pnCarJpegData = new UINT8[nFileSize];

                    if(m_pTrafficData->pnCarJpegData == NULL)
                    {
                        strTmp.Format("new CarJpeg fail\n");
                        OutputDebugString(strTmp);
                        CPowerInfo::WLog(strTmp);
                        LeaveCriticalSection(&m_cs);
                        return FALSE;
                    }
                }
                nReadBytes =file.Read(m_pTrafficData->pnCarJpegData,nFileSize);

                if(nReadBytes != nFileSize)
                {
                    strTmp.Format("ReadPlateFile fail,with error code:%d",GetLastError());
                    OutputDebugString(strTmp);
                    CPowerInfo::WLog(strTmp);
                    LeaveCriticalSection(&m_cs);
                    return FALSE;
                }

                LeaveCriticalSection(&m_cs);
                return TRUE;

            }

            LeaveCriticalSection(&m_cs);
            return FALSE;
            
        }
        
        LeaveCriticalSection(&m_cs);
        nFileSize = 0;
        return FALSE;     
    }
    catch(CFileException &e)
    {
        char chError[256] ={0};
        e.GetErrorMessage(chError,256);
        strTmp.Format("%s",chError);
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        LeaveCriticalSection(&m_cs);
        nFileSize = 0;
        return FALSE;
    }
    
    // code could not execute
    return TRUE;
 

}


BOOL DataConvert::GetDBUCIData(CStringArray &arr,BOOL bRealTime)
{
    CString strTmp;
    CString strSql =_T("");
    
    if(bRealTime)
    {
        strSql.Format("SELECT TEMPID, PLATENO, VEHICLETYPE, VEHICLECOLOR, OWNERNAME, CARDNO, RESERVE1 FROM TEMPINFO ORDER BY TEMPID DESC LIMIT 0,1");// limit 0,1只取1条记录,降序排列，实时数据优先上传
    
    }
    else
    {
        strSql.Format("SELECT TEMPID, PLATENO, VEHICLETYPE, VEHICLECOLOR, OWNERNAME, CARDNO, RESERVE1 FROM TEMPINFO ORDER BY TEMPID ASC LIMIT 0,1");// limit 0,1只取1条记录,升序排列，历史数据优先上传
    }
 
	EnterCriticalSection(&g_csTread);
	EnterCriticalSection(&g_csMysql);
    m_pUCIRes = m_opMysql.OpenRecordset(strSql);
    LeaveCriticalSection(&g_csMysql);
    if(m_pUCIRes == NULL)
    {
        strTmp.Format("hc--- OpenRecordset fail\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    //清空CStringArray
    if(!arr.IsEmpty())
    {
        arr.RemoveAll();
    }
    
    MYSQL_ROW row;
    int nFieldNum = 0;
    
    CString strData = _T("");

    while(row = mysql_fetch_row(m_pUCIRes))
    { 
        nFieldNum = mysql_num_fields(m_pUCIRes); // 获取字段个数
        for(int i =0;i<nFieldNum;i++)
        {

            strData.Format("%s",row[i]);
            strData.Trim();
            arr.Add(strData);      
        }				
    }
    
   
    
    m_opMysql.CloseRecordset(m_pUCIRes);
    m_pUCIRes = NULL;
    LeaveCriticalSection(&g_csTread);

    if(arr.IsEmpty())
    {
        strTmp.Format("目前没有需要上传的联检车辆记录\n");
        OutputDebugString(strTmp);
        CPowerInfo::WLog(strTmp);
        return FALSE;
    }
    
    return TRUE;

}


BOOL DataConvert::PutUCIData(CStringArray &arr,int nIndex,CStringArray & arrField)
{
    CString strTmp;    
    CString strVal;
    
    switch(nIndex)
    {
        case 0:
        {
            arrField[nIndex] = "TEMPID"; 
            strVal = arr[nIndex];
            m_uciid = atoi(strVal)%65535;
        }
        break;
        case 1:
        {
            arrField[nIndex] = "PLATENO"; 
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chPlateNo, MAX_PLATE_LEN,"%s",strVal);
        }
        break;
        case 2:
        {
            arrField[nIndex] = "VEHICLETYPE"; 
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chCarType,20*sizeof(char),"%s",strVal);
        }
        break;
        case 3:
        {
            arrField[nIndex] = "VEHICLECOLOR";
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chCarColor,12*sizeof(char),"%s",strVal);
        
        }
        break;
        case 4:
        {
            arrField[nIndex] = "OWNERNAME";
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chOwnerName, 32*sizeof(char),"%s",strVal);
        }
        break;
        case 5:
        {
            arrField[nIndex] = "CARDNO";
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chCardNum, 30*sizeof(char),"%s",strVal);
        }
        break;
        case 6:
        {
            arrField[nIndex] = "RESERVE1"; 
            strVal = arr[nIndex];
            sprintf_s(g_UCIData.chReserved, 2*sizeof(char),"%s",strVal);
        }
        break;
    }
    return TRUE;
}
BOOL DataConvert::ConvertUCIData(CStringArray &arr)
{
    CString strTmp;    
    CStringArray arrField;
    //arrVal.SetSize(20);
    arrField.SetSize(8);
	memset(&g_UCIData,0,sizeof(g_UCIData));
    for(int i = 0;i<arr.GetCount();i++)
    {
       if(!PutUCIData(arr,i,arrField))
       {
            strTmp.Format("Put UCI Filed:%s fail\n",arrField[i]);
            OutputDebugString(strTmp);
            CPowerInfo::WLog(strTmp);
            return FALSE;
       }
    }
    
    return TRUE;
}
BOOL DataConvert::DelUCITmpInfo(UINT64 nUCIID)
{
    CString strSql,strTmp;
    BOOL bRet = FALSE;
    strSql.Format("DELETE FROM TEMPINFO WHERE TEMPID = %d", nUCIID);
    if(m_bConnDB)
    {
        EnterCriticalSection(&g_csMysql);  
		bRet = m_opMysql.ExecuteSQL(strSql);
		LeaveCriticalSection(&g_csMysql);
          return bRet;
    }
    
    strTmp.Format("UCI数据库连接失败!\n");
    CPowerInfo::WLog(strTmp);
    return bRet;    
}


BOOL DataConvert::SaveTmpToDB()
{
    CString strTmp;
    CString strSql =_T("");
    BOOL bRet = FALSE;
    strSql.Format("INSERT INTO TEMPINFO (PLATENO, VEHICLETYPE, VEHICLECOLOR, OWNERNAME, CARDNO, RESERVE1) VALUES (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", \
    	g_UCIData.chPlateNo,g_UCIData.chCarType,g_UCIData.chCarColor,g_UCIData.chOwnerName,g_UCIData.chCardNum,g_UCIData.chReserved);
    if(m_bConnDB)
    {
	EnterCriticalSection(&g_csMysql);
	bRet = m_opMysql.ExecuteSQL(strSql);
	LeaveCriticalSection(&g_csMysql);
	if(bRet)
	{
		strTmp.Format("UCI 数据库执行成功!\n");
		CPowerInfo::WLog(strTmp);
		return TRUE;
	}
	else
	{
		strTmp.Format("UCI 数据库执行失败!\n");
		CPowerInfo::WLog(strTmp);
		return FALSE;
	}
    }
    else
    {
	strTmp.Format("UCI 数据库连接失败!\n");
	CPowerInfo::WLog(strTmp);
    	return FALSE;
    } 
}


