/***************************************************************************
                          tnlFunctionDiscretizer_impl.h  -  description
                             -------------------
    begin                : Nov 24, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#ifndef TNLFUNCTIONDISCRETIZER_IMPL_H_
#define TNLFUNCTIONDISCRETIZER_IMPL_H_

template< typename Mesh, typename Function, typename DiscreteFunction >
   template< int XDiffOrder, int YDiffOrder, int ZDiffOrder >
void tnlFunctionDiscretizer< Mesh, Function, DiscreteFunction >::discretize( const Mesh& mesh,
                                                                             const Function& function,
                                                                             DiscreteFunction& discreteFunction )
{
   //tnlAssert( Mesh::Dimensions == Function::Dimensions, ); // TODO: change this to tnlStaticAssert
   typename Mesh::IndexType i = 0;
   discreteFunction.setSize( mesh.getNumberOfCells() );
   while( i < mesh.getNumberOfCells() )
   {
      typename Mesh::VertexType v;
      typename Mesh::CoordinatesType c;
      c = mesh.getCellCoordinates( i );
      v = mesh.getCellCenter( c );
      discreteFunction[ i ] = function.template getF< XDiffOrder, YDiffOrder, ZDiffOrder >( v );
      i++;
   }
}



#endif /* TNLFUNCTIONDISCRETIZER_IMPL_H_ */
