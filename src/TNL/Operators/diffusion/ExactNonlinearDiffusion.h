/***************************************************************************
                          ExactLinearDiffusion.h  -  description
                             -------------------
    begin                : Aug 8, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Functions/Domain.h>
#include <TNL/Operators/ExactIdentityOperator.h>


namespace TNL {
namespace Operators {   

template<  int Dimension,
           typename Nonlinearity,
           typename InnerOperator = ExactIdentityOperator< Dimension > >
class ExactNonlinearDiffusion
{};


template< typename Nonlinearity,
          typename InnerOperator >
class ExactNonlinearDiffusion< 1, Nonlinearity, InnerOperator >
   : public Functions::Domain< 1, Functions::SpaceDomain >
{
   public:

      static String getType()
      {
         return "ExactNonlinearDiffusion< 1, " + Nonlinearity::getType() + " >";
      };
 
      Nonlinearity& getNonlinearity()
      {
         return this->nonlinearity;
      }
 
      const Nonlinearity& getNonlinearity() const
      {
         return this->nonlinearity;
      }
 
      InnerOperator& getInnerOperator()
      {
         return this->innerOperator;
      }
 
      const InnerOperator& getInnerOperator() const
      {
         return this->innerOperator;
      }
 
      template< typename Function >
      __cuda_callable__
      typename Function::RealType
      operator()( const Function& function,
                  const typename Function::VertexType& v,
                  const typename Function::RealType& time = 0.0 ) const
      {
         typedef typename Function::RealType RealType;
         const RealType u_x = innerOperator.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         const RealType u_xx = innerOperator.template getPartialDerivative< Function, 2, 0, 0 >( function, v, time );
         const RealType g = nonlinearity( function, v, time );
         const RealType g_x = nonlinearity.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         return u_xx * g + u_x * g_x;
      }
 
      protected:
 
         Nonlinearity nonlinearity;

         InnerOperator innerOperator;
};

template< typename Nonlinearity,
          typename InnerOperator >
class ExactNonlinearDiffusion< 2, Nonlinearity, InnerOperator >
   : public Functions::Domain< 2, Functions::SpaceDomain >
{
   public:
 
      static String getType()
      {
         return "ExactNonlinearDiffusion< " + Nonlinearity::getType() + ", 2 >";
      };
 
      Nonlinearity& getNonlinearity()
      {
         return this->nonlinearity;
      }
 
      const Nonlinearity& getNonlinearity() const
      {
         return this->nonlinearity;
      }
 
      InnerOperator& getInnerOperator()
      {
         return this->innerOperator;
      }
 
      const InnerOperator& getInnerOperator() const
      {
         return this->innerOperator;
      }

      template< typename Function >
      __cuda_callable__
      typename Function::RealType
      operator()( const Function& function,
                  const typename Function::VertexType& v,
                  const typename Function::RealType& time = 0.0 ) const
      {
         typedef typename Function::RealType RealType;
         const RealType u_x  = innerOperator.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         const RealType u_y  = innerOperator.template getPartialDerivative< Function, 0, 1, 0 >( function, v, time );
         const RealType u_xx = innerOperator.template getPartialDerivative< Function, 2, 0, 0 >( function, v, time );
         const RealType u_yy = innerOperator.template getPartialDerivative< Function, 0, 2, 0 >( function, v, time );
         const RealType g   = nonlinearity( function, v, time );
         const RealType g_x = nonlinearity.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         const RealType g_y = nonlinearity.template getPartialDerivative< Function, 0, 1, 0 >( function, v, time );

         return  ( u_xx + u_yy ) * g + g_x * u_x + g_y * u_y;
      }

      protected:
 
         Nonlinearity nonlinearity;
 
         InnerOperator innerOperator;
 
};

template< typename Nonlinearity,
          typename InnerOperator  >
class ExactNonlinearDiffusion< 3, Nonlinearity, InnerOperator >
   : public Functions::Domain< 3, Functions::SpaceDomain >
{
   public:
 
      static String getType()
      {
         return "ExactNonlinearDiffusion< " + Nonlinearity::getType() + ", 3 >";
      }
 
      Nonlinearity& getNonlinearity()
      {
         return this->nonlinearity;
      }
 
      const Nonlinearity& getNonlinearity() const
      {
         return this->nonlinearity;
      }
 
      InnerOperator& getInnerOperator()
      {
         return this->innerOperator;
      }
 
      const InnerOperator& getInnerOperator() const
      {
         return this->innerOperator;
      }
 
      template< typename Function >
      __cuda_callable__
      typename Function::RealType
      operator()( const Function& function,
                  const typename Function::VertexType& v,
                  const typename Function::RealType& time = 0.0 ) const
      {
         typedef typename Function::RealType RealType;
         const RealType u_x  = innerOperator.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         const RealType u_y  = innerOperator.template getPartialDerivative< Function, 0, 1, 0 >( function, v, time );
         const RealType u_z  = innerOperator.template getPartialDerivative< Function, 0, 0, 1 >( function, v, time );
         const RealType u_xx = innerOperator.template getPartialDerivative< Function, 2, 0, 0 >( function, v, time );
         const RealType u_yy = innerOperator.template getPartialDerivative< Function, 0, 2, 0 >( function, v, time );
         const RealType u_zz = innerOperator.template getPartialDerivative< Function, 0, 0, 2 >( function, v, time );
         const RealType g   = nonlinearity( function, v, time ) ;
         const RealType g_x = nonlinearity.template getPartialDerivative< Function, 1, 0, 0 >( function, v, time );
         const RealType g_y = nonlinearity.template getPartialDerivative< Function, 0, 1, 0 >( function, v, time );
         const RealType g_z = nonlinearity.template getPartialDerivative< Function, 0, 0, 1 >( function, v, time );

         return ( u_xx + u_yy + u_zz ) * g + g_x * u_x + g_y * u_y + g_z * u_z;
      }
 
      protected:
 
         Nonlinearity nonlinearity;
 
         InnerOperator innerOperator;
};

} // namespace Operators
} // namespace TNL

