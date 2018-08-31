/***************************************************************************
                          Logger_impl.h  -  description
                             -------------------
    begin                : Mar 10, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <sstream>
#include <iomanip>

namespace TNL {

template< typename T >
void Logger::writeParameter( const String& label,
                                const String& parameterName,
                                const Config::ParameterContainer& parameters,
                                int parameterLevel )
{
   stream << "| ";
   int i;
   for( i = 0; i < parameterLevel; i ++ )
      stream << " ";
   std::stringstream str;
   str << parameters.getParameter< T >( parameterName );
   stream  << label
           << std::setw( width - label.getLength() - parameterLevel - 3 )
           << str.str() << " |" << std::endl;
}

template< typename T >
void Logger :: writeParameter( const String& label,
                                  const T& value,
                                  int parameterLevel )
{
   stream << "| ";
   int i;
   for( i = 0; i < parameterLevel; i ++ )
      stream << " ";
   std::stringstream str;
   str << value;
   stream  << label
           << std::setw( width - label.getLength() - parameterLevel - 3 )
           << str.str() << " |" << std::endl;
};

} // namespace TNL
