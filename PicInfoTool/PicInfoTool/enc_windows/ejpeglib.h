/*
***********************************************************************
* 
* 版权信息：Copyright (c) 2011, 杭州海康威视数字技术有限公司
* 
* 文件名称：ejpeglib.h
* 文件标识：HIKVISION_JPEG_ENCODER_LIB
* 摘    要：海康威视 x86 JPGENC 编码库的接口函数执行文件
*
*
* 当前版本：1.3.0
* 作    者：黄崇基
* 日    期：2011年08月29号
* 备    注：这是高德威定制的一个版本，增加一个在文件头中添加调试信息的接口
*           同时为了避免与SDK组的jpeg编码模块产生名字冲突，库接口名字加了前
            缀“HKGDW”
*
* 当前版本：1.2.7
* 作    者：黄崇基
* 日    期：2011年05月26号
* 备    注：更改内部函数名字，防止与标准编码库（如ffmpeg）函数名字冲突
*
* 当前版本：1.2.6
* 作    者：戚红命, 黄崇基
* 日    期：2010年11月8号
* 备    注：x86平台优化版本
*
* 当前版本：1.2.5
* 作    者：戚红命
* 日    期：2010年10月20号
* 备    注：支持水印编码
*
* 当前版本：1.2.4
* 作    者：戚红命
* 日    期：2010年9月10号
* 备    注：修正quality在90以上图像编码发生锯齿现象的bug
*
*
*
************************************************************************
*/

#ifndef _HIK_JPGENC_LIB_H_
#define _HIK_JPGENC_LIB_H_

/******************************************************************************
* 作用域控制宏 (必须使用，以便C++语言能调用)
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
* 宏声明
******************************************************************************/


/* 状态码：小于零表示有错误(0x80000000开始)，零表示失败，大于零表示成功  */
#define HIK_JPGENC_LIB_S_OK			    1		        /* 成功		    */
#define HIK_JPGENC_LIB_S_FAIL			0		        /* 失败		    */
#define HIK_JPGENC_LIB_E_PARA_NULL		0x80000000	    /* 参数指针为空 */
#define HIK_JPGENC_LIB_E_MEM_OVER		0x80000001	    /* 内存溢出     */
#define HIK_JPGENC_LIB_E_WIDTH		    0x80000002	    /* 宽度错误，必须是16的整数倍 */
#define HIK_JPGENC_LIB_E_HEIGHT		    0x80000003	    /* 高度错误，必须是16的整数倍 */

	// 水印类型标记
#define HIK_WATERMARK_INFO          0x494d5748 // "HWMI": 海康格式
#define USR_WATERMARK_INFO          0x494d5755 // "UWMI"：用户自定义格式



/******************************************************************************
* 结构体声明
******************************************************************************/
#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef int HRESULT;
#endif // !_HRESULT_DEFINED

/* XXX结构体说明*/
typedef struct _HKGDW_JPGENC_PARAM_
{
	unsigned int width;		            // 图像宽度
	unsigned int height;		        // 图像高度
	unsigned int quality;			    // 压缩后的图像质量
	unsigned int insert_watermarker;    //是否添加水印信息

	void *       sdram_buf;				// 编码器可被CACHE的外部RAM缓存区地址  已经对齐
	unsigned int sdram_buf_size;		// 编码器所需可被CACHE的外部RAM缓存的大小，由JPGENC_GetMemSize填充
}HKGDW_JPGENC_PARAM;

// 帧数据指针(YUV 420格式)
#ifndef _YUV_FRAME_
#define _YUV_FRAME_
typedef struct
{
	unsigned char *y;
	unsigned char *u;
	unsigned char *v;
}YUV_FRAME;
#endif 

/* 帧编码参数 */
typedef struct _HKGDW_JPGENC_IMAGE_PARAM_ 
{
	YUV_FRAME    * frame;		// 帧数据指针(编码器中没有使用DMA,为了保证速度，应该分配在可被CACHE的内存中)
	unsigned int   width;		// 图像宽度
	unsigned int   height;		// 图像高度

	unsigned int   watermarker_len;
	unsigned char  *watermarker_data;

	unsigned int   comment_len;      
	unsigned char  *comment_data;

	unsigned char* bitstream;	// 码流输出缓冲区首指针
	unsigned int   size;		// 码流输出缓冲区的大小
	unsigned int   length;		// 输出码流长度(以字节为单位)
} HKGDW_JPGENC_IMAGE_PARAM;

/******************************************************************************
* 接口函数声明
******************************************************************************/
/******************************************************************************
* 功  能：获取所需内存大小
* 参  数：param - 参数结构指针
* 返回值：返回状态码
* 备  注：参数结构中 buffer_size变量用来表示所需内存大小
******************************************************************************/
HRESULT HKGDW_JPGENC_GetMemSize (HKGDW_JPGENC_PARAM *param); 

/******************************************************************************
* 功  能：创建XXX模块
* 参  数：param  	- 参数结构指针
*         **handle	- 返回XXX模块句柄
* 返回值：返回状态码
* 备  注：
******************************************************************************/
HRESULT HKGDW_JPGENC_Create (HKGDW_JPGENC_PARAM *param, void **handle); 

/******************************************************************************
*功能：设置压缩后的图像质量
*参数：param   - 编码器参数指针
*	   quality - 图像质量(范围：差 0 - 100 好),推荐默认值为75或80
* 返回值：返回状态码
* 备  注：当编码开始前和图像质量参数变化时，需要设置图像质量
* 说  明：JPGENC_COMPRESS_PARAM 中的参数，在JPGENC_SetQuality中被初始化
******************************************************************************/
HRESULT HKGDW_JPGENC_SetQuality (void *handle, int quality);

/******************************************************************************
* 功  能：编码一帧图像
* 参  数：param - 编码器参数指针
*         image - 当前帧参数指针
* 返回值：返回状态码
* 备  注：
******************************************************************************/
HRESULT HKGDW_JPGENC_Compress (void *handle, HKGDW_JPGENC_IMAGE_PARAM *image);

#ifdef __cplusplus
}
#endif 

#endif //_HIK_JPGENC_LIB_H_
