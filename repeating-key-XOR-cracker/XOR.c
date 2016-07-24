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
char* encryptRKXOR(char* input, char* key);
char* decryptRKXOR(char* input, char* key);
int getHammingDis(char* str1, char* str2);
int compareBits(unsigned char a, unsigned char b);

int main(int argc, char* argv[]){
	printf("%d\n", getHammingDis("this is a test", "wokka wokka!!!")); 
	return EXIT_SUCCESS;
}

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

int getHammingDis(char* str1, char* str2){
	if(strlen(str1) != strlen(str2)){
		printf("HAMMING: UNEQUAL STRING LENGTHS\n");
		return -1;
	}
	int len = strlen(str1);
	int i = 0;
	char c1 = 0;
	char c2 = 0;
	int distance = 0;
	while(i < len){
		c1 = str1[i];
		c2 = str2[i];
		distance = distance + compareBits(c1, c2);
		i++;
	}
	return distance;
}

int compareBits(unsigned char a, unsigned char b){
	int i = 0;
	unsigned char tempA = 0;
	unsigned char tempB = 0;
	unsigned char clearAllButLast = 1;
	int diff = 0;
	for(i=0; i < 8; i++){
		tempA = a >> i;
		tempB = b >> i;
		tempA = tempA & clearAllButLast;
		tempB = tempB & clearAllButLast;
		if(tempB != tempA) diff++;
	}
	return diff;
}

//cracks a given single char XOR encrypted hex
char* crackSKXOR(char* hex){
	int keyLen = strlen(hex);
	char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	int alphLen = strlen(alphabet);
	int i = 0;
	char* key = NULL;
	char* result = NULL;
	double currScore = 0;
	double maxScore = 0;
	char* bestMatch = NULL;
	char* resultString = NULL;
	char* bestMatchHex = NULL;
	char* bestMatchKey = NULL;

	char charString[2];
	charString[1] = '\0';
	//frequency analysis
	for(i=0; i < alphLen; i++){
		charString[0] = alphabet[i];
		key = generateKey(charString, keyLen);
		result = hexXOR(hex, key);
		resultString = hexToString(result);
		currScore = score(resultString);
		if (currScore > maxScore){
			maxScore = currScore;
			bestMatch = resultString;
			bestMatchHex = result;
			bestMatchKey = key;
		}
	}
	//return
	return bestMatch;
}

char* crackRKXOR(char* input){
	
}
