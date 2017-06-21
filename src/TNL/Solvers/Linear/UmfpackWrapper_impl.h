/***************************************************************************
                          UmfpackWrapper_impl.h  -  description
                             -------------------
    begin                : Mar 21, 2016
    copyright            : (C) 2016 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#ifdef HAVE_UMFPACK

#include "UmfpackWrapper.h"

namespace TNL {
namespace Solvers {
namespace Linear {   

template< typename Preconditioner >
UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
UmfpackWrapper()
{}

template< typename Preconditioner >
void
UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
configSetup( Config::ConfigDescription& config,
             const String& prefix )
{
}

template< typename Preconditioner >
bool
UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
    return true;    
}

template< typename Preconditioner >
void UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
setMatrix( const MatrixPointer& matrix )
{
    this -> matrix = matrix;
}

template< typename Preconditioner >
void UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
setPreconditioner( const PreconditionerPointer& preconditioner )
{
    this -> preconditioner = preconditioner;
}


template< typename Preconditioner >
    template< typename Vector, typename ResidueGetter >
bool UmfpackWrapper< Matrices::CSR< double, Devices::Host, int >, Preconditioner >::
solve( const Vector& b,
       Vector& x )
{
    TNL_ASSERT_EQ( matrix->getRows(), matrix->getColumns(), "matrix must be square" );
    TNL_ASSERT_EQ( matrix->getColumns(), x.getSize(), "wrong size of the solution vector" );
    TNL_ASSERT_EQ( matrix->getColumns(), b.getSize(), "wrong size of the right hand side" );

    const IndexType size = matrix -> getRows();

    this->resetIterations();
    this->setResidue( this -> getConvergenceResidue() + 1.0 );

    RealType bNorm = b. lpNorm( ( RealType ) 2.0 );

    // UMFPACK objects
    void* Symbolic = nullptr;
    void* Numeric = nullptr;

    int status = UMFPACK_OK;
    double Control[ UMFPACK_CONTROL ];
    double Info[ UMFPACK_INFO ];

    // umfpack expects Compressed Sparse Column format, we have Compressed Sparse Row
    // so we need to solve  A^T * x = rhs
    int system_type = UMFPACK_Aat;

    // symbolic reordering of the sparse matrix
    status = umfpack_di_symbolic( size, size,
                                  matrix->getRowPointers(),
                                  matrix->getColumnIndexes(),
                                  matrix->getValues(),
                                  &Symbolic, Control, Info );
    if( status != UMFPACK_OK ) {
        std::cerr << "error: symbolic reordering failed" << std::endl;
        goto finished;
    }

    // numeric factorization
    status = umfpack_di_numeric( matrix->getRowPointers(),
                                 matrix->getColumnIndexes(),
                                 matrix->getValues(),
                                 Symbolic, &Numeric, Control, Info );
    if( status != UMFPACK_OK ) {
        std::cerr << "error: numeric factorization failed" << std::endl;
        goto finished;
    }

    // solve with specified right-hand-side
    status = umfpack_di_solve( system_type,
                               matrix->getRowPointers(),
                               matrix->getColumnIndexes(),
                               matrix->getValues(),
                               x.getData(),
                               b.getData(),
                               Numeric, Control, Info );
    if( status != UMFPACK_OK ) {
        std::cerr << "error: umfpack_di_solve failed" << std::endl;
        goto finished;
    }

finished:
    if( status != UMFPACK_OK ) {
        // increase print level for reports
        Control[ UMFPACK_PRL ] = 2;
        umfpack_di_report_status( Control, status );
//        umfpack_di_report_control( Control );
//        umfpack_di_report_info( Control, Info );
    }

    if( Symbolic )
        umfpack_di_free_symbolic( &Symbolic );
    if( Numeric )
        umfpack_di_free_numeric( &Numeric );

    this->setResidue( ResidueGetter::getResidue( *matrix, x, b, bNorm ) );
    this->refreshSolverMonitor( true );
    return status == UMFPACK_OK;
};

} // namespace Linear
} // namespace Solvers
} // namespace TNL

#endif
