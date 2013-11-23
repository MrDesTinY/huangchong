/**
@file: DataConvert.h
@Breif: 主要是处理数据转换，得到协议对应的数据
@Author: skid
@Date:2011/09/26
@History:
*/
/* 这个类进行数据的转换主要是将从MYSQL数据库中得到的数据转换成协议数据*/

#pragma once

#include "OpMySQL.h"
#include "basicType.h"
#include "Config.h"

class DataConvert
{
    public:
        
        DataConvert(CConfig &config);// 类的初始化
        
        virtual ~DataConvert(void);
        
        BOOL Init(CConfig &config);
        
        COpMySQL  m_opMysql;
        CMR_TrafficData *m_pTrafficData;

        UCI_TrafficData *m_pUCIData;
        
        
        BOOL m_bConnDB;
        
        CString m_strDBServer;
        CString m_strDBName;
        CString m_strDBUser;
        CString m_strDBPwd;
        CString m_strDBPort;
        
        INT64 m_tranid; // 过车流水号
        INT64 m_uciid; // 联检车辆流水号
        
        MYSQL_RES* m_pRes; // 得到的数据集
		MYSQL_RES* m_pUCIRes; // 得到的数据集
     
        
        BOOL GetDBData(CStringArray &arr,BOOL bRealTime); // 从过车表中读取1行过车数据
        CMR_TrafficData* GetTrafficData();
        UCI_TrafficData *GetUCIData();
        BOOL ConvertData(CStringArray &arr); // 将从数据库中读出的数据 转换成 协议对应的数据
        BOOL UpdateTrafficID(UINT64 nTrafficID, UINT16 nStatus); // 更新数据库
        BOOL PutData(CStringArray &arr,int nIndex,CStringArray & arrField); // 将读取的数据取一个字段名
        BOOL GetDBUCIData(CStringArray &arr,BOOL bRealTime);
        BOOL ConvertUCIData(CStringArray &arr);
        BOOL DelUCITmpInfo(UINT64 nUCIID);
        BOOL SaveTmpToDB();
        BOOL PutUCIData(CStringArray &arr,int nIndex,CStringArray & arrField);
        BOOL ReadJpegFile(CString strFile,UINT32 &nFileSize,int nType); // 读取图片文件大小及数据
        
        CRITICAL_SECTION m_cs; // 读取文件的临界区
        
};
