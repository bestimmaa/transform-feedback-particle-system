
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


#include <Quaternion.h>



namespace gloost
{

/**
  \class Quaternion
  \brief Calculates a rotation around an axis and provide this transformation in a Matrix
  \author Konstantin Silin, Felix Weiszig
  \date   December 2009
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Constructor
*/


Quaternion::Quaternion():
  _last_valid_mat()
  ,_dirty(true)
  ,_angle(0.0f)
  ,_axis(1.0f, 0.0f, 0.0f)
  ,_quaternion(0.0f, 0.0f, 0.0f, 0.0f)

{
  _last_valid_mat.setIdentity();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Constructor
  \param axis The axis you want rotate arounds
  \param angle The angle you want rotate with in (rad)
  \see gloost::deg2rad()
*/

Quaternion::Quaternion(const Vector3& axis,const float angle):
  _last_valid_mat()
  ,_dirty(true)
  ,_angle(angle)
  ,_axis(axis)
  ,_quaternion(0.0f, 0.0f, 0.0f, 0.0f)

{
  _last_valid_mat.setIdentity();
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Constructor
  \param quaternion Another quaternion
*/


Quaternion::Quaternion(const Quaternion& quaternion):
  _last_valid_mat(quaternion._last_valid_mat)
  ,_dirty(quaternion._dirty)
  ,_angle(quaternion._angle)
  ,_axis(quaternion._axis)
  ,_quaternion(quaternion._quaternion)

{
  _last_valid_mat.setIdentity();
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief Destructor
*/

Quaternion::~Quaternion()
{

}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets the angle to 0.0, the axis to (1.0f, 0.0f, 0.0f) and the result matrix to identity
*/

void
Quaternion::reset()
{
  _last_valid_mat.setIdentity();

  _quaternion = Vector3();
  _angle = 0.0f;
  _axis = Vector3();

  setClean();
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the angle
  \return The angle
*/

float
Quaternion::getAngle() const
{
  return _angle;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Set the angle
  \param angle The angle in rad
  \see gloost::deg2rad()
*/

void
Quaternion::setAngle(const float angle)
{
  _angle = angle;

  setDirty();
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the axis
  \return The axis
*/

const Vector3&
Quaternion::getAxis() const
{
  return _axis;
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets the axis
  \param Return The axis
*/

void
Quaternion::setAxis(const Vector3& axis)
{
  _axis = axis;

  setDirty();
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets axis and angle
  \param axis The axis you want rotate arounds
  \param angle The angle you want rotate with in (rad)
  \see gloost::deg2rad()
*/

void
Quaternion::setAxisAndAngle(const Vector3& axis,const float angle)
{
  _axis  = axis;
  _angle = angle;

  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Rotate a vector with the result matrix
  \param vec A Vector3 you want to rotate with the result Matrix
  \return The rotated vector
  \remarks 1. The formula was taken from http://blog.rusteddreams.net
  \remarks 2. This will call validate() and recalculate the result matrix if necessary
*/

Vector3
Quaternion::rotate(const Vector3& vec)
{
  validate();

  return vec + 2.0 * cross (_quaternion, ( cross(_quaternion, (vec)) + _quaternion[3]*vec));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Rotate a Point3 around the origin using the result matrix
  \param point A Point3 you want to rotate around the origin
  \return A Point3 with the new position

  \remarks 1. The formula was taken from http://blog.rusteddreams.net
  \remarks 2. This will call validate() and recalculate the result matrix if necessary
*/

Point3
Quaternion::rotate(const Point3& point)
{
  validate();

  return point + 2.0 * cross (_quaternion, ( cross(_quaternion, point) + _quaternion[3]*point));
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets the Quaternion <i>dirty</i> to indicate that a recalculation of the result matrix is required
*/

void
Quaternion::setDirty()
{
  _dirty = true;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Sets <i>clean</i> (this will be done automaticly if you call validate())
*/

void
Quaternion::setClean()
{
  _dirty = false;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns true if the result matrix is not valid
  \return True if the result matrix is invalid
*/

bool
Quaternion::isDirty()
{
  return _dirty;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Maps the transformation to the result matrix
  \remarks Math found at http://www.gamedev.net/reference/articles/article1199.asp
*/

void
Quaternion::validate()
{

  /// do this fancy math only if its necessary
  if (_dirty)
  {

    /// recalculate quaternion
    Vector3 _scaledAxis = (_axis.normalized()*(sin(_angle/2.0f)));;

    _quaternion = Vector3(_scaledAxis[0],
                          _scaledAxis[1],
                          _scaledAxis[2],
                          cos(_angle/2.0f));


    double x2 = _scaledAxis[0]*_scaledAxis[0];
    double y2 = _scaledAxis[1]*_scaledAxis[1];
    double z2 = _scaledAxis[2]*_scaledAxis[2];
    double xy = _scaledAxis[0]*_scaledAxis[1];
    double xz = _scaledAxis[0]*_scaledAxis[2];
    double yz = _scaledAxis[1]*_scaledAxis[2];
    double wx = _quaternion[3]*_scaledAxis[0];
    double wy = _quaternion[3]*_scaledAxis[1];
    double wz = _quaternion[3]*_scaledAxis[2];


    /// recalculate the result matrix
    _last_valid_mat.setIdentity();

    _last_valid_mat[0] = 1 - 2*y2 - 2*z2;
    _last_valid_mat[1] = 2*xy + 2*wz;
    _last_valid_mat[2] = 2*xz - 2*wy;

    _last_valid_mat[4] = 2*xy - 2*wz;
    _last_valid_mat[5] = 1 - 2*x2 - 2*z2;
    _last_valid_mat[6] = 2*yz + 2*wx;

    _last_valid_mat[8] = 2*xz + 2*wy;
    _last_valid_mat[9] = 2*yz - 2*wx;
    _last_valid_mat[10]= 1 - 2*x2 - 2*y2;

    setClean();
  }
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the valide Matrix of the complete transformation
  \return The result matrix holding a rotation around the axis
*/

const Matrix&
Quaternion::getMatrix()
{
  validate();
  return _last_valid_mat;
}



} // namespace gloost


