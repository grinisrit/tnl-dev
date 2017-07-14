/***************************************************************************
                          MIC.h  -  description
                                by hanouvit
                          -------------------
    begin                : Nov 7, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLMIC_H
#define TNLMIC_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <TNL/String.h>
#include <TNL/Assert.h>
#include <TNL/SmartPointersRegister.h>
#include <TNL/Timer.h>

#include <TNL/Devices/Cuda.h>
#include <TNL/Debugging/StackBacktrace.h>

namespace TNL {

namespace Devices {
    
//makra oblíbená v příkladech v přednáškách Intelu    
#define ALLOC alloc_if(1) //naalokuj promenou na zacatku offload  bloku -- default
#define FREE free_if(1) // smaz promenou na konci offload bloku -- default
#define RETAIN free_if(0) //nesmaz promenou na konci bloku
#define REUSE alloc_if(0) //nealokuj proměnnou na zacatku

//struktura která dovoluje kopírovat raw pointer z paměti MICu do RAM a zpět -- obcházení mapování proměnných
template< typename Type >
struct MICHider{
    Type *pointer;
};

//nafukovací struktura -- struktury kopítrovat lze, objekty nikoliv 
//objekt se dá nakopírovat do takovéto strukury a poslat na MIC
template <unsigned int VELIKOST>
struct MICStruct{
	uint8_t data[VELIKOST];
};

//Makra zpřehledňující pozdější kód
//verze kdy se objekt nakopíruje do nově alokované nafukovací struktury
#define TNLMICSTRUCT(bb,typ) Devices::MICStruct<sizeof(typ)> s ## bb; \
                             memcpy((void*)& s ## bb,(void*)& bb,sizeof(typ));
#define TNLMICSTRUCTOFF(bb,typ) s ## bb
#define TNLMICSTRUCTUSE(bb,typ) typ * kernel ## bb = (typ*) &s ## bb;
#define TNLMICSTRUCTALLOC(bb,typ) typ * kernel ## bb = (typ*) malloc (sizeof(typ)); \
                                memcpy((void*)kernel ## bb,(void*) & s ## bb, sizeof(typ));

//verze kdy se ukazatel na objekt reprezentuje jako ukazatel na pole uint_8
#define TNLMICHIDE(bb,typ) uint8_t * u ## bb=(uint8_t *)&bb; \
                           MICHider<typ> kernel ## bb;
#define TNLMICHIDEALLOCOFF(bb,typ) in(u ## bb:length(sizeof(typ))) out(kernel ## bb)
#define TNLMICHIDEALLOC(bb,typ) kernel ## bb.pointer=(typ*)malloc(sizeof(typ)); \
                                memcpy((void*)kernel ## bb.pointer,(void*)u ## bb,sizeof(typ));
#define TNLMICHIDEFREEOFF(bb,typ) in(kernel ## bb)
#define TNLMICHIDEFREE(bb,typ) free((void*)kernel ## bb.pointer

#define MICSupportMissingMessage \
   std::cerr << "The MIC support is missing in the source file " << __FILE__ << " at line " << __LINE__ << "..." << std::endl;

class MIC
{
   public:
   
        static String getDeviceType()
        {
            return String( "MIC" );
        }
        
#ifdef HAVE_MIC  
        
       //useful debuging -- but produce warning
       __cuda_callable__ static inline void CheckMIC(void)
       {
            #ifdef __MIC__
                    std::cout<<"ON MIC"<<std::endl;
            #else
                    std::cout<<"ON CPU" <<std::endl;
            #endif
        }
        
       
        //původní funkce kopírující na MIC  -- nepoužíváse
        template <typename TYP>
        static
        TYP * passToDevice(TYP &objektCPU)
        {
                uint8_t * uk=(uint8_t *)&objektCPU; 
                MICHider<TYP> ret;
                
                #pragma offload target(mic) in(uk:length(sizeof(TYP))) out(ret)
                {
                    ret.pointer=(TYP*)malloc(sizeof(TYP));
                    std::memcpy((void*)ret.pointer,(void*)uk,sizeof(TYP));
                }
                return ret.pointer;
                
                std::cout << "Někdo mně volá :-D" <<std::endl;
        }
        
        //původní funkce mazající z MIC -- nepoužíváse
        template <typename TYP>
        static
        void freeFromDevice(TYP *objektMIC)
        {
            MICHider<TYP> ptr;
            ptr.pointer=objektMIC;
            #pragma offload target(mic) in(ptr)
            {
                free((void*)ptr.pointer);
            }
        }
        
        static inline
        void CopyToMIC(void* mic_ptr,void* ptr,size_t size)
        {
            uint8_t image[size];
            std::memcpy((void*)&image,ptr,size);
            Devices::MICHider<void> hide_ptr;
            hide_ptr.pointer=mic_ptr;
            #pragma offload target(mic) in(hide_ptr) in(image) in(size)
            {
                std::memcpy((void*)hide_ptr.pointer,(void*)&image,size);
            }
        }

        static inline
        void* AllocMIC(size_t size)
        {
                Devices::MICHider<void> hide_ptr;
                #pragma offload target(mic) out(hide_ptr) in(size)
            {
                hide_ptr.pointer=malloc(size);
            }
                return hide_ptr.pointer;
        }

        static inline
        void FreeMIC(void* ptr)
        {
                Devices::MICHider<void> hide_ptr;
                hide_ptr.pointer=ptr;
                #pragma offload target(mic) in(hide_ptr)
                {
                        free(hide_ptr.pointer);
                }
        }
       
        
#endif
        
   static void insertSmartPointer( SmartPointer* pointer );
   
   static void removeSmartPointer( SmartPointer* pointer );
   
   // Negative deviceId means that CudaDeviceInfo::getActiveDevice will be
   // called to get the device ID.
   static bool synchronizeDevice( int deviceId = -1 );
   
   static Timer smartPointersSynchronizationTimer;
   
   protected:
   
   static SmartPointersRegister smartPointersRegister;

};

}//namespace Devices

}//namespace TNL





#endif /* TNLMIC_H */

