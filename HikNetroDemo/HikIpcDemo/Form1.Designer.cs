namespace HikIpcDemo
{
    partial class Form1
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

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.initBtn = new System.Windows.Forms.Button();
            this.loginBtn = new System.Windows.Forms.Button();
            this.quitBtn = new System.Windows.Forms.Button();
            this.alarmBtn = new System.Windows.Forms.Button();
            this.closealarmBtn = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.txtPsd = new System.Windows.Forms.TextBox();
            this.txtUsername = new System.Windows.Forms.TextBox();
            this.ipAddress = new IPAddressControlLib.IPAddressControl();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnStopListen = new System.Windows.Forms.Button();
            this.btnStartListen = new System.Windows.Forms.Button();
            this.btnLoginOut = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnSetParam = new System.Windows.Forms.Button();
            this.chbSuspend = new System.Windows.Forms.CheckBox();
            this.label8 = new System.Windows.Forms.Label();
            this.tbUpExtraTime = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tbUpIntTime = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.ipaddrListenIP = new IPAddressControlLib.IPAddressControl();
            this.tbListenPort = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // initBtn
            // 
            this.initBtn.Location = new System.Drawing.Point(120, 181);
            this.initBtn.Name = "initBtn";
            this.initBtn.Size = new System.Drawing.Size(75, 23);
            this.initBtn.TabIndex = 0;
            this.initBtn.Text = "初始化";
            this.initBtn.UseVisualStyleBackColor = true;
            this.initBtn.Click += new System.EventHandler(this.initBtn_Click);
            // 
            // loginBtn
            // 
            this.loginBtn.Enabled = false;
            this.loginBtn.Location = new System.Drawing.Point(120, 215);
            this.loginBtn.Name = "loginBtn";
            this.loginBtn.Size = new System.Drawing.Size(75, 23);
            this.loginBtn.TabIndex = 1;
            this.loginBtn.Text = "登录";
            this.loginBtn.UseVisualStyleBackColor = true;
            this.loginBtn.Click += new System.EventHandler(this.loginBtn_Click);
            // 
            // quitBtn
            // 
            this.quitBtn.Enabled = false;
            this.quitBtn.Location = new System.Drawing.Point(120, 344);
            this.quitBtn.Name = "quitBtn";
            this.quitBtn.Size = new System.Drawing.Size(75, 23);
            this.quitBtn.TabIndex = 3;
            this.quitBtn.Text = "反初始化";
            this.quitBtn.UseVisualStyleBackColor = true;
            this.quitBtn.Click += new System.EventHandler(this.quitBtn_Click);
            // 
            // alarmBtn
            // 
            this.alarmBtn.Enabled = false;
            this.alarmBtn.Location = new System.Drawing.Point(99, 249);
            this.alarmBtn.Name = "alarmBtn";
            this.alarmBtn.Size = new System.Drawing.Size(61, 23);
            this.alarmBtn.TabIndex = 4;
            this.alarmBtn.Text = "布防";
            this.alarmBtn.UseVisualStyleBackColor = true;
            this.alarmBtn.Click += new System.EventHandler(this.alarmBtn_Click);
            // 
            // closealarmBtn
            // 
            this.closealarmBtn.Enabled = false;
            this.closealarmBtn.Location = new System.Drawing.Point(99, 282);
            this.closealarmBtn.Name = "closealarmBtn";
            this.closealarmBtn.Size = new System.Drawing.Size(61, 23);
            this.closealarmBtn.TabIndex = 5;
            this.closealarmBtn.Text = "撤防";
            this.closealarmBtn.UseVisualStyleBackColor = true;
            this.closealarmBtn.Click += new System.EventHandler(this.closealarmBtn_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ControlText;
            this.pictureBox1.Location = new System.Drawing.Point(15, 19);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(400, 300);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 6;
            this.pictureBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("SimSun", 13F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(337, 360);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 18);
            this.label1.TabIndex = 7;
            this.label1.Text = "无车牌";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.pictureBox1);
            this.groupBox1.Location = new System.Drawing.Point(340, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(427, 333);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "车辆近景图";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(779, 18);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(136, 324);
            this.richTextBox1.TabIndex = 9;
            this.richTextBox1.Text = "使用说明：\n1、首先请在左边IP地址栏中输入要连接的相机IP、用户名、密码。\n2、输入完成后，按照左边按钮顺序进行操作。\n3、该程序不支持手动触发，需相机IO触发" +
                "\n4、抓怕图片可到程序运行目录下查看\nNEW：\n支持Netro的监听和回调获取，Netro设备的信息获取是通过监听来实现的，\n其他功能不变。如果是Netro设备" +
                "，则点击“开始监听”和“停止监听”来\n处理。";
            // 
            // txtPsd
            // 
            this.txtPsd.Location = new System.Drawing.Point(6, 113);
            this.txtPsd.Name = "txtPsd";
            this.txtPsd.Size = new System.Drawing.Size(116, 21);
            this.txtPsd.TabIndex = 15;
            this.txtPsd.Text = "12345";
            // 
            // txtUsername
            // 
            this.txtUsername.Location = new System.Drawing.Point(6, 73);
            this.txtUsername.Name = "txtUsername";
            this.txtUsername.Size = new System.Drawing.Size(116, 21);
            this.txtUsername.TabIndex = 14;
            this.txtUsername.Text = "admin";
            // 
            // ipAddress
            // 
            this.ipAddress.BackColor = System.Drawing.SystemColors.Window;
            this.ipAddress.Location = new System.Drawing.Point(6, 33);
            this.ipAddress.MinimumSize = new System.Drawing.Size(116, 21);
            this.ipAddress.Name = "ipAddress";
            this.ipAddress.ReadOnly = false;
            this.ipAddress.Size = new System.Drawing.Size(116, 21);
            this.ipAddress.TabIndex = 16;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 17);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 17;
            this.label2.Text = "IP：";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 57);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 18;
            this.label3.Text = "用户名：";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 97);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 19;
            this.label4.Text = "密码：";
            // 
            // btnStopListen
            // 
            this.btnStopListen.Enabled = false;
            this.btnStopListen.Location = new System.Drawing.Point(166, 282);
            this.btnStopListen.Name = "btnStopListen";
            this.btnStopListen.Size = new System.Drawing.Size(61, 23);
            this.btnStopListen.TabIndex = 21;
            this.btnStopListen.Text = "停止监听";
            this.btnStopListen.UseVisualStyleBackColor = true;
            this.btnStopListen.Click += new System.EventHandler(this.btnStopListen_Click);
            // 
            // btnStartListen
            // 
            this.btnStartListen.Enabled = false;
            this.btnStartListen.Location = new System.Drawing.Point(166, 249);
            this.btnStartListen.Name = "btnStartListen";
            this.btnStartListen.Size = new System.Drawing.Size(61, 23);
            this.btnStartListen.TabIndex = 20;
            this.btnStartListen.Text = "开始监听";
            this.btnStartListen.UseVisualStyleBackColor = true;
            this.btnStartListen.Click += new System.EventHandler(this.btnStartListen_Click);
            // 
            // btnLoginOut
            // 
            this.btnLoginOut.Enabled = false;
            this.btnLoginOut.Location = new System.Drawing.Point(120, 315);
            this.btnLoginOut.Name = "btnLoginOut";
            this.btnLoginOut.Size = new System.Drawing.Size(75, 23);
            this.btnLoginOut.TabIndex = 22;
            this.btnLoginOut.Text = "登出";
            this.btnLoginOut.UseVisualStyleBackColor = true;
            this.btnLoginOut.Click += new System.EventHandler(this.btnLoginOut_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.ipAddress);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.txtUsername);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.txtPsd);
            this.groupBox2.Location = new System.Drawing.Point(12, 18);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(139, 142);
            this.groupBox2.TabIndex = 23;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "设备登录设置";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnSetParam);
            this.groupBox3.Controls.Add(this.chbSuspend);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.tbUpExtraTime);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Controls.Add(this.tbUpIntTime);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.ipaddrListenIP);
            this.groupBox3.Controls.Add(this.tbListenPort);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(157, 18);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(177, 142);
            this.groupBox3.TabIndex = 24;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Netro上传参数配置";
            // 
            // btnSetParam
            // 
            this.btnSetParam.Enabled = false;
            this.btnSetParam.Location = new System.Drawing.Point(110, 116);
            this.btnSetParam.Name = "btnSetParam";
            this.btnSetParam.Size = new System.Drawing.Size(61, 23);
            this.btnSetParam.TabIndex = 25;
            this.btnSetParam.Text = "设置参数";
            this.btnSetParam.UseVisualStyleBackColor = true;
            this.btnSetParam.Click += new System.EventHandler(this.btnSetParam_Click);
            // 
            // chbSuspend
            // 
            this.chbSuspend.AutoSize = true;
            this.chbSuspend.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.chbSuspend.Location = new System.Drawing.Point(4, 120);
            this.chbSuspend.Name = "chbSuspend";
            this.chbSuspend.Size = new System.Drawing.Size(84, 16);
            this.chbSuspend.TabIndex = 26;
            this.chbSuspend.Text = "暂停上传：";
            this.chbSuspend.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(5, 94);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(89, 12);
            this.label8.TabIndex = 25;
            this.label8.Text = "上传超时时间：";
            // 
            // tbUpExtraTime
            // 
            this.tbUpExtraTime.Location = new System.Drawing.Point(95, 91);
            this.tbUpExtraTime.Name = "tbUpExtraTime";
            this.tbUpExtraTime.Size = new System.Drawing.Size(78, 21);
            this.tbUpExtraTime.TabIndex = 24;
            this.tbUpExtraTime.Text = "5000";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(5, 69);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(89, 12);
            this.label7.TabIndex = 23;
            this.label7.Text = "上传间隔时间：";
            // 
            // tbUpIntTime
            // 
            this.tbUpIntTime.Location = new System.Drawing.Point(95, 66);
            this.tbUpIntTime.Name = "tbUpIntTime";
            this.tbUpIntTime.Size = new System.Drawing.Size(78, 21);
            this.tbUpIntTime.TabIndex = 22;
            this.tbUpIntTime.Text = "200";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(5, 44);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(65, 12);
            this.label6.TabIndex = 21;
            this.label6.Text = "监听端口：";
            // 
            // ipaddrListenIP
            // 
            this.ipaddrListenIP.BackColor = System.Drawing.SystemColors.Window;
            this.ipaddrListenIP.Location = new System.Drawing.Point(57, 14);
            this.ipaddrListenIP.MinimumSize = new System.Drawing.Size(116, 21);
            this.ipaddrListenIP.Name = "ipaddrListenIP";
            this.ipaddrListenIP.ReadOnly = false;
            this.ipaddrListenIP.Size = new System.Drawing.Size(116, 21);
            this.ipaddrListenIP.TabIndex = 18;
            // 
            // tbListenPort
            // 
            this.tbListenPort.Location = new System.Drawing.Point(80, 41);
            this.tbListenPort.Name = "tbListenPort";
            this.tbListenPort.Size = new System.Drawing.Size(93, 21);
            this.tbListenPort.TabIndex = 20;
            this.tbListenPort.Text = "7200";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 18);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 12);
            this.label5.TabIndex = 19;
            this.label5.Text = "监听IP：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(929, 413);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.btnLoginOut);
            this.Controls.Add(this.btnStopListen);
            this.Controls.Add(this.btnStartListen);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.closealarmBtn);
            this.Controls.Add(this.alarmBtn);
            this.Controls.Add(this.quitBtn);
            this.Controls.Add(this.loginBtn);
            this.Controls.Add(this.initBtn);
            this.MinimumSize = new System.Drawing.Size(543, 439);
            this.Name = "Form1";
            this.Text = "单通道智能网络摄像机测试程序";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button initBtn;
        private System.Windows.Forms.Button loginBtn;
        private System.Windows.Forms.Button quitBtn;
        private System.Windows.Forms.Button alarmBtn;
        private System.Windows.Forms.Button closealarmBtn;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.TextBox txtPsd;
        private System.Windows.Forms.TextBox txtUsername;
        private IPAddressControlLib.IPAddressControl ipAddress;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnStopListen;
        private System.Windows.Forms.Button btnStartListen;
        private System.Windows.Forms.Button btnLoginOut;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox tbUpExtraTime;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox tbUpIntTime;
        private System.Windows.Forms.Label label6;
        private IPAddressControlLib.IPAddressControl ipaddrListenIP;
        private System.Windows.Forms.TextBox tbListenPort;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox chbSuspend;
        private System.Windows.Forms.Button btnSetParam;
    }
}

