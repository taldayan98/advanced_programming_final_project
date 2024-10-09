//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

//This function gets file name 
//The function convert compressed file to PGM file in format P2
//The function writes the header values at top and than the new pixels values
void convertCompressedImageToPGM(char* fname) {
	FILE* fpRead, * fpWrite;
	unsigned short rows, cols;
	unsigned char depth;
	int maxBits;
	char* name = createFileName(fname, "pgm");

	fpRead = fopen(fname, "rb");
	checkMemoryAllocate(fpRead);
	fpWrite = fopen(name, "w");
	checkMemoryAllocate(fpWrite);

	fread(&rows, sizeof(unsigned short), 1, fpRead);
	fread(&cols, sizeof(unsigned short), 1, fpRead);
	fread(&depth, sizeof(unsigned char), 1, fpRead);

	writeHeaderToFile(fpWrite, "P2", cols, rows, depth);

	maxBits = bitsSize(depth);

	decompressPixels(fpRead, fpWrite, rows, cols, maxBits);
	free(name);
	fclose(fpRead);
	fclose(fpWrite);
}
//This function gets 2 pointer to files, rows, cols and maxBits
//The function gets chars from fpRead file and creates the pixels values (takes every maxBits bit from char and creates pixel)
//The function writes to fpWrite file the new values of pixels
//write first the header values and than the pixels array
//bitsTaken for the number of bits taken from the char we get from fpread file
void decompressPixels(FILE* fpRead, FILE* fpWrite, int rows, int cols, int maxBits) {
	int i, j;
	unsigned char prev, temp, remain;
	int bitsTaken = 0;

	fread(&prev, sizeof(char), 1, fpRead);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			if (8 - bitsTaken > maxBits) { 
				//the number of bits remain in prev bigger than maxBits
				temp = prev;
				temp = temp << bitsTaken;
				temp = temp >> (BTYE_SIZE - maxBits);
				fprintf(fpWrite, "%3d ", temp);
				bitsTaken += maxBits;
			}
			else if (BTYE_SIZE - bitsTaken == maxBits) { //The number of bits remain in prev is equal to maxBits 
				temp = prev;
				temp = temp << bitsTaken;
				temp = temp >> (BTYE_SIZE - maxBits);
				fprintf(fpWrite, "%3d ", temp);
				fread(&prev, sizeof(char), 1, fpRead);
				bitsTaken = 0;

			}
			else { //The number of bits remain in prev smaller than maxBits
				temp = prev;
				temp = temp << bitsTaken;
				temp = temp >> (BTYE_SIZE - maxBits);
				fread(&prev, sizeof(char), 1, fpRead);
				remain = prev;
				remain = remain >> (BTYE_SIZE - (maxBits - BTYE_SIZE + bitsTaken)); 
				temp = temp | remain;
				fprintf(fpWrite, "%3d ", temp);
				bitsTaken = maxBits - (BTYE_SIZE - bitsTaken);
				prev = prev << bitsTaken;
				prev = prev >> bitsTaken;
			}

		}
		fprintf(fpWrite, "\n");
	}
}

