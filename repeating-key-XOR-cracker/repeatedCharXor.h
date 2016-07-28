/*
	encrypts text with repeating key xor 
	and returns the hash in hex
*/
char* encryptRKXOR(char* input, char* key);

/*
	decrypts hex hash with repeating key xor 
	and returns the original text
*/
char* decryptRKXOR(char* input, char* key);