

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Oct 09 13:44:05 2013
 */
/* Compiler settings for .\SCCP.idl:
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

MIDL_DEFINE_GUID(IID, LIBID_SCCPLib,0x2C648E7C,0x84B3,0x4893,0x98,0xCB,0x3E,0x90,0xAF,0x89,0x77,0x86);


MIDL_DEFINE_GUID(IID, DIID__DSCCP,0x804DAB8E,0x7240,0x41B9,0xB1,0x12,0x9E,0x16,0xF7,0x7C,0xD8,0x12);


MIDL_DEFINE_GUID(IID, DIID__DSCCPEvents,0x81E3F915,0x054F,0x49B6,0xA3,0x68,0xFE,0x0B,0x12,0x3D,0x5D,0x9C);


MIDL_DEFINE_GUID(CLSID, CLSID_SCCP,0x7A8C7463,0x738C,0x4F63,0xAB,0x50,0x31,0x64,0x23,0x96,0x0F,0x6F);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



