/***********************************************************************
* 
* 版权信息：Copyright (c) 2010, 杭州海康威视软件有限公司
* 
* 文件名称：jpegdec.h
* 文件标识：HIKVISION_JPGDEC_LIB_JPEGDEC_H_
* 摘    要：海康威视JPEG解码器接口头文件
*
* 当前版本：1.1.0
* 作    者：黄崇基
* 日    期：2011年8月29号
* 备    注：这是高德威定制的一个版本，增加一个在文件头中添加调试信息的接口
*           同时为了避免与SDK组的jpeg编码模块产生名字冲突，库接口名字加了前
*           缀“HKGDW”
*
* 当前版本：1.0.2
* 作    者：戚红命
* 日    期：2011年7月7号
* 备    注：码流大小按照实际输入来解码
*
* 当前版本：1.0.1
* 作    者：戚红命
* 日    期：2010年9月9号
* 备    注：支持Basline YUV422 JPEG图片解码
*
* 当前版本：1.0.0
* 作    者：戚红命
* 日    期：2010年8月5号
* 备    注：支持Baseline JPEG解码
************************************************************************
*/

#ifndef HIK_JPGDEC_LIB_JPEGDEC_H_
#define HIK_JPGDEC_LIB_JPEGDEC_H_

#ifdef __cplusplus
extern "C" {
#endif 


//状态码定义
#define HIK_JPGDEC_LIB_S_OK			0x00000001   //操作成功


//错误码定义
#define HIK_JPGDEC_LIB_E_NULL		0x80000001	 //内存操作遇到空指针

#define HIK_JPGDEC_LIB_E_DQT		0x80000010	 //错误的量化表
#define HIK_JPGDEC_LIB_E_DHT		0x80000011	 //错误的huffman码表
#define HIK_JPGDEC_LIB_E_LSEJPG		0x80000012	 //不支持LS-JPEG
#define HIK_JPGDEC_LIB_E_S0F5_15	0x80000013	 //不支持SOF5~SOF15
#define HIK_JPGDEC_LIB_E_APP        0x80000014	 //APP段解码出错
#define HIK_JPGDEC_LIB_E_NOSOS      0x80000015	 //SOS段不存在
#define HIK_JPGDEC_LIB_E_SIZE   	0x80000016	 //图像大小出错
#define HIK_JPGDEC_LIB_E_BITSP  	0x80000017	 //像素精度出错
#define HIK_JPGDEC_LIB_E_SOS    	0x80000018	 //错误的SOS
#define HIK_JPGDEC_LIB_E_SOF    	0x80000019	 //错误的SOF
#define HIK_JPGDEC_LIB_E_PIXFMT 	0x8000001A	 //错误的像素格式
#define HIK_JPGDEC_LIB_E_PRGJPG		0x8000001B	 //不支持Progressive DCT
#define HIK_JPGDEC_LIB_E_COM        0x8000001C   //COM信息出错


//像素格式定义
#define HIK_JPGDEC_PIXFMT_YUV420     1			//YUV 420格式
#define HIK_JPGDEC_PIXFMT_YUV440     2			//
#define HIK_JPGDEC_PIXFMT_YUV422     3			//
#define HIK_JPGDEC_PIXFMT_YUV444     4			//
#define HIK_JPGDEC_PIXFMT_GRAY8      5			//
#define HIK_JPGDEC_PIXFMT_UNKNOWN    0			//未知格式


#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
	typedef int HRESULT;
#endif // 



#ifndef _YUV_FRAME_
#define _YUV_FRAME_
	typedef struct
	{
		unsigned char *y;
		unsigned char *u;
		unsigned char *v;

	}YUV_FRAME;
#endif 

//JPEG解码器配置参数
typedef struct _HKGDW_JPEG_DECODER_PARAMETERS 
{
	unsigned int image_width;		//图像显示宽度
	unsigned int image_height;		//图像显示高度

	unsigned int   sdram_size;		//解码器需要的缓存大小
	unsigned char *sdram_buffer;    //解码器缓存地址

}HKGDW_JPEG_DECODER_PARAMETERS;


//JPEG图片信息
typedef struct _HKGDW_JPEG_IMAGE_INOF 
{
	unsigned int image_width;     //图像宽度
	unsigned int image_height;    //图像高度
	unsigned int pix_fmt;         //像素格式
	unsigned int nb_components;   //颜色分量数目
	unsigned int progressive_dct; //是否支持ProgressiveDCT

}HKGDW_JPEG_IMAGE_INOF;

//调试信息结构体
typedef struct _HKGDW_JPEG_COM_INFO 
{
    int   com_len;		  //调试信息长度，由库内部输出
    int   buffer_size;    //调试信息缓存大小，不能小于调试信息长度，如果缓存大小小于信息长度，库内会返回错误
    unsigned char *com_buffer;    //调试缓存地址，此缓存地址由库外部分配和释放

}HKGDW_JPEG_COM_INFO;

/******************************************************************************
* 功  能：获取JPEG图像信息
* 参  数：param - JPEG解码器初始化参数	[out]
*		   bits - 存放JPEG码流的缓存	[in]
*          bits_size - 码流大小			[in]
*         
* 返回值：成功返回HIK_JPGDEC_LIB_S_OK, 失败返回错误码
* 备  注：
* 修  改: 2010/8/9
******************************************************************************/
HRESULT HKGDW_JPGDEC_GetImageInfo(HKGDW_JPEG_IMAGE_INOF *param, unsigned char *bits, unsigned int bits_size);




/******************************************************************************
* 功  能：获取JPEG解码器所需缓存大小
* 参  数：param - JPEG解码器初始化参数	[in]
*         
* 返回值：成功返回HIK_JPGDEC_LIB_S_OK, 失败返回错误码
* 备  注：
* 修  改: 2010/8/9
******************************************************************************/
HRESULT HKGDW_JPGDEC_GetMemSize(HKGDW_JPEG_DECODER_PARAMETERS *param);



/******************************************************************************
* 功  能：创建JPEG解码器
* 参  数：param  - JPEG解码器初始化参数	[in]
*         handle - 指向解码器句柄       [out]
*         
* 返回值：成功返回HIK_JPGDEC_LIB_S_OK, 失败返回错误码
* 备  注：
* 修  改: 2010/8/9
******************************************************************************/
HRESULT HKGDW_JPGDEC_Create(HKGDW_JPEG_DECODER_PARAMETERS *param, void **handle);




/******************************************************************************
* 功  能：对一帧JPEG码流进行解码
* 参  数： handle	 - 解码器句柄			[in]
*		   bits		 - 存放JPEG码流的缓存	[in]
*          bits_size - 码流大小			    [in]
*		   frame	 - 指向输出的YUV帧缓存	[out]
*          com_info  - 调试信息             [out]
*         
* 返回值：成功返回HIK_JPGDEC_LIB_S_OK, 失败返回错误码
* 备  注：建议存放bits的缓存大小为(image_width * image_height * 3 / 2) 或更大
* 修  改: 2010/8/9
******************************************************************************/
HRESULT HKGDW_JPGDEC_Decode(void *handle, unsigned char *bits, unsigned int bits_size, YUV_FRAME *outfrm, HKGDW_JPEG_COM_INFO *com_info);




#ifdef __cplusplus
}
#endif

#endif