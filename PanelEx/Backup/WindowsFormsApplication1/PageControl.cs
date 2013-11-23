using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class PageControl : UserControl
    {
        //public PageControl()
        //{
        //    InitializeComponent();
        //}
                /**//// <summary> 
        /// Required designer variable.
        /// </summary>
        private int pageCount;
        private int currPage;
    //    private int pageSize;
        private int pageShow;
        private int xDistance;
        private bool bolPrev;
        private bool bolNext;
        private int start, end;

        private EventHandler lnkLblClicked;

        public event EventHandler LnkLblClicked
        {
            add{lnkLblClicked += value;}
            remove{lnkLblClicked -= value;}
        }
        protected virtual void OnLnkLblClicked(EventArgs e)
        {
            if (lnkLblClicked != null)
                lnkLblClicked(this,e);                
        }

        #region Field
        public int PageCount
        {
            get{return pageCount;}
            set{pageCount = value;}        
        }
        public int CurrPage
        {
            get{return currPage;}
            set{currPage = value;}
        }
//        public int PageSize
//        {
//            get{return pageSize;}
//            set{pageSize = value;}
//        }
        public int PageShow
        {
            get{return pageShow;}
            set{pageShow = value;}
        }
        public int XDistance
        {
            get{return xDistance;}
            set{xDistance = value;}
        }
        #endregion
        

        //private System.ComponentModel.Container components = null;

        public PageControl()
        {
            // This call is required by the Windows.Forms Form Designer.
            InitializeComponent();

            // TODO: Add any initialization after the InitializeComponent call

        }
        public PageControl(int pageCount,int pageSize,int pageShow):this()
        {
            this.pageCount = pageCount;
        //    this.pageSize = pageSize;
            this.pageShow = pageShow;
        }

        /**//// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        //protected override void Dispose( bool disposing )
        //{
        //    if( disposing )
        //    {
        //        if(components != null)
        //        {
        //            components.Dispose();
        //        }
        //    }
        //    base.Dispose( disposing );
        //}

        #region Component Designer generated code
        /**//// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        //private void InitializeComponent()
        //{
        //    this.SuspendLayout();
        //    // 
        //    // PageControl
        //    // 
        //    this.Name = "PageControl";
        //    this.Size = new System.Drawing.Size(320, 24);
        //    this.Load += new System.EventHandler(this.PageControl_Load);
        //    this.ResumeLayout(false);

        //}
        #endregion

        private void PageControl_Load(object sender, System.EventArgs e)
        {
            xDistance = 0;
            bolPrev = pageCount > pageShow;
        //    currPage = pageCount;
            end = pageCount;
            start = Math.Max(1,end - pageShow + 1);
            DrawControl();
        }
        private void DrawControl()
        {
            this.Controls.Clear();
            System.Windows.Forms.Label lblPageCount = new Label();
            lblPageCount.Text = "共" + pageCount + "页";
            this.AddControl(lblPageCount,new Point(8,8),new Size(40,16));
            int x = lblPageCount.Bounds.Right;
            int y = lblPageCount.Location.Y;
            System.Windows.Forms.Control currCtl = lblPageCount;

            //calculate page start and end
            int pageShowCount = Math.Min(pageCount,pageShow);
//            start = Math.Max(1,currPage - pageShow/2);
//            end = start + pageShowCount -1;
//            if (pageCount - currPage < pageShow/2)
//            {
//                end = pageCount;
//                start = pageCount - pageShowCount + 1;
//            }
            bolPrev = start != 1;
            bolNext = end != pageCount;

            if (bolPrev)
            {//add Linklabel "prev"
                System.Windows.Forms.LinkLabel lblPrev = new LinkLabel();
                lblPrev.Text = "前一页";
                x += xDistance;
                ((LinkLabel)lblPrev).LinkClicked += new LinkLabelLinkClickedEventHandler(lblCtl_Clicked);
                this.AddControl(lblPrev,new Point(x,y),new Size(45, 16));
                currCtl = lblPrev;
            }
            
            for (int i = start; i <= end; i ++)
            {// add linklabel "page"
                x = currCtl.Bounds.Right;
                System.Windows.Forms.Control lblCtl = null;
                if ( i == currPage)
                    lblCtl = new System.Windows.Forms.Label();
                else
                {
                    lblCtl = new System.Windows.Forms.LinkLabel();
                    ((LinkLabel)lblCtl).LinkClicked += new LinkLabelLinkClickedEventHandler(lblCtl_Clicked);
                }
                lblCtl.Text = i.ToString();
                x += xDistance;
                this.AddControl(lblCtl,new Point(x,y),new Size(10, 16));
                currCtl = lblCtl;
            }
            if (bolNext)
            {// add linklabel "next"
                System.Windows.Forms.LinkLabel lblNext = new LinkLabel();
                lblNext.Text = "后一页";
                x = currCtl.Bounds.Right;
                x += xDistance;
                ((LinkLabel)lblNext).LinkClicked += new LinkLabelLinkClickedEventHandler(lblCtl_Clicked);
                this.AddControl(lblNext,new Point(x,y),new Size(45, 16));
            }
        }
        private void AddControl(Control ctlObj,Point point,Size size)
        {
            ctlObj.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
                | System.Windows.Forms.AnchorStyles.Left)));
            ctlObj.Location = point;
            int x = ctlObj.Text.Length * 10;
            if (ctlObj.Text.IndexOf("页") > 0 )
            {
                if (ctlObj.Text.IndexOf("共") >= 0)
                    x += 10;
                else
                    x *= 2;
            }
            ctlObj.Size = new Size(x,16);
            this.Controls.Add(ctlObj);
        }

        private void lblCtl_Clicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
        {
            if (sender is LinkLabel)
            {
                try
                {
                    string lblText = ((LinkLabel)sender).Text;
                    if (lblText == "前一页")
                    {
                        end = start -1;
                        start = Math.Max(1,end - pageShow + 1);
                        currPage = end;
                    }
                    else if (lblText == "后一页")
                    {
                        start = end + 1;
                        end = Math.Min(pageCount,start + pageShow -1);
                        currPage = end;
                    }
                    else
                    {
                        int page = Convert.ToInt32(lblText);
                        currPage = page;
                    }
                    this.DrawControl();
                    this.OnLnkLblClicked(e);
                }
                catch{}
            }
        }

        public void ReDraw()
        {
            bolPrev = pageCount > pageShow;
        //    currPage = pageCount;
            end = pageCount;
            start = Math.Max(1,end - pageShow + 1);
            this.DrawControl();
            this.OnLnkLblClicked(null);
        }

    }
}
