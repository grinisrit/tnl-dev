/***************************************************************************
                          tnlExactLinearDiffusion.h  -  description
                             -------------------
    begin                : Aug 8, 2014
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

#ifndef TNLEXACTLINEARDIFFUSION_H_
#define TNLEXACTLINEARDIFFUSION_H_

template< int Dimensions >
class tnlExactLinearDiffusion
{};

template<>
class tnlExactLinearDiffusion< 1 >
{
   public:

      enum { Dimensions = 1 };
      typedef typename Function::RealType RealType;
      typedef typename Function::VertexType VertexType;

      template< typename Function, typename Vertex, typename Real = Vertex::RealType >
      static Real getValue( const Function& function,
                            const Vertex& v );

};

template<>
class tnlExactLinearDiffusion< 2 >
{
   public:

      enum { Dimensions = 2 };
      typedef typename Function::RealType RealType;
      typedef typename Function::VertexType VertexType;

      template< typename Function, typename Vertex, typename Real = Vertex::RealType >
      static Real getValue( const Function& function,
                            const Vertex& v );

};

template<>
class tnlExactLinearDiffusion< 3 >
{
   public:

      enum { Dimensions = 3 };
      typedef typename Function::RealType RealType;
      typedef typename Function::VertexType VertexType;

      template< typename Function, typename Vertex, typename Real = Vertex::RealType >
      static Real getValue( const Function& function,
                            const Vertex& v );

};



#endif /* TNLEXACTLINEARDIFFUSION_H_ */
