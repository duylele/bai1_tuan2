#include <stdint.h>

#define RCC_BASE        0x40021000
#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x18)

#define GPIOA_BASE      0x40010800
#define GPIOA_CRH       *(volatile uint32_t *)(GPIOA_BASE + 0x04)

#define USART1_BASE     0x40013800
#define USART1_SR       *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_DR       *(volatile uint32_t *)(USART1_BASE + 0x04)
#define USART1_BRR      *(volatile uint32_t *)(USART1_BASE + 0x08)
#define USART1_CR1      *(volatile uint32_t *)(USART1_BASE + 0x0C)

#define BUFFER_SIZE     128
#define HEADER_PREFIX   "D23DCDT075: " // Đã cập nhật mã D23DCDT075

char rx_buffer[BUFFER_SIZE];
uint16_t rx_index = 0;

void SystemInit(void) {}
void NMI_Handler(void) { while(1); }
void HardFault_Handler(void) { while(1); }
void USART1_IRQHandler(void) { while(1); }

void UART1_SendChar(char c) {
    while (!(USART1_SR & (1 << 7)));
    USART1_DR = c;
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

char UART1_ReceiveChar(void) {
    while (!(USART1_SR & (1 << 5)));
    return (char)(USART1_DR & 0xFF);
}

int main(void) {
    // 1. Enable Clock GPIOA & USART1
    RCC_APB2ENR |= (1 << 2) | (1 << 14);

    // 2. PA9 (TX1 - AF_PP), PA10 (RX1 - IN_FLOAT)
    GPIOA_CRH &= ~(0x00000FF0);
    GPIOA_CRH |= (0x0B << 4) | (0x04 << 8);

    // 3. Baudrate 9600
    USART1_BRR = 0x341;

    // 4. Enable USART, TX, RX
    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2);

    while (1) {
        char c = UART1_ReceiveChar();

        if (c == '!') {
            rx_buffer[rx_index] = '\0';

            UART1_SendString(HEADER_PREFIX);
            UART1_SendString(rx_buffer);
            UART1_SendString("\n\r");

            rx_index = 0;
        } else {
            if (rx_index < BUFFER_SIZE - 1) {
                rx_buffer[rx_index++] = c;
            }
        }
    }
}
