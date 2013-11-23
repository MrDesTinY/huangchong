using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace HikIpcDemo
{
    public class SdkClass
    {
        public const int NET_ITS_GET_UPLOAD_CFG = 5065;//获取数据上传配置
        public const int NET_ITS_SET_UPLOAD_CFG = 5066;//设置数据上传配置
        public const int COMM_ITS_PARK_VEHICLE = 0x3056;//回调信号
        #region 结构体
        //手动抓拍请求参数结构体
        [StructLayout(LayoutKind.Sequential)]
        public struct NET_DVR_MANUALSNAP
        {
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 24, ArraySubType = UnmanagedType.I1)]
            public byte[] byRes;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct NET_DVR_DEVICEINFO_V30
        {
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 48, ArraySubType = UnmanagedType.I1)]
            public byte[] sSerialNumber;  //序列号
            public byte byAlarmInPortNum;		        //报警输入个数
            public byte byAlarmOutPortNum;		        //报警输出个数
            public byte byDiskNum;				    //硬盘个数
            public byte byDVRType;				    //设备类型, 1:DVR 2:ATM DVR 3:DVS ......
            public byte byChanNum;				    //模拟通道个数
            public byte byStartChan;			        //起始通道号,例如DVS-1,DVR - 1
            public byte byAudioChanNum;                //语音通道数
            public byte byIPChanNum;					//最大数字通道个数  
            public byte byZeroChanNum;			//零通道编码个数 //2010-01-16
            public byte byMainProto;			//主码流传输协议类型 0-private, 1-rtsp
            public byte bySubProto;				//子码流传输协议类型0-private, 1-rtsp
            public byte bySupport;        //能力，位与结果为0表示不支持，1表示支持，    
            public byte bySupport1;        // 能力集扩充，位与结果为0表示不支持，1表示支持						
            public byte byRes1;
            Int16 wDevType;              //设备型号
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.I1)]
            public byte[] byRes2;					//保留    
        };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct NET_DVR_ALARMINFO
        {
            public int dwAlarmType;
            public int dwAlarmInputNumber;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = System.Runtime.InteropServices.UnmanagedType.I4)]
            public int[] dwAlarmOutputNumber;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst = 4, ArraySubType = System.Runtime.InteropServices.UnmanagedType.I4)]
            public int[] dwAlarmRelateChannel;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = System.Runtime.InteropServices.UnmanagedType.I4)]
            public int[] dwChannel;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = System.Runtime.InteropServices.UnmanagedType.I4)]
            public int[] dwDiskNumber;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct NET_DVR_ALARMINFO_V30
        {
            Int32 dwAlarmType;
            Int32 dwAlarmInputNumber;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 96, ArraySubType = UnmanagedType.I1)]
            byte[] byAlarmOutputNumber;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 64, ArraySubType = UnmanagedType.I1)]
            byte[] byAlarmRelateChannel;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 64, ArraySubType = UnmanagedType.I1)]
            byte[] byChannel;
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 33, ArraySubType = UnmanagedType.I1)]
            byte[] byDiskNumber;
        };

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct NET_VCA_RECT
        {
            public float fX;
            public float fY;
            public float fWidth;
            public float fHeight;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_ITS_PICTURE_INFO
        {
            public int dwDataLen;
            public byte byType;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 3)]
            public string byRes1;
            public int dwRedLightTime;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 32)]
            public string byAbsTime;
            public NET_VCA_RECT struPlateRect;
            public NET_VCA_RECT struPlateRecgRect;
            public System.IntPtr pBuffer;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 12)]
            public string byRes2;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_DVR_PLATE_INFO
        {
            public byte byPlateType;
            public byte byColor;
            public byte byBright;
            public byte byLicenseLen;
            public byte byEntireBelieve;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 35)]
            public string byRes;
            public NET_VCA_RECT struPlateRect;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string sLicense;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string byBelieve;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_DVR_VEHICLE_INFO
        {
            public int dwIndex;
            public byte byVehicleType;
            public byte byColorDepth;
            public byte byColor;
            public byte byRes1;
            public short wSpeed;
            public short wLength;
            public byte byIllegalType;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 3)]
            public string byRes2;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string byCustomInfo;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string byRes3;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_ITS_PARK_VEHICLE
        {
            public int dwSize;
            public byte byGroupNum;
            public byte byPicNo;
            public byte byLocationNum;
            public byte byParkError;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string byParkingNo;
            public byte byLocationStatus;
            public byte bylogicalLaneNum;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 6)]
            public string byRes0;
            public int dwChanIndex;
            public NET_DVR_PLATE_INFO struPlateInfo;
            public NET_DVR_VEHICLE_INFO struVehicleInfo;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 48)]
            public string byMonitoringSiteID;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 48)]
            public string byDeviceID;
            public int dwPicNum;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = System.Runtime.InteropServices.UnmanagedType.Struct)]
            public NET_ITS_PICTURE_INFO[] struPicInfo;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 256)]
            public string byRes2;
        }


        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_DVR_IPADDR
        {
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 16)]
            public string sIpV4;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 128)]
            public string byIPv6;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential, CharSet = System.Runtime.InteropServices.CharSet.Ansi)]
        public struct NET_ITS_TRAFFIC_DATA_HOST
        {
            public NET_DVR_IPADDR struHostAddr;
            public short wHostPort;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 2)]
            public string byRes1;
            public int dwDataType;
            public byte bySuspendUpload;
            public byte byUploadStrategy;
            public short wUploadInterval;
            public int dwUploadTimeOut;
            [System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.ByValTStr, SizeConst = 24)]
            public string byRes;
        }

        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct NET_ITS_UPLOAD_CFG
        {
            public int dwSize;
            public NET_ITS_TRAFFIC_DATA_HOST struRemoteDataHost1;
            public NET_ITS_TRAFFIC_DATA_HOST struRemoteDataHost2;
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        [StructLayout(LayoutKind.Sequential)]
        public struct NET_DVR_PLATE_RESULT
        {
            public Int32 dwSize;						//结构长度
            public byte byResultType;			//0-视频识别结果，-图像识别结果
            public byte byChanIndex;			//通道号
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 2, ArraySubType = UnmanagedType.I1)]
            public byte[] byRes1;				//保留
            public Int32 dwRelativeTime;		//相对时间点
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 32, ArraySubType = UnmanagedType.I1)]
            public byte[] byAbsTime;		//绝对时间点,yyyymmddhhmmssxxx,e.g.20090810235959999（毫秒）
            public Int32 dwPicLen;					//图片长度
            public Int32 dwPicPlateLen;			//车牌小图片长度
            public Int32 dwVideoLen;					//录像内容长度
            public byte byTrafficLight;				//0-非红绿灯抓拍，1-绿灯时抓拍；2-红灯时抓拍
            public byte byPicNum;				//连拍的图片序号
            public byte byDriveChan;				//触发车道号
            public byte byRes2;                      //保留
            public Int32 dwBinPicLen; //二值图长度
            public Int32 dwCarPicLen;//近景图长度
            public Int32 dwFarCarPicLen;  //远景图长度
            public IntPtr pBuffer3;   //车牌二值图
            public IntPtr pBuffer4;  //近景图
            public IntPtr pBuffer5;  //远景图
            [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 8, ArraySubType = UnmanagedType.I1)]
            public byte[] byRes3;                      //保留 
            public NET_DVR_PLATE_INFO struPlateInfo;    //车牌信息结构
            public NET_DVR_VEHICLE_INFO struVehicleInfo; //车辆信息
            public IntPtr pBuffer1;
            public IntPtr pBuffer2;                  // 当上传的是图片时，指向车牌图片的指针（DVS车牌彩图）
        };

        //[StructLayout(LayoutKind.Sequential)]
        //public struct NET_DVR_PLATE_INFO
        //{
        //    public byte byPlateType;
        //    public byte byColor;
        //    public byte byBright;
        //    public byte byLicenseLen;
        //    public byte byEntireBelieve;
        //    [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 35, ArraySubType = UnmanagedType.I1)]
        //    public byte[] byRes;
        //    NET_VCA_RECT struPlateRect;
        //    [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.I1)]
        //    public byte[] sLicense;
        //    [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.I1)]
        //    public byte[] byBelieve;
        //};
        //public struct NET_VCA_RECT
        //{
        //    public float fX;
        //    public float fY;
        //    public float fWidth;
        //    public float fHeight;
        //};
        //[StructLayout(LayoutKind.Sequential)]
        //public struct NET_DVR_VEHICLE_INFO
        //{
        //    public Int32 dwIndex;
        //    public byte byVehicleType;
        //    public byte byColorDepth;
        //    public byte byColor;
        //    public byte byRes1;
        //    public Int16 wSpeed;
        //    public Int16 wLength;
        //    public byte byIllegalType;
        //    [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 35, ArraySubType = UnmanagedType.I1)]
        //    public byte[] byRes;
        //};
        #endregion
        #region DLL接口
        [UnmanagedFunctionPointerAttribute(CallingConvention.StdCall)]
        public delegate void dgtMessageCallback(Int32 lCommand,ref NET_DVR_ALARMINFO_V30 pAlarmer,ref NET_DVR_PLATE_RESULT pAlarmInfo,Int32 dwBufLen,IntPtr pUser);

        [UnmanagedFunctionPointerAttribute(CallingConvention.StdCall)]
        public delegate void MSGCallBack(Int32 lCommand, ref NET_DVR_ALARMINFO pAlarmer, IntPtr pAlarmInfo, Int32 dwBufLen, IntPtr pUser);


        const string dllName = "HCNetSDK.dll";
        [DllImport(dllName)]
        public static extern int NET_DVR_Init();

        [DllImport(dllName)]
        public static extern int NET_DVR_Login_V30(string sDVRIP, Int16 wDVRPort, string sUserName, string sPassword, NET_DVR_DEVICEINFO_V30 lpDeviceInfo);

        [DllImport(dllName)]
        public static extern int NET_DVR_SetDVRMessageCallBack_V30(dgtMessageCallback fMessageCallBack, IntPtr pUser);

        [DllImport(dllName)]
        public static extern int NET_DVR_StartListen_V30(string sLocalIP, short wLocalPort, MSGCallBack DataCallback, IntPtr pUserData);

        [DllImport(dllName)]
        public static extern int NET_DVR_StopListen_V30(int lListenHandle);

        [DllImport(dllName)]
        public static extern int NET_DVR_SetDVRConfig(int lUserID, int dwCommand, int lChannel, IntPtr lpInBuffer, int dwInBufferSize);

        [DllImport(dllName)]
        public static extern int NET_DVR_GetDVRConfig(int lUserID, int dwCommand, int lChannel, IntPtr lpOutBuffer, int dwInBufferSize, ref int lpBytesReturned);

        [DllImport(dllName)]
        public static extern int NET_DVR_SetupAlarmChan_V30(int lUserID);

        [DllImport(dllName)]
        public static extern int NET_DVR_CloseAlarmChan_V30(int lHandle);

        [DllImport(dllName)]
        public static extern int NET_DVR_Logout_V30(int lUserID);

        [DllImport(dllName)]
        public static extern int NET_DVR_RebootDVR(int lUserID);

        [DllImport(dllName)]
        public static extern int NET_DVR_Cleanup();

        [DllImport(dllName)]
        public static extern int NET_DVR_ManualSnap(int lUserID, ref NET_DVR_MANUALSNAP lpInter, ref NET_DVR_PLATE_RESULT lpOuter);

        [DllImport(dllName)]
        public static extern int NET_DVR_GetLastError();

        #endregion
    }
}
