namespace WindowsFormsApplication1
{
    partial class ComboxControl
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.comboBoxEx1 = new ComboBoxEx.ComboBoxEx();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBoxEx1
            // 
            this.comboBoxEx1.BorderColor = System.Drawing.Color.Empty;
            this.comboBoxEx1.BorderSize = 1;
            this.comboBoxEx1.DisableWheel = false;
            this.comboBoxEx1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.comboBoxEx1.FormattingEnabled = true;
            this.comboBoxEx1.ImageList = null;
            this.comboBoxEx1.Location = new System.Drawing.Point(0, 2);
            this.comboBoxEx1.Name = "comboBoxEx1";
            this.comboBoxEx1.Size = new System.Drawing.Size(179, 22);
            this.comboBoxEx1.TabIndex = 0;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.DropDown;
            this.pictureBox1.Location = new System.Drawing.Point(157, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(22, 20);
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // ComboxControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.comboBoxEx1);
            this.Name = "ComboxControl";
            this.Size = new System.Drawing.Size(181, 25);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private ComboBoxEx.ComboBoxEx comboBoxEx1;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}
