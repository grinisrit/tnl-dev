/***************************************************************************
                          tnlConfigEntryType.h  -  description
                             -------------------
    begin                : Jul 5, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLCONFIGENTRYTYPE_H_
#define TNLCONFIGENTRYTYPE_H_

template< typename EntryType >
inline tnlString getUIEntryType() { return "Unknown type."; };

template<> inline tnlString getUIEntryType< tnlString >() { return "string"; };
template<> inline tnlString getUIEntryType< bool >()      { return "bool"; };
template<> inline tnlString getUIEntryType< int >()       { return "integer"; };
template<> inline tnlString getUIEntryType< double >()    { return "real"; };

template<> inline tnlString getUIEntryType< tnlList< tnlString > >() { return "list of string"; };
template<> inline tnlString getUIEntryType< tnlList< bool > >()      { return "list of bool"; };
template<> inline tnlString getUIEntryType< tnlList< int > >()       { return "list of integer"; };
template<> inline tnlString getUIEntryType< tnlList< double > >()    { return "list of real"; };

struct tnlConfigEntryType
{
   tnlString basic_type;

   bool list_entry;

   tnlConfigEntryType(){};

   tnlConfigEntryType( const tnlString& _basic_type,
                     const bool _list_entry )
   : basic_type( _basic_type ),
     list_entry( _list_entry ){}

   void Reset()
   {
      basic_type. setString( 0 );
      list_entry = false;
   };
};

#endif /* TNLCONFIGENTRYTYPE_H_ */