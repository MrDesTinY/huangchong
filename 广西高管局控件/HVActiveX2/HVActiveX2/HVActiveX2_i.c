

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Sep 28 19:14:38 2012
 */
/* Compiler settings for .\HVActiveX2.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_HVActiveX2Lib,0x4157D2EB,0x7391,0x4182,0xA5,0x2A,0x0A,0x55,0x20,0x5D,0xD4,0x7A);


MIDL_DEFINE_GUID(IID, DIID__DHVActiveX2,0x1726300F,0xF81A,0x4CB8,0xA8,0x47,0xB8,0x49,0x72,0x93,0xD6,0xB3);


MIDL_DEFINE_GUID(IID, DIID__DHVActiveX2Events,0x7C6CF805,0x5D3F,0x4465,0xB2,0xE1,0x48,0x71,0x7C,0xB9,0x0E,0x0E);


MIDL_DEFINE_GUID(CLSID, CLSID_HVActiveX2,0x6335727F,0xF190,0x41DC,0xBC,0x93,0x86,0x58,0x66,0xE8,0x5E,0xD6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



