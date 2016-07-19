/***************************************************************************
                          tnl-dicom-reader.cpp  -  description
                             -------------------
    begin                : Jul 19, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <tnlConfig.h>
#include <config/tnlConfigDescription.h>
#include <config/tnlParameterContainer.h>
#include <core/images/tnlDicomSeries.h>
#include <core/mfilename.h>

using namespace TNL;

void setupConfig( tnlConfigDescription& config )
{
   config.addDelimiter( "General parameters" );
   config.addList         < tnlString >( "dicom-files",   "Input DICOM files." );
   config.addList         < tnlString >( "dicom-series",   "Input DICOM series." );
   config.addEntry        < tnlString >( "mesh-file",     "Mesh file.", "mesh.tnl" );
   config.addEntry        < bool >     ( "one-mesh-file", "Generate only one mesh file. All the images dimensions must be the same.", true );
   config.addEntry        < int >      ( "roi-top",       "Top (smaller number) line of the region of interest.", -1 );
   config.addEntry        < int >      ( "roi-bottom",    "Bottom (larger number) line of the region of interest.", -1 );
   config.addEntry        < int >      ( "roi-left",      "Left (smaller number) column of the region of interest.", -1 );
   config.addEntry        < int >      ( "roi-right",     "Right (larger number) column of the region of interest.", -1 );
   config.addEntry        < bool >     ( "verbose",       "Set the verbosity of the program.", true );
}

#ifdef HAVE_DCMTK_H
bool processDicomFiles( const tnlParameterContainer& parameters )
{
 
}

bool processDicomSeries( const tnlParameterContainer& parameters )
{
   const tnlList< tnlString >& dicomSeriesNames = parameters.getParameter< tnlList< tnlString > >( "dicom-series" );
   tnlString meshFile = parameters.getParameter< tnlString >( "mesh-file" );
   bool verbose = parameters.getParameter< bool >( "verbose" );

   typedef tnlGrid< 2, double, tnlHost, int > GridType;
   GridType grid;
   tnlVector< double, tnlHost, int > vector;
   tnlRegionOfInterest< int > roi;
   for( int i = 0; i < dicomSeriesNames.getSize(); i++ )
   {
      const tnlString& seriesName = dicomSeriesNames[ i ];
      std::cout << "Reading a file " << seriesName << std::endl;
      tnlDicomSeries dicomSeries( seriesName.getString() );
      if( !dicomSeries.isDicomSeriesLoaded() )
      {
         std::cerr << "Loading of the DICOM series " << seriesName << " failed." << std::endl;
      }
      if( i == 0 )
      {
         if( ! roi.setup( parameters, &dicomSeries ) )
            return false;
         roi.setGrid( grid, verbose );
         vector.setSize( grid.template getEntitiesCount< typename GridType::Cell >() );
         std::cout << "Writing grid to file " << meshFile << std::endl;
         grid.save( meshFile );
      }
      std::cout << "The series consists of " << dicomSeries.getImagesCount() << " images." << std::endl;
      for( int imageIdx = 0; imageIdx < dicomSeries.getImagesCount(); imageIdx++ )
      {
         dicomSeries.getImage( imageIdx, grid, roi, vector );
         tnlString fileName;
         FileNameBaseNumberEnding( seriesName.getString(), imageIdx, 2, ".tnl", fileName );
         std::cout << "Writing file " << fileName << " ... " << std::endl;
         vector.save( fileName );
      }
   }
}
#endif

int main( int argc, char* argv[] )
{
   tnlParameterContainer parameters;
   tnlConfigDescription configDescription;
   setupConfig( configDescription );
   if( ! parseCommandLine( argc, argv, configDescription, parameters ) )
   {
      configDescription.printUsage( argv[ 0 ] );
      return EXIT_FAILURE;
   }
   if( ! parameters.checkParameter( "dicom-files" ) &&
       ! parameters.checkParameter( "dicom-series") )
   {
       std::cerr << "Neither DICOM series nor DICOM files are given." << std::endl;
       configDescription.printUsage( argv[ 0 ] );
       return EXIT_FAILURE;
   }
#ifdef HAVE_DCMTK_H
   if( parameters.checkParameter( "dicom-files" ) && ! processDicomFiles( parameters ) )
      return EXIT_FAILURE;
   if( parameters.checkParameter( "dicom-series" ) && ! processDicomSeries( parameters ) )
      return EXIT_FAILURE;
   return EXIT_SUCCESS;
#else
   std::cerr << "TNL was not compiled with DCMTK support." << std::endl;
#endif
}
