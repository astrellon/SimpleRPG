using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace SimpleRPGAnalyser
{
    public class KeyPair : IComparable<KeyPair>
    {
        public string name;
        public float value;
        public float max = -1;
        public Color color = Color.Transparent;
        
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
        public KeyPair(string name, float value, float max, Color color)
        {
            this.name = name;
            this.value = value;
            this.max = max;
            this.color = color;
        }
        public KeyPair(string name, float value, Color color)
        {
            this.name = name;
            this.value = value;
            this.color = color;
        }

        public int CompareTo(KeyPair other)
        {
            return name.CompareTo(other.name);
        }
    }
}
