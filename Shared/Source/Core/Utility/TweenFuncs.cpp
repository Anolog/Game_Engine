#include "pch.h"

float TweenFuncs::TweenFunc_Linear(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + valuerange*timeperc;
}

float TweenFuncs::TweenFunc_SineEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + -valuerange * cos(timeperc * PI/2) + valuerange;
}

float TweenFuncs::TweenFunc_SineEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + valuerange * sin(timeperc * PI/2);
}

float TweenFuncs::TweenFunc_SineEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + -valuerange/2 * (cos(timeperc * PI)-1);
}

float TweenFuncs::TweenFunc_BounceEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseIn( timepassed, startvalue, valuerange, totaltime );
}

float TweenFuncs::TweenFunc_BounceEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseOut( timepassed, startvalue, valuerange, totaltime );
}

float TweenFuncs::TweenFunc_BounceEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseInOut( timepassed, startvalue, valuerange, totaltime );
}

float TweenFuncs::TweenFunc_ElasticEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseIn( timepassed, startvalue, valuerange, totaltime, 3 );
}

float TweenFuncs::TweenFunc_ElasticEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseOut( timepassed, startvalue, valuerange, totaltime, 3 );
}

float TweenFuncs::TweenFunc_ElasticEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseInOut( timepassed, startvalue, valuerange, totaltime, 3 );
}

//http://www.kirupa.com/forum/archive/index.php/t-76799.html
double TweenFuncs::BounceEaseIn(double t, double b, double c, double d)
{
    return c - BounceEaseOut( d-t, 0, c, d ) + b;
}

double TweenFuncs::BounceEaseOut(double t, double b, double c, double d)
{
    if( (t/=d) < (1/2.75f) )
    {
        return c*(7.5625f*t*t) + b;
    }
    else if (t < (2/2.75f))
    {
        double postFix = t-=(1.5f/2.75f);
        return c*(7.5625f*(postFix)*t + .75f) + b;
    }
    else if (t < (2.5/2.75))
    {
        double postFix = t-=(2.25f/2.75f);
        return c*(7.5625f*(postFix)*t + .9375f) + b;
    }
    else
    {
        double postFix = t-=(2.625f/2.75f);
        return c*(7.5625f*(postFix)*t + .984375f) + b;
    }
}

double TweenFuncs::BounceEaseInOut(double t, double b, double c, double d)
{
    if( t < d/2 )
        return BounceEaseIn( t*2, 0, c, d ) * .5f + b;
    else
        return BounceEaseOut( t*2-d, 0, c, d ) * .5f + c*.5f + b;
}

double TweenFuncs::ElasticEaseIn(double t, double b, double c, double d, float elasticity)
{
    if( t == 0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d ) == 1 )
        return b+c;

    double p = d * 0.3f;
    double a = c;
    double s = p/4;

    double postFix = a * pow( 10, elasticity*(t-=1) );

    return -( postFix * sin( ( t*d-s ) * ( 2*PI )/p ) ) + b;
}

double TweenFuncs::ElasticEaseOut(double t, double b, double c, double d, float elasticity)
{
    if( t==0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d ) == 1 )
        return b+c;

    double p = d * 0.3f;
    double a = c;
    double s = p/4;

    return ( a * pow( 10, -elasticity*t ) * sin( ( t*d-s ) * ( 2*PI )/p ) + c + b);
}

double TweenFuncs::ElasticEaseInOut(double t, double b, double c, double d, float elasticity)
{
    if( t == 0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d/2 ) == 2 )
        return b+c;

    double p = d * ( 0.3f * 1.5f );
    double a = c;
    double s = p/4;

    if( t < 1 )
    {
        double postFix = a * pow( 10, elasticity*( t-=1 ) );
        return -0.5f * ( postFix * sin( ( t*d-s ) * ( 2*PI )/p ) ) + b;
    }

    double postFix = a * pow( 10, -elasticity*( t-=1 ) );
    return postFix * sin( ( t*d-s ) * ( 2*PI )/p ) * 0.5f + c + b;
}
