using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

using WeifenLuo.WinFormsUI.Docking;

namespace EditorApplication.Forms
{
    public partial class MainForm : Form
    {
        public OutputWindow OutputWindow { get; private set; }
        public AssetsWindow AssetsWindow { get; private set; }
        public ActorsWindow ActorsWindow { get; private set; }
        public PropertiesWindow PropertiesWindow { get; private set; }

        public string WorkingDirectory { get; private set; }
        public string AssetsDirectory { get; private set; }
        public string CurrentLevelPath { get; private set; }

        public MainForm()
        {
            InitializeComponent();

            WorkingDirectory = Directory.GetCurrentDirectory();
            AssetsDirectory = WorkingDirectory + "\\Assets";

            OutputWindow = new OutputWindow();
            AssetsWindow = new AssetsWindow(this);
            ActorsWindow = new ActorsWindow(this);
            PropertiesWindow = new PropertiesWindow(this);

            IntPtr handle = OutputWindow.OutputHandle;
            IntPtr instance = System.Runtime.InteropServices.Marshal.GetHINSTANCE(this.GetType().Module);
            Native.NativeMethods.Initialize(instance, handle);
            Application.Idle += OnApplicationIdle;

            openFileDialog.InitialDirectory = AssetsDirectory;

            OutputWindow.Show(DockPanel);
            AssetsWindow.Show(DockPanel);
            ActorsWindow.Show(DockPanel);
            PropertiesWindow.Show(DockPanel);
        }

        private void OnApplicationIdle(object sender, EventArgs e)
        {
            Native.NativeMethods.Tick();
            this.Invalidate();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Native.NativeMethods.ShutDown();
        }

        private void outputWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OutputWindow.Show(DockPanel);
        }

        private void assetsWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AssetsWindow.Show(DockPanel);
        }

        private void actorsWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ActorsWindow.Show(DockPanel);
        }

        private void propertiesWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PropertiesWindow.Show(DockPanel);
        }

        private void refreshAssetsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AssetsWindow.RefreshAssets();
        }

        private void refreshSceneWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ActorsWindow.RefreshActors();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void openLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "Level Files (*.level)|*.level";
            openFileDialog.Title = "Open Map";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string levelPath = openFileDialog.FileName;
                if (levelPath.StartsWith(AssetsDirectory))
                {
                    CurrentLevelPath = levelPath;
                    Native.NativeMethods.LoadLevel(levelPath);
                    ActorsWindow.RefreshActors();
                }
                else
                {
                    MessageBox.Show("The selected level does not belonge to the assets directory", "Illegal Path", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void reloadLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Native.NativeMethods.LoadLevel(CurrentLevelPath);
        }
    }
}