using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleRPGAnalyser
{
    public class AnimalSorter : System.Collections.IComparer
    {
        private string convertDate(string date)
        {
            string[] split = date.Split(new char[] { ' ' });
            float num = float.Parse(split[0]) * Analyser.mDayLength + float.Parse(split[1]);
            return num.ToString();
        }

        public int Compare(object o1, object o2)
        {
            ListViewItem lvi1 = (ListViewItem)o2;
            string str1 = lvi1.SubItems[ByColumn].Text;
            
            ListViewItem lvi2 = (ListViewItem)o1;
            string str2 = lvi2.SubItems[ByColumn].Text;

            if (Column == 4 || Column == 5)
            {
                str1 = convertDate(str1);
                str2 = convertDate(str2);
            }

            int result;
            if (lvi1.ListView.Sorting == SortOrder.Ascending)
            {
                //Console.WriteLine("Ascending >" + str1 + "< >" + str2 + "<");
                result = String.Compare(str1, str2);
            }
            else
            {
                //Console.WriteLine("Descending >" + str1 + "< >" + str2 + "<");
                result = String.Compare(str2, str1);
            }

            LastSort = ByColumn;

            return (result);
        }


        public int ByColumn
        {
            get { return Column; }
            set { Column = value; }
        }
        int Column = 0;

        public int LastSort
        {
            get { return LastColumn; }
            set { LastColumn = value; }
        }
        int LastColumn = 0;
    }
}
