//-----------------------------------------------------------------------------
// Copyright (c) 2007 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#if !defined(MODEL_OBJ_H)
#define MODEL_OBJ_H


//-----------------------------------------------------------------------------
// Alias|Wavefront OBJ file loader.
//
// This OBJ file loader contains the following restrictions:
// 1. Group information is ignored. Faces are grouped based on the material
//    that each face uses.
// 2. Object information is ignored. This loader will merge everything longo a
//    single object.
// 3. The MTL file must be located in the same directory as the OBJ file. If
//    it isn't then the MTL file will fail to load and a default material is
//    used instead.
// 4. This loader triangulates all polygonal faces during importing.
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <map>


class ModelOBJ
{
public:
    struct Material
    {
        float ambient[4];
        float diffuse[4];
        float specular[4];
        float shininess;        // [0 = min shininess, 1 = max shininess]
        float alpha;            // [0 = fully transparent, 1 = fully opaque]

        std::string name;
        std::string colorMapFilename;
        std::string bumpMapFilename;
    };

    struct Vertex
    {
        float position[3];
        float texCoord[2];
        float normal[3];
        float tangent[4];
        float bitangent[3];
    };

    struct Mesh
    {
        long startIndex;
       long triangleCount;
        const Material *pMaterial;
    };

    ModelOBJ();
    ~ModelOBJ();

    void destroy();
    bool import(const char *pszFilename, bool rebuildNormals = false);
    void normalize(float scaleTo = 1.0f, bool center = true);
    void reverseWinding();

    // Getter methods.

    void getCenter(float &x, float &y, float &z) const;
    float getWidth() const;
    float getHeight() const;
    float getLength() const;
    float getRadius() const;

    const long *getIndexBuffer() const;
    long getIndexSize() const;

    const Material &getMaterial(long i) const;
    const Mesh &getMesh(long i) const;

    long getNumberOfIndices() const;
    long getNumberOfMaterials() const;
    long getNumberOfMeshes() const;
    long getNumberOfTriangles() const;
    long getNumberOfVertices() const;

    const std::string &getPath() const;

    const Vertex &getVertex(long i) const;
    const Vertex *getVertexBuffer() const;
    long getVertexSize() const;

    bool hasNormals() const;
    bool hasPositions() const;
    bool hasTangents() const;
    bool hasTextureCoords() const;
	std::vector<long> m_indexBuffer;

private:
    void addTrianglePos(long index, long material,
        long v0, long v1, long v2);
    void addTrianglePosNormal(long index, long material,
        long v0, long v1, long v2,
        long vn0, long vn1, long vn2);
    void addTrianglePosTexCoord(long index, long material,
        long v0, long v1, long v2,
        long vt0, long vt1, long vt2);
    void addTrianglePosTexCoordNormal(long index, long material,
        long v0, long v1, long v2,
        long vt0, long vt1, long vt2,
        long vn0, long vn1, long vn2);
    long addVertex(long hash, const Vertex *pVertex);
    void bounds(float center[3], float &width, float &height,
        float &length, float &radius) const;
    void buildMeshes();
    void generateNormals();
    void generateTangents();
    void importGeometryFirstPass(FILE *pFile);
    void importGeometrySecondPass(FILE *pFile);
    bool importMaterials(const char *pszFilename);
    void scale(float scaleFactor, float offset[3]);

    bool m_hasPositions;
    bool m_hasTextureCoords;
    bool m_hasNormals;
    bool m_hasTangents;

    long m_numberOfVertexCoords;
    long m_numberOfTextureCoords;
    long m_numberOfNormals;
    long m_numberOfTriangles;
    long m_numberOfMaterials;
    long m_numberOfMeshes;

    float m_center[3];
    float m_width;
    float m_height;
    float m_length;
    float m_radius;

    std::string m_directoryPath;

    std::vector<Mesh> m_meshes;
    std::vector<Material> m_materials;
    std::vector<Vertex> m_vertexBuffer;
    std::vector<long> m_attributeBuffer;
    std::vector<float> m_vertexCoords;
    std::vector<float> m_textureCoords;
    std::vector<float> m_normals;

    std::map<std::string, long> m_materialCache;
    std::map<long, std::vector<long> > m_vertexCache;
};

//-----------------------------------------------------------------------------

inline void ModelOBJ::getCenter(float &x, float &y, float &z) const
{ x = m_center[0]; y = m_center[1]; z = m_center[2]; }

inline float ModelOBJ::getWidth() const
{ return m_width; }

inline float ModelOBJ::getHeight() const
{ return m_height; }

inline float ModelOBJ::getLength() const
{ return m_length; }

inline float ModelOBJ::getRadius() const
{ return m_radius; }

inline const long *ModelOBJ::getIndexBuffer() const
{ return &m_indexBuffer[0]; }

inline long ModelOBJ::getIndexSize() const
{ return static_cast<long>(sizeof(long)); }

inline const ModelOBJ::Material &ModelOBJ::getMaterial(long i) const
{ return m_materials[i]; }

inline const ModelOBJ::Mesh &ModelOBJ::getMesh(long i) const
{ return m_meshes[i]; }

inline long ModelOBJ::getNumberOfIndices() const
{ return m_numberOfTriangles * 3; }

inline long ModelOBJ::getNumberOfMaterials() const
{ return m_numberOfMaterials; }

inline long ModelOBJ::getNumberOfMeshes() const
{ return m_numberOfMeshes; }

inline long ModelOBJ::getNumberOfTriangles() const
{ return m_numberOfTriangles; }

inline long ModelOBJ::getNumberOfVertices() const
{ return static_cast<long>(m_vertexBuffer.size()); }

inline const std::string &ModelOBJ::getPath() const
{ return m_directoryPath; }

inline const ModelOBJ::Vertex &ModelOBJ::getVertex(long i) const
{ return m_vertexBuffer[i]; }

inline const ModelOBJ::Vertex *ModelOBJ::getVertexBuffer() const
{ return &m_vertexBuffer[0]; }

inline long ModelOBJ::getVertexSize() const
{ return static_cast<long>(sizeof(Vertex)); }

inline bool ModelOBJ::hasNormals() const
{ return m_hasNormals; }

inline bool ModelOBJ::hasPositions() const
{ return m_hasPositions; }

inline bool ModelOBJ::hasTangents() const
{ return m_hasTangents; }

inline bool ModelOBJ::hasTextureCoords() const
{ return m_hasTextureCoords; }

#endif
