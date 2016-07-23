
Compile with: gcc -Wall -Werror -o run encrypt.c
when running use
	./run [hex to encode]

NOTE because this version does not pad it:
	assumes valid hexadecimal input
	assumes input is a multiple of 24 bits

if input is invalid the program will notify the user