/***************************************************************************
                          tnlFiniteDifferencesTest.h  -  description
                             -------------------
    begin                : Jan 12, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
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

#include <tnlConfig.h>
#include <core/tnlHost.h>
#include <cstdlib>

#include "../tnlPDEOperatorEocTester.h"
#include "../../tnlUnitTestStarter.h"
#include <mesh/tnlGrid.h>
#include <operators/fdm/tnlBackwardFiniteDifference.h>
#include <operators/fdm/tnlForwardFiniteDifference.h>
#include <operators/fdm/tnlCentralFiniteDifference.h>
#include <operators/fdm/tnlExactDifference.h>
#include "../tnlPDEOperatorEocTestResult.h"
#include <functions/tnlExpBumpFunction.h>

template< typename MeshType,
          typename RealType,
          int XDifference,
          int YDifference,
          int ZDifference,
          typename IndexType,   
          typename TestFunction >
class tnlPDEOperatorEocTestResult< 
   tnlForwardFiniteDifference< MeshType, XDifference, YDifference, ZDifference, RealType, IndexType >, TestFunction >
{
   public:
      static RealType getL1Eoc() 
      {
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0;
      };
      static RealType getL1Tolerance() { return ( RealType ) 0.05; };

      static RealType getL2Eoc()
      { 
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0;
      };
      static RealType getL2Tolerance() { return ( RealType ) 0.05; };

      static RealType getMaxEoc()
      {
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0; 
      };
      static RealType getMaxTolerance() { return ( RealType ) 0.05; };
};

template< typename MeshType,
          typename RealType,
          int XDifference,
          int YDifference,
          int ZDifference,
          typename IndexType,   
          typename TestFunction >
class tnlPDEOperatorEocTestResult< 
   tnlBackwardFiniteDifference< MeshType, XDifference, YDifference, ZDifference, RealType, IndexType >, TestFunction >
{
   public:
      static RealType getL1Eoc() 
      {
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0;
      };
      static RealType getL1Tolerance() { return ( RealType ) 0.05; };

      static RealType getL2Eoc()
      { 
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0;
      };
      static RealType getL2Tolerance() { return ( RealType ) 0.05; };

      static RealType getMaxEoc()
      {
         if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 1.0; 
      };
      static RealType getMaxTolerance() { return ( RealType ) 0.05; };

};

template< typename MeshType,
          typename RealType,
          int XDifference,
          int YDifference,
          int ZDifference,
          typename IndexType,   
          typename TestFunction >
class tnlPDEOperatorEocTestResult< 
   tnlCentralFiniteDifference< MeshType, XDifference, YDifference, ZDifference, RealType, IndexType >, TestFunction >
{
   public:
      static RealType getL1Eoc() 
      {
         //if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 2.0;
      };
      static RealType getL1Tolerance() { return ( RealType ) 0.05; };

      static RealType getL2Eoc()
      { 
         //if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 2.0;
      };
      static RealType getL2Tolerance() { return ( RealType ) 0.05; };

      static RealType getMaxEoc()
      {
         //if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
            return ( RealType ) 2.0; 
      };
      static RealType getMaxTolerance() { return ( RealType ) 0.05; };

};

template< typename FiniteDifferenceOperator,
          typename ExactOperator,
          typename Function,
          int MeshSize,
          bool writeFunctions,
          bool verbose >
bool testFiniteDifferenceOperator()
{
    if( !tnlUnitTestStarter::run<
            tnlPDEOperatorEocTester< 
                FiniteDifferenceOperator,
                ExactOperator,
                Function,
                typename FiniteDifferenceOperator::MeshType::Cell,
                MeshSize,
                writeFunctions,
                verbose > >() )
        return false;
    return true;
}

template< typename Mesh,
          typename Function,
          typename RealType,
          typename IndexType,
          int XDifference,
          int YDifference,
          int ZDifference,
          int MeshSize,
          bool WriteFunctions,
          bool Verbose >
bool setFiniteDifferenceOperator()
{
    typedef tnlForwardFiniteDifference< Mesh, XDifference, YDifference, ZDifference, RealType, IndexType > ForwardFiniteDifference;
    typedef tnlBackwardFiniteDifference< Mesh, XDifference, YDifference, ZDifference, RealType, IndexType > BackwardFiniteDifference;
    typedef tnlCentralFiniteDifference< Mesh, XDifference, YDifference, ZDifference, RealType, IndexType > CentralFiniteDifference;
    typedef tnlExactDifference< Function, XDifference, YDifference, ZDifference > ExactOperator;
    if( XDifference < 2 && YDifference < 2 && ZDifference < 2 )
      return ( testFiniteDifferenceOperator< ForwardFiniteDifference, ExactOperator, Function, MeshSize, WriteFunctions, Verbose >() &&
               testFiniteDifferenceOperator< BackwardFiniteDifference, ExactOperator, Function, MeshSize, WriteFunctions, Verbose >() &&
               testFiniteDifferenceOperator< CentralFiniteDifference, ExactOperator, Function, MeshSize, WriteFunctions, Verbose >() );
    else
      return ( testFiniteDifferenceOperator< CentralFiniteDifference, ExactOperator, Function, MeshSize, WriteFunctions, Verbose >() );
}

template< typename Mesh,
          typename RealType,
          typename IndexType,
          int XDifference,
          int YDifference,
          int ZDifference,        
          int MeshSize,
          bool WriteFunctions,
          bool Verbose >
bool setFunction()
{
    const int Dimensions = Mesh::meshDimensions;
    typedef tnlExpBumpFunction< Dimensions, RealType >  Function;
    return setFiniteDifferenceOperator< Mesh, Function, RealType, IndexType, XDifference, YDifference, ZDifference, MeshSize, WriteFunctions, Verbose  >();
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename RealType,
          typename IndexType,
          int MeshSize,
          bool WriteFunctions,
          bool Verbose >
bool setDifferenceOrder()
{
    typedef tnlGrid< 1, MeshReal, Device, MeshIndex > Grid1D;
    typedef tnlGrid< 2, MeshReal, Device, MeshIndex > Grid2D;
    typedef tnlGrid< 3, MeshReal, Device, MeshIndex > Grid3D;
    return ( setFunction< Grid1D, RealType, IndexType, 1, 0, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid1D, RealType, IndexType, 2, 0, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid2D, RealType, IndexType, 1, 0, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid2D, RealType, IndexType, 0, 1, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid2D, RealType, IndexType, 2, 0, 0, MeshSize, WriteFunctions, Verbose >() &&            
             setFunction< Grid2D, RealType, IndexType, 0, 2, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid3D, RealType, IndexType, 1, 0, 0, MeshSize, WriteFunctions, Verbose >() &&             
             setFunction< Grid3D, RealType, IndexType, 0, 1, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid3D, RealType, IndexType, 0, 0, 1, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid3D, RealType, IndexType, 2, 0, 0, MeshSize, WriteFunctions, Verbose >() &&
             setFunction< Grid3D, RealType, IndexType, 0, 2, 0, MeshSize, WriteFunctions, Verbose >() &&             
             setFunction< Grid3D, RealType, IndexType, 0, 0, 2, MeshSize, WriteFunctions, Verbose >() );            
}

bool test()
{
   const bool writeFunctions( false );
   const bool verbose( false );
   if( ! setDifferenceOrder< double, tnlHost, int, double, int, 64, writeFunctions, verbose >() )
      return false;
#ifdef HAVE_CUDA
   if( ! setDifferenceOrder< double, tnlCuda, int, double, int, 64, writeFunctions, verbose >() )
      return false;
#endif    
    return true;    
}
