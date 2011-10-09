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

namespace SimpleRPGAnalyser
{
    public partial class Analyser : Form
    {
        private char[] mTiles = { '.', '^', ',', ';', '#', 'S', '~', '*', 'R', 'r', 'T', 'd', 'w', 's', 'c', 'P' };
        private Color[] mColors = { Color.Green, Color.PaleGreen, Color.LightGreen, Color.ForestGreen, Color.BurlyWood, 
                                    Color.SandyBrown, Color.LightBlue, Color.Blue, Color.Gray, Color.LightGray, Color.DarkGreen, Color.DarkRed,
                                    Color.Cyan, Color.LightSeaGreen, Color.LawnGreen, Color.LimeGreen };
        private List<string> mMapList;
        private Dictionary<Color, char> mTileMap = new Dictionary<Color, char>();
        private Dictionary<char, Color> mColourMap = new Dictionary<char, Color>();
        private List<List<Vector2f>> mLocationHistory = new List<List<Vector2f>>();

        private Bitmap mMapImg = null;
        private Bitmap mMapOverlay = null;
        private Bitmap mMapTotal = null;
        private int mColourCount = 0;
        private int mCharCount = 0;
        private AboutBox mAbout = null;

        private int mCurrentDay = 0;
        private float mCurrentTime = 0;
        public static float mDayLength = 600;

        private List<KeyPair> mValues = null;

        private enum MapState { UNKNOWN, TILES, MAP, ENTITIES, OPTIONS, TILEDATA, REMOVEDENTITIES, END };

        public Analyser()
        {
            InitializeComponent();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Map files |*.txt;*.out| All files |*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    int extIndex = openFileDialog.FileName.LastIndexOf('.');
                    if (extIndex < 0)
                    {
                        MessageBox.Show("Unsupported file " + openFileDialog.FileName);
                        return;
                    }
                    string ext = openFileDialog.FileName.Substring(extIndex + 1);

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
                    mColourCount = 0;
                    mCharCount = 0;

                    if (ext.Equals("txt", StringComparison.InvariantCultureIgnoreCase) ||
                        ext.Equals("out", StringComparison.InvariantCultureIgnoreCase))
                    {
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

                        bool findState = false;
                        string endState = null;
                        string state = "none";

                        //bool readingAnimal = false;

                        Dictionary<string, float> values = new Dictionary<string, float>();
                        int numAnimals = 0;

                        char tile = '\0';

                        int endSkip = 0;

                        mMapList = new List<string>();
                        int mapWidth = -1;

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

                        string [] cc = cleanedUp.ToArray();
                        for (int i = 0; i < cc.Length; i++)
                        {
                            string line = cc[i];
                            if (line == "--")
                            {
                                findState = true;
                                continue;
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
                                    //mMapImg = new Bitmap(mapWidth, mMapList.Count);
                                    renderImage();
                                    picMain.Image = mMapTotal;
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
                                    Console.WriteLine("Loaded Animal " + a.LongName);
                                    string[] subItems = new string[] { a.id.ToString(), a.name, a.species, a.age.ToString(), a.birthdate, a.deathdate, a.deathby };
                                    ListViewItem item = new ListViewItem(subItems);
                                    item.Tag = a;
                                    viewAnimals.Items.Add(item);
                                }
                            }
                            else if (state.Equals("locationhistory", StringComparison.CurrentCultureIgnoreCase))
                            {
                                if (line == "begin")
                                {
                                    List<Vector2f> list = new List<Vector2f>();
                                    mLocationHistory.Add(list);
                                    i++;
                                    line = cc[i];
                                    while (line != "end")
                                    {
                                        float x = float.Parse(line); i++;
                                        line = cc[i];
                                        float y = float.Parse(line); i++;
                                        line = cc[i];
                                        list.Add(new Vector2f(x, y));
                                    }
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

                    trcDay.Maximum = mLocationHistory.Count - 1;

                    tabPage1.Invalidate();

                    chrtPopulation.Series[0].Points.Clear();
                    chrtPopulation.Series[1].Points.Clear();
                    chrtPopulation.Series[2].Points.Clear();

                    for (int i = 0; i < mCurrentDay; i++)
                    {
                        chrtPopulation.Series[0].Points.AddY((double)(Animal.getAnimalsAliveOn(i).Count));
                        List<Animal> died = Animal.getAnimalsDiedOn(i);
                        double starvation = 0;
                        double oldAge = 0;
                        for (int j = 0; j < died.Count; j++)
                        {
                            string d = died[j].deathby;
                            if (d[0] == 'S' || d[0] == 's')
                            {
                                starvation += 1;
                            }
                            else if (d[0] == 'O' || d[0] == 'o')
                            {
                                oldAge += 1;
                            }
                        }
                        chrtPopulation.Series[1].Points.AddY(starvation);
                        chrtPopulation.Series[2].Points.AddY(oldAge);
                    }
                }

            }
        }

        private void renderImage()
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

            if (mMapTotal != null)
            {
                mMapTotal.Dispose();
            }
            mMapTotal = new Bitmap(mMapImg);
            Graphics g = Graphics.FromImage(mMapTotal);

            if (mMapOverlay != null)
            {
                g.DrawImage(mMapOverlay, new Point(0, 0));
            }
            picMain.Image = mMapTotal;
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

            List<Vector2f> list = mLocationHistory[day];
            foreach (Vector2f p in list)
            {
                int x = (int)Math.Round(p.x);
                int y = (int)Math.Round(p.y);
                mMapOverlay.SetPixel(x, y, invertColor(mMapImg.GetPixel(x, y)));
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
            List<Animal> animals = new List<Animal>();
            foreach (ListViewItem item in selected)
            {
                animals.Add((Animal)item.Tag);
            }
            Animal averaged = Animal.averageAnimals(animals);

            mValues = new List<KeyPair>();
            mValues.Add(new KeyPair("Diet", averaged.diet, 1, Color.Gray));
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
            renderImage();
        }
    }
}
