using System;
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
    public class KeyPair
    {
        public string name;
        public float value;
        public float max = -1;

        public KeyPair()
        {

        }
        public KeyPair(string name, float value)
        {
            this.name = name;
            this.value = value;
        }
        public KeyPair(string name, float value, float max)
        {
            this.name = name;
            this.value = value;
            this.max = max;
        }
    }

    public partial class Analyser : Form
    {
        private char[] mTiles = { '.', '^', ',', ';', '#', 'S', '~', '*', 'R', 'r', 'T', 'd', 'w' };
        private Color[] mColors = { Color.Green, Color.PaleGreen, Color.LightGreen, Color.ForestGreen, Color.BurlyWood, Color.SandyBrown, Color.LightBlue, Color.Blue, Color.Gray, Color.LightGray, Color.DarkGreen, Color.DarkRed, Color.Cyan };
        private List<string> mMapList;
        private Dictionary<Color, char> mTileMap = new Dictionary<Color, char>();
        private Dictionary<char, Color> mColourMap = new Dictionary<char, Color>();
        private string[] _mValidProperties = { "strength", "dexterity", "intelligence", "running_speed", "walking_speed", "turning_speed",
	"entity_size", "entity_mass", "diet", "damage_base", "attack_rate", "sight_radius", "mutation_rate", "mutation_amount", "life_expectancy", "breeding_age", "breeding_rate", "fertility" };

        private Dictionary<string, bool> mValidProperties = new Dictionary<string, bool>();
        private Bitmap mMapImg = null;
        private int mColourCount = 0;
        private int mCharCount = 0;
        private AboutBox mAbout = null;

        private List<KeyPair> mValues = null;

        private enum MapState { UNKNOWN, TILES, MAP, ENTITIES, OPTIONS, TILEDATA, REMOVEDENTITIES, END };

        public Analyser()
        {
            InitializeComponent();

            for (int i = 0; i < _mValidProperties.Length; i++)
            {
                mValidProperties.Add(_mValidProperties[i], true);
            }
        }

        private bool isValidProperty(string property)
        {
            return mValidProperties.ContainsKey(property);
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Map files |*.txt| All files |*.*";
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

                        bool readingAnimal = false;

                        Dictionary<string, float> values = new Dictionary<string, float>();
                        int numAnimals = 0;

                        char tile = '\0';

                        int endSkip = 0;

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
                            else if (state.Equals("entities", StringComparison.CurrentCultureIgnoreCase) ||
                                     state.Equals("RemovedEntities", StringComparison.CurrentCultureIgnoreCase))
                            {
                                if (!readingAnimal && line == "Animal")
                                {
                                    readingAnimal = true;
                                    Console.WriteLine("New Animal");
                                    numAnimals++;
                                }
                                else if (readingAnimal)
                                {
                                    if (line == "end")
                                    {
                                        if (endSkip <= 0)
                                        {
                                            readingAnimal = false;
                                            Console.WriteLine("End Animal");
                                        }
                                        else
                                        {
                                            endSkip--;
                                        }
                                    }
                                    else
                                    {
                                        if (line.Equals("Action") ||
                                            line.Equals("action_history", StringComparison.CurrentCultureIgnoreCase))
                                        {
                                            endSkip++;
                                        }
                                        else
                                        {
                                            if (isValidProperty(line))
                                            {
                                                string prop = line;
                                                if (!values.ContainsKey(prop))
                                                {
                                                    values.Add(prop, 0);
                                                }
                                                do
                                                {
                                                    line = results[++i].Trim();
                                                } while (line.Length == 0);

                                                float value = float.Parse(line);
                                                values[prop] = values[prop] + value;
                                            }
                                        }
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
                }
            }
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

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mAbout == null || mAbout.IsDisposed)
            {
                mAbout = new AboutBox();
            }
            
            mAbout.Show();
        }

        private void tabPage1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            //g.Clear(Color.White);
            
            if (mValues == null || mValues.Count == 0)
            {
                return;
            }
            float maxHeight = e.ClipRectangle.Height - 5;

            float maxValue = -1.0f;

            foreach (KeyPair pair in mValues)
            {
                if (pair.max < 0.0f)
                {
                    maxValue = Math.Max(maxValue, pair.value);
                }
            }

            float width = (e.ClipRectangle.Width - 5) / (mValues.Count);

            StringFormat format = new StringFormat(StringFormatFlags.DirectionRightToLeft);
            Font font = new Font(FontFamily.GenericSansSerif, 10);
            Brush textBrush = Brushes.Black;
            Brush textBrushShadow = Brushes.White;

            float i = 0;
            Matrix mx = new Matrix(1, 0, 0, 1, 0, 0);
            mx.Rotate(90, MatrixOrder.Append);
            g.Transform = mx;

            Brush line = Brushes.SkyBlue;
            Pen p = new Pen(line, 4);
            Brush fill = Brushes.LightSkyBlue;

            foreach (KeyPair pair in mValues)
            {
                float height = 0.0f;
                float m = maxValue;
                if (pair.max > 0)
                {
                    m = pair.max;
                }
                height = pair.value / m * maxHeight;

                Rectangle rect = new Rectangle((int)(maxHeight - height), (int)(-(i + 1) * width), (int)height, (int)width);
                //continue;
                //LinearGradientBrush fill = new LinearGradientBrush(rect, Color.Blue, Color.LightBlue, 0.0f);

                g.FillRectangle(fill, rect);
                g.DrawRectangle(p, rect);
                g.DrawString(pair.name, font, textBrushShadow, maxHeight - 4, -(i + 0.5f) * width + 1, format);
                g.DrawString(pair.name, font, textBrush, maxHeight - 5, -(i + 0.5f) * width, format);
                
                i++;
            }
        }
    }
}
