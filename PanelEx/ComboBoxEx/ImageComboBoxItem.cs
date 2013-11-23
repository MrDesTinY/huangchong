//****************************************
//功能描述：设置ComboBox下拉项目文本及图标
//作者：    吴海飞
//日期：    2011-01-26
//****************************************
using System;
using System.Collections.Generic;
using System.Text;

namespace ComboBoxEx
{
    public class ImageComboBoxItem
    {
        //定义文本属性
        private string _text;
        public string Text
        {
        get {return _text;}
        set {_text = value;}
        }
        //定义图象索引属性
        private int _imageIndex;
        public int ImageIndex
        {
        get {return _imageIndex;}
        set {_imageIndex = value;}
        }
        //初始化函数之一：即没有图象也没有文本
        public ImageComboBoxItem():this("",-1) 
        {
        }
        //初始化函数之二：没有图象，只有文本
        public ImageComboBoxItem(string text): this(text, -1) 
        {
        }
        //初始化函数之三：文本与图象都有
        public ImageComboBoxItem(string text, int imageIndex)
        {
        _text = text;
        _imageIndex = imageIndex;
        }

        public override string ToString()
        {
        return _text;
        }
    }
}
