/***************************************************************************
                          MPI/Wrappers.h  -  description
                             -------------------
    begin                : Apr 23, 2005
    copyright            : (C) 2005 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <iostream>
#include <stdexcept>

#ifdef HAVE_MPI
   #include <mpi.h>
#else
   #include "DummyDefs.h"
   #include <cstring>  // std::memcpy
   #include <TNL/Exceptions/MPISupportMissing.h>
#endif

#include <TNL/Assert.h>
#include "getDataType.h"
#include "Profiling.h"

namespace TNL {
namespace MPI {

// forward declaration to break cyclic inclusion
inline void selectGPU();

// function wrappers for MPI constants

inline MPI_Comm AllGroup()
{
#ifdef HAVE_MPI
   return MPI_COMM_WORLD;
#else
   return 1;
#endif
}

inline MPI_Comm NullGroup()
{
#ifdef HAVE_MPI
   return MPI_COMM_NULL;
#else
   return 0;
#endif
}

inline MPI_Request NullRequest()
{
#ifdef HAVE_MPI
   return MPI_REQUEST_NULL;
#else
   return 0;
#endif
}

// wrappers for basic MPI functions

inline void Init( int& argc, char**& argv, int required_thread_level = MPI_THREAD_SINGLE )
{
#ifdef HAVE_MPI
   switch( required_thread_level ) {
      case MPI_THREAD_SINGLE:       // application is single-threaded
      case MPI_THREAD_FUNNELED:     // application is multithreaded, but all MPI calls will be issued from the master thread only
      case MPI_THREAD_SERIALIZED:   // application is multithreaded and any thread may issue MPI calls, but different threads will never issue MPI calls at the same time
      case MPI_THREAD_MULTIPLE:     // application is multithreaded and any thread may issue MPI calls at any time
         break;
      default:
         std::cerr << "ERROR: invalid argument for the 'required' thread level support: " << required_thread_level << std::endl;
         MPI_Abort(MPI_COMM_WORLD, 1);
   }

   int provided;
   MPI_Init_thread( &argc, &argv, required_thread_level, &provided );
   if( provided < required_thread_level ) {
      const char* level = "";
      switch( required_thread_level ) {
         case MPI_THREAD_SINGLE:
            level = "MPI_THREAD_SINGLE";
            break;
         case MPI_THREAD_FUNNELED:
            level = "MPI_THREAD_FUNNELED";
            break;
         case MPI_THREAD_SERIALIZED:
            level = "MPI_THREAD_SERIALIZED";
            break;
         case MPI_THREAD_MULTIPLE:
            level = "MPI_THREAD_MULTIPLE";
            break;
      }
      std::cerr << "ERROR: The MPI library does not have the required level of thread support: " << level << std::endl;
      MPI_Abort(MPI_COMM_WORLD, 1);
   }

   selectGPU();
#endif
}

inline void Finalize()
{
#ifdef HAVE_MPI
   MPI_Finalize();
#endif
}

inline bool Initialized()
{
#ifdef HAVE_MPI
    int flag;
    MPI_Initialized(&flag);
    return flag;
#else
    return true;
#endif
}

inline bool Finalized()
{
#ifdef HAVE_MPI
    int flag;
    MPI_Finalized(&flag);
    return flag;
#else
    return false;
#endif
}

inline int GetRank( MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "GetRank cannot be called with NullGroup" );
   int rank;
   MPI_Comm_rank( group, &rank );
   return rank;
#else
   return 0;
#endif
}

inline int GetSize( MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "GetSize cannot be called with NullGroup" );
   int size;
   MPI_Comm_size( group, &size );
   return size;
#else
   return 1;
#endif
}

// wrappers for MPI helper functions

inline MPI_Comm Comm_split( MPI_Comm comm, int color, int key )
{
#ifdef HAVE_MPI
   MPI_Comm newcomm;
   MPI_Comm_split( comm, color, key, &newcomm );
   return newcomm;
#else
   return comm;
#endif
}

/**
 * \brief Wrapper for \ref MPI_Dims_create.
 *
 * \param nproc - number of processes in the group to be distributed
 * \param ndims - number of dimensions of the Cartesian grid
 * \param dims - distribution of processes into the \e dim-dimensional
 *               Cartesian grid (array of length \e ndims)
 *
 * Negative input values of \e dims[i] are erroneous. An error will occur if
 * \e nproc is not a multiple of the product of all non-zero values \e dims[i].
 *
 * See the MPI documentation for more information.
 */
inline void Compute_dims( int nproc, int ndims, int* dims )
{
#ifdef HAVE_MPI
   int prod = 1;
   for( int i = 0; i < ndims; i++ ) {
      if( dims[ i ] < 0 )
         throw std::invalid_argument( "Negative value passed to MPI::Compute_dims in the dims array argument." );
      if( dims[ i ] > 0 )
         prod *= dims[ i ];
   }

   if( nproc % prod != 0 )
      throw std::logic_error( "The program tries to call MPI_Dims_create with wrong dimensions."
            "The product of the non-zero values dims[i] is " + std::to_string(prod) + " and the "
            "number of processes (" + std::to_string(nproc) + ") is not a multiple of the product." );

   MPI_Dims_create( nproc, ndims, dims );
#else
   for( int i = 0; i < ndims; i++)
      dims[ i ] = 1;
#endif
}

// wrappers for MPI communication functions

inline void Barrier( MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Barrier cannot be called with NullGroup" );
   MPI_Barrier(group);
#endif
}

inline void Waitall( MPI_Request* reqs, int length )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   MPI_Waitall( length, reqs, MPI_STATUSES_IGNORE );
#endif
}

template< typename T >
void Send( const T* data,
           int count,
           int dest,
           int tag,
           MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Send cannot be called with NullGroup" );
   MPI_Send( (const void*) data, count, getDataType<T>(), dest, tag, group );
#endif
}

template< typename T >
void Recv( T* data,
           int count,
           int src,
           int tag,
           MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Recv cannot be called with NullGroup" );
   MPI_Recv( (void*) data, count, getDataType<T>(), src, tag, group, MPI_STATUS_IGNORE );
#endif
}

template< typename T >
void Sendrecv( const T* sendData,
               int sendCount,
               int destination,
               int sendTag,
               T* receiveData,
               int receiveCount,
               int source,
               int receiveTag,
               MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Sendrecv cannot be called with NullGroup" );
   MPI_Sendrecv( (void*) sendData,
                 sendCount,
                 getDataType<T>(),
                 destination,
                 sendTag,
                 (void*) receiveData,
                 receiveCount,
                 getDataType<T>(),
                 source,
                 receiveTag,
                 group,
                 MPI_STATUS_IGNORE );
#else
   throw Exceptions::MPISupportMissing();
#endif
}

template< typename T >
MPI_Request Isend( const T* data,
                   int count,
                   int dest,
                   int tag,
                   MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Isend cannot be called with NullGroup" );
   MPI_Request req;
   MPI_Isend( (const void*) data, count, getDataType<T>(), dest, tag, group, &req );
   return req;
#else
   return NullRequest();
#endif
}

template< typename T >
MPI_Request Irecv( T* data,
                   int count,
                   int src,
                   int tag,
                   MPI_Comm group = AllGroup() )
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Irecv cannot be called with NullGroup" );
   MPI_Request req;
   MPI_Irecv( (void*) data, count, getDataType<T>(), src, tag, group, &req );
   return req;
#else
   return NullRequest();
#endif
}

template< typename T >
void Allreduce( const T* data,
                T* reduced_data,
                int count,
                const MPI_Op& op,
                MPI_Comm group)
{
#ifdef HAVE_MPI
   TNL_ASSERT_NE( group, NullGroup(), "Allreduce cannot be called with NullGroup" );
   getTimerAllreduce().start();
   MPI_Allreduce( (const void*) data, (void*) reduced_data, count, getDataType<T>(), op, group );
   getTimerAllreduce().stop();
#else
   std::memcpy( (void*) reduced_data, (const void*) data, count * sizeof(T) );
#endif
}

// in-place variant of Allreduce
template< typename T >
void Allreduce( T* data,
                int count,
                const MPI_Op& op,
                MPI_Comm group)
{
#ifdef HAVE_MPI
   TNL_ASSERT_NE( group, NullGroup(), "Allreduce cannot be called with NullGroup" );
   getTimerAllreduce().start();
   MPI_Allreduce( MPI_IN_PLACE, (void*) data, count, getDataType<T>(), op, group );
   getTimerAllreduce().stop();
#endif
}

template< typename T >
void Reduce( const T* data,
             T* reduced_data,
             int count,
             const MPI_Op& op,
             int root,
             MPI_Comm group)
{
#ifdef HAVE_MPI
   TNL_ASSERT_NE( group, NullGroup(), "Reduce cannot be called with NullGroup" );
   MPI_Reduce( (const void*) data, (void*) reduced_data, count, getDataType<T>(), op, root, group );
#else
   std::memcpy( (void*) reduced_data, (void*) data, count * sizeof(T) );
#endif
}

template< typename T >
void Bcast( T* data, int count, int root, MPI_Comm group)
{
#ifdef HAVE_MPI
   TNL_ASSERT_TRUE( Initialized() && ! Finalized(), "Fatal Error - MPI is not initialized" );
   TNL_ASSERT_NE( group, NullGroup(), "Bcast cannot be called with NullGroup" );
   MPI_Bcast( (void*) data, count, getDataType<T>(), root, group );
#endif
}

template< typename T >
void Alltoall( const T* sendData,
               int sendCount,
               T* receiveData,
               int receiveCount,
               MPI_Comm group )
{
#ifdef HAVE_MPI
   TNL_ASSERT_NE( group, NullGroup(), "Alltoall cannot be called with NullGroup" );
   MPI_Alltoall( (const void*) sendData,
                 sendCount,
                 getDataType<T>(),
                 (void*) receiveData,
                 receiveCount,
                 getDataType<T>(),
                 group );
#else
   TNL_ASSERT_EQ( sendCount, receiveCount, "sendCount must be equal to receiveCount when running without MPI." );
   std::memcpy( (void*) receiveData, (const void*) sendData, sendCount * sizeof(T) );
#endif
}

} // namespace MPI
} // namespace TNL

// late inclusion to break cyclic inclusion
#include "selectGPU.h"