// SimpleRPG Image to Map converted by Alan Lawrey for Thesis 2011.

using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.IO;

namespace SimpleRPGMapEditor
{
    public partial class MainView : Form
    {
        private char[] mTiles = {'.', '^', ',', ';', '#', 'S', '~', '*'};
        private Color[] mColors = { Color.Green, Color.DarkGreen, Color.LightGreen, Color.ForestGreen, Color.BurlyWood, Color.SandyBrown, Color.LightBlue, Color.Blue };
        private List<string> mMapList;
        private Dictionary<Color, char> mTileMap = new Dictionary<Color, char>();
        private Dictionary<char, Color> mColourMap = new Dictionary<char, Color>();
        private Bitmap mMapImg = null;
        private int mColourCount = 0;
        private int mCharCount = 0;

        private enum MapState {UNKNOWN, TILES, MAP, ENTITIES, OPTIONS, END};

        public MainView()
        {
            InitializeComponent();
            //mUsedColors = (Color[])mColors.Clone();
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All Supported |*.bmp;*.jpg;*.png;*.txt|Images files |*.bmp;*.jpg;*.png|Map files |*.txt| All files |*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    tmrClear.Stop();

                    int extIndex = openFileDialog.FileName.LastIndexOf('.');
                    if (extIndex < 0)
                    {
                        MessageBox.Show("Unsupported file " + openFileDialog.FileName);
                        return;
                    }
                    string ext = openFileDialog.FileName.Substring(extIndex + 1);

                    if (ext.Equals("txt", StringComparison.InvariantCultureIgnoreCase))
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

                        char tile = '\0';

                        mCharCount = 0;

                        mMapList = new List<string>();
                        int mapWidth = -1;

                        string[] results = fileParse.Split(fileText);
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

                            if (state == null || state.Equals("none", StringComparison.CurrentCultureIgnoreCase))
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
                                            addColour(mColors[t], tile, true);
                                            mCharCount++;
                                            tile = '\0';
                                        }
                                    }
                                }
                            }
                            else if (state.Equals("map", StringComparison.CurrentCultureIgnoreCase))
                            {
                                if (endState != null)
                                {
                                    mMapImg = new Bitmap(mapWidth, mMapList.Count);
                                    renderImage();
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
                        tssMain.Text = "Loaded '" + openFileDialog.FileName + "' (" + mCharCount + ")";
                        tmrClear.Start();
                    }
                    else
                    {
                        mMapImg = new Bitmap(openFileDialog.FileName);
                        picMain.Image = mMapImg;

                        mColourCount = 0;
                        mTileMap.Clear();
                        pnlColours.Controls.Clear();

                        for (int y = 0; y < mMapImg.Height; y++)
                        {
                            for (int x = 0; x < mMapImg.Width; x++)
                            {
                                Color c = mMapImg.GetPixel(x, y);
                                if (!mTileMap.ContainsKey(c) && mColourCount < mTiles.Length)
                                {
                                    char t = mTiles[mColourCount];
                                    addColour(c, t);
                                    mTileMap[c] = t;
                                    mColourCount++;
                                }
                            }
                        }

                        tssMain.Text = "Loaded '" + openFileDialog.FileName + "' (" + mColourCount + ")";
                        tmrClear.Start();
                    }
                }
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (mMapImg == null)
            {
                return;
            }

            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                tmrClear.Stop();
                saveFileDialog.Filter = "Map file |*.txt|PNG Image |*.png|BMP Image |*.bmp|JPG Image |*.jpg|All files |*.*";
                saveFileDialog.FilterIndex = 1;
                saveFileDialog.RestoreDirectory = true;

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    int extIndex = saveFileDialog.FileName.LastIndexOf('.');
                    if (extIndex < 0)
                    {
                        MessageBox.Show("Unsupported file " + saveFileDialog.FileName);
                        return;
                    }

                    string ext = saveFileDialog.FileName.Substring(extIndex + 1);

                    if (ext.Equals("txt", StringComparison.InvariantCultureIgnoreCase))
                    {
                        StreamWriter writer = new StreamWriter(saveFileDialog.FileName);
                        StringBuilder builder = new StringBuilder("// Converted by SimpleRPG Img to Map program at ");

                        builder.Append(DateTime.Now.ToString());

                        builder.AppendLine();
                        builder.Append("-- Tiles");
                        builder.AppendLine();

                        for (int i = 0; i < mTiles.Length; i++)
                        {
                            builder.Append(mTiles[i]);
                            builder.Append(' ');
                            builder.Append(i);
                            builder.AppendLine();
                        }

                        builder.AppendLine();
                        builder.AppendLine("-- Map");
                        for (int y = 0; y < mMapImg.Height; y++)
                        {
                            for (int x = 0; x < mMapImg.Width; x++)
                            {
                                Color c = mMapImg.GetPixel(x, y);
                                if (!mTileMap.ContainsKey(c))
                                {
                                    builder.Append('?');
                                }
                                else
                                {
                                    char t = mTileMap[c];
                                    builder.Append(t);
                                }
                            }
                            builder.AppendLine();
                        }

                        builder.AppendLine();
                        builder.AppendLine("-- End");
                        writer.Write(builder.ToString());
                        writer.Close();
                    }
                    else
                    {
                        mMapImg.Save(saveFileDialog.FileName);
                    }
                    tssMain.Text = "Saved '" + saveFileDialog.FileName + "'";
                    tmrClear.Start();
                }
            }
        }

        private void addColour(Color c, char t, bool editColour = false)
        {
            Panel panel = new Panel();
            panel.BackColor = c;
            panel.Width = 24;
            panel.Height = 24;
            panel.Left = 5;
            panel.Tag = t;
            panel.Top = 6 + pnlColours.Controls.Count / 2 * 30;
            if (editColour)
            {
                panel.Click += new EventHandler(panel_Click);
            }
            pnlColours.Controls.Add(panel);

            TextBox textbox = new TextBox();
            textbox.Font = new Font("Arial", 12);
            textbox.Width = 24;
            textbox.Height = 23;
            textbox.MaxLength = 1;
            textbox.Top = panel.Top;
            textbox.Left = pnlColours.Width - 5 - textbox.Width;
            textbox.Text = t.ToString();
            if (!editColour)
            {
                textbox.TextChanged += new EventHandler(textbox_TextChanged);
            }
            else
            {
                textbox.Enabled = false;
            }
            textbox.Tag = c;
            pnlColours.Controls.Add(textbox);
        }

        void panel_Click(object sender, EventArgs e)
        {
            Panel p = sender as Panel;
            char c = (char)p.Tag;
            mColourMap[c] = Color.Maroon;
            p.BackColor = mColourMap[c];
            renderImage();
            MessageBox.Show(p.BackColor.ToString());
        }

        void textbox_TextChanged(object sender, EventArgs e)
        {
            TextBox t = sender as TextBox;
            if (t == null)
            {
                return;
            }

            Color c = (Color)t.Tag;

            if (mTileMap.ContainsKey(c))
            {
                mTileMap[c] = t.Text[0];
            }
        }

        private void tmrClear_Tick(object sender, EventArgs e)
        {
            tmrClear.Enabled = false;
            tssMain.Text = "";
        }

        private void renderImage()
        {
            int width = mMapList[0].Length;
            int height = mMapList.Count;
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

            picMain.Image = mMapImg;
        }
    }
}
