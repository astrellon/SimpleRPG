using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Media;

namespace SimpleRPGAnalyser
{
    public class Animal : ILoadable
    {
        public static Dictionary<int, Animal> Animals = new Dictionary<int, Animal>();
        public static List<Animal> getAnimalsBornOn(int day)
        {
            List<Animal> list = new List<Animal>();

            foreach (KeyValuePair<int, Animal> pair in Animals)
            {
                Animal a = pair.Value;
                if (Math.Floor(a.birthtime / Analyser.mDayLength) == day)
                {
                    list.Add(a);
                }
            }

            return list;
        }

        public static List<Animal> getAnimalsDiedOn(int day)
        {
            List<Animal> list = new List<Animal>();

            foreach (KeyValuePair<int, Animal> pair in Animals)
            {
                Animal a = pair.Value;
                if (Math.Floor(a.deathtime / Analyser.mDayLength) == day)
                {
                    list.Add(a);
                }
            }

            return list;
        }

        public static List<Animal> getAnimalsAliveOn(int day, string species = null)
        {
            List<Animal> list = new List<Animal>();

            foreach (KeyValuePair<int, Animal> pair in Animals)
            {
                Animal a = pair.Value;
                if (species != null && a.species != species)
                {
                    continue;
                }
                int birth = (int)Math.Floor(a.birthtime / Analyser.mDayLength);
                int death = (int)Math.Floor(a.deathtime / Analyser.mDayLength);
                if (birth <= day && (death >= day || death < 0.1))
                {
                    list.Add(a);
                }
            }

            return list;
        }

        public int id;
        public string name = "Johnny Noname";
        public string species = "Nullis";
        public Vector2f position = new Vector2f(-1, -1);
        public float facing;
        public float amount_eaten;
        public Destination destination = new Destination();
        public TargetAction current_action = new TargetAction();
        public List<TargetAction> action_history = new List<TargetAction>();
        public float health;
        public float maxHealth;

        public float strength;
        public float dexterity;
        public float intelligence;

        public float running_speed;
        public float walking_speed;
        public float turning_speed;
        public float entity_size;
        public float entity_mass;
        public float rest_energy_per_day;
        public float hunger_limits_lower;
        public float hunger_limits_upper;
        public float hunger_limits_breed;
        public float life_expectancy;
        public float breeding_age;
        public float age;
        public float breeding_rate;
        public float breeding_count;
        public float diet;
        public float sight_radius;
        public Pixel graphic = new Pixel();

        public float damage_base;
        public float attack_rate;
        public float attack_cooldown;
        public int attacked_by;
        public float attacked_by_cooldown;
        public float energy;
        public float accumulated_energy;
        public string birthdate = "0 0";
        public string deathdate = "0 0";
        public float deathtime;
        public float birthtime;
        public float mate_find_cooldown;
        public float fertility;
        public int parent1;
        public int parent2;
        public List<int> children = new List<int>();
        protected float _numAliveChildren = -1;
        public float numAliveChildren
        {
            get
            {
                if (_numAliveChildren < 0)
                {
                    int num = 0;
                    foreach (int i in children)
                    {
                        if (!Animals[i].isDead)
                        {
                            num++;
                        }
                    }
                    _numAliveChildren = num;
                    return num;
                }
                else
                {
                    return _numAliveChildren;
                }
            }
            set
            {
                _numAliveChildren = value;
            }
        }

        public float desired_num_children;
        public float local_population_max;
        public float hunger_damage_cooldown;
        public float hunger_heal_cooldown;
        public float mutation_rate;
        public float mutation_amount;

        public bool isDead
        {
            get
            {
                return health <= 0.0f;
            }
        }
        public Dictionary<string, int> deathbys = new Dictionary<string, int>();

        public string deathby
        {
            get
            {
                foreach (KeyValuePair<string, int> s in deathbys)
                {
                    return s.Key;
                }
                return "Not Dead";
            }
        }

        public string LongName
        {
            get { return name + " (" + species + ")"; }
        }
        
        public Animal()
        {

        }

        public static float convertDate(string date)
        {
            string[] split = date.Split(new char[] { ' ', ':' });
            float num = float.Parse(split[0]) * Analyser.mDayLength;
            if (split.Length == 2)
            {
                num += float.Parse(split[1]);
            }
            else if (split.Length == 3)
            {
                num += float.Parse(split[1]) * Analyser.mDayLength / 24 + float.Parse(split[2]);
            }
            return num;
        }

        public static string convertDate(float date)
        {
            float day = (float)Math.Floor(date / Analyser.mDayLength);
            float time = date - day * Analyser.mDayLength;
            return day.ToString() + ' ' + time.ToString();
        }

        public static Animal averageAnimals(List<Animal> animals)
        {
            Animal average = new Animal();
            float b = 0;
            float d = 0;

            foreach (Animal a in animals)
            {
                average.accumulated_energy += a.accumulated_energy / animals.Count;
                average.age += a.age / animals.Count;
                average.amount_eaten += a.amount_eaten / animals.Count;
                average.attack_cooldown += a.attack_cooldown / animals.Count;
                average.attack_rate += a.attack_rate / animals.Count;
                average.attacked_by_cooldown += a.attacked_by_cooldown / animals.Count;
                average.breeding_age += a.breeding_age / animals.Count;
                average.breeding_count += a.breeding_count / animals.Count;
                average.breeding_rate += a.breeding_rate / animals.Count;
                average.damage_base += a.damage_base / animals.Count;
                average.deathtime += a.deathtime / animals.Count;
                average.dexterity += a.dexterity / animals.Count;
                average.diet += a.diet / animals.Count;
                average.energy += a.energy / animals.Count;
                average.entity_mass += a.entity_mass / animals.Count;
                average.entity_size += a.entity_size / animals.Count;
                average.facing += a.facing / animals.Count;
                average.fertility += a.fertility / animals.Count;
                average.health += a.health / animals.Count;
                average.hunger_damage_cooldown += a.hunger_damage_cooldown / animals.Count;
                average.hunger_heal_cooldown += a.hunger_heal_cooldown / animals.Count;
                average.hunger_limits_breed += a.hunger_limits_breed / animals.Count;
                average.hunger_limits_lower += a.hunger_limits_lower / animals.Count;
                average.hunger_limits_upper += a.hunger_limits_upper / animals.Count;
                average.intelligence += a.intelligence / animals.Count;
                average.life_expectancy += a.life_expectancy / animals.Count;
                average.mate_find_cooldown += a.mate_find_cooldown / animals.Count;
                average.maxHealth += a.maxHealth / animals.Count;
                average.mutation_amount += a.mutation_amount / animals.Count;
                average.mutation_rate += a.mutation_rate / animals.Count;
                average.rest_energy_per_day += a.rest_energy_per_day / animals.Count;
                average.running_speed += a.running_speed / animals.Count;
                average.sight_radius += a.sight_radius / animals.Count;
                average.strength += a.strength / animals.Count;
                average.turning_speed += a.turning_speed / animals.Count;
                average.walking_speed += a.walking_speed / animals.Count;
                average.local_population_max += a.local_population_max / animals.Count;
                average.desired_num_children += a.desired_num_children / animals.Count;
                average.numAliveChildren += a.numAliveChildren / animals.Count;

                average.position.x += a.position.x / animals.Count;
                average.position.y += a.position.y / animals.Count;
                average.destination.location.x = a.destination.location.x / animals.Count;
                average.destination.location.y = a.destination.location.y / animals.Count;

                foreach (KeyValuePair<string, int> s in a.deathbys)
                {
                    if (average.deathbys.ContainsKey(s.Key))
                    {
                        average.deathbys[s.Key] += s.Value;
                    }
                    else
                    {
                        average.deathbys[s.Key] = s.Value;
                    }
                }

                b += convertDate(a.birthdate);
                d += convertDate(a.deathdate);
            }

            average.birthdate = convertDate(b);
            average.deathdate = convertDate(d);

            return average;
        }

        public void load(ref string[] iter, ref int index)
        {
            string line = iter[index];
            
            while (line != "end")
            {
                line = iter[index];
                index++;

                switch (line)
                {
                    case "id":
                        id = int.Parse(iter[index++]);
                        Animals[id] = this;
                        break;
                    case "name":
                        name = iter[index++];
                        if (name[0] == '"')
                        {
                            name = name.Substring(1, name.Length - 2);
                        }
                        break;
                    case "species":
                        species = iter[index++];
                        if (species[0] == '"')
                        {
                            species = species.Substring(1, species.Length - 2);
                        }
                        break;
                    case "position":
                        position.x = float.Parse(iter[index++]);
                        position.y = float.Parse(iter[index++]);
                        break;
                    case "facing":
                        facing = float.Parse(iter[index++]);
                        break;
                    case "amount_eaten":
                        amount_eaten = float.Parse(iter[index++]);
                        break;
                    case "destination":
                        destination.load(ref iter, ref index);
                        break;
                    case "current_action":
                        current_action.load(ref iter, ref index);
                        break;
                    case "action_history":
                        string actionName = iter[index];
                        while (actionName != "end")
                        {
                            if (actionName == "Action" || actionName == "TargetAction")
                            {
                                TargetAction action = new TargetAction();
                                action.load(ref iter, ref index);
                                action_history.Add(action);
                            }
                            actionName = iter[index];
                        }
                        index++;
                        break;
                    case "health":
                        health = float.Parse(iter[index++]);
                        maxHealth = float.Parse(iter[index++]);
                        break;
                    case "strength":
                        strength = float.Parse(iter[index++]);
                        break;
                    case "dexterity":
                        dexterity = float.Parse(iter[index++]);
                        break;
                    case "intelligence":
                        intelligence = float.Parse(iter[index++]);
                        break;
                    case "running_speed":
                        running_speed = float.Parse(iter[index++]);
                        break;
                    case "walking_speed":
                        walking_speed = float.Parse(iter[index++]);
                        break;
                    case "turning_speed":
                        turning_speed = float.Parse(iter[index++]);
                        break;
                    case "entity_size":
                        entity_size = float.Parse(iter[index++]);
                        break;
                    case "entity_mass":
                        entity_mass = float.Parse(iter[index++]);
                        break;
                    case "rest_energy_per_day":
                        rest_energy_per_day = float.Parse(iter[index++]);
                        break;
                    case "hunger_limits":
                        hunger_limits_lower = float.Parse(iter[index++]);
                        hunger_limits_upper = float.Parse(iter[index++]);
                        hunger_limits_breed = float.Parse(iter[index++]);
                        break;
                    case "life_expectancy":
                        life_expectancy = float.Parse(iter[index++]);
                        break;
                    case "breeding_age":
                        breeding_age = float.Parse(iter[index++]);
                        break;
                    case "age":
                        age = float.Parse(iter[index++]);
                        break;
                    case "breeding_rate":
                        breeding_rate = float.Parse(iter[index++]);
                        breeding_count = float.Parse(iter[index++]);
                        break;
                    case "diet":
                        diet = float.Parse(iter[index++]);
                        break;
                    case "sight_radius":
                        sight_radius = float.Parse(iter[index++]);
                        break;
                    case "graphic":
                        graphic.load(ref iter, ref index);
                        break;
                    case "damage_base":
                        damage_base = float.Parse(iter[index++]);
                        break;
                    case "attack_rate":
                        attack_rate = float.Parse(iter[index++]);
                        break;
                    case "attack_cooldown":
                        attack_cooldown = float.Parse(iter[index++]);
                        break;
                    case "attacked_by":
                        attacked_by = int.Parse(iter[index++]);
                        break;
                    case "attacked_by_cooldown":
                        attacked_by_cooldown = float.Parse(iter[index++]);
                        break;
                    case "energy":
                        energy = float.Parse(iter[index++]);
                        break;
                    case "accumulated_energy":
                        accumulated_energy = float.Parse(iter[index++]);
                        break;
                    case "birthdate":
                        birthdate = iter[index++];
                        if (birthdate[0] == '"')
                        {
                            birthdate = birthdate.Substring(1, birthdate.Length - 2);
                        }
                        birthtime = convertDate(birthdate);
                        break;
                    case "deathdate":
                        deathdate = iter[index++];
                        if (deathdate[0] == '"')
                        {
                            deathdate = deathdate.Substring(1, deathdate.Length - 2);
                        }
                        deathtime = convertDate(deathdate);
                        break;
                    case "deathby":
                        string deathby = iter[index++];
                        if (deathby[0] == '"')
                        {
                            deathby = deathby.Substring(1, deathby.Length - 2);
                        }
                        deathbys[deathby] = 1;
                        break;
                    case "deathtime":
                        index++;
                        break;
                    case "mate_find_cooldown":
                        mate_find_cooldown = float.Parse(iter[index++]);
                        break;
                    case "fertility":
                        fertility = float.Parse(iter[index++]);
                        break;
                    case "parents":
                        parent1 = int.Parse(iter[index++]);
                        parent2 = int.Parse(iter[index++]);
                        break;
                    case "hunger_damage_cooldown":
                        hunger_damage_cooldown = float.Parse(iter[index++]);
                        break;
                    case "hunger_heal_cooldown":
                        hunger_heal_cooldown  = float.Parse(iter[index++]);
                        break;
                    case "mutation_rate":
                        mutation_rate = float.Parse(iter[index++]);
                        break;
                    case "mutation_amount":
                        mutation_amount = float.Parse(iter[index++]);
                        break;
                    case "local_population_max":
                        local_population_max = float.Parse(iter[index++]);
                        break;
                    case "desired_num_children":
                        desired_num_children = float.Parse(iter[index++]);
                        break;
                    case "children":
                        line = iter[index++];
                        while (line != "end")
                        {
                            children.Add(int.Parse(line));
                            line = iter[index++];
                        }
                        line = iter[index++];
                        break;
                    case "species_alignment":
                        while (line != "end")
                        {
                            line = iter[index++];
                        }
                        line = iter[index++];
                        break;
                    case "end":
                        index--;
                        break;
                    default:
                        Console.WriteLine("Unknown property: " + line);
                        break;
                }
            }
        }
    }
}
