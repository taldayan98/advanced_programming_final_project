//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

//This function gets file name that contaion picture in formart PPM with magic number P3
//The function creates picture type COLOR_IMAGE (with width and hieght values from file) and add the pixels value to pixels array
//Return a pointer of color image
COLOR_IMAGE* readPPM(char* fname) {
	unsigned short width, height, depth;
	char* magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	FILE* fp;
	COLOR_IMAGE* img = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE)); 
	checkMemoryAllocate(img);
	fp = fopen(fname, "r");
	checkMemoryAllocate(fp);
	headerValues(fp, magicNumber, &width, &height, &depth);
	pointToFirstIndexInArray(fp);
	img->rows = height;
	img->cols = width;
	img->pixels = createColorImage(fp, height, width);
	fclose(fp);
	free(magicNumber);
	return img;
}

//This function gets pointer to file, rows and cols of the pixels array
//The function creates color image with the data in fp file 
//Update in each pixel the value - r,g,b
//return pointer to RGB array
RGB** createColorImage(FILE* fp, unsigned short rows, unsigned short cols) {
	int i, j;
	int r, g, b;
	RGB** img = (RGB**)malloc((rows) * sizeof(RGB*));
	checkMemoryAllocate(img);
	for (i = 0; i < rows; i++) {
		img[i] = (RGB*)malloc(cols * sizeof(RGB));
		checkMemoryAllocate(img[i]);
		for (j = 0; j < cols; j++) {
			fscanf(fp, "%d%d%d", &r, &g, &b);
			img[i][j].r = r;
			img[i][j].g = g;
			img[i][j].b = b;

		}
	}
	return img;

}
//This function gets file name that contaion picture in formart PPM with magic number P6 (binary)
//The function creates picture type COLOR_IMAGE (with width and hieght values from file) and add the pixels value to pixels array
//Return a pointer of color image
COLOR_IMAGE* readPPM_Bin(char* fname) {
	unsigned short width, height, depth;
	char* magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	FILE* fp;
	COLOR_IMAGE* img = (COLOR_IMAGE*)malloc(sizeof(COLOR_IMAGE)); 
	checkMemoryAllocate(img);
	fp = fopen(fname, "rb");
	checkMemoryAllocate(fp);
	headerValues(fp, magicNumber, &width, &height, &depth);
	pointToFirstIndexInArray(fp);
	img->rows = height;
	img->cols = width;
	img->pixels = createColorImage_Bin(fp, height, width);

	fclose(fp);
	free(magicNumber);
	return img;
}

//This function gets pointer to binary file, rows and cols of the pixels array
//The function creates color image with the data in fp file 
//Update in each pixel the value - r,g,b
//return pointer to RGB array

RGB** createColorImage_Bin(FILE* fp, unsigned short rows, unsigned short cols) {
	int i, j;
	unsigned char r, g, b;
	fseek(fp, 1, SEEK_CUR);
	RGB** img = (RGB**)malloc((rows) * sizeof(RGB*));
	checkMemoryAllocate(img);
	for (i = 0; i < rows; i++) {
		img[i] = (RGB*)malloc(cols * sizeof(RGB));
		checkMemoryAllocate(img[i]);
		for (j = 0; j < cols; j++) {
			fread(&r, sizeof(char), 1, fp);
			img[i][j].r = r;
			fread(&g, sizeof(char), 1, fp);
			img[i][j].g = g;
			fread(&b, sizeof(char), 1, fp);
			img[i][j].b = b;

		}
	}
	return img;
}
//This function gets pointer to COLOR_IMAGE type
//The function free the pixels array in COLOR_IMAGE
void freeColorImage(COLOR_IMAGE* colorImg) {
	int i;
	for (i = 0; i < colorImg->rows; i++) {
		free(colorImg->pixels[i]);
	}
	free(colorImg);
}