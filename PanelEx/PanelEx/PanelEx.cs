using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.IO;

namespace PanelEx
{
    public partial class PanelEx : Panel
    {
        #region 局部变量
        private bool _LeftBorder = true;
        private Color _LeftBorderColor = Color.AliceBlue;
        private float _LeftBorderWidth = 1;
        private DashStyle _LeftBorderStyle = DashStyle.Solid;

        private bool _RightBorder = true;
        private Color _RightBorderColor = Color.AliceBlue;
        private float _RightBorderWidth = 1;
        private DashStyle _RightBorderStyle = DashStyle.Solid;

        private bool _TopBorder = true;
        private Color _TopBorderColor = Color.AliceBlue;
        private float _TopBorderWidth = 1;
        private DashStyle _TopBorderStyle = DashStyle.Solid;

        private bool _BottomBorder = true;
        private Color _BottomBorderColor = Color.AliceBlue;
        private float _BottomBorderWidth = 1;
        private DashStyle _BottomBorderStyle = DashStyle.Solid;

        private CornerMode _CornerMode = CornerMode.Monoceras;
        private float _MonocerasSize = 1;

        /// <summary>
        /// 设置边角大小
        /// </summary>
        [Description("设置边角大小"), Category("外观")]
        public float MonocerasSize
        {
            get { return _MonocerasSize; }
            set { _MonocerasSize = value; }
        }
        
        /// <summary>
        /// 设置边角样式
        /// </summary>
        [Description("设置边角样式"), Category("外观")]
        public CornerMode CornerMode
        {
            get { return _CornerMode; }
            set { _CornerMode = value; }
        }

        /// <summary>
        /// 左边框是否显示
        /// </summary>
        //[DefaultValue("True"), Description("是否显示左边框"), Category("外观"),Browsable(false)] 
        [Description("是否显示左边框"), Category("外观")]
        public bool LeftBorder
        {
            get { return _LeftBorder; }
            set { _LeftBorder = value; }
        }

        /// <summary>
        /// 左边框颜色
        /// </summary>
        [Description("左边框颜色"), Category("外观")]
        public Color LeftBorderColor
        {
            get { return _LeftBorderColor; }
            set { _LeftBorderColor = value; }
        }

        /// <summary>
        /// 左边框宽度
        /// </summary>
        [Description("左边框宽度"), Category("外观")]
        public float LeftBorderWidth
        {
            get { return _LeftBorderWidth; }
            set { _LeftBorderWidth = value; }
        }

        /// <summary>
        /// 左边框样式
        /// </summary>
        [Description("左边框样式"), Category("外观")]
        public DashStyle LeftBorderStyle
        {
            get { return _LeftBorderStyle; }
            set { _LeftBorderStyle = value; }
        }
        


        /// <summary>
        /// 是否显示右边框
        /// </summary>
        [Description("是否显示右边框"), Category("外观")]
        public bool RightBorder
        {
            get { return _RightBorder; }
            set { _RightBorder = value; }
        }

        /// <summary>
        /// 右边框颜色
        /// </summary>
        [Description("右边框颜色"), Category("外观")]
        public Color RightBorderColor
        {
            get { return _RightBorderColor; }
            set { _RightBorderColor = value; }
        }

        /// <summary>
        /// 右边框宽度
        /// </summary>
        [Description("右边框宽度"), Category("外观")]
        public float RightBorderWidth
        {
            get { return _RightBorderWidth; }
            set { _RightBorderWidth = value; }
        }

        /// <summary>
        /// 右边框样式
        /// </summary>
        [Description("右边框样式"), Category("外观")]
        public DashStyle RightBorderStyle
        {
            get { return _RightBorderStyle; }
            set { _RightBorderStyle = value; }
        }



        /// <summary>
        /// 是否显示上边框
        /// </summary>
        [Description("是否显示上边框"), Category("外观")]
        public bool TopBorder
        {
            get { return _TopBorder; }
            set { _TopBorder = value; }
        }

        /// <summary>
        /// 上边框颜色
        /// </summary>
        [Description("上边框颜色"), Category("外观")]
        public Color TopBorderColor
        {
            get { return _TopBorderColor; }
            set { _TopBorderColor = value; }
        }

        /// <summary>
        /// 上边框宽度
        /// </summary>
        [Description("上边框宽度"), Category("外观")]
        public float TopBorderWidth
        {
            get { return _TopBorderWidth; }
            set { _TopBorderWidth = value; }
        }

        /// <summary>
        /// 上边框样式
        /// </summary>
        [Description("上边框样式"), Category("外观")]
        public DashStyle TopBorderStyle
        {
            get { return _TopBorderStyle; }
            set { _TopBorderStyle = value; }
        }



        /// <summary>
        /// 是否显示下边框
        /// </summary>
        [Description("是否显示下边框"), Category("外观")]
        public bool BottomBorder
        {
            get { return _BottomBorder; }
            set { _BottomBorder = value; }
        }

        /// <summary>
        /// 下边框颜色
        /// </summary>
        [Description("下边框颜色"), Category("外观")]
        public Color BottomBorderColor
        {
            get { return _BottomBorderColor; }
            set { _BottomBorderColor = value; }
        }

        /// <summary>
        /// 下边框宽度
        /// </summary>
        [Description("下边框宽度"), Category("外观")]
        public float BottomBorderWidth
        {
            get { return _BottomBorderWidth; }
            set { _BottomBorderWidth = value; }
        }

        /// <summary>
        /// 下边框样式
        /// </summary>
        [Description("下边框样式"), Category("外观")]
        public DashStyle BottomBorderStyle
        {
            get { return _BottomBorderStyle; }
            set { _BottomBorderStyle = value; }
        }



        /// <summary>
        /// 原边框样式
        /// </summary>
        [Description("原边框样式"), Category("外观"), Browsable(false)]
        public new BorderStyle BorderStyle
        {
            get { return BorderStyle.None; }
            set { base.BorderStyle = BorderStyle.None; }
        }
        #endregion


        #region 构造函数
        /// <summary>
        /// 构造函数
        /// </summary>
        public PanelEx()
        {
            base.ResizeRedraw = true;
            //base.DoubleBuffered   =   true;   
        }
        #endregion


        #region 控件重绘事件
        /// <summary>
        /// 控件重绘事件
        /// </summary>
        /// <param name="e"></param>
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            this.PaintBorder();
        }
        #endregion

        public static void WriteToFile(string item,string errorContent)
        {
            StreamWriter sw = null;
            DateTime date = DateTime.Now;
            string FileName = string.Empty;
            try
            {
                FileName = "./Logs/GeneralDocPdf/" + "PanelEx" + "_" + date.Month + date.Day + ".log";
                if (!File.Exists(FileName))
                {
                    sw = File.CreateText(FileName);//不存在该文件，就创建并添加内容
                }
                else
                {
                    sw = File.AppendText(FileName);//日志文件已经存在，则向该文件追加内容
                }
                sw.WriteLine(item);
                sw.WriteLine(errorContent);//写入行
                sw.WriteLine("【Time】" + System.DateTime.Now);
                sw.WriteLine("≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡");
                sw.Flush();
            }
            catch (System.Exception ex)
            {
            	
            }
            finally
            {
                if (sw != null)
                    sw.Close();
            }

        }



        #region 绘制边框
        /// <summary>
        /// 绘制边框
        /// </summary>
        protected virtual void PaintBorder()
        {
            Graphics g = this.CreateGraphics();
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            Pen pen = null;
            try
            {
                switch (_CornerMode)
                {
                    case CornerMode.Monoceras:
                        if (_LeftBorder)
                        {
                            pen = new Pen(_LeftBorderColor, _LeftBorderWidth);
                            pen.DashStyle = _LeftBorderStyle;
                            g.DrawLine(pen, 0, 0, 0, this.Height - 1);
                            //pen.Dispose();
                            //g.Dispose();
                        }

                        if (_BottomBorder)
                        {
                            pen = new Pen(_BottomBorderColor, _BottomBorderWidth);
                            pen.DashStyle = _BottomBorderStyle;
                            g.DrawLine(pen, 0, this.Height - 1, this.Width - 1, this.Height - 1);
                            //pen.Dispose();
                            //g.Dispose();
                        }

                        if (_RightBorder)
                        {
                            pen = new Pen(_RightBorderColor, _RightBorderWidth);
                            pen.DashStyle = _RightBorderStyle;
                            g.DrawLine(pen, Width - 1, this.Height - 1, this.Width - 1, 0);
                            //pen.Dispose();
                            //g.Dispose();
                        }

                        if (_TopBorder)
                        {
                            pen = new Pen(_TopBorderColor, _TopBorderWidth);
                            pen.DashStyle = _TopBorderStyle;
                            g.DrawLine(pen, this.Width - 1, 0, 0, 0);
                            //pen.Dispose();
                            //g.Dispose();
                        }
                        break;
                    case CornerMode.Fillet:
                        pen = new Pen(_LeftBorderColor, _LeftBorderWidth);
                        pen.DashStyle = _LeftBorderStyle;
                        g.DrawLine(pen, 0, _MonocerasSize, 0, this.Height - _MonocerasSize - 1);
                        g.DrawLine(pen, 0, this.Height - _MonocerasSize - 1, _MonocerasSize, this.Height - 1);
                        g.DrawLine(pen, _MonocerasSize, this.Height - 1, this.Width - _MonocerasSize - 1, this.Height - 1);
                        g.DrawLine(pen, this.Width - _MonocerasSize - 1, this.Height - 1, this.Width - 1, this.Height - _MonocerasSize - 1);
                        g.DrawLine(pen, this.Width - 1, this.Height - _MonocerasSize - 1, this.Width - 1, _MonocerasSize);
                        g.DrawLine(pen, this.Width - 1, _MonocerasSize, this.Width - _MonocerasSize - 1, 0);
                        g.DrawLine(pen, this.Width - _MonocerasSize - 1, 0, _MonocerasSize, 0);
                        g.DrawLine(pen, _MonocerasSize, 0, 0, _MonocerasSize);
                        //pen.Dispose();
                        //g.Dispose();
                        break;
                }
            }
            catch (System.Exception ex)
            {
                if (pen != null)
                    pen.Dispose();
                if (g != null)
                    g.Dispose();
            }
            finally
            {
                if (pen != null)
                    pen.Dispose();
                if (g != null)
                    g.Dispose();
            }
        }
        #endregion

        /////   <summary>   
        /////   绘制填充区域   
        /////   </summary>   
        //protected virtual void PaintRegion(Color start, Color end, float angle)
        //{
        //    Graphics g = this.CreateGraphics();
        //    g.SmoothingMode = SmoothingMode.AntiAlias;
        //    Brush brush;

        //    brush = new LinearGradientBrush(this.ClientRectangle, end, start, angle);
        //    RectangleF top = new RectangleF(1, 1, this.Width - 3, (this.Height - 2) * this.OnePart / 100);
        //    g.FillRectangle(brush, top);

        //    brush = new LinearGradientBrush(this.ClientRectangle, start, end, angle);
        //    RectangleF down = new RectangleF(top.Left, top.Bottom, (float)(this.Width - 3), (float)(this.Height - top.Height - 3));
        //    g.FillRectangle(brush, down);
        //}


        ///   <summary>   
        ///   获取矩形顶点数组   
        ///   </summary>   
        ///   <param   name="rect"></param>   
        ///   <returns></returns>   
        protected Point[] GetPoints(Rectangle rect)
        {
            return new Point[]   
                          {   
                                  new   Point(rect.Left,   rect.Top),           //左上顶点   
                                  new   Point(rect.Right,   rect.Top),         //右上顶点   
                                  new   Point(rect.Right,   rect.Bottom),   //右下顶点   
                                  new   Point(rect.Left,   rect.Bottom)       //坐下顶点   
                          };
        }


    }



    /// <summary>
    /// 指定panel的边角样式
    /// </summary>
    public enum CornerMode
    {
        /// <summary>
        /// 指定圆角
        ///  此时的边框设置以上边框为准
        /// </summary>
        Fillet =1,

        /// <summary>
        /// 指定尖角
        /// </summary>
        Monoceras = 2,
    }

 
}
