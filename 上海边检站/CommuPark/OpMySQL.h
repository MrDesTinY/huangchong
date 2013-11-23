/* @fileName: OpMySQL.h
   @Author: skid
   @Date:   20110819
   @className COpMySQL
   @Brief:    support some operation for mysql datebase 
              such as open conn
			          close conn
					  open recordset
					  close recordset
					  excute sql sentence
					  ...
  @note: if you openrecordset(), you should closerecordset first,and then 
         you can excute Sql sentence,or you just excute sql sentence fail...

*/

#pragma once
#include <winsock.h>
#include "mysql.h"
#pragma comment (lib,"libmysql.lib")

class COpMySQL
{
public:
	COpMySQL(void);
	~COpMySQL(void);

public:
	BOOL OpenConn(LPCTSTR szServerName,LPCTSTR szUserName,LPCTSTR szPassword,LPCTSTR szDBName,LPCTSTR szPort);
	void CloseConn();
	BOOL ExecuteSQL(LPCTSTR szSql);
	BOOL ExecuteMulSQL(LPCTSTR szSql[], int iNum);

	BOOL ExecuteSQL(CString strSql);
	BOOL ExecuteMulSQL(CString strSql[],int iNum);

	MYSQL_RES* OpenRecordset(LPCTSTR szSql);  /* if you openrecordset(), you should closerecordset first,and then 
											  you can excute Sql sentence,or you just excute sql sentence fail...*/

	void CloseRecordset(MYSQL_RES *pResult);
	BOOL GetConnState();

private:
	MYSQL m_connMysql; // mysql 数据库连接句柄
	MYSQL_RES *m_pResult; // 当前记录集指针
	MYSQL_ROW m_row;    //当前记录集
	CRITICAL_SECTION	m_csLock; // 保证一次只执行一条SQL语句
	BOOL m_bConnState; // 连接数据库成功 为 TRUE,否则 FALSE
};
