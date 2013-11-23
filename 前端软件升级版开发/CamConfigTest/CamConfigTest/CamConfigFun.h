#ifndef CAM_CONFIG_FUN
#define  CAM_CONFIG_FUN

#ifdef NET_IO_NVR_API_DLL
#define NET_IO_NVR_API _declspec(dllexport)
#else
#define NET_IO_NVR_API _declspec(dllimport)
#endif

extern "C" 
{
	NET_IO_NVR_API void __stdcall ServerStart();
	NET_IO_NVR_API void __stdcall ServerStop();
}

#endif