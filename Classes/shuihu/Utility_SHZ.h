#ifndef SHZ___XLBE_UTILITY_H__
#define SHZ___XLBE_UTILITY_H__
#include <time.h>

//////////////////////////////////////////////////////////////////////////////////////
//
//const float M_PI = 3.14159265358979323846f;
//const float M_PI_2 = 1.57079632679489661923f;
//const float M_PI_4 = 0.785398163397448309616f;
//const float M_1_PI = 0.318309886183790671538f;
//const float M_2_PI = 0.636619772367581343076f;

//////////////////////////////////////////////////////////////////////////////////////
namespace xlbe_utility {

//////////////////////////////////////////////////////////////////////////////////////

static unsigned int g_seed=0;
static void random_seed(int seed)
{
	if(!seed) g_seed = (unsigned int)time(0);
	else g_seed = seed;
}

static int random_int(int min, int max)
{
	g_seed = 214013 * g_seed + 2531011;
	return min + (g_seed ^ g_seed>>15) % (max - min + 1);
}

static float random_float(float min, float max)
{
	g_seed = 214013 * g_seed + 2531011;

	return min + (g_seed >> 16) * (1.0f / 65535.0f) * (max - min);
}
//////////////////////////////////////////////////////////////////////////////////////
}
#endif