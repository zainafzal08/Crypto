#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

unsigned char hexToDec(char* hex);
char* decToHex(unsigned char input);
char* hexXOR(char* first, char* second);
char* generateKey(char* s, int len);
void extendedHexToDec(char* hex, unsigned char* output, int bufferLen);
char* hexToString(char* input);
char* stringToHex(char* string);
void encryptXOR(char* inputFile, char* outputFile, char* key);

int main(int argc, char* argv[]){
	if(argc != 4){
		printf("ERROR: INVALID ARGUMENTS\n");
		printf("    ./run [key] [input file] [output file]\n");
		return EXIT_FAILURE;
	}
	char* key = argv[1];
	char* inputFile = argv[2];
	char* outputFile = argv[3];
	encryptXOR(inputFile, outputFile, key);
	return EXIT_SUCCESS;
}

//Xors two hex numbers together
char* hexXOR(char* first, char* second){
	int firstLen = strlen(first);
	int secondLen = strlen(second);
	if(firstLen != secondLen){
		printf("ERROR: BUFFERS OF UNEQUAL SIZE\n");
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

//generates a repeated key XOR encryption
char* generateKey(char* s, int len){
	char* key = malloc(len);
	int i = 0;
	int j = 0;
	char* hex = stringToHex(s);
	int hLen = strlen(hex);
	while(i < len){
		for(j=0; j < hLen; j++){
			if(i >= len) break;
			key[i] = hex[j];
			i++;
		}
	}
	return key;
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

//writes an encrypted output file
void encryptXOR(char* inputFile, char* outputFile, char* key){
	int keyLen = strlen(key);
	FILE* input = fopen(inputFile, "r");
	FILE* output = fopen(outputFile, "w");
	char c = fgetc(input);
	int i = 0;
	char* xorHex = malloc(3);
	char* tempHex = malloc(3); // so i can free in the loop
	while(c != EOF){
		free(tempHex);
		free(xorHex);
		tempHex = decToHex(c);
		xorHex = hexXOR(tempHex, decToHex(key[i]));
		if(i+1 < keyLen) i++;
		else i = 0;
		fwrite(xorHex, 1, 2, output);
		c = fgetc(input);
	}

	fclose(output);
	fclose(input);
	printf("\nCompleted\n");
}

