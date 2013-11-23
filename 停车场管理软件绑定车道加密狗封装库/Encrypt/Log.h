#ifndef  __LOG_H__
#define  __LOG_H__
#include "Encrypt.h"

class  CLog
{
	#define  MAX_LOG_MSG_SIZE 1024
	#define  MAX_LOG_SIZE 5*1024*1024
	#define  LOG_FILE_NAME ""//"CryptLog.txt"
	#define  LOG_FILE_LEN  ( strlen( LOG_FILE_NAME ) )
	public:
		CLog();
		~CLog();
		//直接写日志
		virtual HRESULT WriteLog( const char * pChError );
	public:
		//传入错误信息代码
		virtual HRESULT PostError( HRESULT nError );
	private:
		//翻译错误信息
		virtual HRESULT TranslateError( HRESULT nError );
		//将日志信息写入文件
		virtual HRESULT  WriteError( void );
	
	protected:
		//错误信息存储空间
		char m_pMsgBuf[ MAX_LOG_MSG_SIZE ];
		//日志文件指针
		FILE *m_pFile;
		//消息长度
		int  m_nMsgLen;
};






#endif	 __LOG_H__