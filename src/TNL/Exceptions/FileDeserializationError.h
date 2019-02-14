/***************************************************************************
                          FileDeserializationError.h  -  description
                             -------------------
    begin                : Nov 17, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#include <string>
#include <stdexcept>

namespace TNL {
namespace Exceptions {

class FileDeserializationError
   : public std::runtime_error
{
public:
   FileDeserializationError( const std::string& objectType, const std::string& fileName )
   : std::runtime_error( "Failed to deserialize object of type '" + objectType + "' from file '" + fileName + "'." )
   {}
};

} // namespace Exceptions
} // namespace TNL
