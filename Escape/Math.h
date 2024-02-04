#pragma once

class Math
{
public:
	int easeOutBounce(int x) 
	{
		float n1 = 7.5625;
		float d1 = 2.75;

		if (x < 1 / d1)
		{
			return n1 * x * x;
		}
		 else if (x < 2 / d1)
		{
		  return n1 * (x -= 1.5 / d1) * x + 0.75;
		}
		 else if (x < 2.5 / d1)
		{
			return n1 * (x -= 2.25 / d1) * x + 0.9375;
		}

		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}

	int easeInOutBounce(int x)
	{
		if (x < 0.5)
		{
			return (1 - easeOutBounce(1 - 2 * x)) / 2;
		}
	
		return (1 + easeOutBounce(2 * x - 1)) / 2;
	}
};