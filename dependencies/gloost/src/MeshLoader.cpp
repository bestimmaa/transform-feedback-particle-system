
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
#include <MeshLoader.h>
#include <Mesh.h>



/// cpp includes
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glut.h>
//#include <string>
//#include <iostream>
//#include <vector>


namespace gloost
{

/**
  \class MeshLoader

  \brief Base class of all mesh loaders

  \author  Felix Weiszig
  \date    March 2011
  \remarks ...
*/

///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

MeshLoader::MeshLoader(const std::string& filePath):
  _filePath(filePath),
  _mesh(new Mesh())
{
  _mesh->takeReference();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief class constructor
  \param ...
  \remarks ...
*/

/*virtual*/
MeshLoader::~MeshLoader()
{
  _mesh->dropReference();
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns a Geometry object, filled with the loaded geometry
*/

/*virtual*/
Mesh*
MeshLoader::getMesh()
{
  return _mesh;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the file path of the mesh file
*/

/*virtual*/
const std::string&
MeshLoader::getFilePath() const
{
  return _filePath;
}


///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////


} // namespace gloost

