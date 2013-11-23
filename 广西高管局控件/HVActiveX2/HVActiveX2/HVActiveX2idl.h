

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __HVActiveX2idl_h__
#define __HVActiveX2idl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DHVActiveX2_FWD_DEFINED__
#define ___DHVActiveX2_FWD_DEFINED__
typedef interface _DHVActiveX2 _DHVActiveX2;
#endif 	/* ___DHVActiveX2_FWD_DEFINED__ */


#ifndef ___DHVActiveX2Events_FWD_DEFINED__
#define ___DHVActiveX2Events_FWD_DEFINED__
typedef interface _DHVActiveX2Events _DHVActiveX2Events;
#endif 	/* ___DHVActiveX2Events_FWD_DEFINED__ */


#ifndef __HVActiveX2_FWD_DEFINED__
#define __HVActiveX2_FWD_DEFINED__

#ifdef __cplusplus
typedef class HVActiveX2 HVActiveX2;
#else
typedef struct HVActiveX2 HVActiveX2;
#endif /* __cplusplus */

#endif 	/* __HVActiveX2_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __HVActiveX2Lib_LIBRARY_DEFINED__
#define __HVActiveX2Lib_LIBRARY_DEFINED__

/* library HVActiveX2Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_HVActiveX2Lib;

#ifndef ___DHVActiveX2_DISPINTERFACE_DEFINED__
#define ___DHVActiveX2_DISPINTERFACE_DEFINED__

/* dispinterface _DHVActiveX2 */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DHVActiveX2;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1726300F-F81A-4CB8-A847-B8497293D6B3")
    _DHVActiveX2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DHVActiveX2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DHVActiveX2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DHVActiveX2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DHVActiveX2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DHVActiveX2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DHVActiveX2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DHVActiveX2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DHVActiveX2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DHVActiveX2Vtbl;

    interface _DHVActiveX2
    {
        CONST_VTBL struct _DHVActiveX2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DHVActiveX2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DHVActiveX2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DHVActiveX2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DHVActiveX2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DHVActiveX2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DHVActiveX2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DHVActiveX2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DHVActiveX2_DISPINTERFACE_DEFINED__ */


#ifndef ___DHVActiveX2Events_DISPINTERFACE_DEFINED__
#define ___DHVActiveX2Events_DISPINTERFACE_DEFINED__

/* dispinterface _DHVActiveX2Events */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DHVActiveX2Events;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7C6CF805-5D3F-4465-B2E1-48717CB90E0E")
    _DHVActiveX2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DHVActiveX2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DHVActiveX2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DHVActiveX2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DHVActiveX2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DHVActiveX2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DHVActiveX2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DHVActiveX2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DHVActiveX2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DHVActiveX2EventsVtbl;

    interface _DHVActiveX2Events
    {
        CONST_VTBL struct _DHVActiveX2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DHVActiveX2Events_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DHVActiveX2Events_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DHVActiveX2Events_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DHVActiveX2Events_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DHVActiveX2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DHVActiveX2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DHVActiveX2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DHVActiveX2Events_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_HVActiveX2;

#ifdef __cplusplus

class DECLSPEC_UUID("6335727F-F190-41DC-BC93-865866E85ED6")
HVActiveX2;
#endif
#endif /* __HVActiveX2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


