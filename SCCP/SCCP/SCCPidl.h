

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SCCPidl_h__
#define __SCCPidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSCCP_FWD_DEFINED__
#define ___DSCCP_FWD_DEFINED__
typedef interface _DSCCP _DSCCP;
#endif 	/* ___DSCCP_FWD_DEFINED__ */


#ifndef ___DSCCPEvents_FWD_DEFINED__
#define ___DSCCPEvents_FWD_DEFINED__
typedef interface _DSCCPEvents _DSCCPEvents;
#endif 	/* ___DSCCPEvents_FWD_DEFINED__ */


#ifndef __SCCP_FWD_DEFINED__
#define __SCCP_FWD_DEFINED__

#ifdef __cplusplus
typedef class SCCP SCCP;
#else
typedef struct SCCP SCCP;
#endif /* __cplusplus */

#endif 	/* __SCCP_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SCCPLib_LIBRARY_DEFINED__
#define __SCCPLib_LIBRARY_DEFINED__

/* library SCCPLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_SCCPLib;

#ifndef ___DSCCP_DISPINTERFACE_DEFINED__
#define ___DSCCP_DISPINTERFACE_DEFINED__

/* dispinterface _DSCCP */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSCCP;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("804DAB8E-7240-41B9-B112-9E16F77CD812")
    _DSCCP : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSCCPVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSCCP * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSCCP * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSCCP * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSCCP * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSCCP * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSCCP * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSCCP * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSCCPVtbl;

    interface _DSCCP
    {
        CONST_VTBL struct _DSCCPVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSCCP_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSCCP_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSCCP_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSCCP_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSCCP_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSCCP_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSCCP_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSCCP_DISPINTERFACE_DEFINED__ */


#ifndef ___DSCCPEvents_DISPINTERFACE_DEFINED__
#define ___DSCCPEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSCCPEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DSCCPEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("81E3F915-054F-49B6-A368-FE0B123D5D9C")
    _DSCCPEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSCCPEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSCCPEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSCCPEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSCCPEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSCCPEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSCCPEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSCCPEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSCCPEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSCCPEventsVtbl;

    interface _DSCCPEvents
    {
        CONST_VTBL struct _DSCCPEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSCCPEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSCCPEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSCCPEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSCCPEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSCCPEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSCCPEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSCCPEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSCCPEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SCCP;

#ifdef __cplusplus

class DECLSPEC_UUID("7A8C7463-738C-4F63-AB50-316423960F6F")
SCCP;
#endif
#endif /* __SCCPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


