#define GetPBClock() (10000000)
#include <plib.h>

void UARTSendString(const char *string);
void UARTSendChar(const char letter);


/*****************************************************/
// Sends string to the computer via UART

void UARTSendString(const char *string) {

    // Configures UART to send to computer
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetDataRate(UART1, GetPBClock(), 9600);

    UARTSetLineControl(UART1,
            UART_DATA_SIZE_8_BITS | UART_PARITY_NONE
            | UART_STOP_BITS_1);

    UARTEnable(UART1, UART_ENABLE | UART_PERIPHERAL | UART_RX |
            UART_TX);


    // Steps through the string sends each letter one at a time.
    while (*string) {
        if(UARTTransmitterIsReady(UART1)) {
            UARTSendDataByte(UART1, *string);
            ++string;
        }
    }
}

void UARTSendChar(const char letter) {

    // Configures UART to send to computer
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);

    UARTSetDataRate(UART1, GetPBClock(), 9600);

    UARTSetLineControl(UART1,
            UART_DATA_SIZE_8_BITS | UART_PARITY_NONE| UART_STOP_BITS_1);

    UARTEnable(UART1, UART_ENABLE | UART_PERIPHERAL | UART_RX | UART_TX);
        if (UARTTransmitterIsReady(UART1)) {
            UARTSendDataByte(UART1, letter);
            
        }
    
}


