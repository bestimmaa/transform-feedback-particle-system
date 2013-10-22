
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



/// gloost system includes
#include <BoundingBox.h>
#include <Vector3.h>
#include <Point3.h>
#include <Ray.h>



/// cpp includes



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


 /// class constructor

BoundingBox::BoundingBox(mathType pMinX, mathType pMinY, mathType pMinZ,
                         mathType pMaxX, mathType pMaxY, mathType pMaxZ):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pMinX, pMinY, pMinZ),
  _pMax(pMaxX, pMaxY, pMaxZ)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////


 /// class constructor

BoundingBox::BoundingBox(const Point3& pMin, const Point3& pMax):
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(pMin),
  _pMax(pMax)
{
	// insert your code here
}

///////////////////////////////////////////////////////////////////////////////


 /// class constructor

BoundingBox::BoundingBox():
  BoundingVolume(GLOOST_BOUNDINGVOLUME_BOX),
  _pMin(0,0,0),
  _pMax(0,0,0)
{
	// insert your code here
}


///////////////////////////////////////////////////////////////////////////////


 /// class destructor

BoundingBox::~BoundingBox()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


 /// copy

const BoundingBox&
BoundingBox::operator= (const BoundingBox& b)
{
  if (this != &b)
  {
    _pMin = b._pMin;
    _pMax = b._pMax;
  }
  return *this;
}


///////////////////////////////////////////////////////////////////////////////


 ///

void
BoundingBox::setPMin(const Point3& pMin)
{
	_pMin = pMin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

const Point3&
BoundingBox::getPMin() const
{
	return _pMin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

Point3&
BoundingBox::getPMin()
{
	return _pMin;
}


///////////////////////////////////////////////////////////////////////////////


 ///

void
BoundingBox::setPMax(const Point3& pMax)
{
	_pMax = pMax;
}


///////////////////////////////////////////////////////////////////////////////


 ///

const Point3&
BoundingBox::getPMax() const
{
	return _pMax;
}


///////////////////////////////////////////////////////////////////////////////


 ///

Point3&
BoundingBox::getPMax()
{
	return _pMax;
}


///////////////////////////////////////////////////////////////////////////////


 /// returns the center of the bounding box

Point3
BoundingBox::getCenter() const
{
	return (_pMin + _pMax) * 0.5;
}


///////////////////////////////////////////////////////////////////////////////


 /// returns a vector storing the side length along each axis

Vector3
BoundingBox::getSize() const
{
	return Vector3( std::abs(_pMax[0] - _pMin[0]),
                  std::abs(_pMax[1] - _pMin[1]),
                  std::abs(_pMax[2] - _pMin[2]) );
}


///////////////////////////////////////////////////////////////////////////////


 /// returns a vector storing half the side length along each axis

Vector3
BoundingBox::getHalfSize() const
{
	return Vector3( std::abs(_pMax[0] - _pMin[0]),
                  std::abs(_pMax[1] - _pMin[1]),
                  std::abs(_pMax[2] - _pMin[2])) * 0.5;
}


///////////////////////////////////////////////////////////////////////////////


 /// intersect with a ray

bool
BoundingBox::intersect(Ray& ray) const
{

	// source: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm


	/*
	For each pair of planes P associated with X, Y, and Z do:
  (example using X planes)
  if direction Xd = 0 then the ray is parallel to the X planes.
  So, if origin Xo is not between the slabs ( Xo < Xl or Xo > Xh) then return false
  else, if the ray is not parallel to the plane then
  begin
  compute the intersection distance of the planes
	T1 = (Xl - Xo) / Xd
  T2 = (Xh - Xo) / Xd
  If T1 > T2 swap (T1, T2) // since T1 intersection with near plane

  If T1 > Tnear set Tnear =T1 // want largest Tnear
  If T2 < Tfar set Tfar="T2" // want smallest Tfar

  If Tnear > Tfar box is missed so return false
  If Tfar < 0 box is behind ray return false end

  end of for loop
  If Box survived all above tests, return true with
  intersection point Tnear and exit point Tfar.
	*/

	double tNear = -10000000;
	double tFar  =  10000000;

	// parallel to plane 0..1
	for (int i=0; i!=3; ++i)
	{
    if (ray.getDirection()[i] == 0)
    {
      if (!(_pMin[i] < ray.getOrigin()[i] && _pMax[i] > ray.getOrigin()[i]))
      {
        return false;
      }
    }


    double t1 = (_pMin[i] - ray.getOrigin()[i]) / ray.getDirection()[i];
    double t2 = (_pMax[i] - ray.getOrigin()[i]) / ray.getDirection()[i];

    // swap
    if (t1 > t2)
    {
      double temp = t1;
      t1 = t2;
      t2 = temp;
    }

    if (t1 > tNear)
    {
      tNear = t1;
    }

    if (t2 < tFar)
    {
      tFar = t2;
    }

    if (tNear > tFar)
    {
      return false;
    }

    if (tFar < 0)
    {
      return false;
    }
	}

  ray.setT(tNear);

  return true;
}


///////////////////////////////////////////////////////////////////////////////


 /// intersect with a ray

bool
BoundingBox::intersect(Ray& ray, Vector3& normal) const
{

	if (intersect(ray))
	{
    Point3 hitPoint = ray.getDestination();

    if (std::abs(_pMin[0] - hitPoint[0]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(-1.0, 0.0, 0.0);
    }
    else if ( std::abs(_pMax[0] - hitPoint[0]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(1.0, 0.0, 0.0);
    }
    else if (std::abs(_pMin[1] - hitPoint[1]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, -1.0, 0.0);
    }
    else if ( std::abs(_pMax[1] - hitPoint[1]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 1.0, 0.0);
    }
    else if (std::abs(_pMin[2] - hitPoint[2]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 0.0, -1.0);
    }
    else if ( std::abs(_pMax[2] - hitPoint[2]) < GLOOST_BOUNDINGVOLUME_COLLISION_EPSILON )
    {
      normal = Vector3(0.0, 0.0, 1.0);
    }

    return true;
	}

  return false;
}


////////////////////////////////////////////////////////////////////////////////


    /// return TRUE if BoundingBox intersects with frustum

/*virtual*/ bool
BoundingBox::intersectWithFrustum(const Frustum& frustum) const
{

/*
      _pMin
          g-----h
         /     /|
        /     / |
        c----d  f
  e --> |    | /
        |    |/
        a----b
             _pMax
*/

  gloost::Point3 a(_pMin[0], _pMax[1], _pMax[2]);
  gloost::Point3 b(_pMax);
  gloost::Point3 c(_pMin[0], _pMin[1], _pMax[2]);
  gloost::Point3 d(_pMax[0], _pMin[1], _pMax[2]);
  gloost::Point3 e(_pMin[0], _pMax[1], _pMin[2]);
  gloost::Point3 f(_pMax[0], _pMax[1], _pMin[2]);
  gloost::Point3 g(_pMin);
  gloost::Point3 h(_pMax[0], _pMin[1], _pMin[2]);

  if (frustum.inside(getCenter())) return true;
  if (frustum.inside(a)) return true;
  if (frustum.inside(b)) return true;
  if (frustum.inside(c)) return true;
  if (frustum.inside(d)) return true;
  if (frustum.inside(e)) return true;
  if (frustum.inside(f)) return true;
  if (frustum.inside(g)) return true;
  if (frustum.inside(h)) return true;

  return false;
}


////////////////////////////////////////////////////////////////////////////////

/// return true if point p is inside;

/*virtual*/
bool
BoundingBox::inside(const Point3& p) const
{
  return (_pMin[0] < p[0]) &&
    (_pMin[1] < p[1]) &&
    (_pMin[2] < p[2]) &&
    (_pMax[0] > p[0]) &&
    (_pMax[1] > p[1]) &&
    (_pMax[2] > p[2]);
}


////////////////////////////////////////////////////////////////////////////////
#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[1] - b*v0[2];			       	   \
	p2 = a*v2[1] - b*v2[2];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[1] + fb * boxhalfsize[2];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[1] - b*v0[2];			           \
	p1 = a*v1[1] - b*v1[2];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[1] + fb * boxhalfsize[2];   \
	if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[0] + b*v0[2];		      	   \
	p2 = -a*v2[0] + b*v2[2];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[0] + fb * boxhalfsize[2];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[0] + b*v0[2];		      	   \
	p1 = -a*v1[0] + b*v1[2];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[0] + fb * boxhalfsize[2];   \
	if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[0] - b*v1[1];			           \
	p2 = a*v2[0] - b*v2[1];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[0] + fb * boxhalfsize[1];   \
	if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[0] - b*v0[1];				   \
	p1 = a*v1[0] - b*v1[1];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[0] + fb * boxhalfsize[1];   \
	if(min>rad || max<-rad) return 0;


  /// returns TRUE if triangle given by v0,v1 and v2 is intersecting the AABB

/*virtual*/
bool
BoundingBox::intersectWithTriangle (Point3 vertex0, Point3 vertex1, Point3 vertex2)
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */

  Point3  boxCenter   = getCenter();
  Vector3 boxhalfsize = getHalfSize();

   Point3 v0,v1,v2;
//   float axis[3];
   float min,max,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed
   Vector3 normal,e0,e1,e2;

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   v0 = vertex0 - boxCenter;
   v1 = vertex1 - boxCenter;
   v2 = vertex2 - boxCenter;

   std::cerr << std::endl << "v0: " << v0;
   std::cerr << std::endl << "v1: " << v1;
   std::cerr << std::endl << "v2: " << v2;

   /* compute triangle edges */
   e0 = v1 - v0;
   e1 = v2 - v1;
   e2 = v0 - v2;

   std::cerr << std::endl << "e0: " << e0;
   std::cerr << std::endl << "e1: " << e1;
   std::cerr << std::endl << "e2: " << e2;

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabsf(e0[0]);
   fey = fabsf(e0[1]);
   fez = fabsf(e0[2]);
   AXISTEST_X01(e0[2], e0[1], fez, fey);
   AXISTEST_Y02(e0[2], e0[0], fez, fex);
   AXISTEST_Z12(e0[1], e0[0], fey, fex);

   fex = fabsf(e1[0]);
   fey = fabsf(e1[1]);
   fez = fabsf(e1[2]);
   AXISTEST_X01(e1[2], e1[1], fez, fey);
   AXISTEST_Y02(e1[2], e1[0], fez, fex);
   AXISTEST_Z0(e1[1], e1[0], fey, fex);

   fex = fabsf(e2[0]);
   fey = fabsf(e2[1]);
   fez = fabsf(e2[2]);
   AXISTEST_X2(e2[2], e2[1], fez, fey);
   AXISTEST_Y1(e2[2], e2[0], fez, fex);
   AXISTEST_Z12(e2[1], e2[0], fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   FINDMINMAX(v0[0],v1[0],v2[0],min,max);
   if(min>boxhalfsize[0] || max<-boxhalfsize[0]) return 0;

   /* test in Y-direction */
   FINDMINMAX(v0[1],v1[1],v2[1],min,max);
   if(min>boxhalfsize[1] || max<-boxhalfsize[1]) return 0;

   /* test in Z-direction */
   FINDMINMAX(v0[2],v1[2],v2[2],min,max);
   if(min>boxhalfsize[2] || max<-boxhalfsize[2]) return 0;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   normal = cross(e0, e1);
   // -NJMP- (line removed here)

   Plane plane(v0, normal);
   if(!intersectPlane(plane)) return 0;	// -NJMP-

   return 1;   /* box and triangle overlaps */
}


////////////////////////////////////////////////////////////////////////////////


 /// returns TRUE if AABB intersects plane p

bool
BoundingBox::intersectPlane(const Plane& plane) const	// -NJMP-
{

  int q;
  Vector3 vmin,vmax;
  mathType v;

  Vector3 nPlaneNormal = plane.getNormal().normalized();

  Vector3 halfSides = getHalfSize();
//  Vector3 halfSides = getPMax() - getCenter();


  for(q=0; q<=2; q++)
  {
    v=plane.getPoint()[q];					// -NJMP-
    if(nPlaneNormal[q]>0.0f)
    {
      vmin[q]=-halfSides[q] - v;	// -NJMP-
      vmax[q]= halfSides[q] - v;	// -NJMP-
    }
    else
    {
      vmin[q]= halfSides[q] - v;	// -NJMP-
      vmax[q]=-halfSides[q] - v;	// -NJMP-
    }
  }

  if(nPlaneNormal * vmin >  0.0f) return 0;	// -NJMP-
  if(nPlaneNormal * vmax >= 0.0f) return 1;	// -NJMP-

  return 0;
}


////////////////////////////////////////////////////////////////////////////////


 /// draw bounding box

///*virtual*/ void
//BoundingBox::draw()
//{
//  glPushAttrib(GL_ALL_ATTRIB_BITS);
//  {
//    glEnable(GL_COLOR_MATERIAL);
//    glColor4f(1.0, 0.3, 0.3, 0.75);
//
//    glDisable(GL_LIGHTING);
//
//    gloost::drawWiredBox(_pMin[0], _pMin[1], _pMin[2],
//                         _pMax[0], _pMax[1], _pMax[2]);
//  }
//  glPopAttrib();
//}



////////////////////////////////////////////////////////////////////////////////


  /// tarnsform the bounding box with a matrix

/*virtual*/
void
BoundingBox::transform(const Matrix& m)
{
  (*this) = m * (*this);
}


////////////////////////////////////////////////////////////////////////////////


  /// operator for notation Matrix*BoundingBox

/*extern*/
BoundingBox
operator* (const Matrix& matrix, const BoundingBox& boundingBox)
{
  return BoundingBox(matrix*boundingBox.getPMin(),
                     matrix*boundingBox.getPMax());
}


////////////////////////////////////////////////////////////////////////////////


  /// adds two bounding boxes so that the result will include both boxes

/*extern*/
BoundingBox
operator+ (const BoundingBox& a, const BoundingBox& b)
{

  const Point3& minA = a.getPMin();
  const Point3& maxA = a.getPMax();

  const Point3& minB = b.getPMin();
  const Point3& maxB = b.getPMax();

  BoundingBox r(a);

  for (unsigned int i=0; i!=3; ++i)
  {
    if (minA[i] > minB[i])
    {
      r.getPMin()[i] = minB[i];
    }

    if (maxA[i] < maxB[i])
    {
      r.getPMax()[i] = maxB[i];
    }
  }

  return r;
}


////////////////////////////////////////////////////////////////////////////////


  ///

/* extern */
std::ostream&
operator<< (std::ostream& os, const BoundingBox& bb)
{
  os << "BoundingBox( " << std::fixed
                        << bb.getPMin() << std::endl <<
        "             " << bb.getPMax() << std::endl << ")";
  return os;
}


///////////////////////////////////////////////////////////////////////////////


 ///










} // namespace GG

