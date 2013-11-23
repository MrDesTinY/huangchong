
#define	RETVAL_OK							0x0		///< 正确
#define RETVAL_DEPLOYMEMT_FAIL				0x1		///< 布防失败
#define	RETVAL_SET_ALARM_CALL_BACK_FAIL		0x2		///< 设置报警回调函数失败
#define RETVAL_CLEAN_ALARM_FAIL				0x3		///< 撤销布防失败
#define RETVAL_CANCEL						0x4		///< 取消

#define RETVAL_ERROR						-0x1	///< 错误
#define RETVAL_STOP_REALPLAY_ERROR			-0x2	///< 停止预览错误
#define RETVAL_LOGOUT_ERROR					-0x3	///< 登出设备失败
#define RETVAL_LOGIN_ERROR					-0x4	///< 登录设备失败
#define RETVAL_DEVICE_NOT_LOGIN				-0x5	///< 设备没有登录
#define RETVAL_REALPLAY_ERROR				-0x6	///< 预览失败
#define RETVAL_INVALID_PARAM				-0x7	///< 无效参数
#define RETVAL_GET_PLATE_INFO_ERROR			-0x8	///< 获取车牌信息失败
#define RETVAL_SAVE_PIC_ERROR				-0x9	///< 保存图片失败
#define RETVAL_HAVE_NO_IMAGE				-0xA	///< 没有图片
#define RETVAL_NOT_RECV_PIC					-0xB	///< 当前情况下不接受图片
#define RETVAL_SHOOT_ERROR					-0xC	///< 抓拍失败
#define RETVAL_DEVICE_CONNECT_ERROR			-0xD	///< 设备不正常



#define SCCP_MAX_PLATEINFO_NUM				1				///< 缓冲区最多保存车牌信息数目
#define SCCP_MAX_PIC_NUM					5				///< 图片缓冲区数目
#define SCCP_MAX_PIC_SIZE				(1024 * 1024 * 2)	///< 每张图片的最大大小

#define LOG_FILE_PATH					_T("C:\\ToHvShow_LogFile\\")		///< 日志文件保存路径

#define GET_PLATE_NUM						0x1				///< 获取车牌号码
#define GET_PLATE_COLOR						0x2				///< 获取车牌颜色
#define GET_CAR_NEAR_PIC					0x4				///< 获取车辆近景图片
#define GET_CAR_FAR_PIC						0x8				///< 获取远景图片
#define GET_PLATE_PIC						0x10			///< 获取车牌图片
#define GET_PLATE_BIN_PIC					0x20			///< 获取扯量二值图