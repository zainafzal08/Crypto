#ifdef BASE
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BASE
#endif

#include "hex.h"
#include "repeatedCharXor.h"

//encrypts a text string with repeating key XOR
//outputs in hex
char* encryptRKXOR(char* input, char* key){
	int keyLen = strlen(key);
	int strIndex = 0;
	int sLen = strlen(input);
	char* output = malloc(sLen*2);
	int outIndex = 0;
	char c = input[strIndex];
	strIndex++;
	int i = 0;
	char* xorHex = malloc(3);
	char* tempHex = malloc(3); // so i can free in the loop
	while(c != '\0'){
		free(tempHex);
		free(xorHex);
		tempHex = decToHex(c);
		xorHex = hexXOR(tempHex, decToHex(key[i]));
		if(i+1 < keyLen) i++;
		else i = 0;
		output[outIndex] = xorHex[0];
		output[outIndex+1] = xorHex[1];
		outIndex = outIndex + 2;
		c = input[strIndex];
		strIndex++;
	}
	free(tempHex);
	free(xorHex);
	return output;
}

//takes in a hex input and decrypts it to text
char* decryptRKXOR(char* input, char* key){
	int keyLen = strlen(key);
	int outLen = strlen(input)/2;
	int i = 0;
	int inIndex = 0;
	int outIndex = 0;
	char* output = malloc(outLen);
	char* xorHex = malloc(3);
	char* tempHex = malloc(3); // so i can free in the loop
	tempHex[0] = input[inIndex];
	tempHex[1] = input[inIndex+1];
	tempHex[2] = '\0';
	inIndex = inIndex + 2;
	unsigned char c = 0;
	while(tempHex[0] != '\0'){
		free(xorHex);
		xorHex = hexXOR(tempHex, decToHex(key[i]));
		if(i+1 < keyLen) i++;
		else i = 0;
		c = hexToDec(xorHex);
		output[outIndex] = c;
		outIndex++;
		tempHex[0] = input[inIndex];
		tempHex[1] = input[inIndex+1];
		inIndex = inIndex + 2;
	}
	free(tempHex);
	return output;
}