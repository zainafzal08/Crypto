/*
	converts single hex byte to a byte
*/
unsigned char hexToDec(char* hex);
/*
	converts single byte to hex
*/
char* decToHex(unsigned char input);
/*
	takes two bytes in hex form, xor's them
	and returns the result in hex
*/
char* hexXOR(char* first, char* second);

/*
	converts a string of hex bytes to bytes
*/
void extendedHexToDec(char* hex, unsigned char* output, int bufferLen);
/*
	converts a string of hex bytes to text
*/
char* hexToString(char* input);

/*
	converts a string to hex representation
*/
char* stringToHex(char* string);