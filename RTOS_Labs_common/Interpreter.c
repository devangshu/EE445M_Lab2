// *************Interpreter.c**************
// Students implement this as part of EE445M/EE380L.12 Lab 1,2,3,4 
// High-level OS user interface
// 
// Runs on LM4F120/TM4C123
// Jonathan W. Valvano 1/18/20, valvano@mail.utexas.edu
#include <stdint.h>
#include <string.h> 
#include <stdio.h>
#include "../RTOS_Labs_common/OS.h"
#include "../RTOS_Labs_common/ST7735.h"
#include "../inc/ADCT0ATrigger.h"
#include "../inc/ADCSWTrigger.h"
#include "../RTOS_Labs_common/UART0int.h"
#include "../RTOS_Labs_common/eDisk.h"
#include "../RTOS_Labs_common/eFile.h"
#include "../RTOS_Labs_common/ADC.h"
#include "../RTOS_Labs_common/Interpreter.h"
#include "../src/globals.h"


#define TRUE 1

// extern int32_t MaxJitter;

// Print jitter histogram
void Jitter(int32_t MaxJitter, uint32_t const JitterSize, uint32_t JitterHistogram[]){
    // write this for Lab 3 (the latest)	
}

static void OutCRLF(void){
    UART_OutChar(CR);
    UART_OutChar(LF);
}

void Interpreter_print_message(char * string) {
    UART_OutString("Displaying message on LCD\n\r");
    char linenum_ = string[3];
    int linenum = linenum_ - '0';
    // rest of string: index 5 to index 19
    char userstring[16];
    strncpy(userstring, &string[5], 16);
    userstring[15] = '\0';

    int d = 0;
    if (string[1] == 't') {
        d = 0;
        ST7735_FillRect(0, 0, 128, 79, 0);
    } else if (string[1] == 'b') {
        d = 1;
        ST7735_FillRect(0, 80, 128, 160, 0);
    }

    ST7735_Message(d, linenum, userstring, -1);
}

void Interpreter_test_adc(char * string) {
    UART_OutString("Testing ADC on channel ");
    UART_OutChar(string[2]);
    UART_OutString("...\n\r");
    uint32_t channel = string[2] - 48;
    uint32_t adc_in = 0;
    ADC_Init(channel);
    adc_in = ADC_In();
    UART_OutUDec(adc_in);
    UART_OutString("\n\r");
}


void Interpreter_test_os_time(char * string) {
    UART_OutString("Testing OS Time Functions\n\r");
    uint32_t time = 0;
    if(string[3] == 'r'){
        OS_ClearMsTime();
        UART_OutString("Time has successfully been cleared\n\r");
        time = OS_MsTime();
        UART_OutString("The current time is ");
        UART_OutUDec(time);
        UART_OutString("ms. \n\r");
        string[3] = 0; // Clears letter from array, can be handled better
    } else {
        time = OS_MsTime();
        UART_OutString("The current time is ");
        UART_OutUDec(time);
        UART_OutString("ms. \n\r");
    }
}

void Interpreter_help() {
    UART_OutString("Welcome to the help menu \n\r");
    UART_OutString("------------------------------\n\r");
    UART_OutString("a - ADC test \n\r");
    UART_OutString("Usage: a <channel number> \n\r");
    UART_OutString("------------------------------\n\r");
    UART_OutString("l - LCD test \n\r");
    UART_OutString("Usage: l [display number] <message> <value> \n\r");
    UART_OutString("------------------------------ \n\r");
    UART_OutString("t - OS time \n\r");
    UART_OutString("Usage: t [flag] \n\r");
    UART_OutString("Flags: -r: reset time\n\r");
    UART_OutString("------------------------------ \n\r");
}

void Interpreter_show_numthreads() {
    UART_OutString("Checking number of threads running\n\r");
    char outstring[50];
    snprintf(outstring, 50, "%d threads", NumCreated);
    UART_OutString(outstring);
    UART_OutString("\n\r");
}

void Interpreter_show_maxjitter() {
    UART_OutString("Checking maximum time jitter\n\r");
    char outstring[50];
    snprintf(outstring, 50, "%d ms", MaxJitter);
    UART_OutString(outstring);
    UART_OutString("\n\r");
}

// *********** Command line interpreter (shell) ************
void Interpreter(void){ 
    UART_Init();
    UART_OutString("EE445M Lab 2 Interpreter");
    OutCRLF();
    
    char string[20];
    
    while (TRUE) {
        UART_OutString("> ");
        UART_InString(string, 19);
        OutCRLF();
        
        if (strstr(string, "NumCreated") != NULL) {
            Interpreter_show_numthreads();
        } else if (strstr(string, "MaxJitter") != NULL) {
            Interpreter_show_maxjitter();
        } else {
            switch(string[0]){
                case 'l':
                    Interpreter_print_message(string);
                    break;
                case 'a':
                    Interpreter_test_adc(string);
                    break;
                case 't':
                    Interpreter_test_os_time(string);
                    break;
                case 'h':
                default:
                    break;
            }
        }
    }
}









