namespace EditorApplication.Forms
{
    partial class OutputWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.OutputSurface = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.OutputSurface)).BeginInit();
            this.SuspendLayout();
            // 
            // OutputSurface
            // 
            this.OutputSurface.Dock = System.Windows.Forms.DockStyle.Fill;
            this.OutputSurface.Location = new System.Drawing.Point(0, 0);
            this.OutputSurface.Name = "OutputSurface";
            this.OutputSurface.Size = new System.Drawing.Size(406, 319);
            this.OutputSurface.TabIndex = 0;
            this.OutputSurface.TabStop = false;
            this.OutputSurface.Resize += new System.EventHandler(this.OutputSurface_Resize);
            // 
            // OutputWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(406, 319);
            this.Controls.Add(this.OutputSurface);
            this.Font = new System.Drawing.Font("Tahoma", 8F);
            this.HideOnClose = true;
            this.Name = "OutputWindow";
            this.ShowHint = WeifenLuo.WinFormsUI.Docking.DockState.Document;
            this.Text = "Output Window";
            this.DockStateChanged += new System.EventHandler(this.OutputWindow_DockStateChanged);
            ((System.ComponentModel.ISupportInitialize)(this.OutputSurface)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox OutputSurface;
    }
}