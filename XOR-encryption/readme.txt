This program will encrypt or decrypt a message using repeating key XOR

when using the flag 1
	this program reads in a text file, and using a
	key given by the user will encrypt the text file
	outputting it into another file in hex.
when using the flag 0
	this program reads in a hex file, and using a 
	key given by the user will decrypt the hex files
	outputting it into another file in text 

to Compile: gcc -Wall -Werror -o run XOR.c
to run: ./run [key] [input file name] [output file name] [1/0]