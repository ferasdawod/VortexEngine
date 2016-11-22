using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

using WeifenLuo.WinFormsUI.Docking;

namespace EditorApplication.Forms
{
    public enum SelectionType
    {
        Actor,
        Component,
        Asset,
    }

    public partial class PropertiesWindow : DockContent
    {
        private MainForm _mainForm;

        public PropertiesWindow(MainForm parent)
        {
            InitializeComponent();

            _mainForm = parent;

            TableLayoutPanel table = new TableLayoutPanel();
            this.Controls.Add(table);
            table.Dock = DockStyle.Fill;
            table.RowCount = 2;
            table.ColumnCount = 3;

            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });
            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });
            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });

            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });
            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });
            table.Controls.Add(new TextBox() { Dock = DockStyle.Fill, Multiline = true });

            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));

            table.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            table.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));

            for (int i = 0; i < table.RowStyles.Count; i++)
            {
                table.RowStyles[i].SizeType = SizeType.Percent;
                table.RowStyles[i].Height = 50;
            }

            for (int i = 0; i < table.ColumnStyles.Count; i++)
            {
                table.ColumnStyles[i].SizeType = SizeType.Percent;
                table.ColumnStyles[i].Width = 50;
            }
        }

        public void UpdateSelection(string nodePath, SelectionType selectionType)
        {
            Debug.WriteLine("Selected Node : " + nodePath);
            Debug.WriteLine("Selection Is : " + selectionType.ToString());
        }

        private TableLayoutPanel CreateTableLayout(int numRows, int numColumns)
        {
            TableLayoutPanel table = new TableLayoutPanel();
            table.RowCount = numRows;
            table.ColumnCount = numColumns;

            for (int i = 0; i < numRows; i++)
                table.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            for (int j = 0; j < numColumns; j++)
                table.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50f));

            return table;
        }
    }
}
