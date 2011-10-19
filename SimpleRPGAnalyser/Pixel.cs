using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class Pixel : ILoadable
    {
        public string[] COLOUR_NAMES = { "black", "blue", "green", "cyan", "red", "magenta", "yellow", "white" };

        public int colour;
        public bool bold;
        public char graphic;

        public Pixel()
        {
            colour = 0;
            bold = false;
            graphic = 'x';
        }

        public int fullColour
        {
            get
            {
                if (bold)
                {
                    return colour | 0x8;
                }
                return colour;
            }
        }

        override public string ToString()
        {
            return graphic + "_" + colour.ToString() + '_' + (bold ? '1' : '0');
        }

        public void load(ref string[] iter, ref int index)
        {
            string c = iter[index++];
            if (c[0] >= '0' && c[0] < '9')
            {
                colour = int.Parse(c);
            }
            else
            {
                for (int i = 0; i < COLOUR_NAMES.Length; i++)
                {
                    if (c.Equals(COLOUR_NAMES[i], StringComparison.CurrentCultureIgnoreCase))
                    {
                        colour = i;
                        break;
                    }
                }
            }
            c = iter[index++];
            bold = c[0] == '1';
            graphic = iter[index++][0];
        }
    }
}
