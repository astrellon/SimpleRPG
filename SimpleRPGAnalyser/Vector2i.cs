using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class Vector2i
    {
        public int x;
        public int y;

        public Vector2i()
        {
            x = 0;
            y = 0;
        }
        public Vector2i(int x, int y)
        {
            this.x = x;
            this.y = y;
        }
    }
}
