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
#include <glErrorUtil.h>

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
unsigned ModelMatrixUniformLocation  = 0;
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
    
    

    exit(EXIT_SUCCESS);
}


/////////////////////////////////////////////////////////////////////////////////////////

//called every frame this functions draw
void Draw(void)
{


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



}


/////////////////////////////////////////////////////////////////////////////////////////


void LoadModel()
{


    


}


/////////////////////////////////////////////////////////////////////////////////////////


void Cleanup()
{

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
}
