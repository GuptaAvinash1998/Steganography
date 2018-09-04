/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * Stego.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <string.h>
#include "image.h"

#define MASK 0x1
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"

#define BYTETOBINARY(byte) \
  (byte & 0x80 ? 1:0), \
  (byte & 0x40 ? 1:0), \
  (byte & 0x20 ? 1:0), \
  (byte & 0x10 ? 1:0), \
  (byte & 0x08 ? 1:0), \
  (byte & 0x04 ? 1:0), \
  (byte & 0x02 ? 1:0), \
  (byte & 0x01 ? 1:0)

#define PRINTBIN(x) printf(BYTETOBINARYPATTERN, BYTETOBINARY(x));
void setlsbs (unsigned char *p, unsigned char b0);

int main(int argc, char *argv[])
{  
  int i, j, k, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  byte b0;
   
   if(argc != 4) 
    {
      printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[3],&b);    // Read binary data
 

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // embed four size bytes for the Buffer's size field
  
	int size = b.size;//stores the size of the buffer in the variable size
	printf(" The size of b.size is %d\n",size);
	unsigned int temp = 0;//temporary variable

	unsigned char bytes_split[4];//I am isolating each byte from size so each byte will go into each address in the array	
	unsigned char byte_from_cover_file[8];//stores the bytes from the cover image

	//ex the size of the buffer (b.size) is 280, so the binary representation will be 00000000 00000000 00000001 00011000
	//The first step is trying to isolate 00011000, so it left shifts the int by 24 bits and then right shifts it by 24 so in the end we will end up getting 00011000
	temp = size<<24;
	bytes_split[0] = temp >> 24;//we store that as the first element in the array
	temp = 0x0;//reset the variable

	temp = size<<16;//gets 00000001
	bytes_split[1] = temp >> 24;//stores that in the array
	temp = 0x0;//reset

	temp = size<<8;//gets 00000000
	bytes_split[2] = temp >> 24;//stores in array
	temp = 0x0;//reset

	temp = size<<0;
	bytes_split[3] = temp >> 24;//gets 00000000 and stores in array
	temp = 0x0;//reset

	int cntr = 0;	
	for(int i=0;i<4;i++){

	  for(int j=0;j<8;j++){

		if(i == 0){
		  byte_from_cover_file[j] = GetGray(j);
		}else{
		  byte_from_cover_file[j] = GetGray(j+cntr);
		}
	  }

	  setlsbs(byte_from_cover_file,bytes_split[i]);

	  for(int k = 0;k<8;k++){

		if(i == 0){
		  SetGray(k,byte_from_cover_file[k]);
		}else{

		  SetGray(k+cntr,byte_from_cover_file[k]);
		}
	  }

	  cntr += 8;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // embed the eight digits of your G# using 4 bits per digit
  
	unsigned int Gnumber[8] = {0x0,0x1,0x0,0x0,0x9,0x1,0x8,0x0};//storing the Gnumber as a hex array
	cntr = 0;//for getting the accurate byte number

	printf("Gnumber is: 01009180");

	temp = 0x0;//temporary variable from last time
	
	unsigned int pairs[4] = {0x0};

	for(int i=0;i<8;i+=2){//so the project spoec says that the in the Gnumber, each number has to be stored as as 4 bits, so in the array above, each number is stored in 8 bits, what i am doimg here is merging the pair of elements of the array and making them 4 bits each, example I am merging 0 and 1 to be from 00000000 00000000 00000000 00000000 and 00000000 00000000 00000000 00000001 to 00000000 00000000 00000000 00001000

		temp = Gnumber[i+1];
		temp  = temp<< 4;

		Gnumber[i] = Gnumber[i]+temp;
		temp = 0x0;
	}

	pairs[0] = Gnumber[0];//after merging them, I store each pair in the array called pairs
	pairs[1] = Gnumber[2];
	pairs[2] = Gnumber[4];
	pairs[3] = Gnumber[6];

	for(int i=0;i<4;i++){


		for(int j=0;j<8;j++){

			if(i == 0){

				byte_from_cover_file[j] = GetGray(j+32);//last time we read from 0 to 31, now we read from 32 to 63
			}else{

				byte_from_cover_file[j] = GetGray(j+32+cntr);
			}
		}

		setlsbs(byte_from_cover_file,pairs[i]);//we set the lsbs of each extracted byte

		for(int k=0;k<8;k++){

			if(i == 0){
				
				SetGray(k+32,byte_from_cover_file[k]);//we write those bytes in the image
			}else{
				
				SetGray(k+32+cntr,byte_from_cover_file[k]);
			}
		}

		cntr += 8;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	   
  cntr = 0;//resets cntr and temp from last time
  temp = 0x0;

  unsigned char bytes_from_sample_program[1];//This stores the byte from the payload
  for (i=0; i<b.size; i++)
    {
      // here you embed information into the image one byte at the time
      // note that you should change only the least significant bits of the image
	
	bytes_from_sample_program[0] = GetByte(i);
	for(int j=0;j<8;j++){

		if(i == 0){
		  byte_from_cover_file[j] = GetGray(i+64);//gets bytes from 64 to 344
		}else{

		  byte_from_cover_file[j] = GetGray(i+64+cntr);
		}
	}
	setlsbs(byte_from_cover_file,bytes_from_sample_program[0]);//sets the lsbs of the bytes from the image with the bits of the byte from the payload

	for(int k=0;k<8;k++){

		if(i == 0){
	 	  SetGray(k+64,byte_from_cover_file[k]);//writes the changed bytes into the image
		}else{
		  SetGray(k+64+cntr,byte_from_cover_file[k]);
		}
	}
	cntr += 8;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)
}

void setlsbs(unsigned char *p, unsigned char b0){//This is used to set the lsbs for the bytes of the image

	byte temp;//temporary variable
		
	for(int i=0;i<8;i++){

		p[i] &= ~MASK;//gets the ith byte, the MASK is 0x1 so ~ of that would be 11111111 11111111 11111111 11111110, so we and it with the byte so that we isolate its lsb

		temp = b0 & MASK;//we take the byte that we want to hide, and it with the MASK so that we get its lsb

		p[i] |= temp;//then we and the lsbs together therefore changing the lsb of the image byte with the lsb of the byte of the number or the file that we are hiding

		b0 = b0 >> 1;//after we done with that lsb, we right shift by 1 to remove that lsb and do the process with the next one
	}
}
