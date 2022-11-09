#include <stdint.h>
#include <stdbool.h>

#include "driverlib/interrupt.h"
#include "flash.h"
#include "uart.h"
#include <string.h>

#define PASSWORD_BUFFER ((uint32_t)0x20004000)

#define INFLOOP_BUFFER (PASSWORD_BUFFER+0x80)

#define TEMP_BUFFER ((uint32_t)0x20005000)

#define FLAG_BUFFER ((uint32_t)0x20007000)

void println(char* str);
void read_into_temp_buffer();

void phase1() {
    char name_buf[512];

    println("begin phase1");

    println("what is your name?");

    // Very secure
    uart_readline(HOST_UART, (uint8_t*) name_buf);

    println(name_buf);


    println("Goodbye!");
}

void phase2() {
    volatile void (*m_function)() = (void*) INFLOOP_BUFFER;
    char name_buf[16];

    println("begin phase2");
    read_into_temp_buffer();


    // read chars into name_buf
    println("what is your name?");
    int i;
    char c;
    for (i = 0; i <= 24; i++) {
        c = (uint8_t)uart_readb(HOST_UART);
        if (c == '\n') break;
        name_buf[i] = c;
    }
    println(name_buf);

    // read 16 chars into PASSWORD_BUFFER
    println("what is your password?");
    char* pw_buf = PASSWORD_BUFFER;
    for (i = 0; i < 16; i++) {
        c = (uint8_t)uart_readb(HOST_UART);
        if (c == '\n') break;
        pw_buf[i] = c;
    }
    pw_buf[i] = 0;
    println(pw_buf);

    println("Goodbye!");
    (*m_function)();
}

// Everything past this is just boilerplate

void read_into_temp_buffer() {
    println("please send exactly 256 bytes to store in the temp buffer");
    
    for (int i = 0; i < 256; i++) {
        while (!uart_avail(HOST_UART));
        *(((char*)TEMP_BUFFER) + i) = uart_readb(HOST_UART);
    }
}

void println(char* str) {
    char x;
    while ((x = *(str++))) uart_writeb(HOST_UART, x);
    uart_writeb(HOST_UART, '\n');
}


int main(void) {
    uart_init();
    memcpy((char*)FLAG_BUFFER, "flag{ectf_2022}", 16);

    *((uint32_t*)INFLOOP_BUFFER) = 0xE7FEE7FE;

    while (true) {
        println("which phase would you like to attempt? (1 or 2)");
        while (!uart_avail(HOST_UART));
        char phase = uart_readb(HOST_UART);
        while (uart_avail(HOST_UART)) uart_readb(HOST_UART);

        if (phase == '1') {
            phase1();
            println("end phase1");
            while(true);
        }

        if (phase == '2') {
            phase2();
            println("end phase2");
            while(true);
        }

        println("invalid phase");
    }
}
