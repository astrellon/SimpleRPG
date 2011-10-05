using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class Vector2f
    {
        public float x;
        public float y;

        public Vector2f()
        {
            x = 0;
            y = 0;
        }
        public Vector2f(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }
}
