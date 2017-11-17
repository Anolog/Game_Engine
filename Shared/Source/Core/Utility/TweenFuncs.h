#ifndef __TweenFuncs_H__
#define __TweenFuncs_H__

class TweenFuncs
{
public:
	float TweenFunc_Linear(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_SineEaseIn(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_SineEaseOut(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_SineEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_BounceEaseIn(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_BounceEaseOut(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_BounceEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_ElasticEaseIn(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_ElasticEaseOut(float startvalue, float valuerange, double timepassed, double totaltime);
	float TweenFunc_ElasticEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime);
	double ElasticEaseInOut(double t, double b, double c, double d, float elasticity);
	double ElasticEaseOut(double t, double b, double c, double d, float elasticity);
	double ElasticEaseIn(double t, double b, double c, double d, float elasticity);
	double BounceEaseInOut(double t, double b, double c, double d);
	double BounceEaseOut(double t, double b, double c, double d);
	double BounceEaseIn(double t, double b, double c, double d);

};
#endif //__TweenFuncs_H__
