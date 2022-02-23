#include <stdint.h>
#include "../RTOS_Labs_common/MyFifo.h"
#include "../RTOS_Labs_common/OS.h"

// AddIndexFifo(Rx, FIFOSIZE, char, FIFOSUCCESS, FIFOFAIL)
// AddIndexFifo(Tx, 1024, char, FIFOSUCCESS, FIFOFAIL)

// REMEMBER TO INITIALIZE THE FIFOS SOMEWHERE IN UART0INT.C


uint32_t volatile RxPutI;
uint32_t volatile RxGetI;
uint32_t volatile TxPutI;
uint32_t volatile TxGetI;

char static RxFifo [FIFOSIZE];
char static TxFifo [FIFOSIZE];

Sema4Type RxDataAvailable;
Sema4Type TxRoomLeft;


// RX:

void RxFifo_Init(void){           
  RxPutI = RxGetI = 0;   
  OS_InitSemaphore(&RxDataAvailable, 0);   
}                                       

int RxFifo_Put (char data){       
  if(( RxPutI - RxGetI ) & ~(FIFOSIZE-1)){  
    return(FIFOFAIL);      
  }                    
  RxFifo[ RxPutI &(FIFOSIZE-1)] = data; 
  RxPutI++;  
  OS_Signal(&RxDataAvailable);
  return(FIFOSUCCESS);     
}                      

int RxFifo_Get (char *datapt){  
  OS_Wait(&RxDataAvailable);
  if( RxPutI == RxGetI ){ 
    return(FIFOFAIL);      
  }                    
  *datapt = RxFifo[ RxGetI &(FIFOSIZE-1)];  
  RxGetI++; 
  return(FIFOSUCCESS);     
}                      
unsigned short RxFifo_Size (void){  
 return ((unsigned short)( RxPutI - RxGetI ));  
}


// TX:

void TxFifo_Init(void){           
  TxPutI = TxGetI = 0;   
  OS_InitSemaphore(&TxRoomLeft, FIFOSIZE);   
}                                       

int TxFifo_Put (char data){   
  OS_Wait(&TxRoomLeft);    
  if(( TxPutI - TxGetI ) & ~(FIFOSIZE-1)){  
    return(FIFOFAIL);      
  }                    
  TxFifo[ TxPutI &(FIFOSIZE-1)] = data; 
  TxPutI++;  
  return(FIFOSUCCESS);     
}                      

int TxFifo_Get (char *datapt){  
  if( TxPutI == TxGetI ){ 
    return(FIFOFAIL);      
  }                    
  *datapt = TxFifo[ TxGetI &(FIFOSIZE-1)];  
  TxGetI++; 
  OS_Signal(&TxRoomLeft);
  return(FIFOSUCCESS);     
}                      
unsigned short TxFifo_Size (void){  
 return ((unsigned short)( TxPutI - TxGetI ));  
}



