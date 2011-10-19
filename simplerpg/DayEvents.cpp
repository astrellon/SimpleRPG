#include "DayEvents.h"

DayEvents::DayEvents()
{
	day = -1;
}
DayEvents::DayEvents(int day)
{
	this->day = day;
}
DayEvents::~DayEvents()
{
	locations.clear();
	populations.clear();
	deaths.clear();
}

void DayEvents::loadFromFile(FormattedFileIterator &iter)
{
	day = lexical_cast<int>(*iter);
	++iter;
	string line = *iter;

	while(!iter.atEnd())
	{
		if(iequals(line, "end"))
		{
			++iter;
			break;
		}
		
		++iter;
		if(iequals("locations", line))
		{
			line = *iter;
			while(!iequals(line, "end"))
			{
				Location *loc = new Location();
				loc->id = lexical_cast<int>(line);			++iter;
				loc->position.x = lexical_cast<int>(*iter);	++iter;
				loc->position.y = lexical_cast<int>(*iter);	++iter;
				locations.push_back(loc);
				line = *iter;
			}
		}
		else if(iequals("populations", line))
		{
			line = *iter;
			while(!iequals(line, "end"))
			{
				string species = line;	++iter;
				int population = lexical_cast<int>(*iter);	++iter;
				populations[species] = population;
				line = *iter;
			}
		}
		else if(iequals("deaths", line))
		{
			line = *iter;
			while(!iequals(line, "end"))
			{
				string species = line;	++iter;
				line = *iter;
				while(!iequals(line, "end"))
				{
					char deathby = line[0];	++iter;
					int deathCount = lexical_cast<int>(*iter);	++iter;
					deaths[species][deathby] = deathCount;
					line = *iter;
				}
				++iter;
				line = *iter;
			}
		}
		else if(iequals("births", line))
		{
			line = *iter;
			while(!iequals(line, "end"))
			{
				string species = line;	++iter;
				int birthCount = lexical_cast<int>(*iter);	++iter;
				births[species] = birthCount;
				line = *iter;
			}
		}
		else
		{
			clog << "Unknown DayEvents property '" << line << "'" << endl;
		}
		++iter;
		line = *iter;
	}
}
void DayEvents::saveToFile(FormattedFile &file)
{
	file << "DayEvent " << day << '\n';
	file.changeTabLevel(1);

	file << "locations\n";
	file.changeTabLevel(1);
	for(vector<Location *>::iterator iter = locations.begin(); iter != locations.end(); ++iter)
	{
		Location *loc = *iter;
		file << loc->id << ' ' << loc->position.x << ' ' << loc->position.y << ' ';
	}
	file.changeTabLevel(-1);
	file << "\nend\n";

	file << "populations\n";
	file.changeTabLevel(1);
	for(map<string, int>::iterator iter = populations.begin(); iter != populations.end(); ++iter)
	{
		file << iter->first << ' ' << iter->second << '\n';
	}
	file.changeTabLevel(-1);
	file << "end\n";

	file << "deaths\n";
	file.changeTabLevel(1);
	for(map<string, map<char, int> >::iterator iter = deaths.begin(); iter != deaths.end(); ++iter)
	{
		file << iter->first << '\n';
		file.changeTabLevel(1);
		for(map<char, int>::iterator deathsIter = iter->second.begin(); deathsIter != iter->second.end(); ++deathsIter)
		{
			file << deathsIter->first << ' ' << deathsIter->second << '\n';
		}
		file.changeTabLevel(-1);
		file << "end\n";
	}
	file.changeTabLevel(-1);
	file << "end\n";

	file << "births\n";
	file.changeTabLevel(1);
	for(map<string, int>::iterator iter = births.begin(); iter != births.end(); ++iter)
	{
		file << iter->first << ' ' << iter->second << '\n';
	}
	file.changeTabLevel(-1);
	file << "end\n";

	file.changeTabLevel(-1);

	file << "end\n";
}