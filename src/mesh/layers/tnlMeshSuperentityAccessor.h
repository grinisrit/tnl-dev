/***************************************************************************
                          tnlMeshSuperentityAccessor.h  -  description
                             -------------------
    begin                : Sep 11, 2015
    copyright            : (C) 2015 by Tomas Oberhuber et al.
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

#ifndef TNLMESHSUPERENTITYACCESSOR_H
#define	TNLMESHSUPERENTITYACCESSOR_H

template< typename IndexMultimapValues >
class tnlMeshSuperentityAccessor
{
   public:
      
      typedef typename IndexMultimapValues::IndexType   GlobalIndexType;
      typedef typename IndexMultimapValues::IndexType   LocalIndexType;
      
      // TODO: Add LocalIndexType to EllpackIndexMultimap
           
      LocalIndexType getSupernetitiesCount() const
      {
         return this->ports.getPortsCount();
      };
      
      void setSuperentityIndex( const LocalIndexType localIndex,
                                const GlobalIndexType globalIndex )
      {
         ports.setOutput( localIndex, globalIndex );
      }
      
      GlobalIndexType getSuperentityIndex( const LocalIndexType localIndex ) const
      {
         return ports.getOutput( localIndex );
      }
      
      GlobalIndexType& operator[]( const LocalIndexType localIndex )
      {
         return this->ports[ localIndex ];
      }
      
      const GlobalIndexType& operator[]( const LocalIndexType localIndex ) const
      {
         return this->ports[ localIndex ];
      }
      
      void print( ostream& str ) const
      {
         str << ports;
      }
      
   protected:
      
      IndexMultimapValues ports;
      
};

template< typename IndexMultimapValues >
ostream& operator << ( ostream& str, const tnlMeshSuperentityAccessor< IndexMultimapValues >& superentityAccessor )
{
   superentityAccessor.print( str );
   return str;
}

#endif	/* TNLMESHSUPERENTITYACCESSOR_H */

