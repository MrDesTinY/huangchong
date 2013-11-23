/*
* cjpeg.c
*
* Copyright (C) 1991-1998, Thomas G. Lane.
* This file is part of the Independent JPEG Group's software.
* For conditions of distribution and use, see the accompanying README file.
*
* This file contains a command-line user interface for the JPEG compressor.
* It should work on any system with Unix- or MS-DOS-style command lines.
*
* Two different command line styles are permitted, depending on the
* compile-time switch TWO_FILE_COMMANDLINE:
*	cjpeg [options]  inputfile outputfile
*	cjpeg [options]  [inputfile]
* In the second style, output is always to standard output, which you'd
* normally redirect to a file or pipe to some other program.  Input is
* either from a named file or from standard input (typically redirected).
* The second style is convenient on Unix but is unhelpful on systems that
* don't support pipes.  Also, you MUST use the first style if your system
* doesn't do binary I/O to stdin/stdout.
* To simplify script writing, the "-outfile" switch is provided.  The syntax
*	cjpeg [options]  -outfile outputfile  inputfile
* works regardless of which command line style is used.
*/

//F:\yuv_file\uxga\uxga_isp_fflf.yuv MARLBOR 1600 1200 0 
//2448x2112.yuv 2448x2112 2448 2112 85 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ejpeglib.h"

unsigned __int64 g_startcount;
unsigned __int64 g_endcount;

int compare_result(unsigned char *p, int len, FILE *f)
{
	int i;
	unsigned char *t;
	unsigned int sum  = 0;

	t = malloc(len);

	if (t == NULL)
	{
		printf("could not malloc buffer in compare_result.\n");
		return 0;
	}

	fread(t, len, 1, f);

	for (i = 0; i < len; i++)
	{
		sum += abs(t[i] - p[i]);
	}

	if (sum > 0)
	{
		printf("different compare result: %d.\n", sum);
	}

	free(t);

	return 1;
}

//海康格式水印，长度为0x80
unsigned char g_hik_watermark[1000] = {0x48, 0x57, 0x4d, 0x49, 0x80, 0x00, 0x60, 0x60, 0x60, 0x60, 0x99, 0x00, 0x00, 0x00, 0x60, 0x61,
0x61, 0x61, 0x61, 0x61, 0x9a, 0x9b, 0x9c, 0x61, 0x61, 0x61, 0x62, 0x62, 0x61, 0x62, 0x61, 0x62,
0x65, 0x65, 0x61, 0x65, 0x62, 0x62, 0x65, 0x62, 0x61, 0x65, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
0x60, 0x60, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x62, 0x62, 0x61,
                                     0x62, 0x61, 0x62, 0x65, 0x65, 0x61 };

void ReadTimestampCounter(unsigned __int64 *count);

int main (int argc, char **argv)
{	
	HKGDW_JPGENC_PARAM enc_param;
	HKGDW_JPGENC_IMAGE_PARAM image;
	YUV_FRAME frame;
	void* handle;

	char input_file_name[256];
	char output_file_name[256];
	char output_main_name[256];

	int width;
	int height ;
	int quality;
	int frames = 0;
	int frame_num = 0;
	int size;
	int length;
	int i;
	
	FILE * input_file;
	FILE * output_file;
	FILE * ref_file;

	int tmp_time, tot_time = 0;
	unsigned __int64 start_count, end_count;

	int ret;
	
	if(argc <= 5) 
	{
		printf("usage: inputfile outputfile width height quality frames\n", argv[0]);
		return 0;
	}

	// get parameters
	sprintf(input_file_name, "%s", argv[1]);
	sprintf(output_main_name, "%s", argv[2]);
	width   = atoi(argv[3]);
	height  = atoi(argv[4]);
	quality = atoi(argv[5]);
	if(argc > 6)
		frames = atoi(argv[6]);

	/* Open the input file. */
	if ((input_file = fopen(input_file_name, "rb")) == NULL) 
	{
		printf("can't open %s\n", argv[1]);
		return 0;
	}

	if ((ref_file = fopen("MARLBOR_std.jpg", "rb")) == NULL)
	{
		printf("cloud not open reffile.\n");
		return 0;
	}
	

	if((width % 16) || (height % 16))
	{
		printf("Unsupported image format x=%d,y=%d, must be a multiple of 16", width, height);
		return 0;
	}

	if(frames <= 0) 
	{
		frames = 120;
	}
	printf("********************************************************************\n");
	printf("* input file : %s\n", input_file_name);
	printf("* output file: %s\n", output_main_name);
	printf("* width      : %d\n", width);
	printf("* height     : %d\n", height);
	printf("* quality    : %d\n", quality);
	printf("* frames     : %d\n", frames);
	printf("********************************************************************\n");

	printf("\n encode start...\n");

	// init buffer
	size = width * height * 3 /2;
	if((frame.y = malloc(size)) == NULL)
	{
		printf("\nERROR! image buffer malloc failed!");
		return 0;
	}
	frame.u = frame.y + width * height;
	frame.v = frame.y + width * height * 5 / 4;
    
	/*++++++++++++++++++++++++++++++++++++++++++++++++*/
	enc_param.width     = width;
    enc_param.height    = height;
    enc_param.quality   = quality;
	enc_param.insert_watermarker = 1;
    if(HKGDW_JPGENC_GetMemSize(&enc_param) != HIK_JPGENC_LIB_S_OK)
	{
        printf("\nJPGENC_GetMemSize ERROR!");
		return 0;    
	}

	if((enc_param.sdram_buf = malloc(enc_param.sdram_buf_size)) == NULL)
	{
		printf("\nERROR! sdram_buf malloc failed!");
		return 0;
	}

    HKGDW_JPGENC_Create(&enc_param, &handle);
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/	
	image.frame  = &frame;
	image.width  = width;
	image.height = height;
	image.size   = size;

	//HIK定义的水印格式为： 第0~4 byte为水印起始码，一般为0x494d5748（海康定义）或0x494d5755（用户定义）
	//                      第5~6 byte为水印长度
	image.watermarker_data = g_hik_watermark;
	image.watermarker_len  = *(unsigned short*)(&g_hik_watermark[4]);

	image.comment_data = (unsigned char*)malloc(1024); //调试信息
	image.comment_len  = 1024;  //调试信息长度，如果没有调试信息，长度一定要置0

	for (i = 0; i < 100; i++)
	{
		image.comment_data[i] = i;
	}



	if((image.bitstream = malloc(size)) == NULL)
	{
		printf("\nERROR! stream_buffer malloc failed!");
		return 0;
	}

	HKGDW_JPGENC_SetQuality(handle, quality);

	for(i = 0; i < 1; i ++)
	{
		if(!fread(frame.y, size, 1, input_file))
		{
			printf("reach the end of the file.\n");
			break;
		}


		if(!quality) HKGDW_JPGENC_SetQuality(handle, i%101);


		// compress one picture
		ReadTimestampCounter(&start_count);
		
		ret = HKGDW_JPGENC_Compress(handle, &image);
		if(ret != HIK_JPGENC_LIB_S_OK)
		{
			printf("\nERROR(0x%x)! jpeg_compress_data failed!", ret);
			break;;
		}
		ReadTimestampCounter(&end_count);
		tmp_time = (int)((end_count - start_count) / 2800);
		tot_time += tmp_time;

		/***************  change here for m-jpeg  *************************/
		length = image.length;
		// ouput one encoded picture
		sprintf(output_file_name, "out.jpg",output_main_name, i);
		if ((output_file = fopen(output_file_name, "wb")) == NULL) 
		{
			printf("\nERROR! can't open %s\n", output_main_name);
			break;
		}

		compare_result(image.bitstream, length, ref_file);

		if(!fwrite(image.bitstream, length, 1, output_file))
		{
			printf("\nERROR! output stream file failed!");
			fclose(output_file);
			break;
		}
		/* After finish_compress, we can close the output file. */
		fclose(output_file);
        /********************************************************/
		frame_num ++;
		printf("\n [%d]_%s size = %d, time = %d us", i%101, output_file_name, length, tmp_time);
	}

	fclose(ref_file);
	fclose(input_file);
	free(frame.y);
	free(enc_param.sdram_buf);
	free(image.bitstream);
	free(image.comment_data);

	/* And we're done! */
	if(frame_num > 0)
	{
		printf("\n总共压缩 %d 帧\n平均每帧 = %d us\n", frame_num, tot_time/frame_num);
	}

	printf("\n\n encode end...\n\n");
}

void ReadTimestampCounter(unsigned __int64 *count)
{ 
#ifdef _WIN64
	*count = 0;
#else 
#ifdef _WIN32
    unsigned __int64 c;
    __asm push eax
    __asm push edx
    __asm _emit 0x0f
    __asm _emit 0x31
    __asm mov dword ptr c,   eax
    __asm mov dword ptr c+4, edx
    __asm pop edx
    __asm pop eax
    *count = c;
#endif
#ifdef LINUX
	*count = 0;
#endif
#endif
}


