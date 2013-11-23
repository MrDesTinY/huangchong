using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace TextBoxEx
{
    [ToolboxBitmap(typeof(TextBox))]
    public partial class TextBoxEx : TextBox
    {
        public enum RegexType
        {
            Custom,
            Number,
            CNString,
            Zip,
            Email,
            Phone,
            CNPhone,
            Mobile,
            Integer,
            NInteger,
            Float,
            NNFloat,
            PFloat,
            NPFloat,
            NFloat,
            ENChar,
            ENUChar,
            ENLChar,
            MixChar,
            MixLineChar,
            Url,
            QQ,
            DCard,
            IP,
            DateTime,
            Date,
            Year,
            Month,
            Day,
            Time,
            DateTimeAm,
            Date2
        }

        #region Const

        private const int WM_ERASEBKGND = 0x0014;
        private const int WM_PAINT = 0xF;
        private const uint WM_NCPAINT = 0x0085;

        #endregion

        #region DllImport


        #endregion

        #region Fields

        private Color _borderColor = Color.FromArgb(107, 191, 111);
        private string _waterMarkText;
        private Color _waterMarkTextColor = Color.DarkGray;
        private float _borderWeight = 1;
        private RegexType _controlType;
        private string _controlTypeText = "默认";


        private Color r;
        #endregion

        #region Properties

        [Description("水印文字")]
        public string WaterMarkText
        {
            get { return _waterMarkText; }
            set
            {
                _waterMarkText = value;
                base.Invalidate();
            }
        }

        [DefaultValue(typeof(Color), "DarkGray"), Description("水印文字颜色")]
        public Color WaterMarkTextColor
        {
            get { return _waterMarkTextColor; }
            set
            {
                _waterMarkTextColor = value;
                base.Invalidate();
            }
        }

        [DefaultValue(typeof(Color), "107, 191, 111"), Description("边框颜色")]
        public Color BorderColor
        {
            get { return _borderColor; }
            set
            {
                _borderColor = value;
                base.Invalidate();
            }
        }

        [DefaultValue(typeof(float), "1"), Description("边框粗细")]
        public float BorderWeight
        {
            get { return _borderWeight; }
            set
            {
                _borderWeight = value;
                base.Invalidate();
            }
        }

        [DefaultValue(RegexType.Custom), Description("文本框类型")]
        public RegexType ControlType
        {
            get { return _controlType; }
            set
            {
                _controlType = value;
                this.ShowDescription(value);
                base.Invalidate();
            }
        }

        [DefaultValue("默认"), Description("控件验证描述")]
        public string ControlTypeText
        {
            get { return _controlTypeText; }
        }

        #endregion

        public TextBoxEx()
            : base()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            // TODO: 在此处添加自定义绘制代码

            // 调用基类 OnPaint
            base.OnPaint(pe);
        }


        #region Method

        protected override Control.ControlCollection CreateControlsInstance()
        {
            r = this.BorderColor;
            return base.CreateControlsInstance();
        }

        private void ShowDescription(RegexType value)
        {
            switch (value)
            {
                case RegexType.Custom:
                    this._controlTypeText = "默认";
                    break;
                case RegexType.Number:
                    this._controlTypeText = "数字";
                    break;
                case RegexType.CNString:
                    this._controlTypeText = "汉字";
                    break;
                case RegexType.Zip:
                    this._controlTypeText = "邮政编码";
                    break;
                case RegexType.Email:
                    this._controlTypeText = "电子邮件";
                    break;
                case RegexType.Phone:
                    this._controlTypeText = "座机电话号码";
                    break;
                case RegexType.CNPhone:
                    this._controlTypeText = "中国电话号码";
                    break;
                case RegexType.Mobile:
                    this._controlTypeText = "手机号码";
                    break;
                case RegexType.Integer:
                    this._controlTypeText = "整数";
                    break;
                case RegexType.NInteger:
                    this._controlTypeText = "负整数";
                    break;
                case RegexType.Float:
                    this._controlTypeText = "浮点数";
                    break;
                case RegexType.NNFloat:
                    this._controlTypeText = "非负浮点数";
                    break;
                case RegexType.PFloat:
                    this._controlTypeText = "正浮点数";
                    break;
                case RegexType.NPFloat:
                    this._controlTypeText = "非正浮点数";
                    break;
                case RegexType.NFloat:
                    this._controlTypeText = "负浮点数";
                    break;
                case RegexType.ENChar:
                    this._controlTypeText = "英文字符";
                    break;
                case RegexType.ENUChar:
                    this._controlTypeText = "大写英文字符";
                    break;
                case RegexType.ENLChar:
                    this._controlTypeText = "小写英文字符";
                    break;
                case RegexType.MixChar:
                    this._controlTypeText = "数字和英文字母";
                    break;
                case RegexType.MixLineChar:
                    this._controlTypeText = "数字、英文字母或下划线";
                    break;
                case RegexType.Url:
                    this._controlTypeText = "URL";
                    break;
                case RegexType.QQ:
                    this._controlTypeText = "QQ";
                    break;
                case RegexType.DCard:
                    this._controlTypeText = "身份证";
                    break;
                case RegexType.IP:
                    this._controlTypeText = "IP";
                    break;
                case RegexType.DateTime:
                    this._controlTypeText = "2000-2-28 23:29:59";
                    break;
                case RegexType.Date:
                    this._controlTypeText = "2000-2-28";
                    break;
                case RegexType.Year:
                    this._controlTypeText = "年份";
                    break;
                case RegexType.Month:
                    this._controlTypeText = "月份";
                    break;
                case RegexType.Day:
                    this._controlTypeText = "日";
                    break;
                case RegexType.Time:
                    this._controlTypeText = "23:29:59";
                    break;
                case RegexType.DateTimeAm:
                    this._controlTypeText = "2000-02-29 10:29:39 pm";
                    break;
                case RegexType.Date2:
                    this._controlTypeText = "2009年2月28日";
                    break;
            }
        }

        protected override void OnTextChanged(System.EventArgs e)
        {
            bool b = Invalid(this.ControlType, this.Text);
            if (!b)
            {
                this.BorderColor = Color.Red;
            }
            else
            {
                this.BorderColor = r;
            }
            base.Invalidate();
        }

        private bool Invalid(RegexType value, string text)
        {
            bool b = false;
            switch (value)
            {
                case RegexType.Custom:
                    b = true;
                    break;
                case RegexType.Number:
                    b = Validation(text, @"^\d+$");
                    break;
                case RegexType.CNString:
                    b = Validation(text, @"^[\u4e00-\u9fa5]$");
                    break;
                case RegexType.Zip:
                    b = Validation(text, @"^[1-9]\d{5}$");
                    break;
                case RegexType.Email:
                    b = Validation(text, @"^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$");
                    break;
                case RegexType.Phone:
                    b = Validation(text, @"^((\(\d{2,3}\))|(\d{3}\-))?(\(0\d{2,3}\)|0\d{2,3}-)?[1-9]\d{6,7}(\-\d{1,4})?$");
                    break;
                case RegexType.CNPhone:
                    b = Validation(text, @"^\d{3}-\d{8}|\d{4}-\d{7}$");
                    break;
                case RegexType.Mobile:
                    b = Validation(text, @"^((\(\d{2,3}\))|(\d{3}\-))?13\d{9}$");
                    break;
                case RegexType.Integer:
                    b = Validation(text, @"^-?\d+$");
                    break;
                case RegexType.NInteger:
                    b = Validation(text, @"^-[0-9]*[1-9][0-9]*$");
                    break;
                case RegexType.Float:
                    b = Validation(text, @"^(-?\d+)(\.\d+)?$");
                    break;
                case RegexType.NNFloat:
                    b = Validation(text, @"^\d+(\.\d+)?$");
                    break;
                case RegexType.PFloat:
                    b = Validation(text, @"^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
                    break;
                case RegexType.NPFloat:
                    b = Validation(text, @"^((-\d+(\.\d+)?)|(0+(\.0+)?))$");
                    break;
                case RegexType.NFloat:
                    b = Validation(text, @"^(-(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*)))$");
                    break;
                case RegexType.ENChar:
                    b = Validation(text, @"^[A-Za-z]+$");
                    break;
                case RegexType.ENUChar:
                    b = Validation(text, @"^[A-Z]+$");
                    break;
                case RegexType.ENLChar:
                    b = Validation(text, @"^[a-z]+$");
                    break;
                case RegexType.MixChar:
                    b = Validation(text, @"^[A-Za-z0-9]+$");
                    break;
                case RegexType.MixLineChar:
                    b = Validation(text, @"^\w+$");
                    break;
                case RegexType.Url:
                    b = Validation(text, @"^(?:https?|ftp)\:\/\/(?:(?:[a-z0-9\-\._~\!\$\&\'\(\)\*\+\,\;\=:]|%[0-9a-f]{2,2})*\@)?(?:((?:(?:[a-z0-9][a-z0-9\-]*[a-z0-9]|[a-z0-9])\.)*(?:[a-z][a-z0-9\-]*[a-z0-9]|[a-z])|(?:\[[^\]]*\]))(?:\:[0-9]*)?)(?:\/(?:[a-z0-9\-\._~\!\$\&\'\(\)\*\+\,\;\=\:\@]|%[0-9a-f]{2,2})*)*(?:\?(?:[a-z0-9\-\._~\!\$\&\'\(\)\*\+\,\;\=\:\@\/\?]|%[0-9a-f]{2,2})*)?(?:\#(?:[a-z0-9\-\._~\!\$\&\'\(\)\*\+\,\;\=\:\@\/\?]|%[0-9a-f]{2,2})*)?$");
                    break;
                case RegexType.QQ:
                    b = Validation(text, @"^[1-9][0-9]{4,}$");
                    break;
                case RegexType.DCard:
                    b = Validation(text, @"^((1[1-5])|(2[1-3])|(3[1-7])|(4[1-6])|(5[0-4])|(6[1-5])|71|(8[12])|91)\d{4}((19\d{2}(0[13-9]|1[012])(0[1-9]|[12]\d|30))|(19\d{2}(0[13578]|1[02])31)|(19\d{2}02(0[1-9]|1\d|2[0-8]))|(19([13579][26]|[2468][048]|0[48])0229))\d{3}(\d|X|x)?$");
                    break;
                case RegexType.IP:
                    b = Validation(text, @"^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$");
                    break;
                case RegexType.DateTime:
                    b = Validation(text, @"^((((1[6-9]|[2-9]\d)\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\d|3[01]))|(((1[6-9]|[2-9]\d)\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\d|30))|(((1[6-9]|[2-9]\d)\d{2})-0?2-(0?[1-9]|1\d|2[0-8]))|(((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))-0?2-29-)) (20|21|22|23|[0-1]?\d):[0-5]?\d:[0-5]?\d$");
                    break;
                case RegexType.Date:
                    b = Validation(text, @"^((((1[6-9]|[2-9]\d)\d{2})-(0?[13578]|1[02])-(0?[1-9]|[12]\d|3[01]))|(((1[6-9]|[2-9]\d)\d{2})-(0?[13456789]|1[012])-(0?[1-9]|[12]\d|30))|(((1[6-9]|[2-9]\d)\d{2})-0?2-(0?[1-9]|1\d|2[0-8]))|(((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))-0?2-29-))$");
                    break;
                case RegexType.Year:
                    b = Validation(text, @"^((1[6-9]|[2-9]\d)\d{2})$");
                    break;
                case RegexType.Month:
                    b = Validation(text, @"^(0?[123456789]|1[012])$");
                    break;
                case RegexType.Day:
                    b = Validation(text, @"^(0?[1-9]|[12]\d|3[01])$");
                    break;
                case RegexType.Time:
                    b = Validation(text, @"^(20|21|22|23|[0-1]?\d):[0-5]?\d:[0-5]?\d$");
                    break;
                case RegexType.DateTimeAm:
                    b = Validation(text, @"^((\d{2}(([02468][048])|([13579][26]))[\-\/\s]?((((0?[13578])|(1[02]))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\-\/\s]?((0?[1-9])|([1-2][0-9])))))|(\d{2}(([02468][1235679])|([13579][01345789]))[\-\/\s]?((((0?[13578])|(1[02]))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(3[01])))|(((0?[469])|(11))[\-\/\s]?((0?[1-9])|([1-2][0-9])|(30)))|(0?2[\-\/\s]?((0?[1-9])|(1[0-9])|(2[0-8]))))))(\s(((0?[1-9])|(1[0-2]))\:([0-5][0-9])((\s)|(\:([0-5][0-9])\s))([AM|PM|am|pm]{2,2})))?$");
                    break;
                case RegexType.Date2:
                    b = Validation(text, @"^((((1[6-9]|[2-9]\d)\d{2})年(0?[13578]|1[02])月(0?[1-9]|[12]\d|3[01])日)|(((1[6-9]|[2-9]\d)\d{2})年(0?[13456789]|1[012])月(0?[1-9]|[12]\d|30)日)|(((1[6-9]|[2-9]\d)\d{2})年0?2月(0?[1-9]|1\d|2[0-8])日)|(((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|((16|[2468][048]|[3579][26])00))年0?2月29日))$");
                    break;
            }
            return b;
        }

        private bool Validation(string validValue, string regularExpression)
        {
            Regex regex;
            try
            {
                regex = new Regex(regularExpression);
            }
            catch
            {
                return false;
            }
            if (regex.IsMatch(validValue))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            this.BorderStyle = BorderStyle.FixedSingle;

            if (m.Msg == WM_PAINT || m.Msg == WM_NCPAINT)
            {
                if (this.BorderWeight % 2 == 0)
                {
                    this.BorderWeight -= 1;
                }
                using (Graphics g = Graphics.FromHwnd(this.Handle))
                {
                    using (Pen pen = new Pen(this.BorderColor, this.BorderWeight))
                    {
                        g.DrawRectangle(pen, 0, 0, Size.Width - 1, Size.Height - 1);
                    }
                }
                WmPaint(ref m);
            }
        }

        private void WmPaint(ref Message m)
        {
            using (Graphics graphics = Graphics.FromHwnd(base.Handle))
            {
                if (Text.Length == 0 && !string.IsNullOrEmpty(_waterMarkText) && !Focused)
                {
                    TextFormatFlags format = TextFormatFlags.EndEllipsis | TextFormatFlags.VerticalCenter;

                    if (RightToLeft == RightToLeft.Yes)
                    {
                        format |= TextFormatFlags.RightToLeft | TextFormatFlags.Right;
                    }

                    TextRenderer.DrawText(graphics, _waterMarkText, Font, base.ClientRectangle, _waterMarkTextColor, format);
                }
            }
        }
        #endregion
    }
}
