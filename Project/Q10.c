//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"


//This function gets a PPM file in P6 format
//The function creates a color image, converts it to a gray image, and created a PGM file in P5 format with the converted gray image.
//the file name will be the same as the received file name but the extention will be replaced with "pgm"
void convertPPMToPGM_Bin(char* fname) {
	unsigned short depth, stamWidth, stamHeight;
	char* stamMagicNumber, * grayName;
	GRAY_IMAGE* grayImg;
	FILE* fpColor, * fpGray;
	COLOR_IMAGE* colorImg;
	colorImg = readPPM_Bin(fname);
	stamMagicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(stamMagicNumber);
	grayImg = createGrayImageFromColorImage(colorImg);

	fpColor = fopen(fname, "rb");
	checkMemoryAllocate(fpColor);
	headerValues(fpColor, stamMagicNumber, &stamWidth, &stamHeight, &depth);
	grayName = createFileName(fname, "pgm");
	fpGray = fopen(grayName, "wb");
	checkMemoryAllocate(fpGray);
	writeHeaderToFile(fpGray, "P5", grayImg->cols, grayImg->rows, depth);
	writeBodyToFile_Bin(fpGray, grayImg);
	fclose(fpGray);
	fclose(fpColor);
	free(stamMagicNumber);
	free(grayName);
	freeGrayImage(grayImg);
	freeColorImage(colorImg);

}

//This function gets a file and a gray image
//The function adds pixels from the gray image to the binary file.
void writeBodyToFile_Bin(FILE* fp, GRAY_IMAGE* img) {
	int i, j;
	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++) {
			fwrite(&(img->pixels[i][j]), sizeof(unsigned char), 1, fp);
		}
	}
}