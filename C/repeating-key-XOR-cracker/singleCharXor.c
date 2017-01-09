#ifdef BASE
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BASE
#endif

#include "singleCharXor.h"
#include "hex.h"
static double score(char* string);
static char* generateSingleKey(char c, int len);

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

	//frequency analysis
	for(i=0; i < alphLen; i++){
		key = generateSingleKey(alphabet[i], keyLen);
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

//scores a charecter for frequency analysis. 
static double scoreChar(char c){
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

//scores a string via frequency analysis
//determines how likely a string is to be
//a message and not random junk
static double score(char* string){
	int len = strlen(string);
	int i = 0;
	double score = 0.000000;
	for(i=0; i < len; i++){
		score = score + scoreChar(string[i]);
	}
	return score;
}

//generates a single char key for XOR encryption
static char* generateSingleKey(char c, int len){
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