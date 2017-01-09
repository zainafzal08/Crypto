#ifdef BASE
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define BASE
#endif

#include "hex.h"
#include "singleCharXor.h"
#include "repeatedCharXor.h"
#include "base64.h"

#define TRUE 1
#define FALSE 0

typedef struct _Hamming{
	int dis;
	int keyLen;
}Hamming;

int getHammingDis(char* str1, char* str2);
int compareBits(unsigned char a, unsigned char b);
char* crackRKXOR(char* input);
void runTests();
void pushArray(Hamming* array, int num, int index, int size, int keyLen);
void pushIfSmall(Hamming* array, int keyLen, int num, int size);
char* getSubString(char* string, int len, int index);
void initilaiseHammingArray(Hamming* smallestHamming, int size);

int main(int argc, char* argv[]){
	runTests();
	printf("%s\n", crackRKXOR("AABBCCDDEE"));
	return EXIT_SUCCESS;
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

char* crackRKXOR(char* input){
	//3 smallest hamming distances
	Hamming smallestHamming[3];
	initilaiseHammingArray(smallestHamming, 3);
	//guess a key length
	int keyLength = 2;
	int currHamDis = 0;
	int currKeyLen = keyLength;
	char* temp1 = NULL; 
	char* temp2 = NULL;
	for(currKeyLen = keyLength; currKeyLen <= 40; currKeyLen++){
		temp1 = getSubString(input, currKeyLen, 0);
		temp2 = getSubString(input, currKeyLen, currKeyLen);
		if(temp1 != NULL && temp2 != NULL){
			currHamDis = getHammingDis(temp1, temp2);
			//see if it is small enough
			pushIfSmall(smallestHamming, currHamDis, 3, currKeyLen);
		}
	}
	return "LOL";
}

void runTests(){
	printf("running tests");
	assert(getHammingDis("this is a test", "wokka wokka!!!") == 37);
	printf(".");
	assert(strcmp(crackSKXOR("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"), "Cooking MC's like a pound of bacon") == 0);
	printf(".");
	assert(strcmp(hexTo64("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"), "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t") == 0);
	printf(".");
	assert(strcmp(hexXOR("1c0111001f010100061a024b53535009181c", "686974207468652062756c6c277320657965"), "746865206b696420646f6e277420706c6179") == 0);
	printf(".");
	Hamming array[3];
	initilaiseHammingArray(array, 3);
	pushIfSmall(array, 32, 10, 3);
	pushIfSmall(array, 40, 500, 3);
	pushIfSmall(array, 6, 3, 3);
	pushIfSmall(array, 3, 2, 3);
	assert(array[0].dis == 2 && array[0].keyLen == 3);
	assert(array[1].dis == 3 && array[1].keyLen == 6);
	assert(array[2].dis == 10 && array[2].keyLen == 32);
	printf(".");
	assert(strcmp(getSubString("lmao really", 3, 0), "lma") == 0);
	assert(strcmp(getSubString("lmao really", 3, 3), "o r") == 0);
	printf("\nall tests complete\n");
}

void pushIfSmall(Hamming* array, int keyLen, int num, int size){
	int i = 0;
	for(i=0; i < 3; i++){
		if(array[i].dis > num){
			pushArray(array, num, i, size, keyLen);
			break;
		}
	}
}

void pushArray(Hamming* array, int num, int index, int size, int keyLen){
	int i = 0;
	for(i=size-1; i >= index; i--){
		if(i-1 < 0) break;
		array[i].dis = array[i-1].dis;
		array[i].keyLen = array[i-1].keyLen;
	}
	array[index].dis = num;
	array[index].keyLen = keyLen;
}

char* getSubString(char* string, int len, int index){
	int strLen = strlen(string);
	if(index+len > strLen) return NULL;
	char* output = malloc(len+1);
	int i = index;
	int j = 0;
	for(i=index; i < index+len; i++){
		output[j] = string[i];
		j++;
	}
	output[len] = '\0';
	return output;
}

void initilaiseHammingArray(Hamming* smallestHamming, int size){
	int i = 0;
	for(i=0; i < size; i++){
		smallestHamming[i].dis = 100;
		smallestHamming[i].keyLen = 0;
	}
}
