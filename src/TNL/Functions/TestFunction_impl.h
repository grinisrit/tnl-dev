/***************************************************************************
                          TestFunction_impl.h  -  description
                             -------------------
    begin                : Aug 3, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Devices/Cuda.h>
#include <TNL/Functions/Analytic/ConstantFunction.h>
#include <TNL/Functions/Analytic/ExpBumpFunction.h>
#include <TNL/Functions/Analytic/SinBumpsFunction.h>
#include <TNL/Functions/Analytic/SinWaveFunction.h>
#include <TNL/Functions/Analytic/ConstantFunction.h>
#include <TNL/Functions/Analytic/ExpBumpFunction.h>
#include <TNL/Functions/Analytic/SinBumpsFunction.h>
#include <TNL/Functions/Analytic/SinWaveFunction.h>
#include <TNL/Functions/Analytic/CylinderFunction.h>
#include <TNL/Functions/Analytic/FlowerpotFunction.h>
#include <TNL/Functions/Analytic/TwinsFunction.h>
#include <TNL/Functions/Analytic/BlobFunction.h>
#include <TNL/Functions/Analytic/PseudoSquareFunction.h>

namespace TNL {
namespace Functions {   

template< int FunctionDimensions,
          typename Real,
          typename Device >
TestFunction< FunctionDimensions, Real, Device >::
TestFunction()
: function( 0 ),
  timeDependence( none ),
  timeScale( 1.0 )
{
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
void
TestFunction< FunctionDimensions, Real, Device >::
configSetup( Config::ConfigDescription& config,
             const String& prefix )
{
   config.addRequiredEntry< String >( prefix + "test-function", "Testing function." );
      config.addEntryEnum( "constant" );
      config.addEntryEnum( "exp-bump" );
      config.addEntryEnum( "sin-wave" );
      config.addEntryEnum( "sin-bumps" );
      config.addEntryEnum( "cylinder" );
      config.addEntryEnum( "flowerpot" );
      config.addEntryEnum( "twins" );
      config.addEntryEnum( "pseudoSquare" );
      config.addEntryEnum( "blob" );
   config.addEntry     < double >( prefix + "constant", "Value of the constant function.", 0.0 );
   config.addEntry     < double >( prefix + "wave-length", "Wave length of the sine based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "wave-length-x", "Wave length of the sine based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "wave-length-y", "Wave length of the sine based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "wave-length-z", "Wave length of the sine based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "phase", "Phase of the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "phase-x", "Phase of the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "phase-y", "Phase of the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "phase-z", "Phase of the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "amplitude", "Amplitude length of the sine based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "waves-number", "Cut-off for the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "waves-number-x", "Cut-off for the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "waves-number-y", "Cut-off for the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "waves-number-z", "Cut-off for the sine based test functions.", 0.0 );
   config.addEntry     < double >( prefix + "sigma", "Sigma for the exp based test functions.", 1.0 );
   config.addEntry     < double >( prefix + "diameter", "Diameter for the cylinder, flowerpot test functions.", 1.0 );
  config.addEntry     < double >( prefix + "height", "Height of zero-level-set function for the blob, pseudosquare test functions.", 1.0 );
   config.addEntry     < String >( prefix + "time-dependence", "Time dependence of the test function.", "none" );
      config.addEntryEnum( "none" );
      config.addEntryEnum( "linear" );
      config.addEntryEnum( "quadratic" );
      config.addEntryEnum( "cosine" );
   config.addEntry     < double >( prefix + "time-scale", "Time scaling for the time dependency of the test function.", 1.0 );

}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< typename FunctionType >
bool
TestFunction< FunctionDimensions, Real, Device >::
setupFunction( const Config::ParameterContainer& parameters,
               const String& prefix )
{
   FunctionType* auxFunction = new FunctionType;
   if( ! auxFunction->setup( parameters, prefix ) )
   {
      delete auxFunction;
      return false;
   }

   if( std::is_same< Device, Devices::Host >::value )
   {
      this->function = auxFunction;
   }
   if( std::is_same< Device, Devices::Cuda >::value )
   {
      this->function = Devices::Cuda::passToDevice( *auxFunction );
      delete auxFunction;
      if( ! checkCudaDevice )
         return false;
   }
   return true;
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
bool
TestFunction< FunctionDimensions, Real, Device >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   using namespace TNL::Functions::Analytic;
   std::cout << "Test function setup ... " << std::endl;
   const String& timeDependence =
            parameters.getParameter< String >(
                     prefix +
                     "time-dependence" );
  std::cout << "Time dependence ... " << timeDependence << std::endl;
   if( timeDependence == "none" )
      this->timeDependence = none;
   if( timeDependence == "linear" )
      this->timeDependence = linear;
   if( timeDependence == "quadratic" )
      this->timeDependence = quadratic;
   if( timeDependence == "cosine" )
      this->timeDependence = cosine;

   this->timeScale = parameters.getParameter< double >( prefix + "time-scale" );

   const String& testFunction = parameters.getParameter< String >( prefix + "test-function" );
  std::cout << "Test function ... " << testFunction << std::endl;
   if( testFunction == "constant" )
   {
      typedef ConstantFunction< Dimensions, Real > FunctionType;
      functionType = constant;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "exp-bump" )
   {
      typedef ExpBumpFunction< Dimensions, Real > FunctionType;
      functionType = expBump;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "sin-bumps" )
   {
      typedef SinBumpsFunction< Dimensions, Real > FunctionType;
      functionType = sinBumps;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "sin-wave" )
   {
      typedef SinWaveFunction< Dimensions, Real > FunctionType;
      functionType = sinWave;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "cylinder" )
   {
      typedef CylinderFunction< Dimensions, Real > FunctionType;
      functionType = cylinder;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "flowerpot" )
   {
      typedef FlowerpotFunction< Dimensions, Real > FunctionType;
      functionType = flowerpot;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "twins" )
   {
      typedef TwinsFunction< Dimensions, Real > FunctionType;
      functionType = twins;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "pseudoSquare" )
   {
      typedef PseudoSquareFunction< Dimensions, Real > FunctionType;
      functionType = pseudoSquare;
      return setupFunction< FunctionType >( parameters );
   }
   if( testFunction == "blob" )
   {
      typedef BlobFunction< Dimensions, Real > FunctionType;
      functionType = blob;
      return setupFunction< FunctionType >( parameters );
   }
   std::cerr << "Unknown function " << testFunction << std::endl;
   return false;
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
const TestFunction< FunctionDimensions, Real, Device >&
TestFunction< FunctionDimensions, Real, Device >::
operator = ( const TestFunction& function )
{
   /*****
    * TODO: if the function is on the device we cannot do the following
    */
   abort();
   using namespace TNL::Functions::Analytic;
   this->functionType   = function.functionType;
   this->timeDependence = function.timeDependence;
   this->timeScale      = function.timeScale;

   this->deleteFunctions();

   switch( this->functionType )
   {
      case constant:
         this->copyFunction< ConstantFunction< FunctionDimensions, Real > >( function.function );
         break;
      case expBump:
         this->copyFunction< ExpBumpFunction< FunctionDimensions, Real > >( function.function );
         break;
      case sinBumps:
         this->copyFunction< SinBumpsFunction< FunctionDimensions, Real > >( function.function );
         break;
      case sinWave:
         this->copyFunction< SinWaveFunction< FunctionDimensions, Real > >( function.function );
         break;
      case cylinder:
         this->copyFunction< CylinderFunction< FunctionDimensions, Real > >( function.function );
         break;
      case flowerpot:
         this->copyFunction< FlowerpotFunction< FunctionDimensions, Real > >( function.function );
         break;
      case twins:
         this->copyFunction< TwinsFunction< FunctionDimensions, Real > >( function.function );
         break;
      case pseudoSquare:
         this->copyFunction< PseudoSquareFunction< FunctionDimensions, Real > >( function.function );
         break;
      case blob:
         this->copyFunction< BlobFunction< FunctionDimensions, Real > >( function.function );
         break;
      default:
         Assert( false, );
         break;
   }

}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder >
__cuda_callable__
Real
TestFunction< FunctionDimensions, Real, Device >::
getPartialDerivative( const VertexType& vertex,
          const Real& time ) const
{
   using namespace TNL::Functions::Analytic;
   Real scale( 1.0 );
   switch( this->timeDependence )
   {
      case none:
         break;
      case linear:
         scale = 1.0 - this->timeScale * time;
         break;
      case quadratic:
         scale = this->timeScale * time;
         scale *= scale;
         scale = 1.0 - scale;
         break;
      case cosine:
         scale = ::cos( this->timeScale * time );
         break;
   }
   switch( functionType )
   {
      case constant:
         return scale * ( ( ConstantFunction< Dimensions, Real >* ) function )->
                   template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case expBump:
         return scale * ( ( ExpBumpFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case sinBumps:
         return scale * ( ( SinBumpsFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case sinWave:
         return scale * ( ( SinWaveFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case cylinder:
         return scale * ( ( CylinderFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case flowerpot:
         return scale * ( ( FlowerpotFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case twins:
         return scale * ( ( TwinsFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case pseudoSquare:
         return scale * ( ( PseudoSquareFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case blob:
         return scale * ( ( BlobFunction< Dimensions, Real >* ) function )->
                  template getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      default:
         return 0.0;
   }
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder >
__cuda_callable__
Real
TestFunction< FunctionDimensions, Real, Device >::
getTimeDerivative( const VertexType& vertex,
                   const Real& time ) const
{
   using namespace TNL::Functions::Analytic;
   Real scale( 0.0 );
   switch( timeDependence )
   {
      case none:
         break;
      case linear:
         scale = -this->timeScale;
         break;
      case quadratic:
         scale = -2.0 * this->timeScale * this->timeScale * time;
         break;
      case cosine:
         scale = -this->timeScale * ::sin( this->timeScale * time );
         break;
   }
   switch( functionType )
   {
      case constant:
         return scale * ( ( ConstantFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case expBump:
         return scale * ( ( ExpBumpFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case sinBumps:
         return scale * ( ( SinBumpsFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
      case sinWave:
         return scale * ( ( SinWaveFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      case cylinder:
         return scale * ( ( CylinderFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      case flowerpot:
         return scale * ( ( FlowerpotFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      case twins:
         return scale * ( ( TwinsFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      case pseudoSquare:
         return scale * ( ( PseudoSquareFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      case blob:
         return scale * ( ( BlobFunction< Dimensions, Real >* ) function )->
                  getPartialDerivative< XDiffOrder, YDiffOrder, ZDiffOrder >( vertex, time );
         break;
      default:
         return 0.0;
   }
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< typename FunctionType >
void
TestFunction< FunctionDimensions, Real, Device >::
deleteFunction()
{
   if( std::is_same< Device, Devices::Host >::value )
   {
      if( function )
         delete ( FunctionType * ) function;
   }
   if( std::is_same< Device, Devices::Cuda >::value )
   {
      if( function )
         Devices::Cuda::freeFromDevice( ( FunctionType * ) function );
   }
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
void
TestFunction< FunctionDimensions, Real, Device >::
deleteFunctions()
{
   using namespace TNL::Functions::Analytic;
   switch( functionType )
   {
      case constant:
         deleteFunction< ConstantFunction< Dimensions, Real> >();
         break;
      case expBump:
         deleteFunction< ExpBumpFunction< Dimensions, Real> >();
         break;
      case sinBumps:
         deleteFunction< SinBumpsFunction< Dimensions, Real> >();
         break;
      case sinWave:
         deleteFunction< SinWaveFunction< Dimensions, Real> >();
         break;
      case cylinder:
         deleteFunction< CylinderFunction< Dimensions, Real> >();
         break;
      case flowerpot:
         deleteFunction< FlowerpotFunction< Dimensions, Real> >();
         break;
      case twins:
         deleteFunction< TwinsFunction< Dimensions, Real> >();
         break;
      case pseudoSquare:
         deleteFunction< PseudoSquareFunction< Dimensions, Real> >();
         break;
      case blob:
         deleteFunction< BlobFunction< Dimensions, Real> >();
         break;
   }
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< typename FunctionType >
void
TestFunction< FunctionDimensions, Real, Device >::
copyFunction( const void* function )
{
   if( std::is_same< Device, Devices::Host >::value )
   {
      FunctionType* f = new FunctionType;
      *f = * ( FunctionType* )function;
   }
   if( std::is_same< Device, Devices::Cuda >::value )
   {
      Assert( false, );
      abort();
   }
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
   template< typename FunctionType >
std::ostream&
TestFunction< FunctionDimensions, Real, Device >::
printFunction( std::ostream& str ) const
{
   FunctionType* f = ( FunctionType* ) this->function;
   if( std::is_same< Device, Devices::Host >::value )
   {
      str << *f;
      return str;
   }
   if( std::is_same< Device, Devices::Cuda >::value )
   {
      Devices::Cuda::print( f, str );
      return str;
   }
   return str;
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
std::ostream&
TestFunction< FunctionDimensions, Real, Device >::
print( std::ostream& str ) const
{
   using namespace TNL::Functions::Analytic;
   str << " timeDependence = " << this->timeDependence;
   str << " functionType = " << this->functionType;
   str << " function = " << this->function << "; ";
   switch( functionType )
   {
      case constant:
         return printFunction< ConstantFunction< Dimensions, Real> >( str );
      case expBump:
         return printFunction< ExpBumpFunction< Dimensions, Real> >( str );
      case sinBumps:
         return printFunction< SinBumpsFunction< Dimensions, Real> >( str );
      case sinWave:
         return printFunction< SinWaveFunction< Dimensions, Real> >( str );
      case cylinder:
         return printFunction< CylinderFunction< Dimensions, Real> >( str );
      case flowerpot:
         return printFunction< FlowerpotFunction< Dimensions, Real> >( str );
      case twins:
         return printFunction< TwinsFunction< Dimensions, Real> >( str );
      case pseudoSquare:
         return printFunction< PseudoSquareFunction< Dimensions, Real> >( str );
      case blob:
         return printFunction< BlobFunction< Dimensions, Real> >( str );
   }
   return str;
}

template< int FunctionDimensions,
          typename Real,
          typename Device >
TestFunction< FunctionDimensions, Real, Device >::
~TestFunction()
{
   deleteFunctions();
}


#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

#ifdef INSTANTIATE_FLOAT
extern template class TestFunction< 1, float, Devices::Host >;
extern template class TestFunction< 2, float, Devices::Host >;
extern template class TestFunction< 3, float, Devices::Host >;
#endif

extern template class TestFunction< 1, double, Devices::Host >;
extern template class TestFunction< 2, double, Devices::Host >;
extern template class TestFunction< 3, double, Devices::Host >;

#ifdef INSTANTIATE_LONG_DOUBLE
extern template class TestFunction< 1, long double, Devices::Host >;
extern template class TestFunction< 2, long double, Devices::Host >;
extern template class TestFunction< 3, long double, Devices::Host >;
#endif

#ifdef HAVE_CUDA
#ifdef INSTANTIATE_FLOAT
extern template class TestFunction< 1, float, Devices::Cuda>;
extern template class TestFunction< 2, float, Devices::Cuda >;
extern template class TestFunction< 3, float, Devices::Cuda >;
#endif

extern template class TestFunction< 1, double, Devices::Cuda >;
extern template class TestFunction< 2, double, Devices::Cuda >;
extern template class TestFunction< 3, double, Devices::Cuda >;

#ifdef INSTANTIATE_LONG_DOUBLE
extern template class TestFunction< 1, long double, Devices::Cuda >;
extern template class TestFunction< 2, long double, Devices::Cuda >;
extern template class TestFunction< 3, long double, Devices::Cuda >;
#endif
#endif

#endif

} // namespace Functions
} // namespace TNL

