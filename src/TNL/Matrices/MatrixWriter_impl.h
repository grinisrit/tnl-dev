/***************************************************************************
                          MatrixWriter_impl.h  -  description
                             -------------------
    begin                : Dec 18, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Matrices/MatrixWriter.h>

namespace TNL {
namespace Matrices {   

template< typename Matrix >
bool MatrixWriter< Matrix >::writeToGnuplot( std::ostream& str,
                                             const Matrix& matrix,
                                             bool verbose )
{
   for( IndexType row = 0; row < matrix.getRows(); row ++ )
   {
      for( IndexType column = 0; column < matrix.getColumns(); column ++ )
      {
         RealType elementValue = matrix.getElement( row, column );
         if(  elementValue != ( RealType ) 0.0 )
            str << column << " " << row << " " << elementValue << "\n";
      }
      if( verbose )
        std::cout << "Drawing the row " << row << "      \r" << std::flush;
   }
   if( verbose )
     std::cout << std::endl;
   return true;
}

template< typename Matrix >
bool MatrixWriter< Matrix >::writeToEps( std::ostream& str,
                                         const Matrix& matrix,
                                         bool verbose )
{
   const int elementSize = 10;
   if( ! writeEpsHeader( str, matrix, elementSize ) )
      return false;
   if( !writeEpsBody( str, matrix, elementSize, verbose ) )
      return false;

   str << "showpage" << std::endl;
   str << "%%EOF" << std::endl;

   if( verbose )
     std::cout << std::endl;
   return true;
}

template< typename Matrix >
bool MatrixWriter< Matrix >::writeEpsHeader( std::ostream& str,
                                             const Matrix& matrix,
                                             const int elementSize )
{
   const double scale = elementSize * max( matrix.getRows(), matrix.getColumns() );
   str << "%!PS-Adobe-2.0 EPSF-2.0" << std::endl;
   str << "%%BoundingBox: 0 0 " << scale << " " << scale << std::endl;
   str << "%%Creator: TNL" << std::endl;
   str << "%%LanguageLevel: 2" << std::endl;
   str << "%%EndComments" << std::endl << std::endl;
   str << "0 " << scale << " translate" << std::endl;
   return true;
}

template< typename Matrix >
bool MatrixWriter< Matrix >::writeEpsBody( std::ostream& str,
                                           const Matrix& matrix,
                                           const int elementSize,
                                           bool verbose )
{
   IndexType lastRow( 0 ), lastColumn( 0 );
   for( IndexType row = 0; row < matrix.getRows(); row ++ )
   {
      for( IndexType column = 0; column < matrix.getColumns(); column ++ )
      {
         RealType elementValue = getElement( row, column );
         if( elementValue != ( RealType ) 0.0 )
         {
            str << ( column - lastColumn ) * elementSize
                << " " << -( row - lastRow ) * elementSize
                << " translate newpath 0 0 " << elementSize << " " << elementSize << " rectstroke\n";
            lastColumn = column;
            lastRow = row;
         }
      }
      if( verbose )
        std::cout << "Drawing the row " << row << "      \r" << std::flush;
   }
   return true;
}

} // namespace Matrices
} // namespace TNL
