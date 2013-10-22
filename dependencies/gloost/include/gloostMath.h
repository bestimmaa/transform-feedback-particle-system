
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).

            gloost is being created by Felix Weiﬂig and Stephan Beck

     Felix Weiﬂig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



#ifndef H_GLOOST_MATH
#define H_GLOOST_MATH



/// cpp includes
#include <sstream>
#include <cmath>
#include <stdlib.h>



// gloost includes
#include  <UniformTypes.h>
#include  <Vector3.h>
#include  <Vector2.h>



/// init rand
#ifndef RAND_MAX
  #define RAND_MAX 2147483647
  //int rand();
#endif



namespace gloost
{


/// define PI
static const float PI    = 3.1415926535;
static const float EULER = 2.7182818;


///////////////////////////////////////////////////////////////////////////////


  /// returns a "random" value between 0.0 ... 1.0

inline float
frand ()
{
  return (rand()/((float)RAND_MAX));
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a "random" value between -1.0 ... 1.0

inline float
crand ()
{
  return (rand()/((float)RAND_MAX))*2.0f - 1.0f;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns a "random" value between fMin ... fMax

template <class T>
inline T
getRandomMinMax( T fMin, T fMax )
{
  float fRandNum = (float)rand () / RAND_MAX;
  return fMin + (fMax - fMin) * fRandNum;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns true or false by a chance of 50%

inline bool chance(float probability = 0.5)
{
  if (frand() < probability)
  {
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns minimum of two values

template <class T>
inline T
min ( T a, T b)
{
  if ( a > b)
  {
    return b;
  }

  return a;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns minimum of two values

template <class T>
inline T
min ( T a, T b, T c)
{
  return min(min(a,b), c);
}


///////////////////////////////////////////////////////////////////////////////


  /// returns maximum of two values

template <class T>
inline T
max ( T a, T b)
{
  if ( a > b)
  {
    return a;
  }

  return b;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns maximum of two values

template <class T>
inline T
max ( T a, T b, T c)
{
  return max(max(a,b), c);
}


///////////////////////////////////////////////////////////////////////////////


  /// orders two values so that a contains the smaller value and b the bigger one

template <class T>
void
swapToIncreasing ( T& a, T& b)
{
  if (a > b)
  {
    T temp = a;
    a = b;
    b = temp;
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// orders two values so that a contains the bigger value and b the smaller one

template <class T>
void
swapToDecreasing ( T& a, T& b)
{
  if (a < b)
  {
    T temp = a;
    a = b;
    b = temp;
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// returns maximum of two values

template <class T>
inline
T
abs ( T a )
{
  if ( a < 0)
  {
    return -a;
  }

  return a;
}



///////////////////////////////////////////////////////////////////////////////


  /// returns 1 if a is >= 0, else -1

template <class T>
inline
T
sign ( T a )
{
  if ( a < 0)
  {
    return -1;
  }

  return 1;
}


///////////////////////////////////////////////////////////////////////////////

  /// clamps a value between min and max

template <class T>
inline
T
clamp ( T value, T min, T max )
{
  if (value < min)
  {
    return min;
  }
  else if (value > max)
  {
    return max;
  }
  return value;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns true if value is between min and max

template <class T>
inline
bool
between ( T value, T min, T max )
{
  if (value > min & value < max)
  {
    return true;
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////


  /// returns true if value is between or equal to min and max

template <class T>
inline
bool
betweenOrEqual ( T value, T min, T max )
{
  if (value >= min & value <= max)
  {
    return true;
  }
  return false;
}


///////////////////////////////////////////////////////////////////////////////


  /// loop a value between min and max

template <class T>
inline
T
loop ( T value, T min, T max )
{
  if (value < min)
  {
    return max - gloost::abs(value-min);
  }
  else if (value > max)
  {
    return value - gloost::abs(max - min);
  }
  return value;
}


///////////////////////////////////////////////////////////////////////////////

/* (this function is templated, so if you want float precision give floats!!!) */

  /// converts degree to rad

template <class T>
inline
T
deg2rad (const T& a)
{
  return (T)(a * PI / 180.0);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   converts degree to rad
  \remarks (this function is templated, so if you want float precision give floats!!!)
*/

template <class T>
inline
T
rad2deg (T a)
{
  return (T)(a * 180.0 / PI);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   converts rad to rad (I wonder if sombody will use it??? ;-)
  \remarks this function is templated, so if you want float precision give floats!!!
*/

template <class T>
inline
T
rad2rad (T a)
{
  std::cout << std::endl;
  std::cout << std::endl << "Laughing from gloost::rad2rad" ;
  std::cout << std::endl << "              Why would you do that ???";
  std::flush(std::cout);

  return a;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
Vector2
angle2Vector(T angle)
{
  return Vector2(cos(angle*0.0174532925199), sin(angle*0.0174532925199));
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
gloost::Vector3
angle2Vector3xy(T angle)
{
  return gloost::Vector3(cos(angle*0.0174532925199), sin(angle*0.0174532925199), 0.0);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a angle in a vector
*/

template <class T>
inline
gloost::Vector3
angle2Vector3xz(T angle)
{
  return gloost::Vector3(cos(angle*0.0174532925199), 0.0, sin(angle*0.0174532925199));
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angleRad(float deltaH, float deltaV)
{
  float the_angle = 0.0;

  if (deltaH != 0.0f )
  {
    float slope = deltaV / deltaH;
    the_angle = atan (slope);
    if (deltaH  < 0)
    {
      the_angle = the_angle + PI;
    }
  }
  else if (deltaV > 0)
  {
    the_angle = PI / 2.0;
  }
  else if (deltaV < 0)
  {
    the_angle = (3 * PI) / 2.0;
  }
  else
  {
    the_angle = 0.0;
  }

  return the_angle;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angle(float deltaH, float deltaV)
{
  float the_angle = 0.0;

  if (deltaH != 0.0f)
  {
    float slope = deltaV / deltaH;
    the_angle = atan (slope);
    if (deltaH  < 0)
    {
      the_angle = the_angle + PI;
    }
  }
  else if (deltaV > 0)
  {
    the_angle = PI / 2.0;
  }
  else if (deltaV < 0)
  {
    the_angle = (3 * PI) / 2.0;
  }
  else
  {
    the_angle = 0.0;
  }

  the_angle = (the_angle * 180) / PI;

  return the_angle;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief coverts a 2D vector in to an angle
*/

inline
float
vector2angle(vec2 vector)
{
  return vector2angle(vector.u, vector.v);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief the modulo or "%" operator for all scalar types
*/

template <class T>
T
fmod(T a, T b)
{
  int result = static_cast<int>( a / b );
  return a - static_cast<double>( result ) * b;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns TRUE if two lines are intersect each other
*/

inline
bool
lineIntersect2D(const Vector2& line1P1, const Vector2& line1P2,
                const Vector2& line2P1, const Vector2& line2P2 )
{
  // direction vectors of both lines
  gloost::Vector2 d1 = line1P2 - line1P1;
  gloost::Vector2 d2 = line2P2 - line2P1;


  float div = d1[0] * d2[1] - d1[1] * d2[0];

  float t = -(line1P1[0] * d2[1] - line1P1[1] * d2[0] - line2P1[0] * d2[1] + line2P1[1] * d2[0]) / div;
  float s = -(line1P1[0] * d1[1] - line1P1[1] * d1[0] + d1[0] * line2P1[1] - d1[1] * line2P1[0]) / div;

  // or u1.multiply(s) u0.plus(u1);
  d1 = d1 * t;

  // der Punkt wo sie sich schneiden
  gloost::Vector2 p = line1P1 + d1;

  if ((t > 0 && s > 0) && (t < 1 && s < 1))
  {
     //std::cout << std::endl << "intersect at: " << "[x: " << p[0] << " y: " << p[1] << "]";
     return 1;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief converts a value in byte into mega byte
  \param byteValue a value in bytes
  \remarks ...
*/

inline
float
convertByteToMegaByte(float byteValue)
{
  return byteValue/1048576.0f;
}


/////////////////////////////////////////////////////////////////////////////////


/**
  \brief converts a value in bit into mega byte
  \param byteValue a value in bytes
  \remarks ...
*/

inline
float
convertBitToMegaByte(float bitValue)
{
  return bitValue/8388608.0f;
}


/////////////////////////////////////////////////////////////////////////////////


/**
  \brief gauss
  \param   ...
  \remarks ...
*/

inline
float
gauss(float x, float a=1.0, float b=0, float c=0.5)
{
  return a*pow(EULER, -( ((x-b)*(x-b)) )/(2*c*c) );
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates linearly between two values a and b
  \param a first control point
  \param b second control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline float
interpolateLinear(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates linearly between two points a and b
  \param a first control point
  \param b second control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateLinear(const gloost::Point3& a, const gloost::Point3& b, float t)
{
	return gloost::Point3( interpolateLinear(a[0], b[0], t),
          						   interpolateLinear(a[1], b[1], t),
					          	   interpolateLinear(a[2], b[2], t));
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates quadratic between two values a and c
  \param a first control value
  \param b second control value
  \param c third control value
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline float
interpolateQuadratic(float a, float b, float c, float t)
{
	float h = 1.0f - t;
	return a * h * h + b * 2 * h * t + c * t * t;
}

///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates quadratic between two points a and c
  \param a first control point
  \param b second control point
  \param c third control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateQuadratic(const gloost::Point3& a, const gloost::Point3& b, const gloost::Point3& c, float t)
{
	return gloost::Point3( interpolateQuadratic(a[0], b[0], c[0], t),
                         interpolateQuadratic(a[1], b[1], c[1], t),
                         interpolateQuadratic(a[2], b[2], c[2], t));
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates cubic between 2 values a and d
  \param a first control value
  \param b second control value
  \param c third control value
  \param d last control value
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline float
interpolateCubic(float a, float b, float c, float d,  float t)
{
	float h  = 1.0f - t;
	return a * h*h*h + b * 3 * h * h * t + c * 3 * h * t * t + d * t * t * t;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   interpolates cubic between two points a and d
  \param a first control point
  \param b second control point
  \param c third control point
  \param d last control point
  \param t time value between 0.0 ... 1.0
  \remarks ...
*/

inline gloost::Point3
interpolateCubic(const gloost::Point3& a, const gloost::Point3& b, const gloost::Point3& c, const gloost::Point3& d,  float t)
{
	return gloost::Point3( interpolateCubic(a[0], b[0], c[0], d[0], t),
                         interpolateCubic(a[0], b[0], c[0], d[0], t),
                         interpolateCubic(a[0], b[0], c[0], d[0], t));
}


///////////////////////////////////////////////////////////////////////////////


}  // namespace gloost


#endif // #ifndef H_GLOOST_MATH


