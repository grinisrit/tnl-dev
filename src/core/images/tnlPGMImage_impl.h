/***************************************************************************
                          tnlPGMImage_impl.h  -  description
                             -------------------
    begin                : Jul 20, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
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

#ifndef TNLPGMIMAGE_IMPL_H
#define	TNLPGMIMAGE_IMPL_H

#include <cstring>
#include <core/images/tnlPGMImage.h>

template< typename Index >
tnlPGMImage< Index >::
tnlPGMImage() : 
   binary( false ), maxColors( 0 ), fileOpen( false )
{
}

template< typename Index >
bool
tnlPGMImage< Index >::
readHeader()
{
   char magicNumber[ 3 ];
   this -> file >> magicNumber;
   if( this -> file.fail() )
   {
      cerr << "Unable to read the magic number." << endl;
      return false;
   }

   if( strcmp( magicNumber, "P5" ) != 0 &&
       strcmp( magicNumber, "P2" ) != 0 )
      return false;
   
   if( strcmp( magicNumber, "P5" ) == 0 )
      this->binary = true;

   char character;
   this -> file.get(character);
   while ( ! this -> file.eof() and ( character == ' ' || character == '\t' || character == '\r' || character == '\n') )
   {
	this -> file.get(character);
	if ( character == '#' )
		while (! this -> file.eof() && ( character != '\n' ) )
			this -> file.get( character );
   }
   this -> file.unget();
   
   this -> file >> this -> width >> this -> height >> this -> maxColors;
   return true;   
}

template< typename Index >
bool
tnlPGMImage< Index >::
openForRead( const tnlString& fileName )
{
   this->close();
   if ( this -> binary )
   	this->file.open( fileName.getString(), fstream::in | fstream::binary);
   else 
	this->file.open( fileName.getString(), fstream::in );
   if( ! this->file )
   {
      cerr << "Unable to open the file " << fileName << endl;
      return false;
   }
   this->fileOpen = true;
   if( ! readHeader() )
      return false;
   return true;
}

template< typename Index >
   template< typename Real,
             typename Device,
             typename Vector >
bool
tnlPGMImage< Index >::
read( const tnlRegionOfInterest< Index > roi,
      const tnlGrid< 2, Real, Device, Index >& grid,
      Vector& vector )
{
   typedef tnlGrid< 2, Real, Device, Index > GridType;
   typedef typename GridType::CoordinatesType CoordinatesType;
   
   Index i, j;
   for( i = 0; i < this->height; i ++ )
      for( j = 0; j < this->width; j ++ )
      {
         int col;
	 unsigned char col_aux;
         if( this->binary ) 
	 {
		this -> file >> col_aux;
		col = (int)col_aux;
	 }
         else this -> file >> col;
         if( roi.isIn( i, j ) )
         {
            Index cellIndex = grid.getCellIndex( CoordinatesType( j - roi.getLeft(),
                                                                  roi.getBottom() - 1 - i ) );
            vector.setElement( cellIndex, ( Real ) col / ( Real ) this->maxColors );
         }
      }
   return true;
}

template< typename Index >
   template< typename Real,
             typename Device >
bool
tnlPGMImage< Index >::
writeHeader( const tnlGrid< 2, Real, Device, Index >& grid,
             bool binary )
{
   if( binary )
      this->file << "P5\n";
   else
      this->file << "P2\n";
   this->file << "# This file was generated by TNL (tnl-image-converter)\n";
   this->file << grid.getDimensions().x() << ' '<< grid.getDimensions().y() << '\n' << "255\n";
   return true;   
}

template< typename Index >
   template< typename Real,
             typename Device >
bool
tnlPGMImage< Index >::
openForWrite( const tnlString& fileName,
              tnlGrid< 2, Real, Device, Index >& grid,
              bool binary )
{
   this->close();
   if( binary )
        this->file.open( fileName.getString(), fstream::out | fstream::binary);
   else 
        this->file.open( fileName.getString(), fstream::out);
   if( this->file.fail() )
   {
      cerr << "Unable to open the file " << fileName << endl;
      return false;
   }
   this->fileOpen = true;
   if( ! writeHeader( grid, binary ) )
      return false;
   this->binary = binary;
   return true;
}

template< typename Index >
   template< typename Real,
             typename Device,
             typename Vector >
bool
tnlPGMImage< Index >::
write( const tnlGrid< 2, Real, Device, Index >& grid,
       Vector& vector )
{
   typedef tnlGrid< 2, Real, Device, Index > GridType;
   typedef typename GridType::CoordinatesType CoordinatesType;
   
   Index i, j;
   for( i = 0; i < grid.getDimensions().y(); i ++ )
   {
      for( j = 0; j < grid.getDimensions().x(); j ++ )
      {
         Index cellIndex = grid.getCellIndex( CoordinatesType( j,
                                              grid.getDimensions().y() - 1 - i ) );
         unsigned char color = 255 * vector.getElement( cellIndex );
         if ( ! this -> binary )
	 {
	     int color_aux = (int)color;
	     this->file << color_aux;
             this->file << ' ';
	 }
	 else this->file << color;
      }
      
      if ( ! this -> binary )
        this->file << '\n';
   }
   return true;
}

template< typename Index >
void
tnlPGMImage< Index >::
close()
{
   if( this->fileOpen )
      this->file.close();
   this->fileOpen = false;
}

template< typename Index >
tnlPGMImage< Index >::
~tnlPGMImage()
{
   close();
}

#endif	/* TNLPGMIMAGE_IMPL_H */

