/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * StegoExtract.c: A program for manipulating images                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <string.h>
#include "image.h"


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

unsigned char getlsbs(unsigned char *p);

int main(int argc, char *argv[])
{  
  int i, j, k, cover_bits, bits;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
  byte b0;
   
  if (argc != 3) 
    {
      printf("\n%s <stego_file> <file_to_extract> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros

  // hidden information 
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    

  b.size = 0;//stores the size of the extarcted number
  unsigned char bytes_from_Stego_image[8];//stores the bytes read from the image
  unsigned int temp = 0x0;//temporary variable
  unsigned int original_number = 0x0;//stores the extracted number
  int cntr = 0;//used to get the accurate byte from the image
  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // extract four size bytes for the Buffer's size field
  // Set this to b.size
  for(int i=0;i<4;i++){

	for(int j=0;j<8;j++){
		
		if(i == 0){
		  bytes_from_Stego_image[j] = GetGray(j);//gets bytes from 0 to 31
		}else{

		  bytes_from_Stego_image[j] = GetGray(j+cntr);
		}
	}

	temp |= getlsbs(bytes_from_Stego_image);//once we get the byte from getlsbs, we or it to temp so that we store it somewhere
	temp = temp << (8*i);//in Stego, we left isolated each byte, now we are taking that isolated byte and we are putting it in its right place
	original_number |= temp;//we then OR temp and the original number so that we add the byte to the variable that holds the extracted number
	temp = 0x0;//resets the temporary variable
	cntr += 8;
  }
  b.size = original_number;//The number is then transfered to b.size
  printf("Extracted number is: %d\n",b.size);

  b.data = malloc(b.size); // Allocates room for the output data file

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  // extract the eight digits of your G# using 4 bits per digit
  cntr = 0;//same counter and temporary varibel as last time
  temp = 0x0;
  unsigned int recovered_gnumber[8] = {0x0};//This stores the extracted Gnumber

  for(int i=0;i<4;i++){

	for(int j=0;j<8;j++){
		
		if(i == 0){
			
			bytes_from_Stego_image[j] = GetGray(j+32);//gets bytes 31 to 63

		}else{

			bytes_from_Stego_image[j] = GetGray(j+32+cntr);

		}
	}
	
	temp |= getlsbs(bytes_from_Stego_image);//once we get the byte from getlsbs, we store it in temp
	recovered_gnumber[2*i] = temp & 00001111;//In Stego.c, we had a loop that paired the number into 4 bits, here we apply this mask. which gets the last 4 bits of the number and stores it in the array
	//for example if we hid 0 and 1 for it to become 00000000 00000000 00000000 00010000, after this process will get the even index to have 0000 and the odd will have 0001
	recovered_gnumber[(2*i)+1] = temp>>4;
	temp = 0x0;//resets the variable
	cntr += 8;
  }

  printf("Recovered Gnumber is: ");
  
  for(int i=0;i<8;i++){//prints the number
    printf("%d",recovered_gnumber[i]);
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   

  cntr = 0;//resets the cntr and temp from last time
  temp = 0x0;
  unsigned char recovered_byte = 0x0;//stores the recovered byte
  for(i=0;i<b.size;i++)
    {
      // here you extract information from the image one byte at the time
      // note that you should extract only the least significant bits of the image
      for(int j=0;j<8;j++){

		if(i == 0){
		  bytes_from_Stego_image[j] = GetGray(j+64);//gets bytes 64 to 344 from the changed image
		}else{

		  bytes_from_Stego_image[j] = GetGray(j+64+cntr);
		}
	}

	temp |= getlsbs(bytes_from_Stego_image);//renus getlsbs and stores it in temp
	recovered_byte |= temp;//this value is the transfered to recovered byte
	SetByte(i,recovered_byte);//the value of rev=covered byte is then written into the ith byte if the out.dat file
	temp = 0x0;//resets both the variables
	recovered_byte = 0x0;
	cntr += 8;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  WriteBinaryFile(argv[2],b);  // output payload file
}

unsigned char getlsbs(unsigned char *p){//extracts the hidden bits

	unsigned char extract = 0x0;//stores the whole byte
	unsigned char temp = 0x0;//temporary variable

	for(int i=0;i<7;i++){

		temp = p[i]&0x1;//gets the byte from the image, then we apply this mask to get the lsb
		temp <<= i;//we then left shift the bit i times so that we place it in its right place
		extract = extract | temp;//then we add that to extracted so that in the end we get the whole byte
	}

	
	return extract;//returns the byte
}
