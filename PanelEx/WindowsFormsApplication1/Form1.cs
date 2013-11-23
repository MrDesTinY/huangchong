using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(textBoxEx1.Text.Trim());
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            DataTable dt = new DataTable();

            DataColumn dc;
            dc = new DataColumn("ID", System.Type.GetType("System.Int32"));
            dt.Columns.Add(dc);

            dc = new DataColumn("Name", System.Type.GetType("System.String"));
            dt.Columns.Add(dc);

            DataRow dr;
            List<ComboBoxEx.ImageComboBoxItem> items = new List<ComboBoxEx.ImageComboBoxItem>();
            for (int i = 0; i < 5; i++)
            {
                ComboBoxEx.ImageComboBoxItem item = new ComboBoxEx.ImageComboBoxItem(i.ToString(), i);
                items.Add(item);

                dr = dt.NewRow();
                dr["ID"] = i+1;
                dr["Name"] = "张三"+i.ToString ();
                dt.Rows.Add(dr); 
            }

            this.comboBoxEx1.DataSource = dt.DefaultView;
            this.comboBoxEx1.DisplayMember = "Name";
            this.comboBoxEx1.ValueMember = "ID";


        }


    }
}
