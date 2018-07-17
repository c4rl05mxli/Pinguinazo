# Pinguinazo
Bootloader para los módulos de entrenamiento Pinguino

Actualización del Bootloader para el Pinguinazo. 
En modo Pingüino (Bootloader_v4.14_18f4550_X20MHz.hex) y modo compatibilidad CCS C, C18 y XC8 (Bootloader_v1.01_XC8.hex)

Acerca de la Actualización al Bootloader XC8 (Bootloader_v1.01_XC8.hex)

Esta desarrollado en el nuevo compilador de Microchip XC8, basado en el modelo base MLA y adaptado a la tarjeta pinguinazo.
El nuevo bootloader es modo HID, por lo cual no requiere drivers y es compatible con Linux.
Se agregó la función de que el LED parpadea cuando está en modo programación.
Se agregó la función de entrar a modo programación mediante un salto absoluto desde el código de programa.


Como usar el pinguinazo en CCS C.

Dado que el bootloader ya tiene la palabra de configuración incluida es necesario omitir esa sección del código en CCS C.

Es necesario utilizar siempre la instrucción: #use delay(clock=48000000)    // Frecuencia a la que opera el pinguinazo.

Es necesario incluir las siguientes instrucciones en el código para reservar el espacio de memoria y evitar sobre escrituras del bootloader.

//Declaración para proteger el Bootloader
#build (reset=0x1000, interrupt=0x1008)
#ORG 0x0000,0x0FFF {}
void bootloader()
{
#asm
nop
#endasm
}


Para entrar al modo programación desde el software es necesario hacer un salto a una posición especifica del código de programa, por lo que se recomienda incluir la siguiente función.

void programmode(void){
// Esta rutina ejecuta un salto al modo programación, 
// el valor aquí mostrado depende de la posición de la rutina al momento 
// de compilar el Bootloader, y debe ser verificada mediante la observación
// del archivo HEX, en un visor Hexadecimal.

#asm
   goto 0x001C          
#endasm
}

La rutina puede ser llamada en cualquier momento, y generara un softreset en el microcontrolador, reconfigurándolo para entrar a modo programación, esto puede causar problemas en el caso de que alguna aplicación este haciendo uso del puerto USB del PIC. (es necesario hacer una liberación del puerto instantes después de enviar la instrucción de reinicio en modo programacion, por lo que se recomienda utilizar un temporizador o un delay en el programa del pic, antes de ejecutar el salto_).

Se incluye además un código simple de ejemplo en CCS C.
