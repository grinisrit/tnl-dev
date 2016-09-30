/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
                          SharedPointer.h  -  description
                             -------------------
    begin                : May 6, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

#pragma once

#include <TNL/Devices/Host.h>
#include <TNL/Devices/Cuda.h>
#include <TNL/SmartPointer.h>

#include <cstring>


//#define TNL_DEBUG_SHARED_POINTERS

#ifdef TNL_DEBUG_SHARED_POINTERS
   #include <typeinfo>
   #include <cxxabi.h>
   #include <iostream>
   #include <string>
   #include <memory>
   #include <cstdlib>

   inline
   std::string demangle(const char* mangled)
   {
      int status;
      std::unique_ptr<char[], void (*)(void*)> result(
         abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
      return result.get() ? std::string(result.get()) : "error occurred";
   }
#endif


namespace TNL {

/***
 * Use the lazy mode if you do not want to call the object constructor in the
 * shared pointer constructor. You may call it later via the method recreate.
 */
template< typename Object,
          typename Device = typename Object::DeviceType,
          bool lazy = false >
class SharedPointer
{
   static_assert( ! std::is_same< Device, void >::value, "The device cannot be void. You need to specify the device explicitly in your code." );
};

/****
 * Specialization for Devices::Host
 */
template< typename Object, bool lazy >
class SharedPointer< Object, Devices::Host, lazy > : public SmartPointer
{
   private:
      // Convenient template alias for controlling the selection of copy- and
      // move-constructors and assignment operators using SFINAE.
      // The type Object_ is "enabled" iff Object_ and Object are not the same,
      // but after removing const and volatile qualifiers they are the same.
      template< typename Object_ >
      using Enabler = std::enable_if< ! std::is_same< Object_, Object >::value &&
                                      std::is_same< typename std::remove_cv< Object >::type, Object_ >::value >;

      // friend class will be needed for templated assignment operators
      template< typename Object_, typename Device_, bool lazy_ >
      friend class SharedPointer;

   public:

      typedef Object ObjectType;
      typedef Devices::Host DeviceType;
      typedef SharedPointer< Object, Devices::Host, lazy > ThisType;

      template< typename... Args >
      explicit  SharedPointer( Args... args )
      : pd( nullptr )
      {
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Creating shared pointer to " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         if( ! lazy )
            this->allocate( args... );
      }

      // this is needed only to avoid the default compiler-generated constructor
      SharedPointer( const ThisType& pointer )
      : pd( (PointerData*) pointer.pd )
      {
         this->pd->counter += 1;
      }

      // conditional constructor for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      SharedPointer( const SharedPointer< Object_, DeviceType, lazy_ >& pointer )
      : pd( (PointerData*) pointer.pd )
      {
         this->pd->counter += 1;
      }

      // this is needed only to avoid the default compiler-generated constructor
      SharedPointer( ThisType&& pointer )
      : pd( (PointerData*) pointer.pd )
      {
         pointer.pd = nullptr;
      }

      // conditional constructor for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      SharedPointer( SharedPointer< Object_, DeviceType, lazy_ >&& pointer )
      : pd( (PointerData*) pointer.pd )
      {
         pointer.pd = nullptr;
      }

      template< typename... Args >
      bool recreate( Args... args )
      {
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Recreating shared pointer to " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         if( ! this->counter )
            return this->allocate( args... );

         if( *this->pd->counter == 1 )
         {
            /****
             * The object is not shared -> recreate it in-place, without reallocation
             */
            this->pd->data.~ObjectType();
            new ( this->pd->data ) ObjectType( args... );
            return true;
         }

         // free will just decrement the counter
         this->free();

         return this->allocate( args... );
      }

      const Object* operator->() const
      {
         return &this->pd->data;
      }

      Object* operator->()
      {
         return &this->pd->data;
      }

      const Object& operator *() const
      {
         return this->pd->data;
      }

      Object& operator *()
      {
         return this->pd->data;
      }

      operator bool()
      {
         return this->pd;
      }

      template< typename Device = Devices::Host >
      __cuda_callable__
      const Object& getData() const
      {
         return this->pd->data;
      }

      template< typename Device = Devices::Host >
      __cuda_callable__
      Object& modifyData()
      {
         return this->pd->data;
      }

      // this is needed only to avoid the default compiler-generated operator
      const ThisType& operator=( const ThisType& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->pd->counter += 1;
         return *this;
      }

      // conditional operator for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      const ThisType& operator=( const SharedPointer< Object_, DeviceType, lazy_ >& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->pd->counter += 1;
         return *this;
      }

      // this is needed only to avoid the default compiler-generated operator
      const ThisType& operator=( ThisType&& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         ptr.pd = nullptr;
         return *this;
      }

      // conditional operator for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      const ThisType& operator=( SharedPointer< Object_, DeviceType, lazy_ >&& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         ptr.pd = nullptr;
         return *this;
      }

      bool synchronize()
      {
         return true;
      }

      ~SharedPointer()
      {
         this->free();
      }


   protected:

      struct PointerData
      {
         Object data;
         int counter;

         template< typename... Args >
         explicit PointerData( Args... args )
         : data( args... ),
           counter( 1 )
         {}
      };

      template< typename... Args >
      bool allocate( Args... args )
      {
         this->pd = new PointerData( args... );
         return this->pd;
      }

      void free()
      {
         if( this->pd )
         {
            if( ! --this->pd->counter )
            {
               delete this->pd;
               this->pd = nullptr;
            }
         }

      }

      PointerData* pd;
};

/****
 * Specialization for CUDA
 */
template< typename Object, bool lazy >
class SharedPointer< Object, Devices::Cuda, lazy > : public SmartPointer
{
   private:
      // Convenient template alias for controlling the selection of copy- and
      // move-constructors and assignment operators using SFINAE.
      // The type Object_ is "enabled" iff Object_ and Object are not the same,
      // but after removing const and volatile qualifiers they are the same.
      template< typename Object_ >
      using Enabler = std::enable_if< ! std::is_same< Object_, Object >::value &&
                                      std::is_same< typename std::remove_cv< Object >::type, Object_ >::value >;

      // friend class will be needed for templated assignment operators
      template< typename Object_, typename Device_, bool lazy_ >
      friend class SharedPointer;

   public:

      typedef Object ObjectType;
      typedef Devices::Cuda DeviceType;
      typedef SharedPointer< Object, Devices::Cuda, lazy > ThisType;

      template< typename... Args >
      explicit  SharedPointer( Args... args )
      : pd( nullptr ),
        cuda_pointer( nullptr )
      {
         if( ! lazy )
            this->allocate( args... );
      }

      // this is needed only to avoid the default compiler-generated constructor
      SharedPointer( const ThisType& pointer )
      : pd( (PointerData*) pointer.pd ),
        cuda_pointer( pointer.cuda_pointer )
      {
         this->pd->counter += 1;
      }

      // conditional constructor for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      SharedPointer( const SharedPointer< Object_, DeviceType, lazy_ >& pointer )
      : pd( (PointerData*) pointer.pd ),
        cuda_pointer( pointer.cuda_pointer )
      {
         this->pd->counter += 1;
      }

      // this is needed only to avoid the default compiler-generated constructor
      SharedPointer( ThisType&& pointer )
      : pd( (PointerData*) pointer.pd ),
        cuda_pointer( pointer.cuda_pointer )
      {
         pointer.pd = nullptr;
         pointer.cuda_pointer = nullptr;
      }

      // conditional constructor for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      SharedPointer( SharedPointer< Object_, DeviceType, lazy_ >&& pointer )
      : pd( (PointerData*) pointer.pd ),
        cuda_pointer( pointer.cuda_pointer )
      {
         pointer.pd = nullptr;
         pointer.cuda_pointer = nullptr;
      }

      template< typename... Args >
      bool recreate( Args... args )
      {
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Recreating shared pointer to " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         if( ! this->pd )
            return this->allocate( args... );

         if( this->pd->counter == 1 )
         {
            /****
             * The object is not shared -> recreate it in-place, without reallocation
             */
            this->pd->data.~Object();
            new ( &this->pd->data ) Object( args... );
#ifdef HAVE_CUDA
            cudaMemcpy( (void*) this->cuda_pointer, (void*) &this->pd->data, sizeof( Object ), cudaMemcpyHostToDevice );
#endif
            this->set_last_sync_state();
            return true;
         }

         // free will just decrement the counter
         this->free();

         return this->allocate( args... );
      }

      const Object* operator->() const
      {
         return &this->pd->data;
      }

      Object* operator->()
      {
         this->pd->maybe_modified = true;
         return &this->pd->data;
      }

      const Object& operator *() const
      {
         return this->pd->data;
      }

      Object& operator *()
      {
         this->pd->maybe_modified = true;
         return this->pd->data;
      }

      operator bool()
      {
         return this->pd;
      }

      template< typename Device = Devices::Host >
      __cuda_callable__
      const Object& getData() const
      {
         static_assert( std::is_same< Device, Devices::Host >::value || std::is_same< Device, Devices::Cuda >::value, "Only Devices::Host or Devices::Cuda devices are accepted here." );
         Assert( this->pd, );
         Assert( this->cuda_pointer, );
         if( std::is_same< Device, Devices::Host >::value )
            return this->pd->data;
         if( std::is_same< Device, Devices::Cuda >::value )
            return *( this->cuda_pointer );
      }

      template< typename Device = Devices::Host >
      __cuda_callable__
      Object& modifyData()
      {
         static_assert( std::is_same< Device, Devices::Host >::value || std::is_same< Device, Devices::Cuda >::value, "Only Devices::Host or Devices::Cuda devices are accepted here." );
         Assert( this->pd, );
         Assert( this->cuda_pointer, );
         if( std::is_same< Device, Devices::Host >::value )
         {
            this->pd->maybe_modified = true;
            return this->pd->data;
         }
         if( std::is_same< Device, Devices::Cuda >::value )
            return *( this->cuda_pointer );
      }

      // this is needed only to avoid the default compiler-generated operator
      const ThisType& operator=( const ThisType& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->cuda_pointer = ptr.cuda_pointer;
         this->pd->counter += 1;
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Copy-assigned shared pointer: counter = " << this->pd->counter << ", type: " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         return *this;
      }

      // conditional operator for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      const ThisType& operator=( const SharedPointer< Object_, DeviceType, lazy_ >& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->cuda_pointer = ptr.cuda_pointer;
         this->pd->counter += 1;
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Copy-assigned shared pointer: counter = " << this->pd->counter << ", type: " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         return *this;
      }

      // this is needed only to avoid the default compiler-generated operator
      const ThisType& operator=( ThisType&& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->cuda_pointer = ptr.cuda_pointer;
         ptr.pd = nullptr;
         ptr.cuda_pointer = nullptr;
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Move-assigned shared pointer: counter = " << this->pd->counter << ", type: " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         return *this;
      }

      // conditional operator for non-const -> const data
      template< typename Object_, bool lazy_,
                typename = typename Enabler< Object_ >::type >
      const ThisType& operator=( SharedPointer< Object_, DeviceType, lazy_ >&& ptr )
      {
         this->free();
         this->pd = (PointerData*) ptr.pd;
         this->cuda_pointer = ptr.cuda_pointer;
         ptr.pd = nullptr;
         ptr.cuda_pointer = nullptr;
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Move-assigned shared pointer: counter = " << this->pd->counter << ", type: " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
         return *this;
      }

      bool synchronize()
      {
         if( ! this->pd )
            return true;
#ifdef HAVE_CUDA
         if( this->modified() )
         {
#ifdef TNL_DEBUG_SHARED_POINTERS
            std::cerr << "Synchronizing shared pointer: counter = " << this->pd->counter << ", type: " << demangle(typeid(Object).name()) << std::endl;
            std::cerr << "   ( " << sizeof( Object ) << " bytes, CUDA adress " << this->cuda_pointer << " )" << std::endl;
#endif
            Assert( this->cuda_pointer, );
            cudaMemcpy( (void*) this->cuda_pointer, (void*) &this->pd->data, sizeof( Object ), cudaMemcpyHostToDevice );
            if( ! checkCudaDevice ) {
               return false;
            }
            this->set_last_sync_state();
            return true;
         }
         return true;
#else
         return false;
#endif
      }

      ~SharedPointer()
      {
         this->free();
         Devices::Cuda::removeSmartPointer( this );
      }

   protected:

      struct PointerData
      {
         Object data;
         char data_image[ sizeof(Object) ];
         int counter;
         bool maybe_modified;

         template< typename... Args >
         explicit PointerData( Args... args )
         : data( args... ),
           counter( 1 ),
           maybe_modified( false )
         {}
      };

      template< typename... Args >
      bool allocate( Args... args )
      {
         this->pd = new PointerData( args... );
         if( ! this->pd )
            return false;
         // pass to device
         this->cuda_pointer = Devices::Cuda::passToDevice( this->pd->data );
         if( ! this->cuda_pointer )
            return false;
         // set last-sync state
         this->set_last_sync_state();
#ifdef TNL_DEBUG_SHARED_POINTERS
         std::cerr << "Created shared pointer to " << demangle(typeid(ObjectType).name()) << " (cuda_pointer = " << this->cuda_pointer << ")" << std::endl;
#endif
         Devices::Cuda::insertSmartPointer( this );
         return true;
      }

      void set_last_sync_state()
      {
         Assert( this->pd, );
         std::memcpy( (void*) &this->pd->data_image, (void*) &this->pd->data, sizeof( Object ) );
         this->pd->maybe_modified = false;
      }

      bool modified()
      {
         Assert( this->pd, );
         // optimization: skip bitwise comparison if we're sure that the data is the same
         if( ! this->pd->maybe_modified )
            return false;
         return std::memcmp( (void*) &this->pd->data_image, (void*) &this->pd->data, sizeof( Object ) ) != 0;
      }

      void free()
      {
         if( this->pd )
         {
#ifdef TNL_DEBUG_SHARED_POINTERS
            std::cerr << "Freeing shared pointer: counter = " << this->pd->counter << ", cuda_pointer = " << this->cuda_pointer << ", type: " << demangle(typeid(ObjectType).name()) << std::endl;
#endif
            if( ! --this->pd->counter )
            {
               delete this->pd;
               this->pd = nullptr;
               if( this->cuda_pointer )
                  Devices::Cuda::freeFromDevice( this->cuda_pointer );
#ifdef TNL_DEBUG_SHARED_POINTERS
               std::cerr << "...deleted data." << std::endl;
#endif
            }
         }
      }

      PointerData* pd;

      // cuda_pointer can't be part of PointerData structure, since we would be
      // unable to dereference this-pd on the device
      Object* cuda_pointer;
};

} // namespace TNL
