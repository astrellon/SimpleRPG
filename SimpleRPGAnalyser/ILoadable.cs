using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public interface ILoadable
    {
        void load(ref string[] iter, ref int index);
    }
}
