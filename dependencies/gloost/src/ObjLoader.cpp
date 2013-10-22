
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
#include <ObjLoader.h>
#include <Mesh.h>
#include <Point3.h>
#include <Vector3.h>
#include <Ray.h>
#include <gloostMath.h>
#include <gloostHelper.h>



/// cpp includes
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif
#include <string>
#include <iostream>
#include <vector>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

ObjLoader::ObjLoader(const std::string& filePath):
  MeshLoader(filePath),
  _materials(new ObjMatFile())
{
  readFile(filePath);
}


///////////////////////////////////////////////////////////////////////////////


  /// class constructor

ObjLoader::~ObjLoader()
{

}


///////////////////////////////////////////////////////////////////////////////

  /// reads a Wavefront Obj file

/*virtual*/
bool
ObjLoader::readFile(const std::string& filePath)
{


  std::vector<Point3>&       vertices   = _mesh->getVertices();
  std::vector<Vector3>&      normals    = _mesh->getNormals();
  std::vector<Point3>&       texcoords  = _mesh->getTexCoords();

  std::vector<Vector3>       tempNormals;
  std::vector<Point3>        tempTexcoords;

  std::vector<TriangleFace>& triangles  = _mesh->getTriangles();

  std::map<std::string, IndexRange>&  indexRanges = _mesh->getIndexRanges();
  std::map<std::string, std::vector<TriangleFace> > facesByMaterial;

  _mesh->clear();


  std::ifstream infile;
  infile.open(filePath.c_str());

  if (!infile)
  {

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_ERRORS
    std::cout << std::endl;
    std::cout << std::endl << "ERROR in ObjLoader::readFile():";
    std::cout << std::endl << "         Cant open \"" << filePath << "\"!";
    std::cout << std::endl;
    std::flush(std::cout);
#endif

    return 0;
  }


#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << std::endl;
  std::cout << std::endl << "Message from ObjLoader::readFile()";
  std::cout << std::endl << "             Loading \"" << filePath << "\".";
  std::cout << std::endl;
  std::flush(std::cout);
#endif


  /// parse the obj file structure
  std::string token;
  std::string currentMaterialName = GLOOST_OBJMATFILE_DEFAULT_MATERIAL;

  // create a default material
  Material* gloost_default_material = new Material();
  gloost_default_material->setAmbient (0.2, 0.2, 0.2, 1.0);
  gloost_default_material->setDiffuse (0.6, 0.6, 0.6, 1.0);
  gloost_default_material->setSpecular(1.0, 1.0, 1.0, 1.0);
  gloost_default_material->setNs(120.0);

  _materials->addMaterial(currentMaterialName, gloost_default_material);



  bool usesNormals   = false;
  bool usesTexcoords = false;

  while(infile >> token)
  {

    if("v" == token)
    {
      Point3 vertex(0.0,0.0,0.0);
      vertex[0] = readNextWordAsFloat(infile);
      vertex[1] = readNextWordAsFloat(infile);
      vertex[2] = readNextWordAsFloat(infile);
      vertices.push_back(vertex);
    }
    else if(token == "vn")
    {
      Vector3 normal(0.0,0.0,0.0);
      normal[0] = readNextWordAsFloat(infile);
      normal[1] = readNextWordAsFloat(infile);
      normal[2] = readNextWordAsFloat(infile);
      tempNormals.push_back(normal);
      usesNormals = true;
    }
    else if(token == "vt")
    {
      Point3 vt(0.0,0.0,0.0);
      vt[0] = readNextWordAsFloat(infile);
      vt[1] = readNextWordAsFloat(infile);
      tempTexcoords.push_back(vt);
      usesTexcoords = true;

    }
    else if("f" == token)
    {
      if (normals.size() == 0)
      {
        normals   = std::vector<Vector3>(vertices.size(), Vector3());
        texcoords = std::vector<Point3>(vertices.size(), Point3());
      }

      /*
        Align normals and texcoords so that the
        vertex position == normal position == texcoord position
        within their containers
      */

      // components: x = vertexIndex, y=texcoordIndex, z=normalIndex
      Point3 indices0 = readFaceString(infile);
      Point3 indices1 = readFaceString(infile);
      Point3 indices2 = readFaceString(infile);

      TriangleFace face(indices0[0]-1, indices1[0]-1, indices2[0]-1);
//      triangles.push_back(face);

      facesByMaterial[currentMaterialName].push_back(face);


      if (indices0[1])
      {
        texcoords[indices0[0]-1] = tempTexcoords[indices0[1]-1];
        texcoords[indices1[0]-1] = tempTexcoords[indices1[1]-1];
        texcoords[indices2[0]-1] = tempTexcoords[indices2[1]-1];
      }

      if (indices0[2])
      {
        normals[indices0[0]-1] = tempNormals[indices0[2]-1];
        normals[indices1[0]-1] = tempNormals[indices1[2]-1];
        normals[indices2[0]-1] = tempNormals[indices2[2]-1];
      }
    }
    else if("usemtl" == token)
    {
      infile >> currentMaterialName;
    }
    else if("mtllib" == token)
    {
      std::string mtlFileName;

      infile >> mtlFileName;

      /// load material
      std::string materialFilePath = gloost::pathToBasePath(filePath) + mtlFileName;
      _materials->load(materialFilePath);
    }
  }

  infile.close();

  // store last range
//  currentRange.end = triangles.size();
//  indexRanges[currentRange.name] = currentRange;



  // store triangles in mesh, ordered by their materials
  std::map<std::string, std::vector<TriangleFace> >::iterator triangesIt    = facesByMaterial.begin();
  std::map<std::string, std::vector<TriangleFace> >::iterator triangesEndIt = facesByMaterial.end();


  for (; triangesIt!=triangesEndIt; ++triangesIt)
  {
    const std::string&         materialName   = triangesIt->first;
    std::vector<TriangleFace>& triangleVector = triangesIt->second;
    std::vector<unsigned>      elements;

    IndexRange currentRange(materialName, GL_TRIANGLES, triangles.size(), 0);

    for (unsigned int i=0; i!=triangleVector.size(); ++i)
    {
      triangles.push_back(triangleVector[i]);
      elements.push_back(triangleVector[i].vertexIndices[0]);
      elements.push_back(triangleVector[i].vertexIndices[1]);
      elements.push_back(triangleVector[i].vertexIndices[2]);
    }

    // store range range
    currentRange.end      = triangles.size();
    currentRange.elements = elements;
    indexRanges[currentRange.name] = currentRange;
  }

  std::map<std::string, IndexRange>::iterator it    = indexRanges.begin();
  std::map<std::string, IndexRange>::iterator endIt = indexRanges.end();

  for (; it!=endIt; ++it)
  {
    std::cerr << std::endl << "name: " << it->second.name << " start: " << it->second.start << " end: " << it->second.end;
  }




  // clear normals and texcoords if not needed by this obj file
  if (!usesNormals)
  {
    normals.clear();
  }

  if (!usesTexcoords)
  {
    texcoords.clear();
  }


  /// recalc the bounding box
  _mesh->recalcBoundingBox();


#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_MESSAGES
  std::cout << std::endl;
  std::cout << std::endl << "Message from ObjLoader::readFile():";
  std::cout << std::endl << "             \"" << filePath << "\" succesfully loaded";
  std::cout << std::endl << "               vertices: ....... " << vertices.size();
  std::cout << std::endl << "               normals:  ....... " << normals.size();
  std::cout << std::endl << "               texcoords: ...... " << texcoords.size();
  std::cout << std::endl << "               triangles: ...... " << triangles.size();
  std::cout << std::endl;
  std::flush(std::cout);
#endif

  return true;

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the next word of an instream as float
*/

Point3
ObjLoader::readFaceString(std::ifstream& inStream)
{

  std::string faceString;
  inStream >> faceString;

  Point3 indices(0,0,0);

  std::string separator("/");

  int iterator = 0;
  int found;
  found = faceString.find_first_of(separator);
  while(found != std::string::npos)
  {
    if(found > 0)
    {
      indices[iterator++] = atoi(faceString.substr(0,found).c_str());
    }
    else
    {
      indices[iterator++] = 0;
    }
    faceString = faceString.substr(found+1);
    found = faceString.find_first_of(separator);
  }

  indices[iterator++] = atoi(faceString.substr(0,found).c_str());

  return indices;
}



///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns a Mesh object, filled with the loaded geometry
*/

Mesh*
ObjLoader::getMesh()
{
  return _mesh;
}


///////////////////////////////////////////////////////////////////////////////






} // namespace gloost

