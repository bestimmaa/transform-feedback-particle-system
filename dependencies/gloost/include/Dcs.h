
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



#ifndef H_GLOOST_DCS_H
#define H_GLOOST_DCS_H



// gloost includes
#include <Vector3.h>
#include <Matrix.h>



namespace gloost
{


#define GLOOST_DCS_SET_IDENTITY 1;



 // DCS with 4 matrices (transMat, scaleMat, rotMat and transMat*scaleMat*rotMat), to store complex transformations by their components

class Dcs
{

 public:

  // constructor
  Dcs(bool setEverythingIdentity = true);

  // destructor
  virtual ~Dcs();


  // set rotation (call validate to apply transformations)
  void setRotate(const Matrix& m);
  // set rotation (call validate to apply transformations)
  void setRotate(const Vector3& v);
  // set rotation (call validate to apply transformations)
  void setRotate(const double x, const double y, const double z);
  // set rotation
	void setRotate(Vector3 const& v, double angle);

  // get rotation
  const Matrix&  getRotate() const;

  /// rotates the Dcs
  void rotate(const Vector3& axis, const mathType& radAngle);
  /// rotates the Dcs
  void rotate(const Matrix& rotMat);



  // set translation (call validate to apply transformations)
  void setTranslate(const Matrix& m);
  // set translation (call validate to apply transformations)
  void setTranslate(const Vector3& v);
  // set translation (call validate to apply transformations)
  void setTranslate(const double x, const double y, const double z);

  // get translation
  const Matrix&  getTranslate() const;
  // get translation
  const Vector3  getTranslateVector() const;

  /// translates the Dcs
  void translate(const mathType& x, const mathType& y, const mathType& z);
  /// translates the Dcs
  void translate(const Vector3& transVec);
  /// translates the Dcs
  void translate(const Matrix& transMat);



  // set scale (call validate to apply transformations)
  void setScale(const Matrix& m);
  // set scale (call validate to apply transformations)
  void setScale(const Vector3& v);
  // set scale (call validate to apply transformations)
  void setScale(const double x, const double y, const double z);
  // set scale (call validate to apply transformations)
  void setScale(mathType n);

  // get scale
  const Matrix&  getScale() const;
  // get scale
  const Vector3  getScaleVector() const;

  // scales the Dcs
  void scale(const mathType& uniformScaleFactor);
  // scales the Dcs
  void scale(const Vector3& scale);
  // scales the Dcs
  void scale(const mathType& x, const mathType& y, const mathType& z);
  // scales the Dcs
  void scale(const Matrix& scaleMat);


  // reset transformations
  void reset();


  // set dirty
  void setDirty();
  // set clean (this will be done automaticly if you call validate())
  void setClean();
  // is this dcs dirty
  bool isDirty();


  // apply all made transformations
  void validate();


  // get the Matrix of the complete transformation
  const Matrix&  getMatrix() const;

  // get the Matrix of the complete transformation
  const Matrix&  getValidMatrix();



  // assignment
  const Dcs& operator= (const Dcs&);


 protected:

  // rotations component matrix
  Matrix _rotMatrix;
  // translation component matrix
  Matrix _transMatrix;
  // scale component matrix
  Matrix _scaleMatrix;
  // result matrix containing rotation, translation and scale component
  Matrix _resultMatrix;

  // flag indicating that the Dcs needs to be validated
  bool   _dirty;
};


} // namespace gloost


#endif // #ifndef H_GLOOST_DCS_H
