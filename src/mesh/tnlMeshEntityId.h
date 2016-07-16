/***************************************************************************
                          tnlMeshEntityId.h  -  description
                             -------------------
    begin                : Feb 28, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLMESHENTITYID_H_
#define TNLMESHENTITYID_H_

template< typename IDType,
          typename GlobalIndexType >
class tnlMeshEntityId
{
   public:

   tnlMeshEntityId()
      : id( -1 )
   {}

   const IDType &getId() const
   {
      tnlAssert( this->id >= 0, );
      return this->id;
   }

   void setId( GlobalIndexType id )
   {
      this->id = id;
   }

   bool operator==( const tnlMeshEntityId< IDType, GlobalIndexType >& id ) const
   {
      return ( this->id == id.id );
   }

   protected:
   IDType id;
};

template< typename GlobalIndexType >
class tnlMeshEntityId< void, GlobalIndexType >
{
   public:
   void setId( GlobalIndexType )
   {}

   bool operator==( const tnlMeshEntityId< void, GlobalIndexType >& id ) const
   {
      return true;
   }
};


#endif /* TNLMESHENTITYID_H_ */
