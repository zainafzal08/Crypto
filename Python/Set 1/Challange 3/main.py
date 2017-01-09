hexA = "1c0111001f010100061a024b53535009181c"
hexB = "686974207468652062756c6c277320657965"
#int will convert a string to a number via base 10
#unless specified another base.

#the ^ will do a bitwise XOR, xoring every bit between
#two numbers. 
result = int(hexA, 16) ^ int(hexB, 16)
#the %X is just print as hex. 
print("%X" % result)