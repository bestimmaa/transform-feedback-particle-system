
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

#include <Dcs.h>
#include <Quaternion.h>

namespace gloost
{
/**
  \class Dcs
  \brief Dcs or Dynamic Coordinate System is basically a matrix. In difference to
         a normal matrix, you can set and get the the translation, rotation and
         scalation part independently.

  \author Felix Weiszig
  \date   December 2009
  \remarks <br>You have to call validate() to apply the changed transformations to the dcs.
         Alternatively you can get the validated result matrix by calling get getValidMatrix().
         <br>Here is one example:
         \code
         // create a Dcs instance. All componet matrices will be set to identity
         gloost::Dcs dcs;

         // set the translation to 10.0 on y
         dcs.setTranslate(0.0, 10.0, 0.0);
         // set the scale with factor 2 on all axis
         dcs.setScale(2.0);
         // set a 90 degrees rotation on the z axis
         dcs.setRotate(0.0, 0.0, gloost::deg2rad(90.0));

         // print the result matrix WITHOUT changes
         std::cout << std::endl << dcs.getMatrix();

         // validate the dcs;
         dcs.validate();

         // print the result matrix WITH changes
         std::cout << std::endl << dcs.getMatrix();
         \endcode
         You have to call validate() only if you need the validated result
         matrix. All getters like getRotate(), getTranslate(), or
         getTranslateVector() will return the current values you just set.
         To save some performance (validate() performs two matrix
         multiplications) call validate() only if you need to.
         <br>If you call validate() but nothing was changed since last validation,
         the Dcs will not perform the redundant calculation due to the _dirty flag.

  \see validate(), getValidMatrix()
*/

////////////////////////////////////////////////////////////////////////////////
/**
  \brief Class constructur
  \param setEverythingIdentity If true (default) all three component matrices and the
         result matrix will be set initially to identity.
         <br>If you choose false, you have to set each
         transformation component at least once befor calling validate or you result matrix will
         be bull plob. <i><b>BULL PLOB, I TELL YOU</b></i>
*/
Dcs::Dcs(bool setEverythingIdentity):
  _rotMatrix(),
  _transMatrix(),
  _scaleMatrix(),
  _resultMatrix(),
  _dirty(0)

{
  if (setEverythingIdentity)
  {
    _rotMatrix.setIdentity();
    _scaleMatrix.setIdentity();
    _transMatrix.setIdentity();
    _resultMatrix.setIdentity();
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Class destructor
*/
/*virtual*/
Dcs::~Dcs()
{
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the rotation component
  \param matrix A rotation matrix
*/
void
Dcs::setRotate(const Matrix& m)
{
  _rotMatrix = m;
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the rotation component

  \param matrix A rotation matrix
*/
void
Dcs::setRotate(const Vector3& v)
{
  _rotMatrix.setRotate(v);
  setDirty();
}


////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the rotation component
  \param x x angle in rad
  \param y y angle in rad
  \param y y angle in rad
*/
void
Dcs::setRotate(const double x, const double y, const double z)
{
  _rotMatrix.setRotate(Vector3(x,y,z));
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the rotation component
  \param v Vector specifiy the fraction of the angle for this axis
  \param angle Vector specifiy the fraction of the angle for this axis
*/
void
Dcs::setRotate(const Vector3& v, double angle)
{
  _rotMatrix.setRotate(v, angle);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the rotation component matrix
  \return The rotation component matrix
*/
const Matrix&
Dcs::getRotate() const
{
  return _rotMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief rotates the Dcs

  \param axis the axis you want rotate around
  \param radAngle the angle you want rotate with
*/

void
Dcs::rotate(const Vector3& axis, const mathType& radAngle)
{
  Quaternion q(axis, radAngle);

  _rotMatrix = q.getMatrix()*_rotMatrix;
  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief rotates the Dcs with a matrix

  \param rotMat A rotation matrix
*/

void
Dcs::rotate(const Matrix& rotMat)
{
  _rotMatrix = rotMat*_rotMatrix;
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the translation component
  \param m A translation matrix
*/
void
Dcs::setTranslate(const Matrix& m)
{
  _transMatrix = m;
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the translation component
  \param v A translation vector
*/
void
Dcs::setTranslate(const Vector3& v)
{
  _transMatrix.setTranslate(v);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the translation component
  \param x Translation along the x axis
  \param y Translation along the y axis
  \param z Translation along the z axis
*/
void
Dcs::setTranslate(const double x, const double y, const double z)
{
  _transMatrix.setTranslate(x,y,z);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the translation component matrix
  \return The translation component matrix
*/
const Matrix&
Dcs::getTranslate() const
{
  return _transMatrix;
}
////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the translation component as a Vector3
  \return A Vector3 with the translation
*/
const Vector3
Dcs::getTranslateVector() const
{
  return _transMatrix.getTranslate();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief translates the Dcs by a vector

  \param transVec Translation vector
*/

void
Dcs::translate(const mathType& x, const mathType& y, const mathType& z)
{
//  Matrix t;
//  t.setIdentity();
//  t.setTranslate(x,y,z);
//
//  _transMatrix = t*_transMatrix;


  _transMatrix[12] += x;
  _transMatrix[13] += y;
  _transMatrix[14] += z;
//  _transMatrix[15] = 1;

  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief translates the Dcs by a vector

  \param transVec Translation vector
*/

void
Dcs::translate(const Vector3& transVec)
{
//  Matrix t;
//  t.setIdentity();
//  t.setTranslate(transVec);
//
//  _transMatrix = t*_transMatrix;


  _transMatrix[12] += transVec[0];
  _transMatrix[13] += transVec[1];
  _transMatrix[14] += transVec[2];
//  _transMatrix[15] = 1;

  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief translates the Dcs

  \param transMat A translation matrix
*/

void
Dcs::translate(const Matrix& transMat)
{
//  _transMatrix = transMat * _transMatrix;

  _transMatrix[12] += transMat[12];
  _transMatrix[13] += transMat[13];
  _transMatrix[14] += transMat[14];

  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the scale component
  \param m A scale matrix
*/
void
Dcs::setScale(const Matrix& m)
{
  _scaleMatrix = m;
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////
/**
  \brief Sets the scale component
  \param v A Vector3 with the scale factors for each axis
*/
void
Dcs::setScale(const Vector3& v)
{
  _scaleMatrix.setScale(v);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the scale component
  \param n Factor by which all axis shall be scaled by uniformly
*/
void
Dcs::setScale(const mathType n)
{
  _scaleMatrix.setScale(n);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the scale component
  \param x Scale factor along the x axis
  \param y Scale factor along the y axis
  \param z Scale factor along the z axis
*/
void
Dcs::setScale(const double x, const double y, const double z)
{
  _scaleMatrix.setScale(x,y,z);
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the scale component matrix

  \return The scale component matrix
*/
const Matrix&
Dcs::getScale() const
{
  return _scaleMatrix;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the scale component as a Vector3
  \return A Vector3 with the scale factors for each axis
*/
const Vector3
Dcs::getScaleVector() const
{
  return _scaleMatrix.getScale();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief scales all axis of the dcs by the same factor

  \param uniformScaleFactor scale factor for all axis
*/

void
Dcs::scale(const mathType& uniformScaleFactor)
{
  Matrix s;
  s.setIdentity();
  s.setScale(uniformScaleFactor);

  _scaleMatrix = s*_scaleMatrix;
  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief scales the Dcs

  \param scale A vector holding one factor for each axis
*/

void
Dcs::scale(const Vector3& scale)
{
  Matrix s;
  s.setIdentity();
  s.setScale(scale);

  _scaleMatrix = s*_scaleMatrix;
  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief scales the Dcs

  \param x scale factor for x-axis
  \param y scale factor for y-axis
  \param y scale factor for z-axis
*/

void
Dcs::scale(const mathType& x, const mathType& y, const mathType& z)
{
  Matrix s;
  s.setIdentity();
  s.setScale(x, y, z);

  _scaleMatrix = s*_scaleMatrix;
  setDirty();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief scales the Dcs

  \param scaleMat A scale matrix
*/

void
Dcs::scale(const Matrix& scaleMat)
{
  _scaleMatrix = scaleMat*_scaleMatrix;
  setDirty();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets all component matrices and the result matrix to identity
*/
void
Dcs::reset()
{
  _rotMatrix.setIdentity();
  _transMatrix.setIdentity();
  _scaleMatrix.setIdentity();
  _resultMatrix.setIdentity();
  setClean();
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the Dcs <i>dirty</i> to indicate that a validations is required
*/
void
Dcs::setDirty()
{
  _dirty = true;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Sets the Dcs <i>clean</i> to indicate that the result matrix is valid
  \remarks This will be done automaticly if you call validate() or getValidMatrix()
*/

void
Dcs::setClean()
{
  _dirty = false;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns true if the Dcs is not valid
  \return True if the Dcs is invalid
*/
bool
Dcs::isDirty()
{
  return _dirty;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Calculates the result matrix. Calculation is done like this:
         _transMatrix*(_scaleMatrix*_rotMatrix)
  \remarks You have to call validate() only if you need the validated result
           matrix. All getters like getRotate(), getTranslate(), or
           getTranslateVector() will return the current values you just set.
           To save some performance (validate() performs two matrix
           multiplications) call validate() only if you need to.
           <br>If you call validate() but nothing was changed since last validation,
           the Dcs will not perform the redundant calculation due to the _dirty flag.
*/
void
Dcs::validate()
{
  if (_dirty)
  {
    _resultMatrix = _transMatrix*(_rotMatrix*_scaleMatrix);
    setClean();
  }
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Returns the last result matrix without validation
  \remarks This matrix is not necessarily valide
*/
const Matrix&
Dcs::getMatrix() const
{
  return _resultMatrix;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Get a valide Matrix of the complete transformation

  \return The valid result matrix
*/
const Matrix&
Dcs::getValidMatrix()
{
  validate();
  return _resultMatrix;
}

////////////////////////////////////////////////////////////////////////////////

/**
  \brief Assignment operator
*/
const Dcs&
Dcs::operator= (const Dcs& dcs)
{
  _rotMatrix    = dcs._rotMatrix;
  _transMatrix  = dcs._transMatrix;
  _scaleMatrix  = dcs._scaleMatrix;
  _resultMatrix = dcs._resultMatrix;

  return (*this);
}
} // namespace gloost


