#include <stdint.h>
#include <string.h>

/* Địa chỉ thanh ghi RCC */
#define RCC_BASE        0x40021000
#define RCC_APB2ENR     *(volatile uint32_t *)(RCC_BASE + 0x18)

/* Địa chỉ thanh ghi GPIOA */
#define GPIOA_BASE      0x40010800
#define GPIOA_CRH       *(volatile uint32_t *)(GPIOA_BASE + 0x04)

/* Địa chỉ thanh ghi USART1 */
#define USART1_BASE     0x40013800
#define USART1_SR       *(volatile uint32_t *)(USART1_BASE + 0x00)
#define USART1_DR       *(volatile uint32_t *)(USART1_BASE + 0x04)
#define USART1_BRR      *(volatile uint32_t *)(USART1_BASE + 0x08)
#define USART1_CR1      *(volatile uint32_t *)(USART1_BASE + 0x0C)

/* Địa chỉ NVIC */
#define NVIC_ISER1      *(volatile uint32_t *)(0xE000E104)

#define MAX_BUF_LEN 100
const char HEADER[] = "B23DCDT075_Le Duc Duy: ";
uint8_t rx_byte;
uint8_t rx_buffer[MAX_BUF_LEN];
uint16_t rx_index = 0;
uint8_t flag_complete = 0;

void SystemInit(void) {
}

/* Định nghĩa các hàm xử lý ngắt ngoại lệ */
void NMI_Handler(void) {
    while (1);
}

void HardFault_Handler(void) {
    while (1);
}

void USART1_IRQHandler(void) {
    if (USART1_SR & (1 << 5)) { // RXNE flag
        rx_byte = (uint8_t)(USART1_DR & 0xFF);
        if (rx_index < MAX_BUF_LEN - 1) {
            rx_buffer[rx_index++] = rx_byte;
            if (rx_byte == '!') {
                rx_buffer[rx_index] = '\0';
                flag_complete = 1;
            }
        } else {
            rx_index = 0;
        }
    }
}

void UART1_SendString(const char *str) {
    while (*str) {
        while (!(USART1_SR & (1 << 7))); // TXE flag
        USART1_DR = (*str++);
    }
}

void UART1_Init(void) {
    RCC_APB2ENR |= (1 << 2) | (1 << 14);
    
    GPIOA_CRH &= ~(0x00000FF0);
    GPIOA_CRH |= (0x0B << 4) | (0x04 << 8);
    
    USART1_BRR = 0x45;
    
    USART1_CR1 |= (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);
    
    NVIC_ISER1 |= (1 << 5);
}

int main(void) {
    UART1_Init();

    while (1) {
        if (flag_complete == 1) {
            UART1_SendString(HEADER);
            UART1_SendString((char*)rx_buffer);
            UART1_SendString("\r\n");
            
            rx_index = 0;
            flag_complete = 0;
        }
    }
}
