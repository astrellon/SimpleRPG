using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class Destination : ILoadable
    {
        public int entity_id;
        public Vector2f location = new Vector2f();

        public Destination()
        {
            entity_id = -1;
            location.x = -1;
            location.y = -1;
        }

        public void load(ref string[] iter, ref int index)
        {
            string c = iter[index++];
            if (c[0] == '@')
            {
                entity_id = int.Parse(iter[index++]);
            }
            else
            {
                location.x = float.Parse(c);
                location.y = float.Parse(iter[index++]);
            }
        }
    }
}
