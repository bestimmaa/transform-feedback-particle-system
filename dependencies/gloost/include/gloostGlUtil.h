
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



#ifndef H_GLOOST_RENDERGOODIES
#define H_GLOOST_RENDERGOODIES


#include <gloostConfig.h>
#include <gloostMath.h>
#include <Matrix.h>
#include <Vector3.h>



namespace gloost
{


inline void gloostPerspective(Matrix& matrix, float fieldOfView, float aspectRatio, float nearPlaneZ, float farPlaneZ);
inline void gloostFrustum(Matrix& matrix,float left,float right,float bottom,float top,float nearPlaneZ,float farPlaneZ);





///////////////////////////////////////////////////////////////////////////////


/**
  \brief   Creates a Matrix for a perspective projection
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/

void gloostPerspective(Matrix& matrix, float fieldOfView, float aspectRatio,
                    float nearPlaneZ, float farPlaneZ)
{
  float ymax, xmax;
  ymax = nearPlaneZ * tan(fieldOfView * PI / 360.0);
  //ymin = -ymax;
  //xmin = -ymax * aspectRatio;
  xmax = ymax * aspectRatio;
  gloostFrustum(matrix, -xmax, xmax, -ymax, ymax, nearPlaneZ, farPlaneZ);
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief   ...
  \remarks Code was taken from http://www.opengl.org/wiki/GluPerspective_code and
           is licensed by terms of the LGPL
*/


void gloostFrustum(Matrix& matrix,
                   float left,
                   float right,
                   float bottom,
                   float top,
                   float nearPlaneZ,
                   float farPlaneZ)
{
  float temp, temp2, temp3, temp4;
  temp = 2.0 * nearPlaneZ;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = farPlaneZ - nearPlaneZ;
  matrix[0] = temp / temp2;
  matrix[1] = 0.0;
  matrix[2] = 0.0;
  matrix[3] = 0.0;
  matrix[4] = 0.0;
  matrix[5] = temp / temp3;
  matrix[6] = 0.0;
  matrix[7] = 0.0;
  matrix[8] = (right + left) / temp2;
  matrix[9] = (top + bottom) / temp3;
  matrix[10] = (-farPlaneZ - nearPlaneZ) / temp4;
  matrix[11] = -1.0;
  matrix[12] = 0.0;
  matrix[13] = 0.0;
  matrix[14] = (-temp * farPlaneZ) / temp4;
  matrix[15] = 0.0;
}


}  // namespace gloost


#endif // #ifndef H_GLOOST_RENDERGOODIES
