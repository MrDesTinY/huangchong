/***********************************************************************
* 
* 版权信息：Copyright (c) 2010, 杭州海康威视软件有限公司
* 
* 文件名称：demo_decjpg.c
* 文件标识：HIKVISION_
* 摘    要：海康威视
*
*
* 当前版本：1.0
* 作    者：
* 日    期：2010年7月30号
* 备    注：
************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>


#include "common.h"
#include "global.h"

#pragma warning(disable: 4996)

#define _X86_ARC
#define PROCESSOR_FREQ  2600
#define MAX_TCNT 8	

uint64_t g_begtsc, g_endtsc;

uint64_t g_timer_beg[MAX_TCNT];
uint32_t g_timer_refs[MAX_TCNT] = {0};
uint64_t g_timer_cost[MAX_TCNT] = {0};



void read_process_tsc(uint64_t *count)
{ 
#ifdef _X86_ARC
	unsigned __int64 c;
		__asm 	push eax
		__asm 	push edx
		__asm 	cpuid
		__asm 	rdtsc
		__asm   mov dword ptr c    , eax
		__asm   mov dword ptr c + 4, edx
		__asm 	cpuid
		__asm 	pop  edx
		__asm 	pop  eax
		*count = c;
#endif
}

void DEMO_start_timer(int i)
{
	read_process_tsc(&g_timer_beg[i]);
}

void DEMO_stop_timer(int i)
{
	uint64_t tsc_end;

	read_process_tsc(&tsc_end);
	g_timer_cost[i] += tsc_end - g_timer_beg[i];
	g_timer_refs[i]++;
}

void DEMO_report_timer(unsigned int totcst)
{
	int i;
	unsigned int cost_us;

	for (i = 0; i < MAX_TCNT; i++)
	{
		if(g_timer_cost[i] > 0)
		{
			cost_us = (uint32_t)(g_timer_cost[i] / PROCESSOR_FREQ);
			printf("TIMER[%d]: cost=%dus, refs=%d, occupy=%.4f %%, ..........\n",
				i, cost_us, g_timer_refs[i], (cost_us*100.0)/totcst);
		}
	}
}

static int DEMO_decode_one_frame(uint8_t *jpgbits, int size, int dis_width, int dis_height, uint8_t *outbuf, uint8_t *refbuf, FILE *outfile);
extern int DEMO_test_libsrc(uint8_t *inpjpgs, uint8_t *cfgpath);



int main(int argc, char **argv)
{
	int32_t dec_reslut;
	int32_t jpgfile_size;
	int32_t image_width, image_height;
	
	FILE *inp_file = NULL;
	FILE *out_file = NULL;
	FILE *ref_file = NULL; 

	uint8_t *stream_buf = NULL;
	uint8_t *output_buf = NULL;
	uint8_t *reffile_buf= NULL;

	HKGDW_JPEG_IMAGE_INOF jpginfo;


	if (strcmp(argv[1], "-d") != 0)
	{
		printf("Invalid input parameter,\n");
		printf("decode jpeg: -d input_jpeg  output_yuv [reffile_yuv]\n");
		printf("test libsrc: -t config_file rand_file \n\n");
		
		return 0;
	}

	inp_file = fopen(argv[2], "rb");//第2个参数为输入文件路径
	out_file = fopen(argv[3], "wb");//第3个参数为输出文件路径
	
	if(argc > 4)
	ref_file = fopen(argv[4], "rb");//第4个参数为参考比较文件路径


	if(inp_file == NULL || out_file == NULL)
	{
		printf("DemoError: open file failed.\n");
		goto func_end;
	}

	if(ref_file == NULL)  printf("DemoWarning: open reference file failed.....\n");

	fseek(inp_file, 0, SEEK_END);

	jpgfile_size = ftell(inp_file);
	
	fseek(inp_file, 0, SEEK_SET);

	if((stream_buf = malloc(jpgfile_size * 3)) == NULL)
	{
		printf("DemoError: malloc stream buffer failed.\n");
		goto func_end;
	}

	
	memset(stream_buf, 0, jpgfile_size * 3);

	fread(stream_buf, jpgfile_size, 1, inp_file);					//读取jpeg码流

	if(HKGDW_JPGDEC_GetImageInfo(&jpginfo, stream_buf, jpgfile_size) < 0) //获取jpeg图片信息
	{
		printf("DemoError: could not get image info.\n");
		goto func_end;
	}

	image_height = jpginfo.image_height;
	image_width  = jpginfo.image_width;
	printf("ImageWidth : %d\nImageHeight: %d\n\n", image_width, image_height);

	reffile_buf  = malloc(image_width * image_height * 3);
	output_buf   = malloc(image_width * image_height * 3);

	if(reffile_buf == NULL || output_buf == NULL)
	{
		printf("DemoError: malloc reffile_buf/output_buf failed.\n");
		goto func_end;
	}

	if(ref_file)  fread(reffile_buf, image_height * image_width * 3 / 2, 1, ref_file);

	dec_reslut = DEMO_decode_one_frame(stream_buf, jpgfile_size, image_width, image_height, output_buf, reffile_buf, out_file);//对一帧JPEG图像进行解码

func_end:

	if(inp_file != NULL) fclose(inp_file);
	if(inp_file != NULL) fclose(out_file);
	if(ref_file != NULL) fclose(ref_file);

	if(stream_buf  != NULL) free(stream_buf);
	if(output_buf  != NULL) free(output_buf);
	if(reffile_buf != NULL) free(reffile_buf);

	printf("DemoMessage: Program end...\n\n");

	return 1;
}


/******************************************************************************
* Description:比较两个缓存数据的差异
*
* Comment    :返回差异大小
* Date       :2010/8/11
******************************************************************************/ 
int DEMO_compare(uint8_t *src0, uint8_t *src1, int size)
{
	int i;
	int totdiff = 0;

	for (i = 0; i < size; i += 4)
	{
		totdiff += abs(*((int *)(src0 + i)) - *((int *)(src1 + i))); 
	}

	return totdiff;
}

/******************************************************************************
* Description:对一帧JPEG图进行解码
*
* Comment    :
* Date       :2010/8/11
******************************************************************************/ 
int DEMO_decode_one_frame(uint8_t *jpgbits, int jpgsize, int dis_width, int dis_height, uint8_t *outbuf, uint8_t *refbuf, FILE *fout)
{
	uint32_t  timecost;
	int dec_result;

	HKGDW_JPEG_DECODER_PARAMETERS decparam, *dec = &decparam;

	void *handle;
	YUV_FRAME outframe;
    HKGDW_JPEG_COM_INFO com_info;
    int i;

	outframe.y = outbuf;
	outframe.u = outframe.y + dis_height * dis_width;
	outframe.v = outframe.u + dis_height * dis_width / 4;

	dec->image_height = dis_height;
	dec->image_width  = dis_width;

	HKGDW_JPGDEC_GetMemSize(dec);

	if((dec->sdram_buffer = malloc(dec->sdram_size)) == NULL)
	{
		printf("DemoError: malloc decoder buffer failed.\n");
		return -1;
	}

	HKGDW_JPGDEC_Create(dec, &handle); 

    com_info.com_buffer = (unsigned char*)malloc(1024 * 2);
    com_info.buffer_size = 1024 * 2;

	read_process_tsc(&g_begtsc);//读取处理器TSC, 用来统计时间用

	dec_result = HKGDW_JPGDEC_Decode(handle, jpgbits, jpgsize, &outframe, &com_info);

	read_process_tsc(&g_endtsc);//读取处理器TSC, 用来统计时间用

	timecost = (uint32_t)((g_endtsc - g_begtsc) / PROCESSOR_FREQ);

    if (com_info.com_len > 0)
    {
        printf("Com_info:\n");
        for (i = 0; i < com_info.com_len; i++)
        {
            printf("%d ", com_info.com_buffer[i]);
        }
        printf("\n");
    }

	if(dec_result > 0)
	{
		printf("\nDecode Successfully, TimeCost: %d us\n", timecost);
	}
	else
	{
		printf("\nDecode Faild, ErrorType:0x%x..............\n", dec_result);
	}


	if(DEMO_compare(outbuf, refbuf, dis_width * dis_height * 3 / 2))//最终解码结果与参考进行比较
	{
		printf("******************Compare Failed.\n");
	}
	else
	{
		printf("CompareOk..............................\n");
		
	}

	DEMO_report_timer(timecost);

	fwrite(outbuf, dis_width * dis_height * 3 / 2, 1, fout);
	
	free(dec->sdram_buffer);
    free(com_info.com_buffer);

	return dec_result;
}