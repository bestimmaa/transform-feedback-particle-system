
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



#ifndef H_GLOOST_MESHLOADER
#define H_GLOOST_MESHLOADER


/// gloost includes
#include <string.h>


/// cpp includes
#include <string>
#include <vector>



namespace gloost
{

  class Mesh;


//////////////////////////////////////////////////////////////////////////////////////////


  ///  Base class of all mesh loader

class MeshLoader
{
	public:

    // class constructor
    MeshLoader(const std::string& filePath);

    // class destructor
	  virtual ~MeshLoader();


    // returns a pointer to the mesh
	  virtual Mesh* getMesh();

    // returns the file path of the mesh file
	  const std::string& getFilePath() const;


	protected:

    // read the file
    virtual bool readFile(const std::string& filePath) = 0;

    std::string _filePath;

    Mesh* _mesh;
};


} // namespace gloost


#endif // H_GLOOST_MESHLOADER

