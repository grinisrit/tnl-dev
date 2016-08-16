/***************************************************************************
                          mfilename.h  -  description
                             -------------------
    begin                : 2007/06/18
    copyright            : (C) 2007 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once 

namespace TNL {

class String;

void FileNameBaseNumberEnding( const char* base_name,
                               int number,
                               int index_size,
                               const char* ending,
                               String& file_name );

String getFileExtension( const String fileName );

void RemoveFileExtension( String& file_name );

} // namespace TNL