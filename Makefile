#Avinash Gupta
#G01009180
#CS262 Lab Section 2B3
#Project 3

all: Stego StegoExtract

#image.o: image.c
	#gcc -g -Wall -c image.c

#Stego.o: Stego.c
	#gcc -g -Wall -c Stego.c

#StegoExtract.o: StegoExtract.c
	#gcc -g -Wall -c StegoExtract.c

Stego: Stego.c
	gcc -g -Wall -c image.c
	gcc -g -Wall -c Stego.c
	gcc -g -Wall -o Stego Stego.o image.o

StegoExtract: StegoExtract.c
	gcc -g -Wall -c image.c
	gcc -g -Wall -c StegoExtract.c
	gcc -g -Wall -o StegoExtract StegoExtract.o image.o

clean:
	rm -f image.o
	rm -f Stego.o
	rm -f StegoExtract.o
	rm -f Stego
	rm -f StegoExtract
