#include "StdAfx.h"
#include ".\opmysql.h"

COpMySQL::COpMySQL(void)
{
	m_pResult = NULL;
	InitializeCriticalSection(&m_csLock);
	m_bConnState = FALSE;
}

COpMySQL::~COpMySQL(void)
{
	DeleteCriticalSection(&m_csLock);
}


BOOL COpMySQL::OpenConn(LPCTSTR szServerName,LPCTSTR szUserName,LPCTSTR szPassword,LPCTSTR szDBName,LPCTSTR szPort)
{
	try
	{
		char szError[512];
		EnterCriticalSection(&m_csLock);
		mysql_init(&m_connMysql);
		
		mysql_options(&m_connMysql, MYSQL_SET_CHARSET_NAME, "gb2312"); //使用GB2312字符集
		if(!mysql_real_connect(&m_connMysql,szServerName,szUserName,szPassword,szDBName,atoi(szPort),NULL,0))
		{
			sprintf(szError,"连接MYSQL数据库失败，错误原因:%s",mysql_error(&m_connMysql));
			OutputDebugString(szError);
			LeaveCriticalSection(&m_csLock);
			m_bConnState = FALSE;
			return FALSE;
		}
		
	}
	catch(CException *e)
	{
		char szException[512] ={0};
		char szError[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szException,"连接MYSQL数据库时发生异常,错误信息：%s",szError);
		
		OutputDebugString(szException);
		LeaveCriticalSection(&m_csLock);
		m_bConnState = FALSE;
		return FALSE;

	}
	LeaveCriticalSection(&m_csLock);
	m_bConnState = TRUE;
	return TRUE;	
}

void COpMySQL::CloseConn()
{
	try
	{
		mysql_close(&m_connMysql);
	}
	catch(CException *e)
	{
		char szError[512] = {0};
		char szMsg[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szMsg,"关闭MYSQL数据库时发生异常，错误信息:%s",szError);
		OutputDebugString(szError);
	}

	m_bConnState = FALSE;

	
}

BOOL COpMySQL::ExecuteSQL(LPCTSTR szSql)
{
	try
	{
		EnterCriticalSection(&m_csLock);
		
		int iResult = 0;
		iResult = mysql_real_query(&m_connMysql,(char*)szSql,strlen(szSql));
		
		if( iResult!= 0 )
		{
			
			char szError[512] ={0};
			sprintf(szError,"执行SQL语句失败，SQL:%s,错误原因:%s",szSql,mysql_error(&m_connMysql));
			OutputDebugString(szError);
			LeaveCriticalSection((&m_csLock));
			return FALSE;
		}

	}
	catch(CException *e)
	{
		char szError[512] = {0};
		char szMsg[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szMsg,"执行SQL语句时发生异常，错误信息:%s",szError);
		OutputDebugString(szError);
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
	LeaveCriticalSection(&m_csLock);
	return TRUE;

}

MYSQL_RES* COpMySQL::OpenRecordset(LPCTSTR szSql)
{
	try
	{
		char szTmp[512] ={0};
		EnterCriticalSection(&m_csLock);
		if(mysql_real_query(&m_connMysql,szSql,strlen(szSql)) != 0)
		{
			sprintf(szTmp,"查询记录集失败，SQL:%s,错误原因:%s",szSql,mysql_error(&m_connMysql));
			OutputDebugString(szTmp);
			LeaveCriticalSection(&m_csLock);
			return NULL;
		}
		if(!(m_pResult = mysql_use_result(&m_connMysql)) )
		{
			sprintf(szTmp,"读取记录集失败，错误原因:%s",mysql_error(&m_connMysql));
			OutputDebugString(szTmp);
			LeaveCriticalSection(&m_csLock);
			return NULL;
		}
	
	}
	catch(CException *e)
	{
		char szError[512] = {0};
		e->GetErrorMessage(szError,sizeof(szError));
		char szMsg[512] ={0};
		sprintf(szMsg,"读取记录集时发生异常，错误信息:%s",szError);
		OutputDebugString(szMsg);
		LeaveCriticalSection(&m_csLock);
		return NULL;

	}
	LeaveCriticalSection(&m_csLock);
	return m_pResult;
	

}

void COpMySQL::CloseRecordset(MYSQL_RES *pResult)
{
	if(pResult)
	{
		mysql_free_result(pResult);
		pResult = NULL;
	}
}

BOOL COpMySQL::GetConnState()
{
	return m_bConnState;
}

BOOL COpMySQL::ExecuteMulSQL(LPCTSTR szSql[], int iNum)
{
	char szTmp[512];
#ifdef _DEBUG	
	for(int i =0 ;i<iNum;i++)
	{
		sprintf(szTmp,"SQL[%d]:%s",i,szSql[i]);
		OutputDebugString(szTmp);
	}
#endif
	try
	{

		EnterCriticalSection(&m_csLock);
		mysql_autocommit(&m_connMysql,FALSE);
		for(int i = 0;i<iNum;i++)
		{
			if(mysql_real_query(&m_connMysql,szSql[i],strlen(szSql[i])) !=0)
			{
				sprintf(szTmp,"执行SQL语句失败，错误原因:%s",mysql_error(&m_connMysql));
				OutputDebugString(szTmp);
				mysql_rollback(&m_connMysql);
				LeaveCriticalSection(&m_csLock);
				return FALSE;
			}
		}
		
	
		
	}	
	catch (CException *e)
	{
		char szError[512] = {0};
		char szMsg[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szMsg,"执行多条SQL语句时发生异常，错误信息:%s",szError);
		OutputDebugString(szError);
		mysql_rollback(&m_connMysql);
		LeaveCriticalSection(&m_csLock);
		return FALSE;

	}
	mysql_commit(&m_connMysql);
	LeaveCriticalSection(&m_csLock);
	return TRUE;
	
}

BOOL COpMySQL::ExecuteMulSQL(CString strSql[],int iNum)
{
	CString strTmp;
#ifdef _DEBUG	
	for(int i =0 ;i<iNum;i++)
	{
		strTmp.Format("sql[%d]:%s",i,strSql[i]);
		OutputDebugString(strTmp);
	}
#endif

	try
	{
		EnterCriticalSection(&m_csLock);
		mysql_autocommit(&m_connMysql,FALSE);
		for(int i = 0;i<iNum;i++)
		{
			if(mysql_real_query(&m_connMysql,strSql[i].GetBuffer(0),strSql[i].GetLength()) !=0)
			{
				strTmp.Format("执行SQL语句失败，错误原因:%s",mysql_error(&m_connMysql));
				OutputDebugString(strTmp);
				mysql_rollback(&m_connMysql);
				LeaveCriticalSection(&m_csLock);
				return FALSE;
			}
		}
	}
	catch (CException *e)
	{
		char szError[512] = {0};
		char szMsg[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szMsg,"执行多条SQL语句时发生异常，错误信息:%s",szError);
		OutputDebugString(szError);
		mysql_rollback(&m_connMysql);
		LeaveCriticalSection(&m_csLock);
		return FALSE;

	}
	mysql_commit(&m_connMysql);
	LeaveCriticalSection(&m_csLock);
	return TRUE;

}

BOOL COpMySQL::ExecuteSQL(CString strSql)
{
	try
	{
		EnterCriticalSection(&m_csLock);

		int iResult = 0;
		iResult = mysql_real_query(&m_connMysql,strSql.GetBuffer(0),strSql.GetLength());

		if( iResult!= 0 )
		{

			char szError[512] ={0};
			sprintf(szError,"执行SQL语句失败，SQL:%s,错误原因:%s",strSql,mysql_error(&m_connMysql));
			OutputDebugString(szError);
			LeaveCriticalSection((&m_csLock));
			return FALSE;
		}

	}
	catch(CException *e)
	{
		char szError[512] = {0};
		char szMsg[512] ={0};
		e->GetErrorMessage(szError,sizeof(szError));
		sprintf(szMsg,"执行SQL语句时发生异常，错误信息:%s",szError);
		OutputDebugString(szError);
		LeaveCriticalSection(&m_csLock);
		return FALSE;
	}
	LeaveCriticalSection(&m_csLock);
	return TRUE;
}