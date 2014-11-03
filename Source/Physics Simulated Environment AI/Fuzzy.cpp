#include "Fuzzy.h"

namespace Fuzzy
{
	double Grade(double x, double x0, double x1)
	{
		double result;
		if(x <= x0)
		{
			result = 0;
		}
		else if(x >= x1)
		{
			result = 1;
		}
		else
		{
			result = (x/(x1-x0))-(x0/(x1-x0));
		}
		return result;
	}

	double ReverseGrade(double x, double x0, double x1)
	{
		double result;
		if(x <= x0)
		{
			result = 1;
		}
		else if(x >= x1)
		{
			result = 0;
		}
		else
		{
			result = (-x/(x1-x0))+(x1/(x1-x0));
		}
		return result;
	}

	double Triangle(double x, double x0, double x1, double x2)
	{
		double result;
		if(x <= x0)
		{
			result = 0;
		}
		else if(x == x1)
		{
			result = 1;
		}
		else if((x>x0) && (x<x1))
		{
			result = (x/(x1-x0))-(x0/(x1-x0));
		}
		else
		{
			result = (-x/(x2-x1))+(x2/(x2-x1));
		}
		return result;
	}

	double Trapezoid(double x, double x0, double x1, double x2, double x3)
	{
		double result;
		if(x <= x0)
		{
			result = 0;
		}
		else if((x>=x1) && (x<=x2))
		{
			result = 1;
		}
		else if((x>x0) && (x<x1))
		{
			result = (x/(x1-x0))-(x0/(x1-x0));
		}
		else
		{
			result = (-x/(x3-x2))+(x3/(x3-x2));
		}
		return result;
	}

	double AND(double a, double b)
	{
		if(a<b)
		{
			return a;
		}
		return b;
	}

	double OR(double a, double b)
	{
		if(a>b)
		{
			return a;
		}
		return b;
	}

	double NOT(double a)
	{
		return 1.0-a;
	}
}
