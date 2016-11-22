using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows.Forms;

using WeifenLuo.WinFormsUI.Docking;

namespace EditorApplication.Forms
{
    public partial class AssetsWindow : DockContent
    {
        private MainForm _mainForm;

        public AssetsWindow(MainForm parent)
        {
            InitializeComponent();

            _mainForm = parent;

            RefreshAssets();
        }

        public void RefreshAssets()
        {
            TreeView.Nodes.Clear();

            var nodesStack = new Stack<TreeNode>();
            DirectoryInfo DirInfo = new DirectoryInfo(_mainForm.AssetsDirectory);
            var rootNode = new TreeNode(DirInfo.Name) { Tag = DirInfo };

            nodesStack.Push(rootNode);
            while (nodesStack.Count > 0)
            {
                var currentNode = nodesStack.Pop();
                var nodeDirInfo = (DirectoryInfo)currentNode.Tag;


                foreach (var directory in nodeDirInfo.GetDirectories())
                {
                    FileAttributes attributes = File.GetAttributes(directory.FullName);
                    if ((attributes & FileAttributes.Hidden) == 0)
                    {
                        var childDirNode = new TreeNode(directory.Name);
                        childDirNode.Tag = directory;
                        currentNode.Nodes.Add(childDirNode);
                        nodesStack.Push(childDirNode);
                    }
                }

                foreach (var file in nodeDirInfo.GetFiles())
                {
                    FileAttributes attributes = File.GetAttributes(file.FullName);
                    if ((attributes & FileAttributes.Hidden) == 0)
                    {
                        var childNode = new TreeNode(file.Name);
                        childNode.Tag = file.FullName;
                        currentNode.Nodes.Add(childNode);
                    }
                }
            }

            TreeView.Nodes.Add(rootNode);
        }

        private void TreeView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var selectedNode = TreeView.SelectedNode;
            if (selectedNode != null && selectedNode.Nodes.Count == 0)
            {
                string file = selectedNode.Tag.ToString();
                if (File.Exists(file))
                {
                    Process.Start(file);
                }
            }
        }

        private void TreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // if the node is valid and its not the root node and it has no children
            if (e.Node != null && e.Node.Parent != null && e.Node.Nodes.Count == 0)
            {
                _mainForm.PropertiesWindow.UpdateSelection(e.Node.FullPath, SelectionType.Asset);
            }
        }
    }
}
