
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



/*

  authors: 2009 by Felix Weiﬂig and Konstantin Silin

*/



#ifndef H_GLOOST_QUATERNION
#define H_GLOOST_QUATERNION



// gloost includes
#include <Vector3.h>
#include <Point3.h>
#include <Matrix.h>



namespace gloost
{


 // Calculates a rotation around an axis and provide this transformation in a Matrix

class Quaternion
{

 public:

  // constructor
  Quaternion();
  Quaternion(const Vector3& axis, const float angle);
  Quaternion(const Quaternion& quaternion);


  // destructor
  virtual ~Quaternion();


  // reset transformations
  void reset();


  // get rotation angle
  float getAngle() const;
  // set rotation angle
  void setAngle(const float angle);

  // get the axis
  const Vector3& getAxis() const;
  // set the axis
  void setAxis(const Vector3& axis);

  // set rotation axis and angle
  void setAxisAndAngle(const Vector3& axis, const float angle);



  // rotate a vector
  Vector3 rotate(const Vector3& vec);


  // rotate a position around the axis
  Point3 rotate(const Point3& point);



  // set dirty
  void setDirty();

  // set clean (this will be done automaticly if you call validate())
  void setClean();

  // is this Quaternion dirty
  bool isDirty();



  // map the transformation to matrix
  void validate();



  // get the Matrix of the complete transformation (recalculated if dirty)
  const Matrix&  getMatrix();


 protected:

    // calc the Matrix of the complete transformation
    const Matrix&  calcMatrix() const;


    /// Matrix storing the result transformation
    Matrix  _last_valid_mat;

    /// flag indicating that the result matrix has to be recalculated
    bool   _dirty;

    /// angle for the rotation
    float   _angle;

    /// axis for the rotation
    Vector3 _axis;

    /// quaternion vector
    Vector3 _quaternion;

};


} // namespace gloost


#endif // #ifndef H_GLOOST_QUATERNION_H




