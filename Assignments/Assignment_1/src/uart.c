// Just a simple UART file for debugging

#include "MK22F51212.h" //Device header

void UART0_Init(void) {
    // Enable clocks
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Set PTB17 as UART0_TX (ALT3)
    PORTB->PCR[17] = PORT_PCR_MUX(3);

    // Disable UART while configuring
    UART0->C2 = 0;

    // 120MHz clock, 115200 baud: SBR = 120000000 / (16 * 115200) = 65
    UART0->BDH = 0;
    UART0->BDL = 65;

    // Enable transmitter
    UART0->C2 = UART_C2_TE_MASK;
}

void UART0_PutChar(char c) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK))
        ;
    UART0->D = c;
}

void UART0_Print(const char *s) {
    while (*s)
        UART0_PutChar(*s++);
}
