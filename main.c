// Rodney McGrath 55352053
// Ashton Garcia 86229480
// ICS 53 Winter 2018
// Lab 4: A Memory Allocator

#define HEAP_SIZE 127
#define INPUT_MAX 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
  int i;
  unsigned char * Heap = (unsigned char *) malloc(HEAP_SIZE *
    sizeof(unsigned char *));
  int blockCounter = 0;
  *Heap = blockCounter;
  *(Heap + 1) = 252;
  char inputString[INPUT_MAX];
  while(1){
    // token
    printf("> ");
    scanf("%[^\n]%*c", inputString);
    char * token;
    token = strtok (inputString," ");

    // allocate (1 arg)
    if (strcmp(token, "allocate") == 0){
      token = strtok (NULL, " ");
      int numBytes = atoi(token);
      unsigned char *q = Heap;
      while ((q < Heap + 125) && ((*(q + 1) & 0x1)
        || (((*(q + 1) >> 1) + 1) < numBytes + 2))){
          q += (*(q + 1) >> 1) + 1;
      }
      if (q > Heap + 124){
        perror("Error: Exceeded Heap's Upperbounds.\n");
        exit(1);
      }
      else if ((*(q + 1) >> 1) + 1 > numBytes + 2){
        int oldSize = (*(q + 1) >> 1) + 1;
        int newSize = numBytes + 2;
        if (oldSize - newSize < 3){
          newSize = oldSize;
          blockCounter += 1;
          *q = blockCounter;
          *(q + 1) = ((newSize - 1) << 1) | 0x1;
        }
        else{
          int remainderSize = oldSize - newSize;
          blockCounter += 1;
          *q = blockCounter;
          *(q + 1) = ((newSize - 1) << 1) | 0x1;
          *(q + 1 + newSize) = ((remainderSize - 1) << 1) & -0x2;
        }
      }
      else {
          blockCounter += 1;
          *q = blockCounter;
          *(q + 1) = ((numBytes + 1) << 1 | 0x1);
        }
      printf("%d\n", blockCounter);
    }


    // free (1 arg)
    else if (strcmp(token, "free") == 0){
      token = strtok (NULL, " ");
      int blockNumber = atoi(token);
      unsigned char *q = Heap;
      while ((q < Heap + 125) && (*q != blockNumber)){
        q += (*(q + 1) >> 1) + 1;
      }
      if (q > Heap + 124){
        perror("Block number does not exist in heap\n");
        exit(1);
      }
      *(q + 1) = *(q + 1) & -0x2;
    }

    // blocklist (0 arg)
    else if (strcmp(token, "blocklist") == 0){
      unsigned char *q = Heap;
      printf("Size\tAllocated\tStart\tEnd\n");
      int size;
      char* allocated;
      while (q < Heap + 125){
        size = (*(q + 1) >> 1) + 1;
        if (*(q + 1) & 0x1){
          allocated = "yes";
        }
        else{
          allocated = "no";
        }
        int start = q - Heap;
        int end = q - Heap + size - 1;
        printf("%i\t%s\t\t%d\t%d\n", size, allocated, start , end);
        q += (*(q + 1) >> 1) + 1;
      }
    }

    // writeheap (3 args)
    else if (strcmp(token, "writeheap") == 0){
      token = strtok(NULL, " ");
      int blockNumber = atoi(token);
      token = strtok(NULL, " ");
      char characterToWrite = *token;
      token = strtok(NULL, " ");
      int numberTimesToWrite = atoi(token);
      unsigned char * q = Heap;
      while ((q < Heap + 125) && (*q != blockNumber)){
        q += (*(q + 1) >> 1) + 1;
      }
      if (q > Heap + 124){
        perror("Block number does not exist in heap\n");
        exit(1);
      }
      if (numberTimesToWrite > (*(q + 1) >> 1) + 1 - 2){
        perror("Write too big\n");
        exit(1);
      }
      q += 2;
      for (i = 0; i < numberTimesToWrite; i++){
        *q = characterToWrite;
        q += 1;
      }
    }
    // printheap (2 args)
    else if (strcmp(token, "printheap") == 0){
      token = strtok(NULL, " ");
      int blockNumber = atoi(token);
      token = strtok(NULL, " ");
      int numberTimesToWrite = atoi(token);
      unsigned char *q = Heap;
      while ((q < Heap + 125) && (*q != blockNumber)){
        q += (*(q + 1) >> 1) + 1;
      }
      if (q > Heap + 124){
        perror("Block number does not exist in heap\n");
        exit(1);
      }
      if (numberTimesToWrite > (*(q + 1) >> 1) + 1 - 2){
        // do i make it print all of payload or error
        numberTimesToWrite = (*(q + 1) >> 1) + 1 - 2;
      }
      q += 2;
      for (i = 0; i < numberTimesToWrite; i++){
        printf("%c", *q);
        q += 1;
      }
      printf("\n");

    }

    // printheader (1 arg)
    else if (strcmp(token, "printheader") == 0){
      token = strtok (NULL, " ");
      int blockNumber = atoi(token);
      unsigned char *q = Heap;
      while ((q < Heap + 125) && (*q != blockNumber)){
        q += (*(q + 1) >> 1) + 1;
      }
      if (q > Heap + 124){
        perror("Block number does not exist in heap\n");
        exit(1);
      }
      else{
        printf("%02X%02x\n", *q, (((*(q + 1) >> 1) + 1) << 1) + 1);
      }
    }

    //quit
    else if (strcmp(token, "quit") == 0){
      free(Heap);
      return 0;
    }
  }
}
