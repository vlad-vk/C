// backclass.h: interface for the backclass class.
//
//////////////////////////////////////////////////////////////////////
#include "opcda.h"

#if !defined(AFX_BACKCLASS_H__78674CE8_EC6F_4613_9610_35EAA0170130__INCLUDED_)
#define AFX_BACKCLASS_H__78674CE8_EC6F_4613_9610_35EAA0170130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class backclass : public IOPCDataCallback  
{
public:
	backclass();
	virtual ~backclass();
		HRESULT STDMETHODCALLTYPE QueryInterface( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
		{
			return S_OK;
		}

        
        ULONG STDMETHODCALLTYPE AddRef( void) 
		{
			return S_OK;
		}
        
        ULONG STDMETHODCALLTYPE Release( void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnDataChange( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
		{
			return S_OK;
		}
        
        HRESULT STDMETHODCALLTYPE OnReadComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
		{
			return S_OK;
		}
        
        HRESULT STDMETHODCALLTYPE OnWriteComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMastererr,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
		{
			return S_OK;
		}
        
        HRESULT STDMETHODCALLTYPE OnCancelComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup)

		{
			return S_OK;
		}


};

#endif // !defined(AFX_BACKCLASS_H__78674CE8_EC6F_4613_9610_35EAA0170130__INCLUDED_)
