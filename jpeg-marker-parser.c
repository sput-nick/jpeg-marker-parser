// jpeg marker parser
// get some basic info from a jpeg, not the exif data

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef READ_BIG_16
#define READ_BIG_16(p) (*(p) << 8) \
                     | (*((u_int8_t *)(p) + 1))
#endif

int main(int argc, char *argv[])
{
   FILE *fp;              // jpeg file
   uint16_t markerLen; // length of marker
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
   cursor = begOfBuf+2;

   // read the remaining tags, printing the tag and length
   while( cursor <= endOfBuf ) {
      // Make sure there is data
      if( cursor+2 > endOfBuf) {
         printf("\nEND OF FILE 0x%x\n", (cursor - begOfBuf));
         exit(EXIT_FAILURE);
      }

      // Get the next marker
      printf("File position: 0x%x\n", (cursor - begOfBuf));
      printf("Next marker: 0x%x 0x%x\n", *cursor, *(cursor+1));

      if( *cursor != 0xFF ) {
         printf("Not a valid marker\n");
         printf("There are %d bytes unaccounted for\n", (endOfBuf - cursor));
         exit(EXIT_FAILURE);
      }
      
      // if this is the start-of-stream marker 0xFF 0xDA
      // just move forward until we find the next marker
      // every FF in the stream should have a null
      if( *(cursor+1) == 0xDA) {
         printf("Start of stream, scanning for next marker\n");
         cursor += 2;
         
         while ( (cursor+1) <= endOfBuf ) {
            if( *cursor == 0xFF && *(cursor+1) != 0x00 ) {
               printf("File position: 0x%x\n", (cursor - begOfBuf));
               printf("Next marker: 0x%x 0x%x\n", *cursor, *(cursor+1));
               break;
            }
            else {
               cursor++;
            }
         }
      }
      
      cursor +=2;

      // Get it's length
      if( cursor+2 > endOfBuf) {
         printf("\nEND OF FILE 0x%x\n", (cursor - begOfBuf));
         exit(EXIT_FAILURE);
      }
      markerLen = READ_BIG_16(cursor);
      //markerLen = *cursor << 8;
      //markerLen = *(cursor+1);
      printf("markerLen: 0x%x\n", markerLen);
      cursor += markerLen;  
   }
}

