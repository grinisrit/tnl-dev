/***************************************************************************
                          MeshFunctionView.h  -  description
                             -------------------
    begin                : Nov 8, 2015
    copyright            : (C) 2015 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/File.h>
#include <TNL/Containers/VectorView.h>
#include <TNL/Functions/Domain.h>
#include <TNL/Pointers/SharedPointer.h>

namespace TNL {
namespace Functions {

template< typename Mesh,
          int MeshEntityDimension = Mesh::getMeshDimension(),
          typename Real = typename Mesh::RealType >
class MeshFunctionView :
   public Object,
   public Domain< Mesh::getMeshDimension(), MeshDomain >
{
   //static_assert( Mesh::DeviceType::DeviceType == Vector::DeviceType::DeviceType,
   //               "Both mesh and vector of a mesh function must reside on the same device.");
   public:

      using MeshType = Mesh;
      using DeviceType = typename MeshType::DeviceType;
      using IndexType = typename MeshType::GlobalIndexType;
      using MeshPointer = Pointers::SharedPointer< MeshType >;
      using RealType = Real;
      using VectorType = Containers::VectorView< RealType, DeviceType, IndexType >;

      static constexpr int getEntitiesDimension() { return MeshEntityDimension; }

      static constexpr int getMeshDimension() { return MeshType::getMeshDimension(); }

      MeshFunctionView();

      MeshFunctionView( const MeshFunctionView& meshFunction );

      template< typename Vector >
      MeshFunctionView( const MeshPointer& meshPointer,
                        Vector& data,
                        const IndexType& offset = 0 );

      template< typename Vector >
      MeshFunctionView( const MeshPointer& meshPointer,
                        Pointers::SharedPointer<  Vector >& data,
                        const IndexType& offset = 0 );

      static String getSerializationType();

      virtual String getSerializationTypeVirtual() const;

      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" );

      bool setup( const MeshPointer& meshPointer,
                  const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      void bind( MeshFunctionView& meshFunction );

      template< typename Vector >
      void bind( Vector& data,
                 const IndexType& offset = 0 );

      template< typename Vector >
      void bind( const MeshPointer& meshPointer,
                 Vector& data,
                 const IndexType& offset = 0 );

      template< typename Vector >
      void bind( const MeshPointer& meshPointer,
                 Pointers::SharedPointer< Vector >& dataPtr,
                 const IndexType& offset = 0 );

      void setMesh( const MeshPointer& meshPointer );

      template< typename Device = Devices::Host >
      __cuda_callable__
      const MeshType& getMesh() const;

      const MeshPointer& getMeshPointer() const;

      static IndexType getDofs( const MeshPointer& meshPointer );

      __cuda_callable__ const VectorType& getData() const;

      __cuda_callable__ VectorType& getData();

      bool refresh( const RealType& time = 0.0 ) const;

      bool deepRefresh( const RealType& time = 0.0 ) const;

      template< typename EntityType >
      RealType getValue( const EntityType& meshEntity ) const;

      template< typename EntityType >
      void setValue( const EntityType& meshEntity,
                     const RealType& value );

      template< typename EntityType >
      __cuda_callable__
      RealType& operator()( const EntityType& meshEntity,
                            const RealType& time = 0 );

      template< typename EntityType >
      __cuda_callable__
      const RealType& operator()( const EntityType& meshEntity,
                                  const RealType& time = 0 ) const;

      __cuda_callable__
      RealType& operator[]( const IndexType& meshEntityIndex );
      __cuda_callable__
      const RealType& operator[]( const IndexType& meshEntityIndex ) const;

      MeshFunctionView& operator = ( const MeshFunctionView& f );

      template< typename Function >
      MeshFunctionView& operator = ( const Function& f );

      template< typename Function >
      MeshFunctionView& operator -= ( const Function& f );

      template< typename Function >
      MeshFunctionView& operator += ( const Function& f );

      RealType getLpNorm( const RealType& p ) const;

      RealType getMaxNorm() const;

      void save( File& file ) const;

      void load( File& file );

      void boundLoad( File& file );

      void boundLoad( const String& fileName );

      bool write( const String& fileName,
                  const String& format = "vtk" ) const;

      using Object::save;

      using Object::load;

   protected:

      MeshPointer meshPointer;

      VectorType data;

      template< typename, typename > friend class MeshFunctionEvaluator;
};

template< typename Mesh,
          int MeshEntityDimension,
          typename Real >
std::ostream& operator << ( std::ostream& str, const MeshFunctionView< Mesh, MeshEntityDimension, Real >& f );

} // namespace Functions
} // namespace TNL

#include <TNL/Functions/MeshFunctionView.hpp>