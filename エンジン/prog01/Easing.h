#pragma once
#include "Vector3.h"

enum type
{
	In, Out, InOut
};
enum ease
{
	Quad, Cubic, Quart, Quint, Sine, Exp, Circ, Back, SoftBack, Elastic, Bounce, Linear
};

namespace EasingFunction
{
	double InQuad(double t);
	double InCubic(double t);
	double InQuart(double t);
	double InQuint(double t);
	double InSine(double t);
	double InExp(double t);
	double InCirc(double t);
	double InBack(double t);
	double InSoftBack(double t);
	double InElastic(double t);
	double InBounce(double t);
	double InLinear(double t);
}

double Ease(type Type, ease Ease, double t, double start,double end);
Vector3 Ease(type Type, ease Ease, double t, Vector3 start, Vector3 end);
XMFLOAT2 Ease(type Type, ease Ease, double t, XMFLOAT2 start, XMFLOAT2 end);