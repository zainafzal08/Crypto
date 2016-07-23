#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

unsigned char hexToDec(char* hex);
char* decToHex(unsigned char input);
char* hexXOR(char* first, char* second);
char* generateKey(char c, int len);
void extendedHexToDec(char* hex, unsigned char* output, int bufferLen);
void crackXOR(char* hex);
char* hexToString(char* input);
double score(char* string);
double scoreChar(char c);
int isSingleXOR(char* hex, char* result);
int printable(char* string);

//reads in a set of hashes from hashes.txt and 
//cracks all of them assuming single char XOR encryption
//will output the decryptions that are most 
//likey to be messages.
int main(int argc, char* argv[]){
	FILE* input = fopen("hashes.txt", "r");
	char line[61];
	line[60] = '\0';
	char c = fgetc(input);
	int i = 0;
	while(c != EOF){
		while(i < 60){
			line[i] = c;
			c = fgetc(input);
			i++;
		}
		c = fgetc(input);
		i = 0;
		crackXOR(line);
	}

	fclose(input);
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

//generates a single char key for XOR encryption
char* generateKey(char c, int len){
	char* key = malloc(sizeof(char) * len);
	int i = 0;
	char* hex = decToHex(c);
	for(i=0; i < len; i = i + 2){
		key[i] = hex[0];
		key[i+1] = hex[1];
	}
	free(hex);
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

//cracks a given single char XOR encrypted hex
void crackXOR(char* hex){
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
	//frequency analysis
	for(i=0; i < alphLen; i++){
		key = generateKey(alphabet[i], keyLen);
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
	if(printable(bestMatch)) printf("BEST MATCH: %s\n", bestMatch);
}

//scores a string via frequency analysis
//determines how likely a string is to be
//a message and not random junk
double score(char* string){
	int len = strlen(string);
	int i = 0;
	double score = 0.000000;
	for(i=0; i < len; i++){
		score = score + scoreChar(string[i]);
	}
	return score;
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

//scores a charecter for frequency analysis. 
double scoreChar(char c){
	if(c >= 65 && c <= 90){
		c = c - 32;
	}
	switch(c){
	    case 'a': return 0.0651738; break;
	    case 'b': return 0.0124248; break;
	    case 'c': return 0.0217339; break;
	    case 'd': return 0.0349835; break;
	    case 'e': return 0.1041442; break;
	    case 'f': return 0.0197881; break;
	    case 'g': return 0.0158610; break;
	    case 'h': return 0.0492888; break;
	    case 'i': return 0.0558094; break;
	    case 'j': return 0.0009033; break;
	    case 'k': return 0.0050529; break;
	    case 'l': return 0.0331490; break;
	    case 'm': return 0.0202124; break;
	    case 'n': return 0.0564513; break;
	    case 'o': return 0.0596302; break;
	    case 'p': return 0.0137645; break;
	    case 'q': return 0.0008606; break;
	    case 'r': return 0.0497563; break;
	    case 's': return 0.0515760; break;
	    case 't': return 0.0729357; break;
	    case 'u': return 0.0225134; break;
	    case 'v': return 0.0082903; break;
	    case 'w': return 0.0171272; break;
	    case 'x': return 0.0013692; break;
	    case 'y': return 0.0145984; break;
	    case 'z': return 0.0007836; break;
	    case ' ': return 0.1918182; break;
	   	default: return 0.0000000; break;
	}
	return 0.0000000;
}

//determines if a string is likely to be human created
//such as a password, or random junk
int printable(char* string){
	int len = strlen(string);
	int i = 0;
	while(i < len){
		if(string[i] == '\n') string[i] = '=';
		if(string[i] == '\b') return FALSE;
		if(string[i] == '\f') return FALSE;
		if(string[i] == '\a') return FALSE;
		if(string[i] == '\r') return FALSE;
		if(string[i] == '\r') return FALSE;
		if(string[i] == '\t') return FALSE;
		if(string[i] == '\v') return FALSE;
		if(string[i] == ']' || string[i] == '[') return FALSE;
		if(string[i] == '<' || string[i] == '>') return FALSE;
		if(string[i] == '~') return FALSE;
		if(string[i] == '@') return FALSE;
		if(string[i] == '+') return FALSE;
		if(string[i] == '-') return FALSE;
		if(string[i] == '/') return FALSE;
		if(string[i] == '\\') return FALSE;
		i++;
	}
	return TRUE;
}
