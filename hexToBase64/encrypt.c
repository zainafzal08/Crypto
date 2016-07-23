#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void hexTo64(char* input);
char decTo64(int num);
unsigned char hexToDec(char* hex);

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("ERROR: NOT ENOUGH ARGUMENTS\n");
		return EXIT_FAILURE;
	}
	char* input = argv[1];
	hexTo64(input);
}

void hexTo64(char* input){
	//assuming valid hexadecimal input
	//assuming input is valid and a multiple of 24 bits
	int inputLen = strlen(input);
	int bitNum = inputLen * 4;
	if(bitNum % 24 != 0){
		printf("ERROR: BITNUM %% 24 != 0\n");
		return;
	}
	if(inputLen % 2 != 0){
		printf("ERROR: INPUTLEN %% 24 != 0\n");
		return;
	}
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
		printf("%c", decTo64(first));

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
		printf("%c", decTo64(decHex));

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
		printf("%c", decTo64(decHex));
		//read in temp as 6 bit block
		printf("%c", decTo64(temp));
		//progress
		index = index + 6;
	}
	printf("\n");
}

char decTo64(int num){
	char* matrix = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	return matrix[num];
}

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