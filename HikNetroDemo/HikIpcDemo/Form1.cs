using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace HikIpcDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        Int32 iUid = -1;//用来保存登录相机返回的标志值
        Int32 lHandle = -1;//用来保存相机布控返回的句柄    
        Int32 lListenHandle = -1;

        private void initBtn_Click(object sender, EventArgs e)
        {           
            if (SdkClass.NET_DVR_Init() == 1)
            {
                this.loginBtn.Enabled = true;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = false;
                this.btnStartListen.Enabled = false;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = false;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnSetParam.Enabled = false;
                MessageBox.Show("初始化成功！");                
            }
            else
            {
                MessageBox.Show("初始化失败！");
            }
        }

        private void loginBtn_Click(object sender, EventArgs e)
        {
            SdkClass.NET_DVR_DEVICEINFO_V30 deviceinfo = new SdkClass.NET_DVR_DEVICEINFO_V30();
            //登录相机
            if ((iUid = SdkClass.NET_DVR_Login_V30(this.ipAddress.Text, 8000, this.txtUsername.Text, this.txtPsd.Text, deviceinfo)) < 0)
            {   
                MessageBox.Show("登录失败！");
            }
            else
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = true;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = true;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = true;
                this.btnSetParam.Enabled = true;
                MessageBox.Show("登录成功！");
            }
        }

        private void quitBtn_Click(object sender, EventArgs e)
        {
           
            if (SdkClass.NET_DVR_Cleanup() == 1)
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = true;
                this.alarmBtn.Enabled = false;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = false;
                this.btnStartListen.Enabled = false;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = false;
                this.btnSetParam.Enabled = false;
                MessageBox.Show("退出成功！");
            }
            else
            {
                MessageBox.Show("退出失败！");
            }
        }

        private void alarmBtn_Click(object sender, EventArgs e)
        {
            //实例化委托
            SdkClass.dgtMessageCallback message = new SdkClass.dgtMessageCallback(GetMessage);
            //注册回调函数
            SdkClass.NET_DVR_SetDVRMessageCallBack_V30(message, IntPtr.Zero);
            lHandle = SdkClass.NET_DVR_SetupAlarmChan_V30(iUid);
            if (lHandle < 0)
            {
                MessageBox.Show("布防失败");
            }
            else
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = false;
                this.closealarmBtn.Enabled = true;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = false;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = true;
                MessageBox.Show("布防成功！");
            }
        }

        private void closealarmBtn_Click(object sender, EventArgs e)
        {
            if (SdkClass.NET_DVR_CloseAlarmChan_V30(lHandle) == 0)
            {
                MessageBox.Show("撤防失败！");
            }
            else
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = true;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = true;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = true;
                MessageBox.Show("撤防成功！");
            }
        }
       
        int num = 0;
        //回调函数，用来获取车辆数据
        public void GetMessage(Int32 lCommand, ref SdkClass.NET_DVR_ALARMINFO_V30 pAlarmer, ref SdkClass.NET_DVR_PLATE_RESULT pAlarmInfo, Int32 dwBufLen, IntPtr pUser)
        {
            //智能高清IPC报警上传
            if (lCommand == 0x2800)
            {
                try
                {
                    if (pAlarmInfo.dwPicLen > 0 && pAlarmInfo.pBuffer1.ToInt32() != 0)
                    {
                        num++;
                        string path = string.Format("{0}\\{1}.jpg", Directory.GetCurrentDirectory().ToString(), num.ToString());
                        FileStream filesave = new FileStream(path, FileMode.OpenOrCreate);
                        byte[] _imageTemp = new byte[pAlarmInfo.dwPicLen];
                        Marshal.Copy(pAlarmInfo.pBuffer1, _imageTemp, 0, pAlarmInfo.dwPicLen);

                        filesave.Write(_imageTemp, 0, pAlarmInfo.dwPicLen);
                        filesave.Flush();
                        filesave.Close();
                        filesave = null;
                        this.pictureBox1.Image = Image.FromFile(path);
                        //settext(System.Text.Encoding.Default.GetString(pAlarmInfo.struPlateInfo.sLicense));
                        settext(pAlarmInfo.struPlateInfo.sLicense);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }             
            }  
        }


        //回调函数，用来获取Netro数据
        public void GetMessageNetro(Int32 lCommand, ref SdkClass.NET_DVR_ALARMINFO pAlarmer, IntPtr pAlarmInfo, Int32 dwBufLen, IntPtr pUser)
        {
            //Netro数据上传
            if (SdkClass.COMM_ITS_PARK_VEHICLE == lCommand)
            {
                SdkClass.NET_ITS_PARK_VEHICLE tmpPark = new SdkClass.NET_ITS_PARK_VEHICLE();
                tmpPark = (SdkClass.NET_ITS_PARK_VEHICLE)Marshal.PtrToStructure(pAlarmInfo, typeof(SdkClass.NET_ITS_PARK_VEHICLE));
                int picno = tmpPark.byPicNo;
            }
        }

        private void settext(string str)
        {
            if (label1.InvokeRequired)
            {
                label1.Invoke(new Action<string>(settext), str);
            }
            else
            {
                label1.Text = str;
            }
        }

        private void captureBtn_Click(object sender, EventArgs e)
        {
            SdkClass.NET_DVR_MANUALSNAP snap = new SdkClass.NET_DVR_MANUALSNAP();
            SdkClass.NET_DVR_PLATE_RESULT result = new SdkClass.NET_DVR_PLATE_RESULT();
            SdkClass.NET_DVR_ManualSnap(iUid,ref snap, ref result);
            if (result.dwPicLen > 0 && result.pBuffer4.ToInt32() != 0)
            {
                num++;
                string path = string.Format("{0}\\{1}.jpg", Directory.GetCurrentDirectory().ToString(), num.ToString());
                FileStream filesave = new FileStream(path, FileMode.OpenOrCreate);


                byte[] _imageTemp = new byte[result.dwPicLen];
                Marshal.Copy(result.pBuffer4, _imageTemp, 0, result.dwPicLen);

                filesave.Write(_imageTemp, 0, result.dwPicLen);
                filesave.Flush();
                filesave.Close();
                filesave = null;
                this.pictureBox1.Image = Image.FromFile(path);
                //settext(System.Text.Encoding.Default.GetString(result.struPlateInfo.sLicense));
                settext(result.struPlateInfo.sLicense);
            }        
        }

        private void btnLoginOut_Click(object sender, EventArgs e)
        {
            if (1 == SdkClass.NET_DVR_Logout_V30(iUid))
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = false;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = false;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = false;
                this.btnSetParam.Enabled = false;
                MessageBox.Show("登出成功！");
            }
            else
            {
                MessageBox.Show("登出失败！");
            }
        }

        private void btnStartListen_Click(object sender, EventArgs e)
        {
            SdkClass.MSGCallBack message = new SdkClass.MSGCallBack(GetMessageNetro);
            short listenport = 0;
            Int16.TryParse(this.tbListenPort.Text.ToString(), out listenport);
            if ((lListenHandle = SdkClass.NET_DVR_StartListen_V30(this.ipaddrListenIP.Text, listenport, message, IntPtr.Zero)) >= 0)
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = false;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = false;
                this.btnStopListen.Enabled = true;
                this.btnLoginOut.Enabled = true;
                MessageBox.Show("开始监听成功！");
            }
            else
            {
                MessageBox.Show("开始监听失败！");
            }
        }

        private void btnStopListen_Click(object sender, EventArgs e)
        {
            if (SdkClass.NET_DVR_StopListen_V30(lListenHandle) == 1)
            {
                this.loginBtn.Enabled = false;
                this.initBtn.Enabled = false;
                this.alarmBtn.Enabled = true;
                this.closealarmBtn.Enabled = false;
                this.quitBtn.Enabled = true;
                this.btnStartListen.Enabled = true;
                this.btnStopListen.Enabled = false;
                this.btnLoginOut.Enabled = true;
                MessageBox.Show("停止监听成功！");
            }
            else
            {
                MessageBox.Show("停止监听失败！");
            }
        }

        private void btnSetParam_Click(object sender, EventArgs e)
        {
            SdkClass.NET_ITS_UPLOAD_CFG struUploadCfg = new SdkClass.NET_ITS_UPLOAD_CFG();
            IntPtr pstruUploadCfg = Marshal.AllocHGlobal(Marshal.SizeOf(struUploadCfg));
            Marshal.StructureToPtr(struUploadCfg, pstruUploadCfg, true);
            int dwReturned = 0;
            int ret = -1 ;
            if ((ret = SdkClass.NET_DVR_GetDVRConfig(iUid, SdkClass.NET_ITS_GET_UPLOAD_CFG, -1, pstruUploadCfg, Marshal.SizeOf(struUploadCfg), ref dwReturned)) == 1)
            {
                struUploadCfg = (SdkClass.NET_ITS_UPLOAD_CFG)Marshal.PtrToStructure(pstruUploadCfg, typeof(SdkClass.NET_ITS_UPLOAD_CFG));
                struUploadCfg.struRemoteDataHost1.struHostAddr.sIpV4 = this.ipaddrListenIP.Text == "..." ? "0.0.0.0" : this.ipaddrListenIP.Text;
                try
                {
                    struUploadCfg.struRemoteDataHost1.wHostPort = Convert.ToInt16(this.tbListenPort.Text.ToString());
                    struUploadCfg.struRemoteDataHost1.wUploadInterval = Convert.ToInt16(this.tbUpIntTime.Text.ToString());
                    struUploadCfg.struRemoteDataHost1.dwUploadTimeOut = Convert.ToInt16(this.tbUpExtraTime.Text.ToString());
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("参数输入异常，原因是：" + ex.ToString());
                }
                struUploadCfg.struRemoteDataHost1.byUploadStrategy = 0;
                struUploadCfg.struRemoteDataHost1.bySuspendUpload = this.chbSuspend.Checked?(byte)0:(byte)1;
                struUploadCfg.struRemoteDataHost1.dwDataType |= 0x01;//卡口数据	
                Marshal.StructureToPtr(struUploadCfg, pstruUploadCfg, false);
                if ((ret = SdkClass.NET_DVR_SetDVRConfig(iUid, SdkClass.NET_ITS_SET_UPLOAD_CFG, -1, pstruUploadCfg, Marshal.SizeOf(struUploadCfg))) == 1)
                {
                    if (SdkClass.NET_DVR_RebootDVR(iUid) == 1)
                    {
                        MessageBox.Show("设置参数成功！等待重启完成，重新运行软件!");
                    }
                    else
                    {
                        MessageBox.Show("设置参数成功！手动重启后生效!");
                    }
                }
                else
                {
                    ret = SdkClass.NET_DVR_GetLastError();
                    MessageBox.Show("设置参数失败！错误码为：" + ret);
                }
            }
            else
            {
                ret = SdkClass.NET_DVR_GetLastError();
                MessageBox.Show("获取参数失败！错误码为：" + ret);
            }
            Marshal.FreeHGlobal(pstruUploadCfg);//释放分配的非托管内存。
        }
    }
}