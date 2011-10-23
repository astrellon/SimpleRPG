namespace SimpleRPGAnalyser
{
    partial class Analyser
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.Title title2 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.Title title3 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Analyser));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.viewAnimals = new System.Windows.Forms.ListView();
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.picGraph = new System.Windows.Forms.PictureBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.trcDay = new System.Windows.Forms.TrackBar();
            this.picMain = new System.Windows.Forms.PictureBox();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.trcTimeline = new System.Windows.Forms.TrackBar();
            this.lstPopulations = new System.Windows.Forms.ListBox();
            this.chkNormalisePopulations = new System.Windows.Forms.CheckBox();
            this.chrtPopulation = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.btnSaveMap = new System.Windows.Forms.Button();
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader10 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.menuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picGraph)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcDay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.picMain)).BeginInit();
            this.tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcTimeline)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chrtPopulation)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(743, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(45, 20);
            this.loadToolStripMenuItem.Text = "Load";
            this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(12, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(719, 531);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.splitContainer1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(711, 505);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Gene Histogram";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 3);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.viewAnimals);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.picGraph);
            this.splitContainer1.Size = new System.Drawing.Size(705, 499);
            this.splitContainer1.SplitterDistance = 249;
            this.splitContainer1.TabIndex = 4;
            // 
            // viewAnimals
            // 
            this.viewAnimals.AllowColumnReorder = true;
            this.viewAnimals.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.viewAnimals.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader5,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader7,
            this.columnHeader8,
            this.columnHeader9,
            this.columnHeader10});
            this.viewAnimals.FullRowSelect = true;
            this.viewAnimals.GridLines = true;
            this.viewAnimals.Location = new System.Drawing.Point(3, 3);
            this.viewAnimals.Name = "viewAnimals";
            this.viewAnimals.Size = new System.Drawing.Size(699, 243);
            this.viewAnimals.TabIndex = 3;
            this.viewAnimals.UseCompatibleStateImageBehavior = false;
            this.viewAnimals.View = System.Windows.Forms.View.Details;
            this.viewAnimals.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.viewAnimals_ColumnClick);
            this.viewAnimals.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Id";
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Species";
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Age";
            this.columnHeader5.Width = 61;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Birth Time";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Death Time";
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "Death By";
            // 
            // picGraph
            // 
            this.picGraph.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.picGraph.Location = new System.Drawing.Point(10, 10);
            this.picGraph.Margin = new System.Windows.Forms.Padding(10);
            this.picGraph.Name = "picGraph";
            this.picGraph.Size = new System.Drawing.Size(685, 226);
            this.picGraph.TabIndex = 0;
            this.picGraph.TabStop = false;
            this.picGraph.Paint += new System.Windows.Forms.PaintEventHandler(this.picGraph_Paint);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.btnSaveMap);
            this.tabPage2.Controls.Add(this.trcDay);
            this.tabPage2.Controls.Add(this.picMain);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(711, 505);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Location Heat Map";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // trcDay
            // 
            this.trcDay.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trcDay.BackColor = System.Drawing.SystemColors.Window;
            this.trcDay.Location = new System.Drawing.Point(87, 451);
            this.trcDay.Name = "trcDay";
            this.trcDay.Size = new System.Drawing.Size(618, 45);
            this.trcDay.TabIndex = 3;
            this.trcDay.Scroll += new System.EventHandler(this.trcDay_Scroll);
            // 
            // picMain
            // 
            this.picMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.picMain.BackColor = System.Drawing.Color.Black;
            this.picMain.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.picMain.Location = new System.Drawing.Point(6, 6);
            this.picMain.Name = "picMain";
            this.picMain.Size = new System.Drawing.Size(699, 439);
            this.picMain.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picMain.TabIndex = 2;
            this.picMain.TabStop = false;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.trcTimeline);
            this.tabPage3.Controls.Add(this.lstPopulations);
            this.tabPage3.Controls.Add(this.chkNormalisePopulations);
            this.tabPage3.Controls.Add(this.chrtPopulation);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(711, 505);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Timeline";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // trcTimeline
            // 
            this.trcTimeline.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trcTimeline.BackColor = System.Drawing.SystemColors.Window;
            this.trcTimeline.Location = new System.Drawing.Point(174, 479);
            this.trcTimeline.Minimum = -1;
            this.trcTimeline.Name = "trcTimeline";
            this.trcTimeline.Size = new System.Drawing.Size(367, 45);
            this.trcTimeline.TabIndex = 4;
            this.trcTimeline.Scroll += new System.EventHandler(this.trcTimeline_Scroll);
            // 
            // lstPopulations
            // 
            this.lstPopulations.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lstPopulations.FormattingEnabled = true;
            this.lstPopulations.Location = new System.Drawing.Point(547, 6);
            this.lstPopulations.Name = "lstPopulations";
            this.lstPopulations.Size = new System.Drawing.Size(158, 485);
            this.lstPopulations.TabIndex = 3;
            // 
            // chkNormalisePopulations
            // 
            this.chkNormalisePopulations.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chkNormalisePopulations.AutoSize = true;
            this.chkNormalisePopulations.Location = new System.Drawing.Point(7, 479);
            this.chkNormalisePopulations.Name = "chkNormalisePopulations";
            this.chkNormalisePopulations.Size = new System.Drawing.Size(161, 17);
            this.chkNormalisePopulations.TabIndex = 2;
            this.chkNormalisePopulations.Text = "Normalise Population Counts";
            this.chkNormalisePopulations.UseVisualStyleBackColor = true;
            this.chkNormalisePopulations.CheckedChanged += new System.EventHandler(this.chkNormalisePopulations_CheckedChanged);
            // 
            // chrtPopulation
            // 
            this.chrtPopulation.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.Name = "ChartArea1";
            this.chrtPopulation.ChartAreas.Add(chartArea1);
            this.chrtPopulation.Cursor = System.Windows.Forms.Cursors.Arrow;
            legend1.Alignment = System.Drawing.StringAlignment.Center;
            legend1.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
            legend1.Name = "Legend1";
            this.chrtPopulation.Legends.Add(legend1);
            this.chrtPopulation.Location = new System.Drawing.Point(6, 6);
            this.chrtPopulation.Name = "chrtPopulation";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.EmptyPointStyle.AxisLabel = "Hello";
            series1.Legend = "Legend1";
            series1.Name = "Rabbit Population";
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Legend = "Legend1";
            series2.Name = "Death By Starvation";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series3.Legend = "Legend1";
            series3.Name = "Wolf Population";
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series4.Legend = "Legend1";
            series4.Name = "Death From Old Age";
            this.chrtPopulation.Series.Add(series1);
            this.chrtPopulation.Series.Add(series2);
            this.chrtPopulation.Series.Add(series3);
            this.chrtPopulation.Series.Add(series4);
            this.chrtPopulation.Size = new System.Drawing.Size(535, 466);
            this.chrtPopulation.TabIndex = 1;
            this.chrtPopulation.Text = "chart1";
            title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            title1.Name = "Title1";
            title1.Text = "Population Counts";
            title2.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Left;
            title2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            title2.Name = "Title2";
            title2.Text = "Animal Count";
            title2.TextOrientation = System.Windows.Forms.DataVisualization.Charting.TextOrientation.Rotated270;
            title3.Docking = System.Windows.Forms.DataVisualization.Charting.Docking.Bottom;
            title3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            title3.Name = "Title3";
            title3.Text = "In Game Days";
            this.chrtPopulation.Titles.Add(title1);
            this.chrtPopulation.Titles.Add(title2);
            this.chrtPopulation.Titles.Add(title3);
            this.chrtPopulation.Click += new System.EventHandler(this.chart1_Click);
            // 
            // btnSaveMap
            // 
            this.btnSaveMap.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSaveMap.Location = new System.Drawing.Point(6, 451);
            this.btnSaveMap.Name = "btnSaveMap";
            this.btnSaveMap.Size = new System.Drawing.Size(75, 45);
            this.btnSaveMap.TabIndex = 4;
            this.btnSaveMap.Text = "Save";
            this.btnSaveMap.UseVisualStyleBackColor = true;
            this.btnSaveMap.Click += new System.EventHandler(this.btnSaveMap_Click);
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "Rest Energy";
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "Life Expectancy";
            // 
            // columnHeader10
            // 
            this.columnHeader10.Text = "Aggression";
            // 
            // Analyser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(743, 570);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Analyser";
            this.Text = "SimpleRPG Analyer";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picGraph)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcDay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.picMain)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trcTimeline)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chrtPopulation)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.PictureBox picMain;
        private System.Windows.Forms.PictureBox picGraph;
        private System.Windows.Forms.ListView viewAnimals;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.DataVisualization.Charting.Chart chrtPopulation;
        private System.Windows.Forms.TrackBar trcDay;
        private System.Windows.Forms.CheckBox chkNormalisePopulations;
        private System.Windows.Forms.ListBox lstPopulations;
        private System.Windows.Forms.TrackBar trcTimeline;
        private System.Windows.Forms.Button btnSaveMap;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.ColumnHeader columnHeader9;
        private System.Windows.Forms.ColumnHeader columnHeader10;
    }
}

