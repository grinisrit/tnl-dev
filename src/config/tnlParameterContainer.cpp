/***************************************************************************
                          tnlParameterContainer.cpp  -  description
                             -------------------
    begin                : 2007/06/15
    copyright            : (C) 2007 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <ctype.h>
#include <cstring>
#include <stdio.h>

#include "tnlParameterContainer.h"
#include <tnlObject.h>

namespace TNL {

bool matob( const char* value, bool& ret_val )
{
   if( strcasecmp( value, "yes" ) == 0 ||
       strcasecmp( value, "true" ) == 0  )
   {
      ret_val =  true;
      return true;
   }
   if( strcasecmp( value, "no" ) == 0 ||
       strcasecmp( value, "false" ) == 0  )
   {
      ret_val = false;
      return true;
   }
   return false;
}

tnlParameterContainer::
tnlParameterContainer()
{
}

bool
tnlParameterContainer::
addParameter( const tnlString& name,
              const tnlString& value )
{
   return parameters. Append( new tnlParameter< tnlString >( name, TNL::getType< tnlString >().getString(), tnlString( value ) ) );
}

bool
tnlParameterContainer::
setParameter( const tnlString& name,
              const tnlString& value )
{
   int i;
   for( i = 0; i < parameters. getSize(); i ++ )
   {
      if( parameters[ i ] -> name == name )
      {
         if( parameters[ i ] -> type == TNL::getType< tnlString >() )
         {
            ( ( tnlParameter< tnlString > * ) parameters[ i ] )->value = value;
            return true;
         }
         else
         {
            std::cerr << "Parameter " << name << " already exists with different type "
                 << parameters[ i ] -> type << " not "
                 << TNL::getType< tnlString>() << std::endl;
            abort();
            return false;
         }
      }
   }
   return addParameter( name, value );
};

bool
tnlParameterContainer::
checkParameter( const tnlString& name ) const
{
   int i;
   const int parameters_num = parameters. getSize();
   for( i = 0; i < parameters_num; i ++ )
      if( parameters[ i ] -> name == name ) return true;
   return false;
}

tnlParameterContainer::
~tnlParameterContainer()
{
   parameters. DeepEraseAll();
}

void tnlParameterContainer :: MPIBcast( int root, MPI_Comm mpi_comm )
{
#ifdef HAVE_MPI
   int i;
   int size = parameters. getSize();
   :: MPIBcast( size, 1, root, mpi_comm );
   for( i = 0; i < size; i ++ )
   {
      if( MPIGetRank() == root )
      {
         tnlParameterBase* param = parameters[ i ];
         param -> type. MPIBcast( root, MPI_COMM_WORLD );
         param -> name. MPIBcast( root, MPI_COMM_WORLD );
         if( param -> type == "mString" )
         {
            ( ( tnlParameter< tnlString >* ) param ) -> value. MPIBcast( root, mpi_comm );
         }
         if( param -> type == "bool" )
         {
            :: MPIBcast( ( ( tnlParameter< bool >* ) param ) -> value, 1, root, mpi_comm );
         }
         if( param -> type == "int" )
         {
            :: MPIBcast( ( ( tnlParameter< int >* ) param ) -> value, 1, root, mpi_comm );
         }
         if( param -> type == "double" )
         {
            :: MPIBcast( ( ( tnlParameter< double >* ) param ) -> value, 1, root, mpi_comm );
         }
      }
      else
      {
         tnlString param_type, param_name;
         param_type. MPIBcast( root, MPI_COMM_WORLD );
         param_name. MPIBcast( root, MPI_COMM_WORLD );
         if( param_type == "mString" )
         {
            tnlString val;
            val. MPIBcast( root, mpi_comm );
            addParameter< tnlString >( param_name. getString(),
                                     val );
         }
         if( param_type == "bool" )
         {
            bool val;
            :: MPIBcast( val, 1, root, mpi_comm );
            addParameter< bool >( param_name. getString(),
                                  val );
         }
         if( param_type == "int" )
         {
            int val;
            :: MPIBcast( val, 1, root, mpi_comm );
            addParameter< int >( param_name. getString(),
                                 val );
         }
         if( param_type == "double" )
         {
            double val;
            :: MPIBcast( val, 1, root, mpi_comm );
            addParameter< double >( param_name. getString(),
                                    val );
         }

      }
   }
#endif
}

bool
parseCommandLine( int argc, char* argv[],
                  const tnlConfigDescription& config_description,
                  tnlParameterContainer& parameters,
                  bool printUsage )
{
   int i;
   bool parse_error( false );
   for( i = 1; i < argc; i ++ )
   {
      const char* _option = argv[ i ];
      if( _option[ 0 ] != '-' )
      {
         std::cerr << "Unknown option " << _option << ". Options must have prefix '--' or '-'." << std::endl;
         parse_error = true;
         continue;
      }
      if( strcmp( _option, "--help" ) == 0 )
      {
          config_description.printUsage( argv[ 0 ] );
          return true;
      }
      const char* option = _option + 2;
      const tnlConfigEntryBase* entry;
      if( ( entry = config_description.getEntry( option ) ) == NULL )
      {
         std::cerr << "Unknown parameter " << _option << "." << std::endl;
         parse_error = true;
      }
      else
      {
         const tnlString& entryType = entry->getEntryType();
         const char* value = argv[ ++ i ];
         if( ! value )
         {
            std::cerr << "Missing value for the parameter " << option << "." << std::endl;
            return false;
         }
         tnlList< tnlString > parsedEntryType;
         if( ! parseObjectType( entryType, parsedEntryType ) )
         {
            std::cerr << "Internal error: Uknown config entry type " << entryType << "." << std::endl;
            return false;
         }
         if( parsedEntryType[ 0 ] == "tnlList" )
         {
            tnlList< tnlString >* string_list( 0 );
            tnlList< bool >* bool_list( 0 );
            tnlList< int >* integer_list( 0 );
            tnlList< double >* real_list( 0 );

            if( parsedEntryType[ 1 ] == "tnlString" )
               string_list = new tnlList< tnlString >;
            if( parsedEntryType[ 1 ] == "bool" )
               bool_list = new tnlList< bool >;
            if( parsedEntryType[ 1 ] == "int" )
               integer_list = new tnlList< int >;
            if( parsedEntryType[ 1 ] == "double" )
               real_list = new tnlList< double >;
 
            while( i < argc && ( ( argv[ i ] )[ 0 ] != '-' || ( atof( argv[ i ] ) < 0.0 && ( integer_list || real_list ) ) ) )
            {
               const char* value = argv[ i ++ ];
               if( string_list )
               {
                  /*if( ! ( ( tnlConfigEntry< tnlList< tnlString > >* )  entry )->checkValue( tnlString( value ) ) )
                  {
                     delete string_list;
                     return false;
                  }*/
                  string_list -> Append( tnlString( value ) );
               }
               if( bool_list )
               {
                  bool bool_val;
                  if( ! matob( value, bool_val ) )
                  {
                     std::cerr << "Yes/true or no/false is required for the parameter " << option << "." << std::endl;
                     parse_error = true;
                  }
                  else bool_list -> Append( bool_val );
               }
               if( integer_list )
               {
                  /*if( ! ( tnlConfigEntry< tnlList< int > >* ) entry->checkValue( atoi( value ) ) )
                  {
                     delete integer_list;
                     return false;
                  }*/
                  integer_list -> Append( atoi( value ) );
               }
               if( real_list )
               {
                  /*if( ! ( tnlConfigEntry< tnlList< double > >* ) entry->checkValue( atof( value ) ) )
                  {
                     delete real_list;
                     return false;
                  }*/
                  real_list -> Append( atof( value ) );
               }
            }
            if( string_list )
            {
               parameters. addParameter< tnlList< tnlString > >( option, *string_list );
               delete string_list;
            }
            if( bool_list )
            {
               parameters. addParameter< tnlList< bool > >( option, *bool_list );
               delete bool_list;
            }
            if( integer_list )
            {
               parameters. addParameter< tnlList< int > >( option, *integer_list );
               delete integer_list;
            }
            if( real_list )
            {
               parameters. addParameter< tnlList< double > >( option, *real_list );
               delete real_list;
            }
            if( i < argc ) i --;
            continue;
         }
         else
         {
            if( parsedEntryType[ 0 ] == "tnlString" )
            {
               if( ! ( ( tnlConfigEntry< tnlString >* ) entry )->checkValue( value ) )
                  return false;
                parameters. addParameter< tnlString >( option, value );
                continue;
            }
            if( parsedEntryType[ 0 ] == "bool" )
            {
               bool bool_val;
               if( ! matob( value, bool_val ) )
               {
                  std::cerr << "Yes/true or no/false is required for the parameter " << option << "." << std::endl;
                  parse_error = true;
               }
               else parameters. addParameter< bool >( option, bool_val );
               continue;
            }
            if( parsedEntryType[ 0 ] == "int" )
            {
               /*if( ! isdigit( value ) )
               {
                  std::cerr << "Integer constant is required for the parameter " << option << "." << std::endl;
                  parse_error = true;
                  continue;
               }*/
               if( ! ( ( tnlConfigEntry< int >* ) entry )->checkValue( atoi( value ) ) )
                  return false;
               parameters. addParameter< int >( option, atoi( value ) );
            }
            if( parsedEntryType[ 0 ] == "double" )
            {
               /*if( ! isdigit( value ) )
               {
                  std::cerr << "Real constant is required for the parameter " << option << "." << std::endl;
                  parse_error = true;
                  continue;
               }*/
               if( ! ( ( tnlConfigEntry< double >* ) entry )->checkValue( atof( value ) ) )
                  return false;
               parameters. addParameter< double >( option, atof( value ) );
            }
         }
      }
   }
   config_description.addMissingEntries( parameters );
   if( ! config_description.checkMissingEntries( parameters, printUsage, argv[ 0 ] ) )
      return false;
   return ! parse_error;
}

} // namespace TNL
