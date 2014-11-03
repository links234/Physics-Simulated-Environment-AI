#include "Random.h"

void InitRandom()
{
	srand((int)time(NULL));
}

int RandomInt(int minv, int maxv)
{
	int gap=0;
	if(minv<0)
	{
		gap=-minv;
		minv=0;
		maxv+=gap;
	}
	if(minv>=maxv)
		return 0;
	return minv+(rand()%(maxv-minv+1))-gap;
}

double RandomDouble(double low, double high)
{
	double temp;

	if (low > high)
	{
		temp = low;
		low = high;
		high = temp;
	}

	temp = (rand() / (static_cast<double>(RAND_MAX) + 1.0))* (high - low) + low;
	return temp;
}

