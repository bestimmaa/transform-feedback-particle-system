
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



#ifndef H_GLOOST_OBJLOADER
#define H_GLOOST_OBJLOADER


/// gloost includes
#include <Vector3.h>
#include <Point3.h>
#include <Ray.h>
#include <MeshLoader.h>
#include <ObjMatFile.h>


/// cpp includes
#include <string>
#include <vector>



namespace gloost
{

  class Geometry;


//////////////////////////////////////////////////////////////////////////////////////////


  ///  Loads the horrible Wavefront Obj file format and provides a gloost::Mesh

class ObjLoader : public MeshLoader
{
	public:
    /// class constructor
    ObjLoader(const std::string& filePath);

    /// class destructor
	  virtual ~ObjLoader();


	  Mesh* getMesh();


	protected:


    /// reads an *.obj file
    bool readFile(const std::string& filePath);

    Point3 readFaceString(std::ifstream& inStream);

    ObjMatFile* _materials;

};


} // namespace gloost


#endif // H_GLOOST_OBJLOADER

