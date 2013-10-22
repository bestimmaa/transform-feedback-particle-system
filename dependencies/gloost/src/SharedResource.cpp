
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



/// gloost system includes
#include <SharedResource.h>



/// cpp includes
#include <string>
#include <iostream>



namespace gloost
{


/**
  \class SharedResource
  \brief Base class of all shared resources. Implements a reference counting mechanism

  \author Felix Weiszig
  \date   December 2009

  \remarks Each SharedResource instance gets its own unique id within your program.
           You can get the id by calling getSharedResourceId().
*/


/*static*/ unsigned int SharedResource::_uniqueResourceIdCounter = 0;


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Class constructor
*/

SharedResource::SharedResource():
    _resourceId(++_uniqueResourceIdCounter),
    _referenceCounter(0)
{

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Class destructor
*/

/*virtual*/
SharedResource::~SharedResource()
{
//  std::cout << std::endl << "SharedResource::~SharedResource(): " << getSharedResourceId();
//  std::flush(std::cout);


	if (_referenceCounter > 0)
	{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cout<< std::endl;
    std::cout << std::endl << "Warning from SharedResource::~SharedResource() on SharedResource " << getSharedResourceId() << ":";
    std::cout<< std::endl << "              You just destroyed a shared resource with";
    std::cout<< std::endl << "              a reference counter > 0";
    std::cout<< std::endl << "              Your program may crash after this Message.";
    std::flush(std::cout);
#endif

	}
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Increments the reference counter
*/

/*virtual*/
SharedResource*
SharedResource::takeReference()
{
	++_referenceCounter;
	return this;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Decrements the reference counter
*/

/*virtual*/
void
SharedResource::dropReference()
{
	if (_referenceCounter == 0)
	{

#ifndef GLOOST_SYSTEM_DISABLE_OUTPUT_WARNINGS
    std::cerr << std::endl;
    std::cerr << std::endl << "Warning from SharedResource::dropReference() " << getSharedResourceId() << ":";
    std::cerr<< std::endl << "              You just about to drop a reference of a shared resource";
    std::cerr<< std::endl << "              with a reference counter that was allready ZERO";
    std::cerr<< std::endl << "              Check your program logic to avoid this message...";
    std::flush(std::cerr);
#endif

    return;
  }

	--_referenceCounter;

  if (_referenceCounter == 0)
	{
//	  std::cerr << std::endl << "deleting: " << getSharedResourceId();
	  delete this;
	}

}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns true if this instance is referenced
*/


/*virtual*/
bool
SharedResource::isReferenced() const
{
	return (bool) _referenceCounter;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the reference counter
*/


/*virtual*/
int
SharedResource::getReferenceCount() const
{
	return _referenceCounter;
}


///////////////////////////////////////////////////////////////////////////////


/**
  \brief Returns the unique resource id of this SharedResource
*/

unsigned int
SharedResource::getSharedResourceId() const
{
	return _resourceId;
}



///////////////////////////////////////////////////////////////////////////////




} // namespace gloost


