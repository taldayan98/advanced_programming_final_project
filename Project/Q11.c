//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"


//This function gets a PGM file name in P5 format 
//The function creates a gray image, converts it to a black and white image, 
// and created a PGM file in P5 format with the converted black and white pixels.
// the file name will be the same as the received file name with an addition of "bw"
void convertPGMToBW_Bin(char* fname) {
	unsigned short depth, stamWidth, stamHeight;
	char* magicNumber;
	FILE* fp = fopen(fname, "rb");
	checkMemoryAllocate(fp);
	GRAY_IMAGE* grayImg = readPGM_Bin(fname);
	magicNumber = (char*)malloc(3 * sizeof(char));
	checkMemoryAllocate(magicNumber);
	headerValues(fp, magicNumber, &stamWidth, &stamHeight, &depth);
	createBWFile_Bin(fname, grayImg, depth, magicNumber);
	fclose(fp);
	free(magicNumber);
	freeGrayImage(grayImg);
}

//This function gets a PGM file in P5 format, a gray image, a depth and a magic number
//The function creates 3 black and white PGM file in P5 format with different masks  
void createBWFile_Bin(char* fname, GRAY_IMAGE* grayImg, unsigned short depth, char* magicNumber) {
	int i;
	char* newName;
	char** mask;
	bool found;
	FILE* fp;
	for (i = 2; i <= 4; i++) {
		newName = createBWFileName(fname, (char)i + '0', ".bw.pgm");
		fp = fopen(newName, "wb");
		checkMemoryAllocate(fp);
		writeHeaderToFile(fp, magicNumber, grayImg->cols, grayImg->rows, 1);
		mask = createMask(i);
		found = writeBWBodyToFile_Bin(fp, grayImg, mask, i, depth);
		if (!found) {
			fseek(fp, 0, SEEK_SET);
			writeHeaderToFile(fp, magicNumber, grayImg->cols, grayImg->rows, 0);
		}
		freeCharTable(mask, i);
		free(newName);
		fclose(fp);
	}
}

//This function gets a file and a gray image, a mask, the mask size and depth
// The function gets the pixels from the gray image and converts them to black and white pixels, according to the mask
// and writes them to the file.
//The function returns a bool that indicates if the max value is white (1) or black (0).
bool writeBWBodyToFile_Bin(FILE* fp, GRAY_IMAGE* grayImg, char** mask, int maskSize, unsigned short depth) {
	int grayRange = depth / (maskSize * maskSize - 1);
	int i, j, maskRow, maskCol, grayShade;
	char white = 1, black = 0;
	bool found = false;

	for (i = 0; i < grayImg->rows; i++) {
		for (j = 0; j < grayImg->cols; j++) {
			maskRow = i % maskSize;
			maskCol = j % maskSize;
			grayShade = grayImg->pixels[i][j] / grayRange;
			if (grayShade > mask[maskRow][maskCol]) {
				fwrite(&white, sizeof(char), 1, fp);
				found = true;
			}
			else {
				fwrite(&black, sizeof(char), 1, fp);
			}
		}
	}
	return found;

}

