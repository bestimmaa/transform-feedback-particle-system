
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



#ifndef H_GLOOST_MESH
#define H_GLOOST_MESH




/// gloost includes
#include <SharedResource.h>
#include <Vector3.h>
#include <Point3.h>
#include <Ray.h>
#include <BoundingBox.h>
#include <ObjMatFile.h>
#include <BitMask.h>


/// cpp includes
#include <string>
#include <vector>
#include <iostream>



namespace gloost
{


 /// A line with 2 indices into the mesh attribute vectors

struct Line
{
  Line(unsigned int index0, unsigned int index1)
  {
    vertexIndices[0] = index0;
    vertexIndices[1] = index1;
  }

  Line()
  {
    vertexIndices[0] = 0;
    vertexIndices[1] = 0;
  }

  unsigned int vertexIndices[2];
};


//////////////////////////////////////////////////////////////////////////////////////////


 /// A triangle face with 3 indices into the mesh attribute vectors

struct TriangleFace
{
  TriangleFace(unsigned int index0, unsigned int index1, unsigned int index2)
  {
    vertexIndices[0] = index0;
    vertexIndices[1] = index1;
    vertexIndices[2] = index2;
  }

  TriangleFace()
  {
    vertexIndices[0] = 0;
    vertexIndices[1] = 0;
    vertexIndices[2] = 0;
  }

  unsigned int vertexIndices[3];
};


//////////////////////////////////////////////////////////////////////////////////////////


 /// A quad face with 4 indices into the mesh attribute vectors

struct QuadFace
{
  QuadFace(unsigned int index0,
               unsigned int index1,
               unsigned int index2,
               unsigned int index3)
  {
    vertexIndices[0] = index0;
    vertexIndices[1] = index1;
    vertexIndices[2] = index2;
    vertexIndices[3] = index3;
  }

  QuadFace()
  {
    vertexIndices[0] = 0;
    vertexIndices[1] = 0;
    vertexIndices[2] = 0;
    vertexIndices[3] = 0;
  }

  unsigned int vertexIndices[4];
};


//////////////////////////////////////////////////////////////////////////////////////////


#define GLOOST_MESH_POSITIONS 0
#define GLOOST_MESH_NORMALS   1
#define GLOOST_MESH_COLORS    2
#define GLOOST_MESH_TEXCOORDS 3

//enum Mesh_interleavedComponents
//{
//  GLOOST_MESH_POSITIONS = 0,
//  GLOOST_MESH_NORMALS   = 1,
//  GLOOST_MESH_COLORS    = 2,
//  GLOOST_MESH_TEXCOORDS = 3
//};


//////////////////////////////////////////////////////////////////////////////////////////


  /// size of interleaved Mesh attributes (position, normal, color, texcoord) in bytes

enum Mesh_sizeof_Attributes
{
  GLOOST_MESH_SIZEOF_VERTEX   = 12,
  GLOOST_MESH_SIZEOF_NORMAL   = 12,
  GLOOST_MESH_SIZEOF_COLOR    = 16,
  GLOOST_MESH_SIZEOF_TEXCOORD = 8
};


//////////////////////////////////////////////////////////////////////////////////////////


  /// size of interleaved Mesh attributes (position, normal, color, texcoord) in bytes

#define  GLOOST_MESH_NUM_COMPONENTS_VERTEX   3
#define  GLOOST_MESH_NUM_COMPONENTS_NORMAL   3
#define  GLOOST_MESH_NUM_COMPONENTS_COLOR    4
#define  GLOOST_MESH_NUM_COMPONENTS_TEXCOORD 2



//////////////////////////////////////////////////////////////////////////////////////////


  /// stores range within primitive indices

struct IndexRange
{

  IndexRange()
  {
    name     = "noname";
    drawMode = 0;
    start    = 0;
    end      = 0;
    elements = std::vector<unsigned>();
  }

  IndexRange(const std::string& iName, unsigned iDrawMode, unsigned iStart, unsigned iEnd)
  {
    name     = iName;
    drawMode = iDrawMode;
    start    = iStart;
    end      = iEnd;
    elements = std::vector<unsigned>();
  }

  void printInfo()
  {
    std::cerr << std::endl << "IndexRange:   ";
    std::cerr << std::endl << "name:     " << name;
    std::cerr << std::endl << "drawMode: " << drawMode;
    std::cerr << std::endl << "start:    " << start;
    std::cerr << std::endl << "end:      " << end;
    std::cerr << std::endl << "elements: " << elements.size();
  }

  std::string name;
  unsigned    drawMode;
  unsigned    start;
  unsigned    end;
  std::vector<unsigned> elements;
};


//////////////////////////////////////////////////////////////////////////////////////////


  /// stores info about the Mesh

struct MeshInterleavedInfo
{

  MeshInterleavedInfo()
  {

    interleavedPackageSize   = 0;
    interleavedPackageStride = 0;

    interleavedVertexOffset = 0;
    interleavedVertexStride = 0;

    interleavedNormalOffset = 0;
    interleavedNormalStride = 0;

    interleavedColorOffset = 0;
    interleavedColorStride = 0;

    interleavedTexcoordOffset = 0;
    interleavedTexcoordStride = 0;
  }


  /// size of one interleaved vertex with attributes
  unsigned interleavedPackageSize;

  /// stride for a whole interleaved package
  unsigned interleavedPackageStride;

  /// position of the vertex within a interleaved package
  unsigned interleavedVertexOffset;
  /// offset of the vertex within a interleaved in bytes
  unsigned interleavedVertexStride;

  /// position of the normal within a interleaved package
  unsigned interleavedNormalOffset;
  /// offset of the normal within a interleaved in bytes
  unsigned interleavedNormalStride;

  /// position of the color within a interleaved package
  unsigned interleavedColorOffset;
  /// offset of the color within a interleaved in bytes
  unsigned interleavedColorStride;

  /// position of the texcoord within a interleaved package
  unsigned interleavedTexcoordOffset;
  /// offset of the texcoord within a interleaved in bytes
  unsigned interleavedTexcoordStride;
};


//////////////////////////////////////////////////////////////////////////////////////////


  class Matrix;


//////////////////////////////////////////////////////////////////////////////////////////


  //  Mesh container storing vertices, normals, texture coordinates, triangles, quads, lines

class Mesh : public SharedResource
{
	public:

    // class constructor
    Mesh();

    // class constructor
    Mesh(Mesh* mesh, bool interleave);

    // class destructor
	  virtual ~Mesh();



	  // returns the vector of vertices
	  std::vector<Point3>& getVertices();

	  // returns the vector of normals
	  std::vector<Vector3>& getNormals();

	  // returns the vector of colors
	  std::vector<vec4>& getColors();

	  // returns the vector of texCoords
	  std::vector<Point3>& getTexCoords();

	  // returns the vector of interleaved components
	  std::vector<float>& getInterleavedAttributes();

	  // returns the vector of Points
	  std::vector<unsigned>& getPoints();

	  // returns the vector of Lines
	  std::vector<Line>& getLines();

	  // returns the vector of triangles
	  std::vector<TriangleFace>& getTriangles();

	  // returns the vector of triangles
	  std::vector<QuadFace>& getQuads();


	  // returns the number of interleaved packages of vertices and their attribut
	  unsigned getNumInterleavedPackages() const;

	  // returns the memory cosumtion of this mesh in cpu ram
	  float getMemoryUsageCpu() const;


	  // returns a map <std::string, IndexRage> with ranges within an element array
	  std::map<std::string, IndexRange>& getIndexRanges();


    // push interleaved component into the container
	  void pushInterleavedComponent(const Point3& pointOrTexCoord);

    // push interleaved component into the container
	  void pushInterleavedComponent(const Vector3& normal);

    // push interleaved component into the container
	  void pushInterleavedComponent(const vec4& color);

    // push interleaved component into the container
	  void pushInterleavedComponent(const vec2& texcoord);

    // push interleaved component into the container
	  void pushInterleavedComponent(float scalar);


	  // returns a BitMask indicating available interleaved attributes
	  const BitMask& getSupportedInterleavedAttributes() const;

	  // enables or disables a available interleaved attribute
	  void setSupportedInterleavedAttribute(unsigned attribute, bool state = true);

	  // returns true if attribute is enabled for interleaved data
	  bool isInterleavedAttributeSupported(unsigned attribute) const;

	  // returns the MeshInfo containing information about interleaved data
	  const MeshInterleavedInfo& getInterleavedInfo() const;


	  // interleaves the vertex positions with available vertex components
	  void interleave(bool clearComponentArraysAfterwards = true);

	  // interleaves a meshs vertex positions with available vertex components
	  void interleaveFromOtherMesh(Mesh* mesh);


	  // Transforms vertex and normal data with a matrix
	  void transform(const Matrix& matrix,
                   bool normalizeNormalVectors = false);


	  // Normalizes normal vectors
	  void normalizeNormals();

    // Generates one normal per face by averaging the participating vertex normals
//	  void generateFlatShadingFaceNormals();

    // Generates per vertex normals from face vertices
	  void generateNormals();


    // Centers the Mesh by altering vertex positons
    void center();

    // Returns the offset to the centered position
    Vector3 getOffsetToCenter();

    // Scales the Mesh so that the longest side of the bounding box is of size maxLength
    void scaleToSize(mathType maxLength = 1.0);

    // returns a factor to scale the mesh by, so the longest side of the bounding box is of size maxlength
    mathType getScaleFactorToSize(mathType maxLength = 1.0);


    /// normalizes the color range of the vertices from 0...maxValue to 0...1
    void convertVertexColorComponentsToFloat(float maxValue = 255.0f);


    /// merges vertices if they have same position
//    void mergeVertices(float maxDist = 0);


    /// adds a Mesh to this one
    void add(Mesh* mesh);


    /// returns BoundingBox of this Mesh
    const BoundingBox& getBoundingBox() const;

    /// recalculate BoundingBox
    void recalcBoundingBox();


    // updates the mesh info
    void updateMeshInfo();

    // prints the meshInfo
    void printMeshInfo() const;


    /// returns true if Mesh is empty
    bool isEmpty() const;


    /// clears the mesh
    void clear();


	protected:


    /// vector of vertices
    std::vector<Point3>       _vertices;
    /// vector of normals
    std::vector<Vector3>      _normals;
    /// vector of colors
    std::vector<vec4>         _colors;
    /// vector of texCoords
    std::vector<Point3>       _texCoords;
    // vector of interleaved components
    std::vector<float>        _interleavedAttributes;
    /// vector of points
    std::vector<unsigned int> _points;
    /// vector of lines
    std::vector<Line>         _lines;
    /// vector of triangles
    std::vector<TriangleFace> _triangles;
    /// vector of triangles
    std::vector<QuadFace>     _quads;


    // indicates supported vertex attributes within the interleaved structure
    gloost::BitMask     _supportedInterleavedComponents;

    MeshInterleavedInfo _interleavedInfo;

    std::map<std::string, IndexRange> _indexRanges;

    /// bounding box
    BoundingBox _boundingBox;
};





/// ostream operator
//extern std::ostream& operator<< (std::ostream&, const Vertex&);




} // namespace gloost







#endif // H_GLOOST_MESH

