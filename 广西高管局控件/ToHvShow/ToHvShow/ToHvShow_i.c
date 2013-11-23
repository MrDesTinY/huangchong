

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Oct 16 15:33:14 2012
 */
/* Compiler settings for .\ToHvShow.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ToHvShowLib,0xD2FE37DE,0x9320,0x4DC7,0x9B,0x78,0xBC,0x77,0x6A,0x0D,0x69,0x25);


MIDL_DEFINE_GUID(IID, DIID__DToHvShow,0xC683820E,0x70D4,0x43E5,0xA0,0x3D,0xBB,0xB9,0x45,0xF2,0xEB,0x2A);


MIDL_DEFINE_GUID(IID, DIID__DToHvShowEvents,0xFB8A8CB8,0x869A,0x4457,0x8A,0x97,0xE6,0x8E,0x58,0xD2,0xA8,0xA0);


MIDL_DEFINE_GUID(CLSID, CLSID_ToHvShow,0xB38E8E73,0x1451,0x470F,0x8E,0x0F,0xE7,0x1B,0x9A,0xFB,0x3F,0x53);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



