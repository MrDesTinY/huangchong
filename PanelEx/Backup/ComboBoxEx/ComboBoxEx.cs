//****************************************
//功能描述：ComboBox的功能扩展，可以设置
//          边框宽度、颜色以及下拉分级和
//          图标
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

namespace ComboBoxEx
{
    public partial class ComboBoxEx : System.Windows.Forms.ComboBox, IMessageFilter//继承自ComboBox 
    {
        #region API函数
        [System.Runtime.InteropServices.DllImport("user32.dll ")]
        static extern IntPtr GetWindowDC(IntPtr hWnd);//返回hWnd参数所指定的窗口的设备环境。     

        [System.Runtime.InteropServices.DllImport("user32.dll ")]
        static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC); //函数释放设备上下文环境（DC）     
        int WM_PAINT = 0xf; //要求一个窗口重画自己,即Paint事件时     
        int WM_CTLCOLOREDIT = 0x133;//当一个编辑型控件将要被绘制时发送此消息给它的父窗口；    
        #endregion 

        #region 属性
        private ImageList _imageList;
        /// <summary>  
        /// 需要显示在选择项前的图标  
        /// </summary>  
        [
        Category("自定义属性"),
        Description("需要显示在选择项前的图标集合")
        ]
        public ImageList ImageList
        {
            get { return _imageList; }
            set { _imageList = value; }
        }
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
                IntPtr hDC = GetWindowDC(m.HWnd);
                if (hDC.ToInt32() == 0) //如果取设备上下文失败则返回     
                {
                    return;
                }
                //建立Graphics对像     
                Graphics g = Graphics.FromHdc(hDC);
                Pen p = new Pen(_bdColor, _bdSize);
                //画边框  
                g.DrawRectangle(p, 0, 0, Width-1, Height-1);
                ReleaseDC(m.HWnd, hDC);

                IntPtr hDC1 = GetWindowDC(m.HWnd);
                if (hDC1.ToInt32() == 0) //如果取设备上下文失败则返回     
                {
                    return;
                }
                Graphics g1 = Graphics.FromHdc(hDC1);
                Pen p1 = new Pen(_bdColor, _bdSize);
                g1.DrawRectangle(p1, Width - 20, 0, Width - 1, Height - 1);

                Point ulCorner = new Point(Width - 19, 1);
                Point urCorner = new Point(Width - 1, 1);
                Point llCorner = new Point(Width - 19, 21);
                Point[] destPara = { ulCorner, urCorner, llCorner };
                Image newImage = global::ComboBoxEx.Resources1.DropDown;
                g1.DrawImage(newImage, destPara);
                ReleaseDC(m.HWnd, hDC1);
            }
        }

        //重载OnDrawItem函数，来绘制组合框中每一列表项  
        protected override void OnDrawItem(DrawItemEventArgs ea)
        {
            ////鼠标划过时改变背景色
            //if (ea.State == DrawItemState.Selected)
            //{
            //    ea.Graphics.FillRectangle(new SolidBrush(Color.Beige), new RectangleF(ea.Bounds.X, ea.Bounds.Y, ea.Bounds.Width, ea.Bounds.Height));
            //}
            //else if (ea.State == DrawItemState.None)
            //{
            //    ea.Graphics.FillRectangle(new SolidBrush(Color.White), new RectangleF(ea.Bounds.X, ea.Bounds.Y, ea.Bounds.Width, ea.Bounds.Height));
            //}

            ea.DrawBackground();
            ea.DrawFocusRectangle();

            ImageComboBoxItem item;
            Size imageSize = new Size();
            if (ImageList != null)
            {
                imageSize = _imageList.ImageSize;
            }

            Rectangle bounds = ea.Bounds;

            try
            {
                item = (ImageComboBoxItem)Items[ea.Index];
                //如果当前项既有图像又有文本  
                if (item.ImageIndex != -1)
                {
                    //画图像  
                    _imageList.Draw(ea.Graphics, bounds.Left, bounds.Top, item.ImageIndex);
                    //绘制文本  
                    ea.Graphics.DrawString(item.Text, ea.Font, new
                    SolidBrush(ea.ForeColor), bounds.Left + imageSize.Width, bounds.Top);
                }
                else//只有文本，没有图像  
                {
                    //写文本  
                    ea.Graphics.DrawString(item.Text, ea.Font, new
                    SolidBrush(ea.ForeColor), bounds.Left, bounds.Top);
                }
            }
            catch
            {
                if (ea.Index != -1)
                {
                    DataRowView row = this.Items[ea.Index] as DataRowView;  // 取序号为e.Index的行对象
                    string itemText = Convert.ToString(row[this.DisplayMember]);  // 由DrawString()方法处理null
                    ea.Graphics.DrawString(itemText, ea.Font, new SolidBrush(ea.ForeColor), bounds.Left, bounds.Top);
                }

                //if (ea.Index != -1)
                //{
                //    ea.Graphics.DrawString(Items[ea.Index].ToString(), ea.Font, new
                //    SolidBrush(ea.ForeColor), bounds.Left, bounds.Top);
                //}
                //else
                //{
                //    ea.Graphics.DrawString(Text, ea.Font, new
                //    SolidBrush(ea.ForeColor), bounds.Left, bounds.Top);
                //}
            }
            
            base.OnDrawItem(ea);
        }

        #region IMessageFilter 成员
        /// <summary>  
        /// 消息筛选器  
        /// </summary>  
        /// <param name="m"></param>  
        /// <returns></returns>  
        public bool PreFilterMessage(ref Message m)
        {
            //拦截0x020A鼠标滚轮消息  
            if (m.Msg == 0x020A && _disableWheel)
                return true;
            return false;
        }
        #endregion  

        public ComboBoxEx()
        {
            InitializeComponent();
            DrawMode = DrawMode.OwnerDrawFixed;//设置绘画模式为手动  
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            // TODO: 在此处添加自定义绘制代码

            // 调用基类 OnPaint
            base.OnPaint(pe);
        }
    }
}
