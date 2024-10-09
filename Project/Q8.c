//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"


//This function gets a PPM file in P3 format 
//The function creates a color image, converts it to a gray image, and created a PGM file in P2 format with the converted gray image.
// the file name will be the same as the received file name but the extention will be replaced with "pgm"
void convertPPMToPGM(char* fname) {
	unsigned short depth, stamWidth, stamHeight;
	char* stamMagicNumber, * grayName;
	GRAY_IMAGE* grayImg;
	FILE* fpColor, * fpGray;
	COLOR_IMAGE* colorImg;
	colorImg = readPPM(fname);
	stamMagicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(stamMagicNumber);
	grayImg = createGrayImageFromColorImage(colorImg);

	fpColor = fopen(fname, "r");
	checkMemoryAllocate(fpColor);
	headerValues(fpColor, stamMagicNumber, &stamWidth, &stamHeight, &depth);
	grayName = createFileName(fname, "pgm");
	fpGray = fopen(grayName, "w");
	checkMemoryAllocate(fpGray);
	writeHeaderToFile(fpGray, "P2", grayImg->cols, grayImg->rows, depth);
	writeBodyToFile(fpGray, grayImg);
	fclose(fpGray);
	fclose(fpColor);
	free(stamMagicNumber);
	free(grayName);
	freeColorImage(colorImg);
	freeGrayImage(grayImg);
}


//This function gets a pointer to a color image 
//The function creates a gray image from the RGB pixels in the color image, according to an alogorithm.
//The function returns the created gray image
GRAY_IMAGE* createGrayImageFromColorImage(COLOR_IMAGE* colorImg) {
	int i, j, grayValue, currR, currG, currB;
	GRAY_IMAGE* grayImg = (GRAY_IMAGE*)malloc(sizeof(GRAY_IMAGE));
	checkMemoryAllocate(grayImg);
	grayImg->rows = colorImg->rows;
	grayImg->cols = colorImg->cols;
	grayImg->pixels = (char**)malloc(grayImg->rows * sizeof(char*));
	checkMemoryAllocate(grayImg->pixels);
	for (i = 0; i < grayImg->rows; i++) {
		grayImg->pixels[i] = (char*)malloc(grayImg->cols * sizeof(char));
		checkMemoryAllocate(grayImg->pixels[i]);
		for (j = 0; j < grayImg->cols; j++) {
			currR = colorImg->pixels[i][j].r;
			currG = colorImg->pixels[i][j].g;
			currB = colorImg->pixels[i][j].b;
			grayValue = (int)((currR + currG + currB) / 3);
			grayImg->pixels[i][j] = grayValue;
		}
	}
	return grayImg;
}

//This function gets a file name and a new extention
//The function creates a new file name, with the name and adds the extention
//The function returns the new file name
char* createFileName(char* name, char* extension) {
	int len, i = 0, j = 0;
	len = strlen(name);
	bool found = false;
	char* newName = (char*)malloc((len + 1) * sizeof(char));
	checkMemoryAllocate(newName);
	while (i < len && !found) {
		if (name[i] == '.')
			found = true;
		newName[i] = name[i];
		i++;
	}
	for (i; i < len; i++, j++) {
		newName[i] = extension[j];
	}
	newName[len] = '\0';
	return newName;
}

//This function gets a file, a magic number, a width, a height and depth 
//The function adds the received values to the file, as a header.
void writeHeaderToFile(FILE* fp, char* magicNumber, int width, int height, int depth) {
	fprintf(fp, "#magic number:\n");
	fprintf(fp, "%s\n", magicNumber);
	fprintf(fp, "#width and height:\n");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "#depth:\n");
	fprintf(fp, "%d\n", depth);
}

//This function gets a file and a gray image
//The function adds pixels from the gray image to the file.
void writeBodyToFile(FILE* fp, GRAY_IMAGE* img) {
	int i, j;
	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++) {
			fprintf(fp, "%3d ", img->pixels[i][j]);
		}
		if (i != (img->rows - 1)) {
			fprintf(fp, "\n");
		}
	}
}
