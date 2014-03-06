// jpeg marker parser
// get some basic info from a jpeg, not the exif data

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
   FILE *fp;              // jpeg file
   uint16_t markerLength; // length of marker
   uint8_t *begOfBuf;     // beginning of file
   uint8_t *endOfBuf;     // end of file
   uint8_t *cursor;       // location in file
   int fileLen;

   if (argc !=2) {
      printf("\nUsage\n%s file.jpg\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   else {
      if (( fp = fopen(argv[1], "r")) == NULL) {
         printf("\nCan not open file %s", argv[1]);
         exit(EXIT_FAILURE);
      }
      else {
         printf("\n%s", argv[1]);
         // get the file length
         fseek(fp, 0, SEEK_END);
         fileLen = ftell(fp);
         if (fileLen < 1) {
            printf("\nWhat is this? ftell(fp) = 0x%x", fileLen);
            exit(EXIT_FAILURE);
         }
         printf("\nSize: %d", fileLen);
         fseek(fp, 0, SEEK_SET);
         
         // allocate buffer
         begOfBuf = calloc(1,(fileLen));
         if (begOfBuf == NULL) {
            printf("\nAllocation Failed");
            exit(EXIT_FAILURE);
         }
         else { // fill the buffer
            fread(begOfBuf, 1, fileLen, fp);
            fclose(fp);
         }
         endOfBuf = begOfBuf + fileLen;
      }
   }

   // Check the file magic
   // 0xFF 0xD8 is expected
   printf("\nFirst byte: 0x%x\n", *begOfBuf);
   printf("Second byte: 0x%x\n", *(begOfBuf+1));

   if( (*begOfBuf != 0xFF) || ( *(begOfBuf+1) != 0xD8) ) {
      printf("\nFile is not jpg\n");
      free(begOfBuf);
      exit(EXIT_FAILURE);
   }

   // check for EOF marker
   // 0xFF 0xD9 is expected
   
   printf("End of file: 0x%x 0x%x\n", *(endOfBuf-2), *(endOfBuf-1));

   if( ( *(endOfBuf-2) != 0xFF ) || ( *(endOfBuf-1) != 0xD9 ) ) {
      printf("EOF marker is incorrect, attempting to parse\n");
   }

   // set the cursor to the location of the next tag
   if( begOfBuf+3 > endOfBuf) {
      printf("\nEND OF FILE\n");
      exit(EXIT_FAILURE);
   }

   // read the remaining tags, printing the tag and length
   //while( cursor <= endOfBuf ) {
   //}
}
