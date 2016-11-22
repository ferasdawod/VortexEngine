using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using System.Threading.Tasks;
using System.Windows.Forms;

using WeifenLuo.WinFormsUI.Docking;

namespace EditorApplication.Forms
{
    public partial class ActorsWindow : DockContent
    {
        private MainForm _mainForm;

        public ActorsWindow(MainForm parent)
        {
            InitializeComponent();

            _mainForm = parent;

            TreeView.Nodes.Clear();
            TreeView.Nodes.Add("Root");
        }

        public void RefreshActors()
        {
            TreeView.Nodes.Clear();

            if (string.IsNullOrWhiteSpace(_mainForm.CurrentLevelPath))
            {
                TreeView.Nodes.Add("Root");
                return;
            }

            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(_mainForm.CurrentLevelPath);

            var rootElement = xmlDocument.DocumentElement;
            var actorsElement = rootElement["Actors"];

            var rootNode = new TreeNode("Root");

            for (int i = 0; i < actorsElement.ChildNodes.Count; i++)
            {
                var childElement = actorsElement.ChildNodes[i];
                var actorNode = GetRealActorNode(childElement);
                var actorName = GetActorName(actorNode);

                var actorTreeNode = new TreeNode(actorName);

                var componentsNode = actorNode["Components"];
                var componentsList = componentsNode.ChildNodes;

                for (int j = 0; j < componentsList.Count; j++)
                {
                    var currentComponent = componentsList[j];
                    var componentName = currentComponent.Attributes["Name"].Value;

                    actorTreeNode.Nodes.Add(componentName);
                }

                rootNode.Nodes.Add(actorTreeNode);
            }

            TreeView.Nodes.Add(rootNode);
        }

        private XmlNode GetRealActorNode(XmlNode actorNode)
        {
            var actorRealNode = actorNode;

            var prefabAtt = actorRealNode.Attributes["PrefabPath"];
            if (prefabAtt != null)
            {
                var actorDocument = new XmlDocument();
                actorDocument.Load(_mainForm.WorkingDirectory + "\\" + prefabAtt.Value);
                actorRealNode = actorDocument.DocumentElement;
            }

            return actorRealNode;
        }

        private string GetActorName(XmlNode actorNode)
        {
            var actorName = actorNode.Attributes["Name"].Value;
            return actorName;
        }

        private void TreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // if the selected node was valid and it wasn't the root node
            if (e.Node != null && e.Node.Parent != null)
            {
                if (e.Node.Nodes.Count == 0)
                {
                    // the selected node is a component
                    _mainForm.PropertiesWindow.UpdateSelection(e.Node.FullPath, SelectionType.Component);
                }
                else
                {
                    // the selected node is an actor
                    _mainForm.PropertiesWindow.UpdateSelection(e.Node.FullPath, SelectionType.Actor);
                }
            }
        }
    }
}
