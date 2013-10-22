
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

            gloost is being created by Felix Weißig and Stephan Beck

     Felix Weißig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



/// gloost system includes
#include <MatrixStack.h>
#include <gloostConfig.h>



/// cpp includes
#include <string>
#include <iostream>



namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

MatrixStack::MatrixStack():
  _stack(),
  _tempMatrix()
{

  _tempMatrix.setIdentity();
  _stack.push(_tempMatrix);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class destructor
  \param ...
  \remarks ...
*/

MatrixStack::~MatrixStack()
{
	// insert your code here
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief pushs a copy of the current top matrix into the stack
  \param ...
  \remarks ...
*/

void
MatrixStack::push()
{
  _stack.push(_stack.top());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief removes the top matrix from the stack
  \param ...
  \remarks ...
*/

void
MatrixStack::pop()
{
  _stack.pop();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns current matrix
  \param ...
  \remarks ...
*/

const Matrix&
MatrixStack::top() const
{
  return _stack.top();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns current matrix
  \param ...
  \remarks ...
*/

Matrix&
MatrixStack::top()
{
  return _stack.top();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief clears the matrix stack
  \param ...
  \remarks ...
*/

void
MatrixStack::clear()
{
  while (!_stack.empty())
  {
    _stack.pop();
  }

  _tempMatrix.setIdentity();
  _stack.push(_tempMatrix);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief sets the current top matrix to identity
  \param ...
  \remarks ...
*/

void
MatrixStack::loadIdentity()
{
  _stack.top().setIdentity();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate a matrix to the top matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::multMatrix(const Matrix& matrix)
{
  _stack.top() = _stack.top() * matrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief replaces the top matrix with another matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::loadMatrix(const Matrix& matrix)
{
  _stack.top() = matrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a translation matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::translate(mathType x, mathType y, mathType z)
{

  _tempMatrix.setIdentity();
  _tempMatrix.setTranslate(x,y,z);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a translation matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::translate(Vector3 offset)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setTranslate(offset);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a rotation matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::rotate(mathType angle, mathType x, mathType y, mathType z)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setRotate(x*angle, y*angle, z*angle);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a rotation matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::rotate(mathType angleX, mathType angleY, mathType angleZ)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setRotate(angleX,
                        angleY,
                        angleZ);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a rotation matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::rotate(Vector3 rotations)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setRotate(rotations[0],
                        rotations[1],
                        rotations[2]);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a scale matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::scale(mathType x, mathType y, mathType z)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setScale(x, y, z);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a scale matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::scale(Vector3 scale)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setScale(scale[0], scale[1], scale[2]);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief multiplicate the current top matrix with a scale matrix
  \param ...
  \remarks ...
*/

void
MatrixStack::scale(mathType scale)
{
  _tempMatrix.setIdentity();
  _tempMatrix.setScale(scale);

  _stack.top() = _stack.top() * _tempMatrix;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the number of elements in the stack
  \param ...
  \remarks ...
*/

unsigned int
MatrixStack::getSize() const
{
  return _stack.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief returns true if the MatrixStack is empty
  \param ...
  \remarks ...
*/

bool
MatrixStack::isEmpty() const
{
  return _stack.size();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief ...
  \param ...
  \remarks ...
*/









///////////////////////////////////////////////////////////////////////////////





} // namespace gloost



