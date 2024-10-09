//Tal Dayan - 207378639
//Amit Neeman - 207388794

#include "header.h"

//This function gets file name, pointer to grey image and max grey level
//The function creates binary file with the information from grey image  
//The information save in file in compressed way (meaning each pixel pronounce with maxBits)
//the calculation for each pixel is ((pixelValue * maxGrayLevel)\255) , (maxGrayLevel gets from user)
void saveCompressed(char* fname, GRAY_IMAGE* image, unsigned char maxGrayLevel) {
	int maxBits;
	FILE* fp;
	maxBits = bitsSize(maxGrayLevel);

	fp = fopen(fname, "wb");
	checkMemoryAllocate(fp);

	fwrite(&(image->rows), sizeof(unsigned short), 1, fp);
	fwrite(&(image->cols), sizeof(unsigned short), 1, fp);
	fwrite(&maxGrayLevel, sizeof(unsigned char), 1, fp);

	compressPixels(fp, image, maxGrayLevel, maxBits);
	fclose(fp);
}

//This function gets pointer to file, pointer to grey image, maxGrayLevel and maxBits
//This function pronounces each pixel with maxBits od bits 
//The calculation for each pixel is ((pixelValue * maxGrayLevel)\255) 
//Write each pixel to file fp
//EmptyBitsInByte for number of free bits to write in char
//putInByte for the number of bits that alreay wrote
void compressPixels(FILE* fp, GRAY_IMAGE* image, unsigned char maxGrayLevel, int maxBits) {
	int i, j;
	unsigned char prev, currPixel, tmpPixel;
	int putInByte = 0, EmptyBitsInByte = BTYE_SIZE;

	prev = image->pixels[0][0];
	prev = (unsigned char)floor((prev * maxGrayLevel) / 255);
	prev = prev << (BTYE_SIZE - maxBits);
	putInByte = maxBits;
	EmptyBitsInByte = BTYE_SIZE - maxBits;

	for (i = 0; i < image->rows; i++) {
		for (j = 0; j < image->cols; j++) {
			if (i != 0 || j != 0) {

				currPixel = image->pixels[i][j];
				currPixel = (unsigned char)floor((currPixel * maxGrayLevel) / 255);

				if (EmptyBitsInByte > maxBits) {
					//this case we have space in prev to put full pixel (curPixel)
					currPixel = currPixel << (EmptyBitsInByte - maxBits);
					EmptyBitsInByte = EmptyBitsInByte - maxBits;
					putInByte = putInByte + maxBits;
					prev = prev | currPixel;
				}
				else if (EmptyBitsInByte == maxBits) {
					//finish char (8 bits)
					prev = prev | currPixel;
					fwrite(&prev, sizeof(char), 1, fp);
					prev = 0x00;
					EmptyBitsInByte = BTYE_SIZE;
					putInByte = 0;
				}
				else { //EmptyBitsInByte < maxBits
					tmpPixel = currPixel << (BTYE_SIZE - (maxBits - EmptyBitsInByte));
					currPixel = currPixel >> (maxBits - EmptyBitsInByte);
					prev = prev | currPixel;

					fwrite(&prev, sizeof(char), 1, fp); //prev is full, need to write to file
					prev = tmpPixel;
					putInByte = maxBits - EmptyBitsInByte;
					EmptyBitsInByte = BTYE_SIZE - putInByte;
				}
			}

		}
	}
	//write the last pixel if putInByte != BTYE_SIZE (with 0 at end to complete to BYTE_SIZE)
	if (prev != 0) {
		fwrite(&prev, sizeof(char), 1, fp);
	}
}

//This function gets maxGrayLevel
//The function calculates the number of bits maxGrayLevel is taken
//return a number between 1-8
int bitsSize(unsigned char maxGrayLevel) {
	int highRange = 1, power = 0;
	while (maxGrayLevel >= highRange) {
		power++;
		highRange *= 2;
	}
	return power;
}

