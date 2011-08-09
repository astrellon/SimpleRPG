#pragma once

class Debug
{
public:
	static bool isBreak(int i)
	{
		return breaks[i];
	}

	static void setBreak(int i, bool b)
	{
		breaks[i] = b;
	}

private:

	static bool breaks[64];
};
