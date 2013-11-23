

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ToHvShowidl_h__
#define __ToHvShowidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DToHvShow_FWD_DEFINED__
#define ___DToHvShow_FWD_DEFINED__
typedef interface _DToHvShow _DToHvShow;
#endif 	/* ___DToHvShow_FWD_DEFINED__ */


#ifndef ___DToHvShowEvents_FWD_DEFINED__
#define ___DToHvShowEvents_FWD_DEFINED__
typedef interface _DToHvShowEvents _DToHvShowEvents;
#endif 	/* ___DToHvShowEvents_FWD_DEFINED__ */


#ifndef __ToHvShow_FWD_DEFINED__
#define __ToHvShow_FWD_DEFINED__

#ifdef __cplusplus
typedef class ToHvShow ToHvShow;
#else
typedef struct ToHvShow ToHvShow;
#endif /* __cplusplus */

#endif 	/* __ToHvShow_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ToHvShowLib_LIBRARY_DEFINED__
#define __ToHvShowLib_LIBRARY_DEFINED__

/* library ToHvShowLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_ToHvShowLib;

#ifndef ___DToHvShow_DISPINTERFACE_DEFINED__
#define ___DToHvShow_DISPINTERFACE_DEFINED__

/* dispinterface _DToHvShow */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DToHvShow;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C683820E-70D4-43E5-A03D-BBB945F2EB2A")
    _DToHvShow : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DToHvShowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DToHvShow * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DToHvShow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DToHvShow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DToHvShow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DToHvShow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DToHvShow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DToHvShow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DToHvShowVtbl;

    interface _DToHvShow
    {
        CONST_VTBL struct _DToHvShowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DToHvShow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DToHvShow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DToHvShow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DToHvShow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DToHvShow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DToHvShow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DToHvShow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DToHvShow_DISPINTERFACE_DEFINED__ */


#ifndef ___DToHvShowEvents_DISPINTERFACE_DEFINED__
#define ___DToHvShowEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DToHvShowEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DToHvShowEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FB8A8CB8-869A-4457-8A97-E68E58D2A8A0")
    _DToHvShowEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DToHvShowEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DToHvShowEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DToHvShowEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DToHvShowEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DToHvShowEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DToHvShowEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DToHvShowEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DToHvShowEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DToHvShowEventsVtbl;

    interface _DToHvShowEvents
    {
        CONST_VTBL struct _DToHvShowEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DToHvShowEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DToHvShowEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DToHvShowEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DToHvShowEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DToHvShowEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DToHvShowEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DToHvShowEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DToHvShowEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ToHvShow;

#ifdef __cplusplus

class DECLSPEC_UUID("B38E8E73-1451-470F-8E0F-E71B9AFB3F53")
ToHvShow;
#endif
#endif /* __ToHvShowLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


