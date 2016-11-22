using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using WeifenLuo.WinFormsUI.Docking;

namespace EditorApplication.Forms
{
    public partial class OutputWindow : DockContent
    {
        public IntPtr OutputHandle
        {
            get { return OutputSurface.Handle; }
        }

        public OutputWindow()
        {
            InitializeComponent();

            OutputSurface.HandleCreated += CreatedHandle;
            OutputSurface.HandleDestroyed += DestroyHandle;
        }

        private void CreatedHandle(object sender, EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Creating Handle");
        }

        private void DestroyHandle(object sender, EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("Destroyed Handle");
        }

        private void OutputSurface_Resize(object sender, EventArgs e)
        {
            Native.NativeMethods.Resize();
        }

        private void OutputWindow_DockStateChanged(object sender, EventArgs e)
        {
            System.Diagnostics.Debug.WriteLine("DockStateChanged");
        }
    }
}
