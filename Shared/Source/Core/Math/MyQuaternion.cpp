#include "pch.h"
#include "MathHelpers.h"
#include "Vector.h"
#include "MyQuaternion.h"

// From gmtl - http://ggt.sourceforge.net/html/QuatOps_8h-source.html
MyQuat MyQuat::Lerp(MyQuat start, MyQuat end, float perc)
{
    // calc cosine theta
    float cosom = start.Dot( end );

    // adjust signs (if necessary)
    if( cosom < 0.0f )
    {
        end.x = -end.x;   // Reverse all signs
        end.y = -end.y;
        end.z = -end.z;
        end.w = -end.w;
    }

    // do linear interp
    float sclp, sclq;
    sclp = 1.0f - perc;
    sclq = perc;

    MyQuat result = start*sclp + end*sclq;
    return result;
}

// From gmtl - http://ggt.sourceforge.net/html/QuatOps_8h-source.html
MyQuat MyQuat::Slerp(MyQuat start, MyQuat end, float perc)
{
    // calc cosine theta
    float cosom = start.Dot( end );

    // adjust signs (if necessary)
    if( cosom < 0.0f )
    {
        cosom = -cosom;
        end.x = -end.x;   // Reverse all signs
        end.y = -end.y;
        end.z = -end.z;
        end.w = -end.w;
    } 

    // Calculate coefficients
    float sclp, sclq;
    if( (1.0f - cosom) > 0.0001f ) // 0.0001 -> some epsilon
    {
        // Standard case (slerp)
        float omega, sinom;
        omega = acos( cosom ); // extract theta from dot product's cos theta
        sinom = sin( omega );
        sclp  = sin( (1.0f - perc) * omega ) / sinom;
        sclq  = sin( perc * omega ) / sinom;
    }
    else
    {
        // Very close, do linear interp (because it's faster)
        sclp = 1.0f - perc;
        sclq = perc;
    }

    MyQuat result = start*sclp + end*sclq;
    return result;
}
