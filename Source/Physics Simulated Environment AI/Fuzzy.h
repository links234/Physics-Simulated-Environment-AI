#ifndef FUZZY_H
#define FUZZY_H

namespace Fuzzy
{
	double Grade(double x, double x0, double x1);
	double ReverseGrade(double x, double x0, double x1);
	double Triangle(double x, double x0, double x1, double x2);
	double Trapezoid(double x, double x0, double x1, double x2, double x3);

	double AND(double a, double b);
	double OR(double a, double b);
	double NOT(double a);
}

#endif
