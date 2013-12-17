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

#define PARTICLE_COUNT 150000
#define FOV 60.0

int CurrentWidth = 800, CurrentHeight = 600, WindowHandle = 0;

unsigned FrameCount = 0;

unsigned ProjectionMatrixUniformLocation = 0;
unsigned ModelMatrixUniformLocation  = 0;
unsigned ViewMatrixUniformLocation  = 0; // View Matrix - Comes in handy when transforming the light to cameraspace without using the model transformations
unsigned ModelViewMatrixUniformLocation  = 0;
unsigned NormalMatrixUniformLocation     = 0;
unsigned textureUniformLocation1         = 0;		//Uniform Texture
unsigned NormalMapUniformLocation = 0; // Normal map
unsigned LightPositionUniformLocation = 0;
unsigned HeightNearPlaneUniformLocation = 0;
unsigned RandomUniformLocation = 0;

unsigned BufferIds[6] = { 0u };
unsigned ShaderIds[5] = { 0u };

GLuint tbo;
GLuint vbo;
GLuint vao;
GLuint vao2;


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
struct Particle {
    float x;
    float y;
    float z;
    float velocityX;
    float velocityY;
    float velocityZ;
    int age;
};

int main(int argc, char* argv[])
{
    srand(time(NULL));
	std::cout << std::string(argv[0]);
	Initialize(argc, argv);
    glutMainLoop();
    
    exit(EXIT_SUCCESS);
}


/////////////////////////////////////////////////////////////////////////////////////////

void SimulateParticles(void){
    // PARTICLE SIMULATION
    
   // int now = glutGet(GLUT_ELAPSED_TIME);
    
    glUseProgram(ShaderIds[3]);
    
    glBindVertexArray(vao);
    float r = (float)rand()/RAND_MAX;
    glUniform1f(RandomUniformLocation,r);
    
    // transform particle position on vertex shader
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
    glEndTransformFeedback();
    glFlush();
    glDisable(GL_RASTERIZER_DISCARD);

    // copy back from vertex buffer
    Particle feedback[PARTICLE_COUNT];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        //printf("Particle %d %f %f %f %f %f %f %d \n", i,feedback[i].x, feedback[i].y, feedback[i].z, feedback[i].velocityX, feedback[i].velocityY, feedback[i].velocityZ, feedback[i].age);
    }

    glBindVertexArray(0);

    glBindVertexArray(vao2);
    //glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(feedback),feedback,GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    CheckErrorsGL("ERROR while simulating particles");
}

//called every frame this functions draw
void Draw(void)
{
    // DRAWING
    glUseProgram(ShaderIds[0]);
    glBindVertexArray(vao2);

    gloost::Matrix cameraTransform;
    cameraTransform.setIdentity();
    cameraTransform.setTranslate(0.0,0.0,4.0);
    cameraTransform.invert();
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    float heightOfNearPlane = (float)abs(viewport[3]-viewport[1]) / (2*tan(0.5*FOV*M_PI/180.0));
    //std::cout<<heightOfNearPlane<<"\n";
    glUniform1f(HeightNearPlaneUniformLocation,heightOfNearPlane);
    
    //reset the modelmatrix
    ModelViewMatrixStack.clear();
    ModelViewMatrixStack.loadMatrix(cameraTransform);
    
    gloost::Matrix viewMatrix = ModelViewMatrixStack.top();
    // ATTENTION we use the modelviewmatrix
    glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());
    glUniform4f(LightPositionUniformLocation,20,30,10,1);
    
    //save the current transformation onto the MatrixStack
    ModelViewMatrixStack.push();
    {
        // create a copy of top matrix element
        ModelViewMatrixStack.push();
        gloost::Matrix modelMatrix;
        modelMatrix.setIdentity();
        // reset the copy to identy
        ModelViewMatrixStack.loadIdentity();
        // store translations for model
        ModelViewMatrixStack.translate(0, 0, 0);
        modelMatrix = ModelViewMatrixStack.top();
        // pop model matrix from stack
        ModelViewMatrixStack.pop();
        ModelViewMatrixStack.multMatrix(modelMatrix);
        
        // transfer ModelViewMatrix for Geometry 1 to Shaders
        glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, ModelViewMatrixStack.top().data());
        glUniformMatrix4fv(ModelMatrixUniformLocation,1,GL_FALSE, modelMatrix.data());

        //set the NormalMatrix for Geometry 1
        gloost::Matrix normalMatrix;
        normalMatrix = ModelViewMatrixStack.top();
        normalMatrix.invert();
        normalMatrix.transpose();
        
        // transfer NormalMatrix for Geometry 1 to Shaders
        glUniformMatrix4fv(NormalMatrixUniformLocation, 1, GL_FALSE, normalMatrix.data());
        // draw Geometry 1
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
        CheckErrorsGL("ERROR while drawing");

        
    }
    
    //load last transformation from stack
    ModelViewMatrixStack.pop();
    
    glBindVertexArray(0);



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
    
    SimulateParticles();
    
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
    
    CheckErrorsGL("ERROR after linking program 1!");

    
    ShaderIds[3] = glCreateProgram();
    {
        ShaderIds[4] = Shader::loadShader("myParticleVertexShader.vs", GL_VERTEX_SHADER);
        glAttachShader(ShaderIds[3], ShaderIds[4]);
    }
    
    const GLchar* feedbackVarying[] = {"out_Position","out_Velocity","out_Age"};
    glTransformFeedbackVaryings(ShaderIds[3],3,feedbackVarying,GL_INTERLEAVED_ATTRIBS);
    
    glLinkProgram(ShaderIds[3]);
    
    CheckErrorsGL("ERROR after linking program 2!");


    //describes how the uniforms in the shaders are named and to which shader they belong
    ModelViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ModelViewMatrix");
    ModelMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ModelMatrix");
    ViewMatrixUniformLocation  = glGetUniformLocation(ShaderIds[0], "ViewMatrix");
    ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
    NormalMatrixUniformLocation     = glGetUniformLocation(ShaderIds[0], "NormalMatrix");
    LightPositionUniformLocation = glGetUniformLocation(ShaderIds[0],"LightPosition");
    RandomUniformLocation = glGetUniformLocation(ShaderIds[3],"Random");
    HeightNearPlaneUniformLocation = glGetUniformLocation(ShaderIds[3],"HeightOfNearPlane");
    
}


/////////////////////////////////////////////////////////////////////////////////////////


void LoadModel()
{

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    Particle data[PARTICLE_COUNT];
    for (int i = 0; i < PARTICLE_COUNT; ++i) {
        float r = (float)rand()/RAND_MAX;
        r = r-0.5;
        Particle p;
        p.x = 0.0;
        p.y = -2.0;
        p.z = 0.0;
        p.velocityX = r;
        r = (float)rand()/RAND_MAX;
        r = r-0.5;
        p.velocityY = 1.0+(r*0.5);
        r = (float)rand()/RAND_MAX;
        r = r-0.5;
        p.velocityZ = r;
        p.age = 0;
        data[i]=p;
    }
    
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);

    // Setup vertex shader input
    GLint inputAttrib;
    
    inputAttrib = glGetAttribLocation(ShaderIds[3],"in_Position");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib,3,GL_FLOAT,GL_FALSE,sizeof(float)*6+sizeof(int),0);
    
    inputAttrib = glGetAttribLocation(ShaderIds[3],"in_Velocity");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib,3,GL_FLOAT,GL_FALSE,sizeof(float)*6+sizeof(int),(GLvoid*)(sizeof(int)*3));
    
    inputAttrib = glGetAttribLocation(ShaderIds[3],"in_Age");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribIPointer(inputAttrib,1,GL_INT,sizeof(float)*6+sizeof(int),(GLvoid*)(sizeof(int)*6));
    
    // Create vertex buffer object (vbo) to hold feedback
    glGenBuffers(1,&tbo);
    glBindBuffer(GL_ARRAY_BUFFER,tbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(data),nullptr,GL_STATIC_READ);
    
    glBindVertexArray(0);
    
    CheckErrorsGL("ERROR after creating vao1!");

    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    // bind particle buffer
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    // Setup vertex shader input
    GLint inputAttribParticleShading;
    inputAttribParticleShading = glGetAttribLocation(ShaderIds[0],"in_Position");
    glEnableVertexAttribArray(inputAttribParticleShading);
    glVertexAttribPointer(inputAttribParticleShading,3,GL_FLOAT,GL_FALSE,(sizeof(float)*6)+sizeof(int),0);
    
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
                              FOV,
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
    // enables setting of point size in vertex shader
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    ModelViewMatrixStack.loadIdentity();
    ProjectionMatrix.setIdentity();
    
    SetupShader();
    LoadModel();
    
}
