/*******************************************************************************
 *                              Mini-Project - HORS                                   *
 *******************************************************************************

 * Created By:      << Sudharshan Balaji >>
 _______________________________________________________________________________*/

/*************************************************************
					F u n c t i o n s
**************************************************************/
/*============================
/*============================
/*============================
==============================*/

#include <stdlib.h>
#include <stdio.h>
#include <tomcrypt.h>

void printBitRepresentation(unsigned char arr[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (arr[i] >> j) & 1);
        }
    }
    printf("\n");
}

void charToBinary(unsigned char *input, int input_uchar_size, int *binarryArray) {
    int i, j;
    for (i = 0; i < input_uchar_size; i++) {
        for (j = 7; j >= 0; j--) {
            binarryArray[(i * 8) + (7 - j)] = (input[i] >> j) & 1;
        }
    }
}
/*============================
        Print Binary Array
==============================*/

void printBinaryArray(int *array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d", array[i]);
        if ((i + 1) % 8 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}
/*============================
        Binary to Int
==============================*/
int binaryToInt(int *input_binary, int start_index, int m) {
    int interpreted_int = 0, i;
    for (i = 0; i < m; i++) {
        interpreted_int = (interpreted_int << 1) | input_binary[start_index + i];
    }
    return interpreted_int;
}
/*============================
        Write to File
==============================*/
void Write_File(char fileName[], char input[]){
  FILE *pFile;
  pFile = fopen(fileName,"w");
  if (pFile == NULL){
    printf("Error opening file. \n");
    exit(0);
  }
  fputs(input, pFile);
  fclose(pFile);
}
/*============================
        Write int to File
==============================*/
void Write_int_to_File(char fileName[], unsigned int input[], int inputsize){
  FILE *pFile;
  pFile = fopen(fileName,"w");
  if (pFile == NULL){
    printf("Error opening file. \n");
    exit(0);
  }
  for (int i = 0; i < inputsize; i++) {
        fprintf(pFile, "%d\n", input[i]);
    }
  //fputs(input, pFile);
  fclose(pFile);
}

/*============================
        Read from File
==============================*/
unsigned char* Read_File (char fileName[], int *fileLen)
{
    FILE *pFile;
	pFile = fopen(fileName, "r");
	if (pFile == NULL)
	{
		printf("Error opening file.\n");
		exit(0);
	}
    fseek(pFile, 0L, SEEK_END);
    int temp_size = ftell(pFile)+1;
    fseek(pFile, 0L, SEEK_SET);
    unsigned char *output = (unsigned char*) malloc(temp_size);
	fgets(output, temp_size, pFile);
	fclose(pFile);

    *fileLen = temp_size-1;
	return output;
}

/*============================
        Read from File
==============================*/
//The file has 8 lines amd each line has 32 characters
void Read_Multiple_Lines_from_File (char fileName[], char **message)
{
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;
  FILE *fp = fopen(fileName, "r");
  if (!fp)
  {
    fprintf(stderr, "Error opening file '%s'\n", fileName);
  }
  int j=0;

      while ((line_size = getline(&line_buf, &line_buf_size, fp)) != -1) {
        if (line_size > 0) {
            // Copy the line from line_buf to message[j], ensuring null-termination
            for (int i = 0; i < line_size && i < 64; i++) {
                message[j][i] = line_buf[i];
            }
            // Null-terminate the string in message[j] if necessary
            if (line_size < 64) {
                message[j][line_size] = '\0';
            }
            j++;
        }
    }

    free(line_buf);
    fclose(fp);
}
/*============================
        Write Multiple Lines to File
==============================*/
void Write_Multiple_Lines_to_File(char fileName[], char **input, int no){ 
  FILE *pFile;
  pFile = fopen(fileName,"w");
  if (pFile == NULL){
    printf("Error opening file. \n");
    exit(0);
  }
  for(int i=0; i<no; i++){
    char temp[64];
    if(i != (no-1)) {
        temp[64] = '\0';
    }
    memcpy(temp, input[i], 64);
    fputs(temp, pFile);
    
    if (i < (no-1)) fputs("\n", pFile);
  }
    fclose(pFile);
}

/*============================
        PRNG Fucntion 
==============================*/
unsigned char* PRNG(unsigned char *seed, unsigned long seedlen, unsigned long prnlen)
{
	int err;
    unsigned char *pseudoRandomNumber = (unsigned char*) malloc(prnlen);

	prng_state prng;                                                                     //LibTomCrypt structure for PRNG
    if ((err = chacha20_prng_start(&prng)) != CRYPT_OK){                                //Sets up the PRNG state without a seed
        printf("Start error: %s\n", error_to_string(err));
    }					                
	if ((err = chacha20_prng_add_entropy(seed, seedlen, &prng)) != CRYPT_OK) {           //Uses a seed to add entropy to the PRNG
        printf("Add_entropy error: %s\n", error_to_string(err));
    }	            
    if ((err = chacha20_prng_ready(&prng)) != CRYPT_OK) {                                   //Puts the entropy into action
        printf("Ready error: %s\n", error_to_string(err));
    }
    chacha20_prng_read(pseudoRandomNumber, prnlen, &prng);                                //Writes the result into pseudoRandomNumber[]

    if ((err = chacha20_prng_done(&prng)) != CRYPT_OK) {                                   //Finishes the PRNG state
        printf("Done error: %s\n", error_to_string(err));
    }

    return (unsigned char*)pseudoRandomNumber;
}

/*============================
        Showing in Hex 
==============================*/
void Show_in_Hex (char name[], unsigned char hex[], int hexlen)
{
	printf("%s: ", name);
	for (int i = 0 ; i < hexlen ; i++)
   		printf("%02x", hex[i]);
	printf("\n");
}

/*============================
        Convert to Hex 
==============================*/
void Convert_to_Hex(char output[], unsigned char input[], int inputlength)
{
    for (int i=0; i<inputlength; i++){
        sprintf(&output[2*i], "%02x", input[i]);
    }
    //printf("Hex format: %s\n", output);  //remove later
}

/*===================================
        Convert to unsigned char 
=====================================*/
void Convert_To_uchar(char* input_hex, unsigned char output_uchar[], int output_uchar_length){
    
    for(int i=0; i<output_uchar_length; i++){
        unsigned char tmp[2];
        tmp[0]= input_hex[2*i];
        tmp[1]= input_hex[2*i+1];
        output_uchar[i] = (unsigned char)strtol(tmp, NULL, 16);
    }
}

/*============================
        SHA-256 Fucntion
==============================*/
unsigned char* Hash_SHA256(unsigned char* input, unsigned long inputlen)
{
    unsigned char *hash_result = (unsigned char*) malloc(inputlen);
    hash_state md;                                                          //LibTomCrypt structure for hash
    sha256_init(&md);                                                       //Initializing the hash set up
    sha256_process(&md, (const unsigned char*)input, inputlen);            //Hashing the data given as input with specified length
    sha256_done(&md, hash_result);                                         //Produces the hash (message digest)
    
    return hash_result;
}