/***************************************************************************
                          Grid2D_impl.h  -  description
                             -------------------
    begin                : Jan 16, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <fstream>
#include <iomanip>
#include <TNL/Assert.h>
#include <TNL/Meshes/GridDetails/GnuplotWriter.h>
#include <TNL/Meshes/GridDetails/GridEntityGetter_impl.h>
#include <TNL/Meshes/GridDetails/NeighbourGridEntityGetter2D_impl.h>
#include <TNL/Meshes/GridDetails/GridEntityMeasureGetter.h>

namespace TNL {
namespace Meshes {

template< typename Real,
          typename Device,
          typename Index >
Grid< 2, Real, Device, Index > :: Grid()
: numberOfCells( 0 ),
  numberOfNxFaces( 0 ),
  numberOfNyFaces( 0 ),
  numberOfFaces( 0 ),
  numberOfVertices( 0 )
{
}

template< typename Real,
          typename Device,
          typename Index >
String Grid< 2, Real, Device, Index > :: getType()
{
   return String( "Meshes::Grid< " ) +
          String( getMeshDimensions() ) + ", " +
          String( TNL::getType< RealType >() ) + ", " +
          String( Device :: getDeviceType() ) + ", " +
          String( TNL::getType< IndexType >() ) + " >";
}

template< typename Real,
           typename Device,
           typename Index >
String Grid< 2, Real, Device, Index > :: getTypeVirtual() const
{
   return this->getType();
}

template< typename Real,
          typename Device,
          typename Index >
String Grid< 2, Real, Device, Index > :: getSerializationType()
{
   return HostType::getType();
};

template< typename Real,
          typename Device,
          typename Index >
String Grid< 2, Real, Device, Index > :: getSerializationTypeVirtual() const
{
   return this->getSerializationType();
};

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__
void Grid< 2, Real, Device, Index > :: computeSpaceSteps()
{
   if( this->getDimensions().x() > 0 && this->getDimensions().y() > 0 )
   {
      this->spaceSteps.x() = this->proportions.x() / ( Real ) this->getDimensions().x();
      this->spaceSteps.y() = this->proportions.y() / ( Real ) this->getDimensions().y();
      const RealType& hx = this->spaceSteps.x();
      const RealType& hy = this->spaceSteps.y();
 
      Real auxX, auxY;
      for( int i = 0; i < 5; i++ )
      {
         switch( i )
         {
            case 0:
               auxX = 1.0 / ( hx * hx );
               break;
            case 1:
               auxX = 1.0 / hx;
               break;
            case 2:
               auxX = 1.0;
               break;
            case 3:
               auxX = hx;
               break;
            case 4:
               auxX = hx * hx;
               break;
         }
         for( int j = 0; j < 5; j++ )
         {
            switch( j )
            {
               case 0:
                  auxY = 1.0 / ( hy * hy );
                  break;
               case 1:
                  auxY = 1.0 / hy;
                  break;
               case 2:
                  auxY = 1.0;
                  break;
               case 3:
                  auxY = hy;
                  break;
               case 4:
                  auxY = hy * hy;
                  break;
            }
            this->spaceStepsProducts[ i ][ j ] = auxX * auxY;
         }
      }
   }
}

template< typename Real,
          typename Device,
          typename Index >
void Grid< 2, Real, Device, Index > :: setDimensions( const Index xSize, const Index ySize )
{
   Assert( xSize > 0, std::cerr << "xSize = " << xSize );
   Assert( ySize > 0, std::cerr << "ySize = " << ySize );

   this->dimensions.x() = xSize;
   this->dimensions.y() = ySize;
   this->numberOfCells = xSize * ySize;
   this->numberOfNxFaces = ySize * ( xSize + 1 );
   this->numberOfNyFaces = xSize * ( ySize + 1 );
   this->numberOfFaces = this->numberOfNxFaces + this->numberOfNyFaces;
   this->numberOfVertices = ( xSize + 1 ) * ( ySize + 1 );
   computeSpaceSteps();
}

template< typename Real,
          typename Device,
          typename Index >
void Grid< 2, Real, Device, Index > :: setDimensions( const CoordinatesType& dimensions )
{
   return this->setDimensions( dimensions. x(), dimensions. y() );
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__ inline
const typename Grid< 2, Real, Device, Index >::CoordinatesType&
Grid< 2, Real, Device, Index > :: getDimensions() const
{
   return this->dimensions;
}

template< typename Real,
          typename Device,
          typename Index >
void Grid< 2, Real, Device, Index > :: setDomain( const VertexType& origin,
                                                     const VertexType& proportions )
{
   this->origin = origin;
   this->proportions = proportions;
   computeSpaceSteps();
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__ inline
const typename Grid< 2, Real, Device, Index >::VertexType&
Grid< 2, Real, Device, Index >::getOrigin() const
{
   return this->origin;
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__ inline
const typename Grid< 2, Real, Device, Index > :: VertexType&
   Grid< 2, Real, Device, Index > :: getProportions() const
{
   return this->proportions;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename EntityType >
__cuda_callable__ inline
Index
Grid< 2, Real, Device, Index >::
getEntitiesCount() const
{
   static_assert( EntityType::entityDimensions <= 2 &&
                  EntityType::entityDimensions >= 0, "Wrong grid entity dimensions." );
 
   switch( EntityType::entityDimensions )
   {
      case 2:
         return this->numberOfCells;
      case 1:
         return this->numberOfFaces;
      case 0:
         return this->numberOfVertices;
   }
   return -1;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename EntityType >
__cuda_callable__ inline
EntityType
Grid< 2, Real, Device, Index >::
getEntity( const IndexType& entityIndex ) const
{
   static_assert( EntityType::entityDimensions <= 2 &&
                  EntityType::entityDimensions >= 0, "Wrong grid entity dimensions." );
 
   return GridEntityGetter< ThisType, EntityType >::getEntity( *this, entityIndex );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename EntityType >
__cuda_callable__ inline
Index
Grid< 2, Real, Device, Index >::
getEntityIndex( const EntityType& entity ) const
{
   static_assert( EntityType::entityDimensions <= 2 &&
                  EntityType::entityDimensions >= 0, "Wrong grid entity dimensions." );
 
   return GridEntityGetter< ThisType, EntityType >::getEntityIndex( *this, entity );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename EntityType >
__cuda_callable__
Real
Grid< 2, Real, Device, Index >::
getEntityMeasure( const EntityType& entity ) const
{
   return GridEntityMeasureGetter< ThisType, EntityType::getDimensions() >::getMeasure( *this, entity );
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__
Real
Grid< 2, Real, Device, Index >::
getCellMeasure() const
{
   return this->template getSpaceStepsProducts< 1, 1 >();
}


template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__ inline
typename Grid< 2, Real, Device, Index >::VertexType
Grid< 2, Real, Device, Index >::
getSpaceSteps() const
{
   return this->spaceSteps;
}

template< typename Real,
          typename Device,
          typename Index >
   template< int xPow, int yPow  >
__cuda_callable__ inline
const Real&
Grid< 2, Real, Device, Index >::
getSpaceStepsProducts() const
{
   Assert( xPow >= -2 && xPow <= 2,
              std::cerr << " xPow = " << xPow );
   Assert( yPow >= -2 && yPow <= 2,
              std::cerr << " yPow = " << yPow );

   return this->spaceStepsProducts[ yPow + 2 ][ xPow + 2 ];
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__ inline
Real Grid< 2, Real, Device, Index > :: getSmallestSpaceStep() const
{
   return min( this->spaceSteps.x(), this->spaceSteps.y() );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         Grid< 2, Real, Device, Index >::getAbsMax( const GridFunction& f ) const
{
   return f.absMax();
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         Grid< 2, Real, Device, Index >::getLpNorm( const GridFunction& f1,
                                                                 const typename GridFunction::RealType& p ) const
{
   typename GridFunction::RealType lpNorm( 0.0 );
   Cell cell( *this );
   for( cell.getCoordinates().y() = 0;
        cell.getCoordinates().y() < getDimensions().y();
        cell.getCoordinates().y()++ )
      for( cell.getCoordinates().x() = 0;
           cell.getCoordinates().x() < getDimensions().x();
           cell.getCoordinates().x()++ )
      {
         IndexType c = this->getEntityIndex( cell );
         lpNorm += ::pow( abs( f1[ c ] ), p );
      }
   lpNorm *= this->getSpaceSteps().x() * this->getSpaceSteps().y();
   return ::pow( lpNorm, 1.0/p );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         Grid< 2, Real, Device, Index >::getDifferenceAbsMax( const GridFunction& f1,
                                                                 const GridFunction& f2 ) const
{
   return f1.differenceAbsMax( f2 );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         Grid< 2, Real, Device, Index >::getDifferenceLpNorm( const GridFunction& f1,
                                                                 const GridFunction& f2,
                                                                 const typename GridFunction::RealType& p ) const
{
   typename GridFunction::RealType lpNorm( 0.0 );
   Cell cell( *this );
   for( cell.getCoordinates().y() = 0;
        cell.getCoordinates().y() < getDimensions().y();
        cell.getCoordinates().y()++ )
      for( cell.getCoordinates().x() = 0;
           cell.getCoordinates().x() < getDimensions().x();
           cell.getCoordinates().x()++ )
      {
         IndexType c = this->getEntityIndex( cell );
         lpNorm += ::pow( abs( f1[ c ] - f2[ c ] ), p );
      }
   lpNorm *= this->getSpaceSteps().x() * this->getSpaceSteps().y();
   return ::pow( lpNorm, 1.0 / p );
}

template< typename Real,
          typename Device,
          typename Index >
bool Grid< 2, Real, Device, Index > :: save( File& file ) const
{
   if( ! Object::save( file ) )
      return false;
   if( ! this->origin.save( file ) ||
       ! this->proportions.save( file ) ||
       ! this->dimensions.save( file ) )
   {
      std::cerr << "I was not able to save the domain description of a Grid." << std::endl;
      return false;
   }
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool Grid< 2, Real, Device, Index > :: load( File& file )
{
   if( ! Object::load( file ) )
      return false;
   CoordinatesType dimensions;
   if( ! this->origin.load( file ) ||
       ! this->proportions.load( file ) ||
       ! dimensions.load( file ) )
   {
      std::cerr << "I was not able to load the domain description of a Grid." << std::endl;
      return false;
   }
   this->setDimensions( dimensions );
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool Grid< 2, Real, Device, Index > :: save( const String& fileName ) const
{
   return Object :: save( fileName );
};

template< typename Real,
           typename Device,
           typename Index >
bool Grid< 2, Real, Device, Index > :: load( const String& fileName )
{
   return Object :: load( fileName );
};

template< typename Real,
          typename Device,
          typename Index >
bool Grid< 2, Real, Device, Index > :: writeMesh( const String& fileName,
                                                     const String& format ) const
{
   std::fstream file;
   file. open( fileName. getString(), std::ios::out );
   if( ! file )
   {
      std::cerr << "I am not able to open the file " << fileName << "." << std::endl;
      return false;
   }
   if( format == "asymptote" )
   {
      file << "size( "
           << this->getProportions(). x() << "cm , "
           << this->getProportions(). y() << "cm );"
           << std::endl << std::endl;
      MeshEntity< 0 > vertex( *this );
      CoordinatesType& vertexCoordinates = vertex.getCoordinates();
      VertexType v;
      for( Index j = 0; j < this->dimensions. y(); j ++ )
      {
         file << "draw( ";
         vertexCoordinates.x() = 0;
         vertexCoordinates.y() = j;
         v = vertex.getCenter();
         file << "( " << v. x() << ", " << v. y() << " )";
         for( Index i = 0; i < this->dimensions. x(); i ++ )
         {
            vertexCoordinates.x() = i + 1;
            vertexCoordinates.y() = j;
            v = vertex.getCenter();
            file << "--( " << v. x() << ", " << v. y() << " )";
         }
         file << " );" << std::endl;
      }
      file << std::endl;
      for( Index i = 0; i < this->dimensions. x(); i ++ )
      {
         file << "draw( ";
         vertexCoordinates.x() = i;
         vertexCoordinates.y() = 0;
         v = vertex.getCenter();
         file << "( " << v. x() << ", " << v. y() << " )";
         for( Index j = 0; j < this->dimensions. y(); j ++ )
         {
            vertexCoordinates.x() = i;
            vertexCoordinates.y() = j + 1;
            v = vertex.getCenter();
            file << "--( " << v. x() << ", " << v. y() << " )";
         }
         file << " );" << std::endl;
      }
      file << std::endl;

      MeshEntity< 2 > cell( *this );
      CoordinatesType& cellCoordinates = cell.getCoordinates();
      const RealType cellMeasure = this->getSpaceSteps().x() * this->getSpaceSteps().y();
      for( Index i = 0; i < this->dimensions. x(); i ++ )
         for( Index j = 0; j < this->dimensions. y(); j ++ )
         {
            cellCoordinates.x() = i;
            cellCoordinates.y() = j;
            v = vertex.getCenter();
            file << "label( scale(0.33) * Label( \"$" << std::setprecision( 3 ) << cellMeasure << std::setprecision( 8 )
                 << "$\" ), ( " << v. x() << ", " << v. y() << " ), S );" << std::endl;
         }

      for( Index i = 0; i < this->dimensions. x(); i ++ )
         for( Index j = 0; j < this->dimensions. y(); j ++ )
         {
            VertexType v1, v2, c;

            /****
             * East edge normal
             */
            /*v1 = this->getVertex( CoordinatesType( i + 1, j ), v1 );
            v2 = this->getVertex( CoordinatesType( i + 1, j + 1 ), v2 );
            c = ( ( Real ) 0.5 ) * ( v1 + v2 );
            this->getEdgeNormal< 1, 0 >( CoordinatesType( i, j ), v );
            v *= 0.5;
            file << "draw( ( " << c. x() << ", " << c. y() << " )--( "
                 << c. x() + v. x() << ", " << c.y() + v. y() << " ), Arrow(size=1mm),p=green);" << std::endl;
            */
            /****
             * West edge normal
             */
            /*this->getVertex< -1, -1 >( CoordinatesType( i, j ), v1 );
            this->getVertex< -1, 1 >( CoordinatesType( i, j ), v2 );
            c = ( ( Real ) 0.5 ) * ( v1 + v2 );
            this->getEdgeNormal< -1, 0 >( CoordinatesType( i, j ), v );
            v *= 0.5;
            file << "draw( ( " << c. x() << ", " << c. y() << " )--( "
                 << c. x() + v. x() << ", " << c.y() + v. y() << " ), Arrow(size=1mm),p=blue);" << std::endl;
            */
            /****
             * North edge normal
             */
            /*this->getVertex< 1, 1 >( CoordinatesType( i, j ), v1 );
            this->getVertex< -1, 1 >( CoordinatesType( i, j ), v2 );
            c = ( ( Real ) 0.5 ) * ( v1 + v2 );
            this->getEdgeNormal< 0, 1 >( CoordinatesType( i, j ), v );
            v *= 0.5;
            file << "draw( ( " << c. x() << ", " << c. y() << " )--( "
                 << c. x() + v. x() << ", " << c.y() + v. y() << " ), Arrow(size=1mm),p=green);" << std::endl;
            */
            /****
             * South edge normal
             */
            /*this->getVertex< 1, -1 >( CoordinatesType( i, j ), v1 );
            this->getVertex< -1, -1 >( CoordinatesType( i, j ), v2 );
            c = ( ( Real ) 0.5 ) * ( v1 + v2 );
            this->getEdgeNormal< 0, -1 >( CoordinatesType( i, j ), v );
            v *= 0.5;
            file << "draw( ( " << c. x() << ", " << c. y() << " )--( "
                 << c. x() + v. x() << ", " << c.y() + v. y() << " ), Arrow(size=1mm),p=blue);" << std::endl;
            */
         }
      return true;
   }
   return false;
}

template< typename Real,
           typename Device,
           typename Index >
   template< typename MeshFunction >
bool Grid< 2, Real, Device, Index > :: write( const MeshFunction& function,
                                                 const String& fileName,
                                                 const String& format ) const
{
   if( this->template getEntitiesCount< Cell >() != function. getSize() )
   {
      std::cerr << "The size ( " << function. getSize()
           << " ) of a mesh function does not agree with the DOFs ( "
           << this->template getEntitiesCount< Cell >() << " ) of a mesh." << std::endl;
      return false;
   }
   std::fstream file;
   file. open( fileName. getString(), std::ios::out );
   if( ! file )
   {
      std::cerr << "I am not able to open the file " << fileName << "." << std::endl;
      return false;
   }
   file << std::setprecision( 12 );
   if( format == "gnuplot" )
   {
      Cell cell( *this );
      CoordinatesType& cellCoordinates = cell.getCoordinates();
      for( cellCoordinates.y() = 0; cellCoordinates.y() < getDimensions(). y(); cellCoordinates.y() ++ )
      {
         for( cellCoordinates.x() = 0; cellCoordinates.x() < getDimensions(). x(); cellCoordinates.x() ++ )
         {
            VertexType v = cell.getCenter();
            GnuplotWriter::write( file,  v );
            GnuplotWriter::write( file,  function[ this->getEntityIndex( cell ) ] );
            file << std::endl;
         }
         file << std::endl;
      }
   }
   file. close();
   return true;
}

template< typename Real,
           typename Device,
           typename Index >
void
Grid< 2, Real, Device, Index >::
writeProlog( Logger& logger )
{
   logger.writeParameter( "Dimensions:", getMeshDimensions() );
   logger.writeParameter( "Domain origin:", this->origin );
   logger.writeParameter( "Domain proportions:", this->proportions );
   logger.writeParameter( "Domain dimensions:", this->dimensions );
   logger.writeParameter( "Space steps:", this->getSpaceSteps() );
}

} // namespace Meshes
} // namespace TNL