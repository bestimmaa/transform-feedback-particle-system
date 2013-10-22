
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

            gloost is being created by Felix Weiﬂig and Stephan Beck

     Felix Weiﬂig (thesleeper@gmx.net), Stephan Beck (stephan@pixelstars.de)
*/



#ifndef H_GLOOST_HELPER
#define H_GLOOST_HELPER



/// cpp includes
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>



namespace gloost
{



///////////////////////////////////////////////////////////////////////////////


  /// convert something into a string using the stream operator and a string stream

template <class T>
inline std::string
toString(T value)
{
  std::ostringstream stream;
  stream << value;
  return stream.str();
}



///////////////////////////////////////////////////////////////////////////////


  /// convert something into a string

inline std::string
floatToString(float value, unsigned int precision = 2)
{
  std::ostringstream stream;

  stream.precision(precision);
  stream.fill('0');

  stream << value;
  return stream.str();
}


///////////////////////////////////////////////////////////////////////////////


  /// return the basename of a file within a path ( "folder/subfolder/someimage.jpg" -> "someimage" )

inline
std::string
pathToBasename(const std::string& filepath)
{
  // find last dot
  int dotPos   = filepath.rfind('.');

  // find last slash
  int slashPos = filepath.rfind('/');

  std::string filename = filepath.substr( slashPos+1, dotPos-slashPos-1);
  return filename;
}


///////////////////////////////////////////////////////////////////////////////


  /// return the basename of a filename or a path ( "folder/subfolder/someimage.jpg" -> "someimage.jpg" )

inline
std::string
pathToFilename(const std::string& filepath)
{
  // find last slash
  int slashPos = filepath.rfind('/');

  std::string basename = filepath.substr( slashPos+1, filepath.length()-slashPos-1);
  return basename;
}


///////////////////////////////////////////////////////////////////////////////


  /// return extension of a filename ore path ( "folder/subfolder/someimage.jpg" -> "jpg" )

inline
std::string
pathToExtension(const std::string& filepath)
{

  std::string filename = pathToFilename(filepath);

  // find last dot
  int dotPos = filename.rfind('.');

  std::string extension = filename.substr( dotPos+1, filename.length()-dotPos-1);

  if (!strcmp(extension.c_str(), filename.c_str()))
  {
    extension = "";
  }

  return extension;
}


///////////////////////////////////////////////////////////////////////////////


  /// return the path of a file without the filename of a path ( "folder/subfolder/someimage.jpg" -> "folder/subfolder" )

inline std::string pathToBasePath(const std::string& filepath)
{
  // find last slash
  int slashPos = filepath.rfind('/');

  return filepath.substr( 0, slashPos+1);
}


///////////////////////////////////////////////////////////////////////////////


  /// remarks: taken from http://www.infernodevelopment.com/perfect-c-string-explode-split

void inline
stringExplode(const std::string& str, const std::string& separator, std::vector<std::string>&results)
{
  std::string tmpString = str;
  int found;
  found = tmpString.find_first_of(separator);
  while(found != (int)std::string::npos)
  {
    if(found > 0)
    {
      results.push_back(tmpString.substr(0,found));
    }
    else
    {
      results.push_back("");
    }
    tmpString = tmpString.substr(found+1);
    found = tmpString.find_first_of(separator);
  }
//  if(tmpString.length() > 0)
  {
    results.push_back(tmpString);
  }
}


///////////////////////////////////////////////////////////////////////////////


  /// creates a string by concatinating the strings from a vecor using a seperator

std::string inline
stringImplode(const std::vector<std::string>& stringVector, const std::string& seperator)
{
  std::string tmpString("");

  if (stringVector.size())
  {
    tmpString = stringVector[0];

    for (unsigned int i=1; i!=stringVector.size(); ++i)
    {
      tmpString = tmpString + seperator + stringVector[i];
    }
  }

  return tmpString;
}


///////////////////////////////////////////////////////////////////////////////


  /// creates a string by cloning n times the input string

std::string inline
repeatString(const std::string& str, unsigned n)
{
  std::string tmpString = "";

  for (unsigned int i=0; i!=n; ++i)
  {
    tmpString = tmpString + str;
  }

  return tmpString;

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the next word of an ifstream as int
*/

inline int
readNextWordAsInt(std::ifstream& inStream)
{
  std::string word;
  inStream >> word;
  return atoi(word.c_str());
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief returns the next word of an instream as float
*/

inline float
readNextWordAsFloat(std::ifstream& inStream)
{
  std::string word;
  inStream >> word;
  return atof(word.c_str());
}


///////////////////////////////////////////////////////////////////////////////



}  // namespace gloost



/// + operator for string+int

inline const std::string operator+ (const std::string& s, const int ui)
{
  return s + gloost::toString(ui);
}
/// + operator for int+string
inline const std::string operator+ (const int ui, const std::string& s)
{
  return gloost::toString(ui) + s;
}

/// + operator for string+unsigned int
inline const std::string operator+ (const std::string& s, const unsigned int ui)
{
  return s + gloost::toString(ui);
}
/// + operator for unsigned int+string
inline const std::string operator+ (const unsigned int ui, const std::string& s)
{
  return gloost::toString(ui) + s;
}

/// + operator for string+float
inline const std::string operator+ (const std::string& s, const float f)
{
  return s + gloost::toString(f);
}
/// + operator for float+string
inline const std::string operator+ (const float f, const std::string& s)
{
  return gloost::toString(f) + s;
}

//bool stringcmp(const std::string& s1, const std::string& s2)
//{
//  return !(strcmp(s1.c_str(), s2.c_str()));
//}



#endif // #ifndef GLOOST_HELPER_H
