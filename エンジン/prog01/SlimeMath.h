#pragma once
#include <math.h>

static float SizeToScaleConvert(float size) {
	float scale = log(size + 1) / log(4);
	return scale;
}