using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SimpleRPGAnalyser
{
    public class AnimalSorter : System.Collections.IComparer
    {
        public string convertDate(string date)
        {
            return Animal.convertDate(date).ToString();
        }

        public int Compare(object o1, object o2)
        {
            ListViewItem lvi1 = (ListViewItem)o2;
            string str1 = lvi1.SubItems[ByColumn].Text;
            
            ListViewItem lvi2 = (ListViewItem)o1;
            string str2 = lvi2.SubItems[ByColumn].Text;

            if (Column == 4 || Column == 5 || Column == 7 || Column == 8 || Column == 9)
            {
                float date1 = Animal.convertDate(str1);
                float date2 = Animal.convertDate(str2);
                float r = 0;
                if (lvi1.ListView.Sorting == SortOrder.Ascending)
                {
                    r = date1 - date2;
                }
                else
                {
                    r = date2 - date1;
                }
                if (r < 0)
                {
                    return -1;
                }
                else if (r > 0)
                {
                    return 1;
                }
                return 0;
            }

            if (Column == 0)
            {
                int id1 = int.Parse(str1);
                int id2 = int.Parse(str2);
                if (lvi1.ListView.Sorting == SortOrder.Ascending)
                {
                    return id1 - id2;
                }
                else
                {
                    return id2 - id1;
                }
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
