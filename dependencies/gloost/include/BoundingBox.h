
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



#ifndef GLOOST_BOUNDINGBOX_H
#define GLOOST_BOUNDINGBOX_H



/// gloost system includes
#include <BoundingVolume.h>
#include <Vector3.h>
#include <Matrix.h>
#include <Point3.h>
#include <Ray.h>


/// cpp includes


namespace gloost
{


struct plane
{
  // normal
  Vector3 n;
  // a point on the plane
  Point3  p;
  // a point on the plane
  mathType d;
};


/*
 _pMin
      o-----
     /     /|
    /     / |
    ------  |
    |    | /
    |    |/
    -----o
         _pMax
*/


  ///  Axis aligned bounding box (AABB)

class BoundingBox : public BoundingVolume
{
	public:

    /// class constructor
    BoundingBox(mathType pMinX, mathType pMinY, mathType pMinZ,
                mathType pMaxX, mathType pMaxY, mathType pMaxZ);
    /// class constructor
    BoundingBox(const Point3& pMin, const Point3& pMax);
    /// class constructor
    BoundingBox();
    /// class destructor
    ~BoundingBox();


    /// assignment
    const BoundingBox& operator= (const BoundingBox&);


    ///
    void setPMin(const Point3& pMin);
    ///
    const Point3& getPMin() const;
    Point3& getPMin();
    ///
    void setPMax(const Point3& pMax);
    ///
    const Point3& getPMax() const;
    Point3& getPMax();

    /// returns the center of the bounding box
    Point3 getCenter() const;

    /// returns a vector storing the side length along each axis
    Vector3 getSize() const;

    /// returns a vector storing half the side length along each axis
    Vector3 getHalfSize() const;


	  /// returns true if ray intersects with AABB
    bool intersect(Ray& ray) const;

	  /// returns true if ray intersects with AABB
    bool intersect(Ray& ray, Vector3& normal) const;

    /// return TRUE if BoundingBox intersects with frustum
    /*virtual*/ bool intersectWithFrustum(const Frustum& frustum) const;

    /// return TRUE if point p is inside;
    /*virtual*/ bool inside(const Point3& p) const;

    // returns TRUE if triangle given by v0,v1 and v2 is intersecting the AABB
    bool intersectWithTriangle(Point3 v0, Point3 v1, Point3 v2);

    // returns TRUE if AABB intersects plane p
    bool intersectPlane(const Plane& plane) const;


    /// draw bounding box
//    /*virtual*/ void draw();



    /// tarnsform the bounding box with a matrix
    /*virtual*/ void transform(const Matrix& m);

  protected:

    /// min and max point;
    Point3 _pMin;
    Point3 _pMax;
};




/// operator for notation Matrix*BoundingBox
extern BoundingBox operator* (const Matrix&, const BoundingBox&);

/// adds two bounding boxes so that the result will include both boxes
extern BoundingBox operator+(const BoundingBox&, const BoundingBox&);


/// ostream operator
extern std::ostream& operator<< (std::ostream&, const BoundingBox&);



} // namespace gloost


#endif // GLOOST_BOUNDINGBOX_H

