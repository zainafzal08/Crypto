#ifdef BASE
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BASE
#endif

#include "hex.h"

//Xors two hex numbers together
char* hexXOR(char* first, char* second){
	int firstLen = strlen(first);
	int secondLen = strlen(second);
	if(firstLen != secondLen){
		printf("HEX_XOR: BUFFERS OF UNEQUAL SIZE\n");
		printf("   %s\n", first);
		printf("   %s\n", second);
		return NULL;
	}
	int bufferLen = firstLen / 2;
	unsigned char hexOne[bufferLen];
	unsigned char hexTwo[bufferLen];

	extendedHexToDec(first, hexOne, bufferLen);
	extendedHexToDec(second, hexTwo, bufferLen);

	int i = 0;
	unsigned char result[bufferLen];
	for(i=0; i < bufferLen; i++){
		result[i] = hexOne[i] ^ hexTwo[i];
	}

	char* output = malloc(firstLen + 1);
	char* tempHex = NULL;
	int index = 0;
	for(i=0; i < firstLen; i = i + 2){
		tempHex = decToHex(result[index]);
		output[i] = tempHex[0];
		output[i+1] = tempHex[1];
		index++;
	}
	output[firstLen] = '\0';
	free(tempHex);
	return output;
}

//single hex to dec converter
unsigned char hexToDec(char* hex){
	//this is prob like the least efficient way to do this 
	//lmao it works though
	char* matrix = "0123456789ABCDEF";
	char* matrixU = "0123456789abcdef";
	unsigned char result = 0;
	int i = 0;
	for(i=0; i < 16; i++){
		if(matrix[i] == hex[1] || matrixU[i] == hex[1]){
			result = i;
			break;
		}
	}

	for(i=0; i < 16; i++){
		if(matrix[i] == hex[0] || matrixU[i] == hex[0]){
			result = result + 16*i;
			break;
		}
	}

	return result;
}

//single decimal to hex converter
char* decToHex(unsigned char input){
	char* matrix = "0123456789abcdef";
	char* hex = malloc(3);
	hex[2] = '\0';
	//get first hex value
	unsigned char firstHex = input >> 4;
	hex[0] = matrix[firstHex];
	//get second hex value
	unsigned char secondHex = input << 4;
	secondHex = secondHex >> 4;
	hex[1] = matrix[secondHex];
	return hex;
}


void extendedHexToDec(char* hexIn, unsigned char* output, int bufferLen){
	int i = 0;
	int index = 0;
	char hex[3];
	hex[2] = '\0';
	for(i=0; i < bufferLen; i++){
		hex[0] = hexIn[index];
		hex[1] = hexIn[index+1];
		index = index + 2;
		output[i] = hexToDec(hex);
	}
}

//takes in hex input and converts to text (ASCII)
char* hexToString(char* input){
	char hex[3]; hex[2] = '\0';
	int index = 0;
	int i = 0;
	char* output = malloc(strlen(input)/2 + 1);
	while(index < strlen(input)){
		hex[0] = input[index];
		hex[1] = input[index+1];
		index = index + 2;
		output[i] = hexToDec(hex);
		i++;
	}
	output[strlen(input)/2] = '\0';
	return output;
}

//takes in a string and converts to a hex representation
char* stringToHex(char* string){
	int sLen = strlen(string);
	int hLen = 2*sLen;
	char* hex = malloc(hLen);
	int i = 0;
	int hexIndex = 0;
	char* tempHex = malloc(3); //just so i can put a free into the loop
	for(i=0; i < sLen; i++){
		free(tempHex);
		tempHex = decToHex(string[i]);
		hex[hexIndex] = tempHex[0];
		hex[hexIndex+1] = tempHex[1];
		hexIndex = hexIndex + 2;
	}
	free(tempHex);
	return hex;
}