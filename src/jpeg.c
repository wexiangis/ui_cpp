
#include "jpeglib.h"
#include <stdio.h>
#include <stdlib.h>

//return : file size
int enjpeg(char *fileOutput, unsigned char *image_buffer, int width, int height, int pw)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
	JSAMPROW row_pointer[1];// pointer to a single row
	int row_stride;			// physical row width in buffer

    // Initialize the JPEG decompression object with default error handling. 
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
    // 
	if ((outfile = fopen(fileOutput, "wb")) == NULL) {
	    printf("can't open %s\n", fileOutput);
	    return -1;
	}
	jpeg_stdio_dest(&cinfo, outfile);

    //压缩参数设置
	cinfo.image_width = width; 	// image width and height, in pixels
	cinfo.image_height = height;
	cinfo.input_components = pw;// # of color components per pixel
	cinfo.in_color_space = JCS_RGB; // colorspace of input image
	jpeg_set_defaults(&cinfo);

    //开始压缩
    jpeg_start_compress(&cinfo, TRUE);

    //逐行扫描数据
	row_stride = width * pw;
	while (cinfo.next_scanline < cinfo.image_height) {
        // printf("next_scanline = %d\r\n", cinfo.next_scanline);
	    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
	    jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

    //
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    //
    fclose(outfile);

    return 0; 
}

//return : RGB array
unsigned char *dejpeg(char *fileInput, int *retBuffSize, int *width, int *height, int *pw)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
	FILE *infile;
    JSAMPARRAY buffer;
    //
    int count = 0, lineLen;
    unsigned char *retBuff;

    // Initialize the JPEG decompression object with default error handling. 
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    //
	if ((infile = fopen(fileInput, "rb")) == NULL) {
	    fprintf(stderr, "can't open %s\n", fileInput);
	    return NULL;
	}
	jpeg_stdio_src(&cinfo, infile);
    
    //
    jpeg_read_header(&cinfo, TRUE);

    //
    jpeg_start_decompress(&cinfo);

    //得到图片基本参数
    if(width) *width = cinfo.output_width;
    if(height) *height = cinfo.output_height;
    if(pw) *pw = cinfo.output_components;
    if(retBuffSize) *retBuffSize = 
        cinfo.output_width*
        cinfo.output_height*
        cinfo.output_components;

    //计算图片RGB数据大小,并分配内存
    retBuff = (unsigned char *)calloc(
        cinfo.output_width*
        cinfo.output_height*
        cinfo.output_components + 1, 1);

    //Process data
    count = 0;
    lineLen = cinfo.output_width*cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, 
        JPOOL_IMAGE, lineLen, 1);
    //
    while (cinfo.output_scanline < cinfo.output_height) {
        // printf("output_scanline = %d\r\n", cinfo.output_scanline);
        buffer[0] = &retBuff[count];
        jpeg_read_scanlines(&cinfo, buffer, 1);//读取一行数据
        count += lineLen;
    }

    //
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    //
    fclose(infile);

    return retBuff; 
}
