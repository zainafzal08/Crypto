#ifdef BASE
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BASE
#endif

#include "hex.h"
#include "base64.h"

char* hexTo64(char* input){
	//assuming valid hexadecimal input
	//assuming input is valid and a multiple of 24 bits
	int inputLen = strlen(input);
	int bitNum = inputLen * 4;
	
	if(bitNum % 24 != 0){
		printf("ERROR: BITNUM %% 24 != 0\n");
		return NULL;
	}
	if(inputLen % 2 != 0){
		printf("ERROR: INPUTLEN %% 2 != 0\n");
		return NULL;
	}

	char* output = malloc(inputLen/6 * 4 + 1);
	output[inputLen/6 * 4] = '\0';
	int outIndex = 0;
	//set up
	char hex[3];
	hex[2] = '\0';
	int index = 0;
	unsigned char decHex = 0;
	unsigned char getFinalTwo = 3;
	unsigned char getFinalFour = 15;
	unsigned char getFinalSix = 63;
	unsigned char temp = 0;
	unsigned char temp2 = 0;
	//read in 24 bit blocks
	while(input[index] != '\0'){
		decHex = 0;
		temp = 0;
		temp2 = 0;
		//get the first hex as a decimal num
		hex[0] = input[index];
		hex[1] = input[index+1];
		decHex = hexToDec(hex);

		//grab the last 2 bits
		temp = decHex & getFinalTwo;

		//get first 6 bits
		unsigned char first = decHex >> 2;
		output[outIndex] = decTo64(first);
		outIndex++;

		//get next hex
		hex[0] = input[index+2];
		hex[1] = input[index+3];
		decHex = hexToDec(hex);
		//get first 4 bits of dec hex
		//store last 4 bits in temp2
		temp2 = decHex & getFinalFour;
		//shift over by 4 to remove last 4 bits
		decHex = decHex >> 4;
		//replace first 4 bits (now 0000) with 00[temp]
		temp = temp << 4;
		decHex = decHex | temp;
		//read in this as the next 6 bit block
		output[outIndex] = decTo64(decHex);
		outIndex++;

		//get next hex
		hex[0] = input[index+4];
		hex[1] = input[index+5];
		decHex = hexToDec(hex);
		//get first 2 bits of dec hex
		//store last 6 bits in temp
		temp = decHex & getFinalSix;
		//shift over by 6 to remvoe last 6
		decHex = decHex >> 6;
		//repalce first 6 digits with 00[temp]
		temp2 = temp2 << 2;
		decHex = decHex | temp2;
		//read in as the next 6 bit block
		output[outIndex] = decTo64(decHex);
		outIndex++;
		//read in temp as 6 bit block
		output[outIndex] = decTo64(temp);
		outIndex++;
		//progress
		index = index + 6;
	}
	return output;
}

char decTo64(int num){
	char* matrix = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	return matrix[num];
}