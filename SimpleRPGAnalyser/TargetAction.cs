using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class TargetAction : ILoadable
    {
        public string action = "NA";
        public bool completed = false;
        public int step = 0;
        public string completed_time = "0 0";
        public Destination target = new Destination();

        public void load(ref string[] iter, ref int index)
        {
            string line = iter[index++];
            while (line != "end")
            {
                line = iter[index++];
                switch (line)
                {
                    case "action":
                        action = iter[index++];
                        break;
                    case "completed":
                        completed = iter[index++][0] == '1';
                        break;
                    case "completed_time":
                        completed_time = iter[index++];
                        if (completed_time[0] != '"')
                        {
                            completed_time += iter[index++];
                        }
                        else
                        {
                            completed_time = completed_time.Substring(1, completed_time.Length - 2);
                        }
                        break;
                    case "step":
                        step = int.Parse(iter[index++]);
                        break;
                    case "target":
                        target.load(ref iter, ref index);
                        break;
                    case "end":
                        break;
                    default:
                        Console.WriteLine("Unknown target property: " + line);
                        break;
                }
            }
        }
    }
}
