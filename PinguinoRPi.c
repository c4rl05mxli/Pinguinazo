#include <18F4553.h>          // Definición de registros internos.
//#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
//#device adc=10                // ADC 10 bits justificación a a la derecha
#use delay(clock=48000000)    // Frecuencia de Reloj



#include "usb_cdc.h"          // Descripción de funciones del USB.
#include "usb_desc_cdc.h"     // Descriptores del dispositivo USB.

//Declaración para proteger el Bootloader
#build (reset=0x1000, interrupt=0x1008)
#ORG 0x0000,0x0FFF {}
void bootloader()
{
#asm
nop
#endasm
} 
//!
//!/////////////////////////////////////////////////////////////////////////// 
//!////                     USB_BOOTLOADER.H                              //// 
//!////                                                                   //// 
//!////                                                                   //// 
//!/////////////////////////////////////////////////////////////////////////// 
//!//how big is the bootloader? 
//!//the bootloader will reside from address 0x0000 to this location.  the 
//!//application will then sit at this location+1 to the end of program memory. 
//!#define LOADER_SIZE                 (0xFFF) 
//!#define LOADER_START                (0) 
//!#define LOADER_END                  (LOADER_SIZE) 
//!#define APPLICATION_START           (LOADER_SIZE+1) 
//!#define APPLICATION_END             (getenv("PROGRAM_MEMORY")-1) 
//!#define APPLICATION_ISR             (APPLICATION_START+8) 
//!
//!#ifndef _bootloader 
//! //in the application, this moves the reset and isr vector out of the bootload 
//! //space.  it then reserves the loader space from being used by the application. 
//! #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR) 
//! #org 0, LOADER_END {} 
//!#endif 

// DEFINICIONES /////////////////////////////////////////////////////////////////
#define     LED             PIN_A4                 // PORTC.1 Led rojo.   
#define     CR              0x0D                   // Carriage Return

                                  
// VARIABLES EN RAM ///////////////////////////////////////////////////////////
int1 flagcommand=0;                    // Flag para indicar comando disponible 
char LETRA;

// Declaración de Funciones ///////////////////////////////////////////////////
void init(void);

void programmode(void){
// Esta rutina ejecuta un salto al modo programacion, 
// el valor aqui mostrado depende de la posicion de la rutina al momento 
// de programar el Bootloader, y debe ser verificada mediante la observacion
// del archivo HEX, en un visor Hexadecimal.

#asm
   goto 0x001C          
#endasm
}


void init(void){
   usb_cdc_init();                        // Configuramos al puerto virtual.
   usb_init();                            // Inicializamos el stack USB.
   usb_task();                            // Inicializo USB
}

// Programa Principal /////////////////////////////////////////////////////////

void main() {
   init();       
  do {
      if(flagcommand){
         flagcommand=0;                            // Desactivo flag de comando pendiente.
       }
         
      
     if(usb_cdc_connected()){                      // espera a detectar una transmisión de la PC (Set_Line_Coding)
       if (usb_enumerated())                       // aquí se hace el acceso HOST<->PC y después sale
          if(usb_cdc_kbhit()){                     // Si recibe un caracter
             LETRA=usb_cdc_getc();                 // guarda el caracter en LETRA
                delay_ms(500);
          if (LETRA=='P'){
                      printf(usb_cdc_putc,"Entrare a modo Programacion \n\r"); // envia una respuesta por USB
                       delay_ms(1000); 
                      programmode();

          }
          else               
               
                printf(usb_cdc_putc,"Recibi Caracter %c\n\r",LETRA ); // envia una respuesta por USB
                             }  
                    
            }
            

   } while (TRUE);

}




