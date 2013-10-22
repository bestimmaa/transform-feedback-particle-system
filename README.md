CGFramework
===========

##Linux

Open the CGFramwork.workspace in /linux with Code::Blocks. This workspace contains two projects: The framework itself (CGFramework) and the gloost library (gloost).

Make sure you are running the latest (proprietary) drivers for your graphics card for full OpenGL 3.2 support. Tested with Ubuntu 12.04 and Nvidia 319 drivers.

###Dependencies

Install dependencies via apt-get

*sudo apt-get install freeglut3;sudo apt-get install freeglut3-dev;sudo apt-get install libfreeimage3sudo apt-get install libfreeimage-dev;sudo apt-get install libglew1.6;sudo apt-get install libglew1.6-dev*

###Ressources

Add all ressource files (objects, shaders, textures...) to /linux/CGFramework/copyressources.sh

##Mac OS X

Open the CGFramwork.xcodeproj in /mac with Xcode.

*Please note that Apple's GLUT implementation is used instead of FreeGLUT. FreeGLUT does not support OpenGL 3.2 on OS X. This is where the ugly IFDEF come from.*

###Dependencies

Install the dependencies via [homebrew](http://brew.sh/)

* sudo brew install glew
* sudo brew install freeimage

###Ressources

Add all ressource files in the corresponding section of "Build Phases" of Xcode's project settings.
