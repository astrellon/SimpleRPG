﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;

namespace SimpleRPGAnalyser
{
    public partial class Analyser : Form
    {
        private char[] mTiles = { '.', '^', ',', ';', '#', 'S', '~', '*', 'R', 'r', 'T', 'd', 'w', 's', 'c', 'P' };
        private Color[] mColors = { Color.Green, Color.PaleGreen, Color.LightGreen, Color.ForestGreen, Color.BurlyWood, 
                                    Color.SandyBrown, Color.LightBlue, Color.Blue, Color.Gray, Color.LightGray, Color.DarkGreen, Color.DarkRed,
                                    Color.Cyan, Color.LightSeaGreen, Color.LawnGreen, Color.LimeGreen };

        private Color[] mColourList = { Color.Black, Color.DarkBlue, Color.DarkGreen, Color.DarkCyan, Color.DarkRed, Color.DarkMagenta, Color.Tan, Color.LightGray,
                                        Color.DarkGray, Color.Blue, Color.Green, Color.Cyan, Color.Red, Color.Magenta, Color.Yellow, Color.White };    
        private List<string> mMapList;
        private Dictionary<Color, char> mTileMap = new Dictionary<Color, char>();
        private Dictionary<char, Color> mColourMap = new Dictionary<char, Color>();
        private Dictionary<string, PopulationStat> mPopulations = new Dictionary<string, PopulationStat>();
        //private List<List<Vector2f>> mLocationHistory = new List<List<Vector2f>>();
        private List<DayEvents> mHistory = new List<DayEvents>();

        private Dictionary<char, string> mDeathNames = new Dictionary<char, string>();

        private Bitmap mMapImg = null;
        private Bitmap mMapOverlay = null;
        private Bitmap mMapTotal = null;
        private int mColourCount = 0;
        private int mCharCount = 0;
        private AboutBox mAbout = null;

        private int mCurrentDay = 0;
        private float mCurrentTime = 0;
        public static float mDayLength = 600;

        private Series mHighLight;

        public string mLoadedFullFileBase = "";
        public string mLoadedFileBase = "";
        public string mTitleBase;

        private List<KeyPair> mValues = null;

        private enum MapState { UNKNOWN, TILES, MAP, ENTITIES, OPTIONS, TILEDATA, REMOVEDENTITIES, END };

        public Analyser()
        {
            InitializeComponent();

            mDeathNames['o'] = mDeathNames['O'] = "Old Age";
            mDeathNames['k'] = mDeathNames['K'] = "Killed";
            mDeathNames['s'] = mDeathNames['S'] = "Starvation";

            mTitleBase = this.Text;
        }

        public void updateTitle(string extra)
        {
            this.Text = mTitleBase + ": " + extra;
        }

        public string[] cc = null;
        public string mFullPath = "";

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cc = null;
            //string fullPath = "";
            
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Map files |*.txt;*.out;*.map;*.srm| All files |*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    mFullPath = openFileDialog.FileName;
                    updateTitle("Loading " + mFullPath);
                    int extIndex = openFileDialog.FileName.LastIndexOf('.');
                    if (extIndex < 0)
                    {
                        MessageBox.Show("Unsupported file " + openFileDialog.FileName);
                        return;
                    }
                    mLoadedFullFileBase = openFileDialog.FileName.Substring(0, extIndex);
                    mLoadedFileBase = mLoadedFullFileBase.Substring(mLoadedFullFileBase.LastIndexOf('\\') + 1);
                    string ext = openFileDialog.FileName.Substring(extIndex + 1);

                    Regex fileParse = new Regex("" +
                        // Multi-line comments.
                        "(/\\*[^/\\*]*\\*/)|" +
                        // Single-line comments.
                        "(//[^\n]*\n)|" +
                        // Double quoted strings.
                        "(\"[^\"]*\")|" +
                        // Single quoted strings.
                        "('[^']*')|" +
                        // Everything that's not whitespace.
                        "(\\S+)");

                    StreamReader streamReader = new StreamReader(openFileDialog.FileName);
                    string fileText = streamReader.ReadToEnd();
                    streamReader.Close();

                    viewAnimals.Items.Clear();
                    Animal.Animals.Clear();
                    if (mMapList != null)
                    {
                        mMapList.Clear();
                    }
                    if (mTileMap != null)
                    {
                        mTileMap.Clear();
                    }
                    if (mColourMap != null)
                    {
                        mColourMap.Clear();
                    }
                    if (mMapImg != null)
                    {
                        mMapImg.Dispose();
                        mMapImg = null;
                    }
                    mPopulations.Clear();

                    mHistory.Clear();
                    mPopulations.Clear();
                    mColourCount = 0;
                    mCharCount = 0;

                    mMapList = new List<string>();
                    
                    string[] results = fileParse.Split(fileText);
                    List<string> cleanedUp = new List<string>();

                    for (int i = 0; i < results.Length; i++)
                    {
                        string line = results[i].Trim();
                        if (line.Length == 0)
                        {
                            continue;
                        }

                        // Skip comments.
                        if (line.Length >= 2 & (line[0] == '/' && (line[1] == '/' || line[1] == '*')))
                        {
                            continue;
                        }
                        cleanedUp.Add(line);
                    }

                    cc = cleanedUp.ToArray();
                }
            }

            BackgroundWorker worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(worker_RunWorkerCompleted);
            worker.DoWork += new DoWorkEventHandler(worker_DoWork);
            worker.ProgressChanged += new ProgressChangedEventHandler(worker_ProgressChanged);
            worker.RunWorkerAsync();
        }

        void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            updateTitle("Loading " + mFullPath + " " + e.ProgressPercentage + "%");
        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            if (cc != null && cc.Length > 0)
            {
                bool findState = false;
                string endState = null;
                string state = "none";
                char tile = '\0';
                int mapWidth = -1;
                Dictionary<string, float> values = new Dictionary<string, float>();
                int numAnimals = 0;
                int oldPercent = -1;

                for (int i = 0; i < cc.Length; i++)
                {
                    string line = cc[i];
                    if (line == "--")
                    {
                        findState = true;
                        continue;
                    }

                    int percent = (int)(((float)i / (float)cc.Length) * 100);
                    if (percent != oldPercent)
                    {
                        oldPercent = percent;
                        worker.ReportProgress(percent);
                    }

                    if (findState)
                    {
                        findState = false;
                        endState = line;
                    }

                    if (state == null || state.Equals("none", StringComparison.CurrentCultureIgnoreCase) ||
                        state.Equals("TileData", StringComparison.CurrentCultureIgnoreCase))
                    {

                    }
                    else if (state.Equals("tiles", StringComparison.CurrentCultureIgnoreCase))
                    {
                        if (mCharCount < mColors.Length)
                        {
                            if (tile == '\0')
                            {
                                tile = line[0];
                            }
                            else
                            {
                                int t = int.Parse(line);
                                if (!mColourMap.ContainsKey(tile))
                                {
                                    Console.WriteLine("Adding char {0} with colour {1}", tile, mColors[t].ToKnownColor().ToString());
                                    mColourMap[tile] = mColors[t];
                                    //addColour(mColors[t], tile, true);
                                    mCharCount++;
                                    tile = '\0';
                                }
                            }
                        }
                    }
                    else if (state.Equals("options", StringComparison.CurrentCultureIgnoreCase))
                    {
                        switch (line)
                        {
                            case "current_time":
                                mCurrentTime = float.Parse(cc[++i]);
                                break;
                            case "current_day":
                                mCurrentDay = int.Parse(cc[++i]);
                                break;
                            case "day_length":
                                mDayLength = float.Parse(cc[++i]);
                                break;
                            default:
                                break;
                        }
                    }
                    else if (state.Equals("map", StringComparison.CurrentCultureIgnoreCase))
                    {
                        if (endState != null)
                        {
                            renderImage(ref mMapTotal);
                        }
                        else
                        {
                            if (mMapList.Count == 0)
                            {
                                mapWidth = line.Length;
                            }
                            mMapList.Add(line);
                        }
                    }
                    else if (state.Equals("entities", StringComparison.CurrentCultureIgnoreCase) ||
                                state.Equals("RemovedEntities", StringComparison.CurrentCultureIgnoreCase))
                    {
                        if (line == "Animal")
                        {
                            i++;
                            Animal a = new Animal();
                            a.load(ref cc, ref i);
                            i--;

                            string identifier = a.graphic.ToString() + '_' + a.species;
                            if (!mPopulations.ContainsKey(identifier))
                            {
                                mPopulations[identifier] = new PopulationStat();
                            }

                            if (a.isDead)
                            {
                                mPopulations[identifier].dead++;
                            }
                            else
                            {
                                mPopulations[identifier].alive++;
                            }
                        }
                    }
                    else if (state.Equals("history", StringComparison.CurrentCultureIgnoreCase))
                    {
                        if (line.Equals("DayEvent", StringComparison.CurrentCultureIgnoreCase))
                        {
                            DayEvents day = new DayEvents();
                            day.load(ref cc, ref i);
                            mHistory.Add(day);
                        }
                    }
                    else
                    {
                        Console.WriteLine("Unknown state {0}", state);
                    }

                    if (endState != null)
                    {
                        state = endState;
                        endState = null;
                    }
                }
                mValues = new List<KeyPair>();
                foreach (KeyValuePair<string, float> pair in values)
                {
                    float value = pair.Value / numAnimals;
                    KeyPair p = new KeyPair(pair.Key, value);
                    if (pair.Key.Equals("diet", StringComparison.CurrentCultureIgnoreCase))
                    {
                        p.max = 1.0f;
                    }
                    mValues.Add(p);
                }
            }
        }

        void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            renderOverlay(0);
            renderImage(ref mMapTotal);
            picMain.Image = mMapTotal;
            foreach (KeyValuePair<int, Animal> pair in Animal.Animals)
            {
                Animal a = pair.Value;
                string[] subItems = new string[] { a.id.ToString(), a.name, a.species, a.age.ToString(), a.birthdate, a.deathdate, a.deathby, a.rest_energy_per_day.ToString(), a.life_expectancy.ToString(), a.aggression.ToString() };
                ListViewItem item = new ListViewItem(subItems);
                item.Tag = a;
                viewAnimals.Items.Add(item);
            }

            trcDay.Maximum = mHistory.Count - 1;
            trcTimeline.Maximum = mHistory.Count - 1;

            tabPage1.Invalidate();

            updateCharts();

            updateTitle(mFullPath);
        }

        private void updateCharts(bool normalise = false)
        {
            chrtPopulation.Series.Clear();

            List<string> species = new List<string>();
            Dictionary<string, float> maxValues = new Dictionary<string, float>();

            foreach (KeyValuePair<string, bool> pair in DayEvents.ALL_SPECIES)
            {
                species.Add(pair.Key);
            }

            species.Sort();

            if (normalise)
            {
                foreach (string s in species)
                {
                    maxValues[s] = 0;
                }
                for (int i = 0; i < mCurrentDay; i++)
                {
                    foreach (string s in species)
                    {
                        maxValues[s] = Math.Max(maxValues[s], mHistory[i].getPopulation(s));
                    }
                }
            }

            float maxPopValue = 0;
            string maxPopSpecies = null;
            foreach (KeyValuePair<string, float> pair in maxValues)
            {
                //maxPopValue = Math.Max(value, maxPopValue);
                if (pair.Value > maxPopValue)
                {
                    maxPopValue = pair.Value;
                    maxPopSpecies = pair.Key;
                }
            }

            foreach (string s in species)
            {
                Series series = new Series("Population " + s);
                series.ChartType = SeriesChartType.Line;
                chrtPopulation.Series.Add(series);

                series = new Series("Births " + s);
                series.ChartType = SeriesChartType.Line;
                chrtPopulation.Series.Add(series);

                series = new Series("Deaths " + s);
                series.ChartType = SeriesChartType.Line;
                chrtPopulation.Series.Add(series);
            }

            mHighLight = new Series("Highlight");
            mHighLight.ChartType = SeriesChartType.Column;
            mHighLight.Color = Color.FromArgb(0x33000000);
            chrtPopulation.Series.Add(mHighLight);

            for (int i = 0; i < mHistory.Count; i++)
            {
                int j = 0;
                foreach (string s in species)
                {
                    float modifier = 1;
                    if (maxPopSpecies != s && maxPopSpecies != null)
                    {
                        modifier = maxPopValue / maxValues[s];
                    }

                    float v = mHistory[i].getPopulation(s) * modifier;
                    chrtPopulation.Series[j++].Points.AddXY((double)i, (double)v);

                    v = mHistory[i].getBirths(s) * modifier;
                    chrtPopulation.Series[j++].Points.AddXY((double)i, (double)v);

                    v = mHistory[i].getTotalDeaths(s) * modifier;
                    chrtPopulation.Series[j++].Points.AddXY((double)i, (double)v);
                }
            }
        }

        private void renderImage(ref Bitmap pic)
        {
            int width = mMapList[0].Length;
            int height = mMapList.Count;

            if (mMapImg == null)
            {
                mMapImg = new Bitmap(width, height);
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        char c = mMapList[y][x];
                        if (mColourMap.ContainsKey(c))
                        {
                            mMapImg.SetPixel(x, y, mColourMap[c]);
                        }
                        else
                        {
                            mMapImg.SetPixel(x, y, Color.Magenta);
                        }
                    }
                }
            }

            if (pic != null)
            {
                pic.Dispose();
            }
            pic = new Bitmap(width, height);
            
            ImageAttributes imageAttrs = new ImageAttributes();

            float[][] colorMatrixElements = { 
                new float[] {1,  0,  0,  0, 0},        // red scaling factor of 2
                new float[] {0,  1,  0,  0, 0},        // green scaling factor of 1
                new float[] {0,  0,  1,  0, 0},        // blue scaling factor of 1
                new float[] {0,  0,  0,.25f, 0},        // alpha scaling factor of 1
                new float[] {0,  0,  0,  0, 1}};    // three translations of 0.2

            ColorMatrix colorMatrix = new ColorMatrix(colorMatrixElements);

            imageAttrs.SetColorMatrix(
                colorMatrix,
                ColorMatrixFlag.Default,
                ColorAdjustType.Bitmap);

            Graphics g = Graphics.FromImage(pic);

            Rectangle rect = new Rectangle(0, 0, width, height);
            g.FillRectangle(Brushes.Black, rect);

            //g.DrawImage(mMapImg, new Point[]{new Point()}, new Rectangle(0, 0, width, height), GraphicsUnit.Display, imageAttrs);
            g.DrawImage(mMapImg, rect,
                0, 0,
                width, height,
                GraphicsUnit.Pixel,
                imageAttrs);

            if (mMapOverlay != null)
            {
                g.DrawImage(mMapOverlay, new Point(0, 0));
            }

            
            //picMain.Image = mMapTotal;
        }

        private void renderOverlay(int day)
        {
            int width = mMapList[0].Length;
            int height = mMapList.Count;

            if (mMapOverlay != null)
            {
                mMapOverlay.Dispose();
            }
            mMapOverlay = new Bitmap(width, height);
            if (day < 0)
            {
                return;
            }

            DayEvents dayEvent = mHistory[day];
            foreach (Location location in dayEvent.locations)
            {
                int x = location.position.x;
                int y = location.position.y;
                mMapOverlay.SetPixel(x, y, mColourList[Animal.Animals[location.id].graphic.fullColour]);
            }
        }

        private Color invertColor(Color c)
        {
            return Color.FromArgb(255 - c.R, 255 - c.G, 255 - c.B);
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mAbout == null || mAbout.IsDisposed)
            {
                mAbout = new AboutBox();
            }
            mAbout.Show();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selected = viewAnimals.SelectedItems;
            if (selected.Count > 0)
            {
                List<Animal> animals = new List<Animal>();
                foreach (ListViewItem item in selected)
                {
                    animals.Add((Animal)item.Tag);
                }
                Animal averaged = Animal.averageAnimals(animals);

                mValues = new List<KeyPair>();
                mValues.Add(new KeyPair("Diet", averaged.diet, 1, Color.Gray));
                mValues.Add(new KeyPair("Base Energy Requirements", averaged.rest_energy_per_day, 800, Color.DarkGray));
                mValues.Add(new KeyPair("Strength", averaged.strength, Color.LightGray));
                mValues.Add(new KeyPair("Dexterity", averaged.dexterity, Color.LightGray));
                mValues.Add(new KeyPair("Intelligence", averaged.intelligence, Color.LightGray));
                mValues.Add(new KeyPair("Life Expectancy", averaged.life_expectancy, Color.SkyBlue));
                mValues.Add(new KeyPair("Fertility", averaged.fertility, 10, Color.SkyBlue));
                mValues.Add(new KeyPair("Breeding Age", averaged.breeding_age, Color.SkyBlue));
                mValues.Add(new KeyPair("Breeding Rate", averaged.breeding_rate, Color.SkyBlue));
                mValues.Add(new KeyPair("Health", averaged.maxHealth, Color.Red));
                mValues.Add(new KeyPair("Mass", averaged.entity_mass, Color.White));
                mValues.Add(new KeyPair("Size", averaged.entity_size, Color.White));
                mValues.Add(new KeyPair("Mutation Rate", averaged.mutation_rate, 1, Color.Purple));
                mValues.Add(new KeyPair("Mutation Amount", averaged.mutation_amount, 1, Color.Purple));
                mValues.Add(new KeyPair("Num Alive Children", averaged.numAliveChildren, Color.Blue));
            }
            picGraph.Invalidate();
        }

        private void viewAnimals_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            if (!(viewAnimals.ListViewItemSorter is AnimalSorter))
            {
                viewAnimals.ListViewItemSorter = new AnimalSorter();
            }
            AnimalSorter sorter = (AnimalSorter)viewAnimals.ListViewItemSorter;

            if (sorter.LastSort == e.Column)
            {
                if (viewAnimals.Sorting == SortOrder.Ascending)
                    viewAnimals.Sorting = SortOrder.Descending;
                else
                    viewAnimals.Sorting = SortOrder.Ascending;
            }
            else
            {
                viewAnimals.Sorting = SortOrder.Descending;
            }

            sorter.ByColumn = e.Column;

            viewAnimals.Sort();
        }

        private Color upColor(Color c)
        {
            int r = c.R;
            int g = c.G;
            int b = c.B;

            int s = 30;

            if (r + s > 255) r = r - s; else r = r + s;
            if (g + s > 255) g = g - s; else g = g + s;
            if (b + s > 255) b = b - s; else b = b + s;

            return Color.FromArgb(r, g, b);
        }

        private void picGraph_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            if (mValues == null || mValues.Count == 0)
            {
                return;
            }
            float maxHeight = e.ClipRectangle.Height - 5;

            float maxValue = 50.0f;

            foreach (KeyPair pair in mValues)
            {
                if (pair.max < 0.0f)
                {
                    maxValue = Math.Max(maxValue, pair.value);
                }
            }

            float width = (e.ClipRectangle.Width - 5) / (mValues.Count);

            Font font = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Bold);
            Brush textBrush = Brushes.Black;
            Brush textBrushShadow = Brushes.White;

            List<KeyPair> sorted = mValues.ToList();
            
            float i = 0;
            Matrix mx = new Matrix(1, 0, 0, 1, 0, 0);
            mx.Rotate(90, MatrixOrder.Append);
            g.Transform = mx;

            SolidBrush fill = new SolidBrush(Color.SkyBlue);
            SolidBrush line = new SolidBrush(upColor(Color.SkyBlue));
            
            Pen outline = new Pen(line, 4);
            
            foreach (KeyPair pair in sorted)
            {
                float height = 0.0f;
                float m = maxValue;
                if (pair.max > 0)
                {
                    m = pair.max;
                }
                height = pair.value / m * maxHeight;

                SolidBrush f = fill;
                SolidBrush l = line;
                if (pair.color != Color.Transparent)
                {
                    f = new SolidBrush(pair.color);
                    l = new SolidBrush(upColor(pair.color));
                }
                outline.Brush = l;

                Rectangle rect = new Rectangle((int)(maxHeight - height), (int)(-(i + 1) * width), (int)height, (int)width);

                string txt = pair.value.ToString() + " " + pair.name;

                SizeF txtSize = g.MeasureString(txt, font);
                g.FillRectangle(f, rect);
                g.DrawRectangle(outline, rect);
                g.DrawString(txt, font, textBrushShadow, maxHeight - 4 - txtSize.Width, -(i + 0.5f) * width + 1);//, format);
                g.DrawString(txt, font, textBrush, maxHeight - 5 - txtSize.Width, -(i + 0.5f) * width);//, format);

                i++;
            }
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void trcDay_Scroll(object sender, EventArgs e)
        {
            renderOverlay(trcDay.Value);
            renderImage(ref mMapTotal);
            picMain.Image = mMapTotal;
        }

        private void chkNormalisePopulations_CheckedChanged(object sender, EventArgs e)
        {
            updateCharts(chkNormalisePopulations.Checked);
        }

        private void updatePopulationList(int day)
        {
            mHighLight.Points.Clear();
            lstPopulations.Items.Clear();
            if (day < 0 || day >= mHistory.Count)
            {
                return;
            }
            int max = 0;
            
            DayEvents dayEvent = mHistory[day];
            lstPopulations.Items.Add("Day " + day);
            foreach (KeyValuePair<string, int> s in dayEvent.populations)
            {
                max = Math.Max(s.Value, max);
                lstPopulations.Items.Add(s.Key + " Population: " + s.Value);
                lstPopulations.Items.Add(s.Key + " Births: " + dayEvent.getBirths(s.Key));
                if(dayEvent.deaths.ContainsKey(s.Key))
                {
                    foreach (KeyValuePair<char, int> pair in dayEvent.deaths[s.Key])
                    {
                        lstPopulations.Items.Add(s.Key + " Deaths by " + mDeathNames[pair.Key] + ": " + pair.Value);
                    }
                }
            }
            mHighLight.Points.AddXY((double)day, (double)max);
        }

        private void trcTimeline_Scroll(object sender, EventArgs e)
        {
            updatePopulationList(trcTimeline.Value);
        }

        private int mSaveImageIndex = 0;

        private void btnSaveMap_Click(object sender, EventArgs e)
        {
            string filename = mLoadedFullFileBase + "_" + mSaveImageIndex + ".png";

            if (File.Exists(filename))
            {
                do
                {
                    mSaveImageIndex++;
                    filename = mLoadedFullFileBase + "_" + mSaveImageIndex + ".png";
                } while (File.Exists(filename));
            }

            mMapTotal.Save(filename, ImageFormat.Png);
        }

        private Bitmap mPicTemp = new Bitmap(10, 10);

        private void renderTimeStamp(int day, Bitmap orig, ref Bitmap pic)
        {
            if (pic != null)
            {
                pic.Dispose();
            }
            pic = new Bitmap(orig.Width, orig.Height + 32);

            Graphics g = Graphics.FromImage(pic);
            g.DrawImage(orig, new Point());

            Font font = new Font(FontFamily.GenericSansSerif, 22);
            g.DrawString(day.ToString(), font, Brushes.Black, 4, orig.Height + 2);
        }

        private void btnSaveAll_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog openFolder = new FolderBrowserDialog())
            {
                openFolder.SelectedPath = Application.StartupPath;
                if (openFolder.ShowDialog() == DialogResult.OK)
                {
                    Console.WriteLine("Saving to folder: " + openFolder.SelectedPath);
                    
                    for (int i = 0; i < mHistory.Count; i++)
                    {
                        renderOverlay(i);
                        renderImage(ref mMapTotal);
                        renderTimeStamp(i, mMapTotal, ref mPicTemp);
                        string filename = openFolder.SelectedPath + "\\" + mLoadedFileBase + "_" + i + ".png";
                        mPicTemp.Save(filename, ImageFormat.Png);
                    }
                    MessageBox.Show("Completed saving " + mHistory.Count + " images.");
                    //MessageBox.Show("Selected Folder: " + openFolder.SelectedPath);
                }
            }
        }
    }
}
