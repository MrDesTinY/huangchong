//****************************************
//功能描述：DateTimePicker的功能扩展，可以设置
//          边框宽度、颜色等
//作者：    吴海飞
//日期：    2011-01-26
//****************************************
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DateTimePickerEx
{
    public partial class DateTimePickerEx : System.Windows.Forms.DateTimePicker
    {
        public DateTimePickerEx()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            // TODO: 在此处添加自定义绘制代码

            // 调用基类 OnPaint
            base.OnPaint(pe);
        }

        #region API函数
        [System.Runtime.InteropServices.DllImport("user32.dll ")]
        static extern IntPtr GetWindowDC(IntPtr hWnd);//返回hWnd参数所指定的窗口的设备环境。     

        [System.Runtime.InteropServices.DllImport("user32.dll ")]
        static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC); //函数释放设备上下文环境（DC）     
        int WM_PAINT = 0xf; //要求一个窗口重画自己,即Paint事件时     
        int WM_CTLCOLOREDIT = 0x133;//当一个编辑型控件将要被绘制时发送此消息给它的父窗口； 
        #endregion

        #region 属性
        private Color _bdColor;
        /// <summary>  
        /// 边框颜色  
        /// </summary>  
        [
        Category("自定义属性"),
        Description("设置边框颜色")
        ]
        public Color BorderColor
        {
            get { return _bdColor; }
            set { _bdColor = value; }
        }

        private int _bdSize = 1;
        /// <summary>  
        /// 边框粗细  
        /// </summary>  
        [
        Category("自定义属性"),
        Description("设置边框粗细")
        ]
        public int BorderSize
        {
            get { return _bdSize; }
            set { _bdSize = value; }
        }

        private bool _disableWheel = false;
        /// <summary>  
        /// 是否禁用鼠标滚轮  
        /// </summary>  
        [
        Category("自定义属性"),
        Description("是否禁用鼠标滚轮来更改当前选项")
        ]
        public bool DisableWheel
        {
            get { return _disableWheel; }
            set { _disableWheel = value; }
        }
        #endregion

        protected override void WndProc(ref   Message m)
        {
            base.WndProc(ref   m);
            if (m.Msg == WM_PAINT || m.Msg == WM_CTLCOLOREDIT)
            {
                //**********绘制边框*************
                IntPtr hDC = GetWindowDC(m.HWnd);
                if (hDC.ToInt32() == 0) //如果取设备上下文失败则返回     
                {
                    return;
                }
                //建立Graphics对像     
                Graphics g = Graphics.FromHdc(hDC);
                Pen p = new Pen(_bdColor, _bdSize);
                //画边框  
                g.DrawRectangle(p, 0, 0, Width - 1, Height - 1);
                ReleaseDC(m.HWnd, hDC);
                //*******************************

                //**********下拉图片*************
                //IntPtr hDC1 = GetWindowDC(m.HWnd);
                //if (hDC1.ToInt32() == 0) //如果取设备上下文失败则返回     
                //{
                //    return;
                //}
                //Graphics g1 = Graphics.FromHdc(hDC1);
                //Pen p1 = new Pen(_bdColor, _bdSize);
                ////g1.DrawRectangle(p1, Width - 20, 0, Width - 1, Height - 1);

                ////Point ulCorner = new Point(Width - 19, 1);
                ////Point urCorner = new Point(Width - 1, 1);
                ////Point llCorner = new Point(Width - 19, 20);

                //g1.DrawRectangle(p1, Width - 30, 0, Width - 10, Height - 1);

                //Point ulCorner = new Point(Width - 29, 1);
                //Point urCorner = new Point(Width - 10, 1);
                //Point llCorner = new Point(Width - 29, 20);

                //Point[] destPara = { ulCorner, urCorner, llCorner };
                //Image newImage = global::DateTimePickerEx.Resource1.DropDown;        
                //g1.DrawImage(newImage, destPara);
                //ReleaseDC(m.HWnd, hDC1);
                //********************************
            }
        }
    }
}
