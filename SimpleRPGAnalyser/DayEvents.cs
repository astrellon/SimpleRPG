using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleRPGAnalyser
{
    public class Location
    {
        public int id = 0;
        public Vector2i position = new Vector2i();
    }

    public class DayEvents : ILoadable
    {
        public int day = 0;
        public List<Location> locations = new List<Location>();
        public Dictionary<string, int> populations = new Dictionary<string, int>();
        public Dictionary<string, Dictionary<char, int>> deaths = new Dictionary<string, Dictionary<char, int>>();
        public Dictionary<string, int> totalDeaths = new Dictionary<string, int>();
        public Dictionary<string, int> births = new Dictionary<string, int>();

        public static Dictionary<string, bool> ALL_SPECIES = new Dictionary<string, bool>();

        public DayEvents()
        {

        }
        public DayEvents(int day)
        {
            this.day = day;
        }

        public int getTotalDeaths(string species)
        {
            if (totalDeaths.ContainsKey(species))
            {
                return totalDeaths[species];
            }
            return 0;
        }

        public int getDeaths(string species, char by)
        {
            if (deaths.ContainsKey(species) && deaths[species].ContainsKey(by))
            {
                return deaths[species][by];
            }
            return 0;
        }

        public int getBirths(string species)
        {
            if (births.ContainsKey(species))
            {
                return births[species];
            }
            return 0;
        }

        public int getPopulation(string species)
        {
            if (populations.ContainsKey(species))
            {
                return populations[species];
            }
            return 0;
        }

        public void load(ref string[] iter, ref int index)
        {
            index++;
            day = int.Parse(iter[index++]);

            string line = iter[index];
            while (line != "end")
            {
                index++;
                if (line.Equals("locations", StringComparison.CurrentCultureIgnoreCase))
                {
                    line = iter[index];
                    while (line != "end")
                    {
                        Location loc = new Location();
                        loc.id = int.Parse(line); index++;
                        loc.position.x = int.Parse(iter[index++]);
                        loc.position.y = int.Parse(iter[index++]);
                        locations.Add(loc);
                        line = iter[index];
                    }
                }
                else if (line.Equals("populations", StringComparison.CurrentCultureIgnoreCase))
                {
                    line = iter[index];
                    while (line != "end")
                    {
                        string species = iter[index++];
                        ALL_SPECIES[species] = true;
                        int population = int.Parse(iter[index++]);
                        populations[species] = population;
                        line = iter[index];
                    }
                }
                else if (line.Equals("deaths", StringComparison.CurrentCultureIgnoreCase))
                {
                    line = iter[index];
                    while (line != "end")
                    {
                        string species = iter[index++];
                        ALL_SPECIES[species] = true;
                        line = iter[index];
                        while (line != "end")
                        {
                            char deathby = line[0]; index++;
                            int deathCount = int.Parse(iter[index++]);
                            if (!totalDeaths.ContainsKey(species))
                            {
                                totalDeaths[species] = deathCount;
                            }
                            else
                            {
                                totalDeaths[species] += deathCount;
                            }

                            if (!deaths.ContainsKey(species))
                            {
                                deaths[species] = new Dictionary<char, int>();
                            }
                            deaths[species][deathby] = deathCount;
                            line = iter[index];
                        }

                        line = iter[++index];
                    }
                }
                else if (line.Equals("births", StringComparison.CurrentCultureIgnoreCase))
                {
                    line = iter[index];
                    while (line != "end")
                    {
                        string species = iter[index++];
                        ALL_SPECIES[species] = true;
                        int birthCount = int.Parse(iter[index++]);
                        births[species] = birthCount;
                        line = iter[index];
                    }
                }
                else
                {
                    Console.WriteLine("Unknown DayEvents property '{0}'", line);
                }

                index++;
                line = iter[index];
            }
        }
    }
}
