// SimpleRPG Image to Map converted by Alan Lawrey for Thesis 2011.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace SimpleRPGMapEditor
{
    public partial class MainView : Form
    {
        private char[] mTiles = {'.', '^', ',', ';', '#', 'S', '~', '*'};
        private Dictionary<Color, char> mTileMap = new Dictionary<Color, char>();
        private Bitmap mMapImg = null;
        private int mColourCount = 0;


        public MainView()
        {
            InitializeComponent();
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                //openFileDialog.InitialDirectory = "c:\\";
                openFileDialog.Filter = "Images files |*.bmp;*.jpg;*.png|All files (*.*)|*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    tmrClear.Stop();
                    mMapImg = new Bitmap(openFileDialog.FileName);
                    picMain.Image = mMapImg;
                    //picMain.Size = mMapImg.Size;

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

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (mMapImg == null)
            {
                return;
            }

            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                tmrClear.Stop();
                saveFileDialog.Filter = "Map file (*.txt)|*.txt|All files (*.*)|*.*";
                saveFileDialog.FilterIndex = 1;
                saveFileDialog.RestoreDirectory = true;

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    StreamWriter writer = new StreamWriter(saveFileDialog.FileName);
                    StringBuilder builder = new StringBuilder("-- Tiles");
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

                    tssMain.Text = "Saved '" + saveFileDialog.FileName + "'";
                    tmrClear.Start();
                }
            }
        }

        private void addColour(Color c, char t)
        {
            Panel panel = new Panel();
            panel.BackColor = c;
            panel.Width = 24;
            panel.Height = 24;
            panel.Left = 5;
            panel.Top = 6 + mColourCount * 30;
            pnlColours.Controls.Add(panel);

            TextBox textbox = new TextBox();
            textbox.Font = new Font("Arial", 12);
            textbox.Width = 24;
            textbox.Height = 23;
            textbox.MaxLength = 1;
            textbox.Top = panel.Top;
            textbox.Left = pnlColours.Width - 5 - textbox.Width;
            textbox.Text = t.ToString();
            textbox.TextChanged += new EventHandler(textbox_TextChanged);
            textbox.Tag = c;
            pnlColours.Controls.Add(textbox);
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
    }
}
