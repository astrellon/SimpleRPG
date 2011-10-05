using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Media;

namespace SimpleRPGAnalyser
{
    public class Animal : ILoadable
    {
        public int id;
        public string name;
        public string species;
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
        public string birthdate;
        public string deathdate;
        public float deathtime;
        public float mate_find_cooldown;
        public float fertility;
        public int parent1;
        public int parent2;
        public float hunger_damage_cooldown;
        public float mutation_rate;
        public float mutation_amount;
        
        public Animal()
        {

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
                        break;
                    case "name":
                        name = iter[index++];
                        break;
                    case "species":
                        species = iter[index++];
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
                        string actionName = iter[index++];
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
                        break;
                    case "deathdate":
                        deathdate = iter[index++];
                        break;
                    case "deathtime":
                        deathtime = float.Parse(iter[index++]);
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
                    case "mutation_rate":
                        mutation_rate = float.Parse(iter[index++]);
                        break;
                    case "mutation_amount":
                        mutation_amount = float.Parse(iter[index++]);
                        break;
                    case "end":
                        break;
                    default:
                        Console.WriteLine("Unknown property: " + line);
                        break;
                }
            }
        }
    }
}
