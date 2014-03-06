// jpeg marker parser
// get some basic info from a jpeg, not the exif data

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
   FILE *fp;              // jpeg file
   uint16_t markerLength; // length of marker
   uint8_t *begOfBuf;
   uint8_t *endOfBuf;
   int fileLen;

   if (argc !=2) {
      printf("\nUsage\n%s file.jpg\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   else {
      if (( fp = fopen(argv[1], "r")) == NULL) {
         printf("Can not open file %s\n", argv[1]);
         exit(EXIT_FAILURE);
      }
      else {
         printf("\n%s\n", argv[1]);
         // get the file length
         fseek(fp, 0, SEEK_END);
         fileLen = ftell(fp);
         if (fileLen < 1) {
            printf("\nWhat is this? ftell(fp) = 0x%x\n", fileLen);
            exit(EXIT_FAILURE);
         }
         printf("\nSize: %d\n", fileLen);
         fseek(fp, 0, SEEK_SET);
         
         // allocate buffer
         begOfBuf = calloc(1,(fileLen));
         if (begOfBuf == NULL) {
            printf("Allocation Failed\n");
            exit(EXIT_FAILURE);
         }
         else { // fill the buffer
            fread(begOfBuf, 1, fileLen, fp);
            fclose(fp);
         }
         endOfBuf = begOfBuf + fileLen;
      }
   }
}
