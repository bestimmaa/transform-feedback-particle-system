//////////////////////////////////////////////////////////////////////////
//Sources: http://openglbook.com/
////////// http://www.dhpoware.com/demos/glObjViewer.html
////////// http://www.arcsynthesis.org/gltut/
/////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <gloostMath.h>
#include <gloostGlUtil.h>
#include "Shader.h"

// include gloost::Mesh wich is a geometry container
#include <Mesh.h>
gloost::Mesh* mesh = 0;

// loader for the wavefront *.obj file format
#include <ObjLoader.h>

//FreeImage Library
#include <FreeImage.h>

#include "Texture.h"


int CurrentWidth = 800, CurrentHeight = 600, WindowHandle = 0;

unsigned FrameCount = 0;

unsigned ProjectionMatrixUniformLocation = 0;
unsigned ViewMatrixUniformLocation  = 0; // View Matrix - Comes in handy when transforming the light to cameraspace without using the model transformations
unsigned ModelViewMatrixUniformLocation  = 0;
unsigned NormalMatrixUniformLocation     = 0;
unsigned textureUniformLocation1         = 0;		//Uniform Texture
unsigned NormalMapUniformLocation = 0; // Normal map
unsigned MVP3UniformLocation = 0; // A crippled 3x3 version of the model view matrix without the translation part
unsigned LightPositionUniformLocation = 0;

Texture* g_texture1 = 0;
Texture* g_texture2 = 0;
Texture* normal_texture_earth = 0;

unsigned BufferIds[6] = { 0u };
unsigned ShaderIds[3] = { 0u };

//the three different matrices for projection, viewing and model transforming
#include <Matrix.h>
gloost::Matrix ProjectionMatrix;

#include <MatrixStack.h>
gloost::MatrixStack ModelViewMatrixStack;


//Function callbacks
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void IdleFunction(void);
void TimerFunction(int);
void Cleanup(void);
void LoadModel(void);
void SetupShader();
void Draw(void);
void RenderFunction(void);
/////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
	std::cout << std::string(argv[0]);
	Initialize(argc, argv);
    glutMainLoop();

    exit(EXIT_SUCCESS);
}


/////////////////////////////////////////////////////////////////////////////////////////

//called every frame this functions draw
void Draw(void)
{
    int now = glutGet(GLUT_ELAPSED_TIME);

    // Rotation
    float rotation = now*0.001;


    //////////////////////////////////////////////////////////////////////////

    glUseProgram(ShaderIds[0]);
    // bind VAO to load configuration 
    glBindVertexArray(BufferIds[0]);


    gloost::Matrix cameraTransform;
    cameraTransform.setIdentity();
    cameraTransform.setTranslate(0.0,0.0,4.0);
    cameraTransform.invert();

    //reset the modelmatrix
    ModelViewMatrixStack.clear();
    ModelViewMatrixStack.loadMatrix(cameraTransform);
    // ATTENTION we use the modelviewmatrix
    glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());
    glUniform4f(LightPositionUniformLocation,0,10,10,0);

    gloost::Matrix normalMatrix;

    //save the current transformation onto the MatrixStack
    ModelViewMatrixStack.push();
    {
        ModelViewMatrixStack.translate(0, 0, 0);
        ModelViewMatrixStack.rotate(rotation, 0.0, 1.0, 0);
        // transfer ModelViewMatrix for Geometry 1 to Shaders
        glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());

        //set the NormalMatrix for Geometry 1
        normalMatrix = ModelViewMatrixStack.top();
        normalMatrix.invert();
        normalMatrix.transpose();

        glUniform1i(textureUniformLocation1, 0); //  Specify the value of text uniform variable for the current program object
        
        // transfer NormalMatrix for Geometry 1 to Shaders
        glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());

        //bind the Geometry
        glBindVertexArray(BufferIds[0]);
        
        g_texture1->bind();

        // draw Geometry 1
        glDrawElements(GL_TRIANGLES, mesh->getTriangles().size()*3, GL_UNSIGNED_INT, 0);

    }
    
    //load last transformation from stack
    ModelViewMatrixStack.pop();

    glBindVertexArray(0);
    glUseProgram(0);
}


/////////////////////////////////////////////////////////////////////////////////////////
void TimerFunction(int value){
    if(0 != value)
    {
        int fps = FrameCount * 4;
        glutSetWindowTitle( (gloost::toString(fps) + " fps").c_str());

    }
    FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}


void RenderFunction(void)
{
    ++FrameCount;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Draw();

    glutSwapBuffers();
    glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////


void SetupShader()
{
    // LOAD AND LINK SHADER
    ShaderIds[0] = glCreateProgram();
    {
        //takes a (shader) filename and a shader-type and returns and id of the compiled shader
        ShaderIds[1] = Shader::loadShader("myVertexShader.vs", GL_VERTEX_SHADER);
        ShaderIds[2] = Shader::loadShader("myPlaygroundFragmentShader.fs", GL_FRAGMENT_SHADER);

        //attaches a shader to a program
        glAttachShader(ShaderIds[0], ShaderIds[1]);
        glAttachShader(ShaderIds[0], ShaderIds[2]);
    }
    glLinkProgram(ShaderIds[0]);

    //describes how the uniforms in the shaders are named and to which shader they belong
    ModelViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ModelViewMatrix");
    ViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
    ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
    NormalMatrixUniformLocation     = glGetUniformLocation(ShaderIds[0], "NormalMatrix");
    textureUniformLocation1         = glGetUniformLocation(ShaderIds[0], "colorMap");
    NormalMapUniformLocation = glGetUniformLocation(ShaderIds[0],"NormalMap");
    MVP3UniformLocation = glGetUniformLocation(ShaderIds[0],"MVP3");
    LightPositionUniformLocation = glGetUniformLocation(ShaderIds[0],"LightPosition");

}


/////////////////////////////////////////////////////////////////////////////////////////


void LoadModel()
{

    //load a wavefront *.obj file
    gloost::ObjLoader loader("sphere.obj");
    mesh = loader.getMesh();

    //IMPORTANT: use this to increase the reference counter
    //gloost::meshes have a garbage collector which throws
    //the mesh away otherwise
    mesh->takeReference();

    mesh->generateNormals();
    mesh->generateTangentsBitangents();

    //normalizes the mesh
    mesh->scaleToSize(1.0);
    //puts the meshdata in one array
    mesh->interleave();

    //mesh->printMeshInfo();

    //create VAO which holds the state of our Vertex Attributes and VertexBufferObjects - a control structure
    //note: for different objects more of these are needed
    glGenVertexArrays(1, &BufferIds[0]);

    //bind Vertex Array - Scope begins
    glBindVertexArray(BufferIds[0]);

    //Create two VertexBufferObject and bind the first one to set its data
    glGenBuffers(2, &BufferIds[1]);
    glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);

    //set the vertex data for the actual buffer; the second parameter is the size in bytes of all Vertices together
    //the third parameter is a pointer to the vertexdata
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(float) * mesh->getInterleavedAttributes().size(),
                 &mesh->getInterleavedAttributes().front(),
                 GL_STATIC_DRAW);

    //enables a VertexAttributePointer for position 0
    //the shader can reference this as layout(location=0)
    glEnableVertexAttribArray(0);

    //specifies where in the GL_ARRAY_BUFFER our data(the vertex position) is exactly
    //(index,size,type,normalized,stride = byte offset between vertices in buffer,pointer = offset to the first component of vertex attribute);
    glVertexAttribPointer(0,
                          GLOOST_MESH_NUM_COMPONENTS_VERTEX,
                          GL_FLOAT,
                          GL_FALSE,
                          mesh->getInterleavedInfo().interleavedPackageStride,
                          (GLvoid*)(mesh->getInterleavedInfo().interleavedVertexStride));

    //enables a VertexAttributePointer for position 1
    //the shader can reference this as layout(location=1)
    glEnableVertexAttribArray(1);

    //specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly
    glVertexAttribPointer(1,
                          GLOOST_MESH_NUM_COMPONENTS_NORMAL,
                          GL_FLOAT, GL_FALSE,
                          mesh->getInterleavedInfo().interleavedPackageStride,
                          (GLvoid*)(mesh->getInterleavedInfo().interleavedNormalStride));
    
    // load texture data (myVertexShader: layout(location=2) in vec2 in_Texcoord)
    
	glEnableVertexAttribArray(2);
    
	//specifies where in the GL_ARRAY_BUFFER our data(the texture coordinates) is exactly
	glVertexAttribPointer(2,
                          GLOOST_MESH_NUM_COMPONENTS_TEXCOORD,
                          GL_FLOAT, GL_FALSE,
                          mesh->getInterleavedInfo().interleavedPackageStride,
                          (GLvoid*)(mesh->getInterleavedInfo().interleavedTexcoordStride));
    
    glEnableVertexAttribArray(3);
    
    //specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly
    glVertexAttribPointer(3,
                          GLOOST_MESH_NUM_COMPONENTS_TANGENTS,
                          GL_FLOAT, GL_FALSE,
                          mesh->getInterleavedInfo().interleavedPackageStride,
                          (GLvoid*)(mesh->getInterleavedInfo().interleavedTangentStride));
    
    glEnableVertexAttribArray(4);
    
    //specifies where in the GL_ARRAY_BUFFER our data(the vertex normal) is exactly
    glVertexAttribPointer(4,
                          GLOOST_MESH_NUM_COMPONENTS_BITANGENTS,
                          GL_FLOAT, GL_FALSE,
                          mesh->getInterleavedInfo().interleavedPackageStride,
                          (GLvoid*)(mesh->getInterleavedInfo().interleavedBitangentStride));
    

    // the seceond VertexBufferObject ist bound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
    // its data are the indices of the vertices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(gloost::TriangleFace) * mesh->getTriangles().size(),
                 &mesh->getTriangles().front(),
                 GL_STATIC_DRAW);

    // unbind the VertexArray - Scope ends
    glBindVertexArray(0);

}


/////////////////////////////////////////////////////////////////////////////////////////


void Cleanup()
{
    glDetachShader(ShaderIds[0], ShaderIds[1]);
    glDetachShader(ShaderIds[0], ShaderIds[2]);
    glDeleteShader(ShaderIds[1]);
    glDeleteShader(ShaderIds[2]);
    glDeleteProgram(ShaderIds[0]);

    glDeleteBuffers(2, &BufferIds[1]);
    glDeleteVertexArrays(1, &BufferIds[0]);
}


/////////////////////////////////////////////////////////////////////////////////////////


void IdleFunction(void)
{
    glutPostRedisplay();
}


/////////////////////////////////////////////////////////////////////////////////////////


void ResizeFunction(int Width, int Height)
{
    CurrentWidth = Width;
    CurrentHeight = Height;
    glViewport(0, 0, CurrentWidth, CurrentHeight);

    gloost::gloostPerspective(ProjectionMatrix,
                              60.0f,
                              (float)CurrentWidth / CurrentHeight,
                              1.0f,
                              100.0f
                              );

    glUseProgram(ShaderIds[0]);
    glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.data());
    glUseProgram(0);
}


/////////////////////////////////////////////////////////////////////////////////////////


void InitWindow(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitWindowSize(CurrentWidth, CurrentHeight);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_3_2_CORE_PROFILE );

#else
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutInitContextVersion(3,2);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(
                  GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS
                  );
#endif

    WindowHandle = glutCreateWindow("");

    std::printf("%s\n%s\n",
                glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
                glGetString(GL_VERSION)    // e.g. 3.2 INTEL-8.0.61
                );

    if(WindowHandle < 1)
    {
        fprintf(
                stderr,
                "ERROR: Could not create a new rendering window.\n"
                );
#ifdef __APPLE__
        exit(0);
#else
        glutExit();
#endif
    }

    //Glut function callbacks
    //TODO: add keyboard and mouse functions
    glutTimerFunc(0, TimerFunction, 0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(IdleFunction);
#ifdef __APPLE__
#else
    glutCloseFunc(Cleanup);
#endif


}

/////////////////////////////////////////////////////////////////////////////////////////

void Initialize(int argc, char* argv[])
{
    GLenum GlewInitResult;

    InitWindow(argc, argv);

    glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        fprintf(
                stderr,
                "ERROR: %s\n",
                glewGetErrorString(GlewInitResult)
                );
#ifdef __APPLE__
        exit(0);
#else
        glutExit();
#endif
    }

    fprintf(
            stdout,
            "INFO: OpenGL Version: %s\n",
            glGetString(GL_VERSION)
            );

    glGetError();
    glClearColor(0.20f, 0.2f, 0.2f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ModelViewMatrixStack.loadIdentity();
    ProjectionMatrix.setIdentity();

    SetupShader();
    LoadModel();
    
    //Load jpg as texture
	g_texture1 = new Texture("earth.jpg");
    normal_texture_earth = new Texture("earth_normalmap.jpg");
}
