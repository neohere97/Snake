#ifndef REGDEF_
#define REGDEF_

#define CLK_DIVR	(*(volatile uint8_t *)0x50c6)
#define CLK_PCKENR1	(*(volatile uint8_t *)0x50c7)

#define UART1_SR	(*(volatile uint8_t *)0x5230)
#define UART1_DR	(*(volatile uint8_t *)0x5231)
#define UART1_BRR1	(*(volatile uint8_t *)0x5232)
#define UART1_BRR2	(*(volatile uint8_t *)0x5233)
#define UART1_CR2	(*(volatile uint8_t *)0x5235)
#define UART1_CR3	(*(volatile uint8_t *)0x5236)

#define SPI_CR1 	(*(volatile uint8_t *)0x5200)
#define SPI_CR2 	(*(volatile uint8_t *)0x5201)
#define SPI_ICR 	(*(volatile uint8_t *)0x5202)
#define SPI_SR  	(*(volatile uint8_t *)0x5203)
#define SPI_DR  	(*(volatile uint8_t *)0x5204)
#define SPI_CRCPR  	(*(volatile uint8_t *)0x5205)
#define SPI_RXCRCR 	(*(volatile uint8_t *)0x5206)
#define SPI_TXCRCR 	(*(volatile uint8_t *)0x5207)


#define ADC_CSR 	(*(volatile uint8_t *)0x5400)
#define ADC_CR1 	(*(volatile uint8_t *)0x5401)
#define ADC_CR2 	(*(volatile uint8_t *)0x5402)
#define ADC_CR3 	(*(volatile uint8_t *)0x5403)
#define ADC_DRH 	(*(volatile uint8_t *)0x5404)
#define ADC_DRL 	(*(volatile uint8_t *)0x5405)
#define ADC_TDRH 	(*(volatile uint8_t *)0x5406)
#define ADC_TDRL 	(*(volatile uint8_t *)0x5407)
#define ADC_HTRH 	(*(volatile uint8_t *)0x5408)
#define ADC_HTRL 	(*(volatile uint8_t *)0x5409)
#define ADC_LTRH 	(*(volatile uint8_t *)0x540A)
#define ADC_LTRL 	(*(volatile uint8_t *)0x540B)
#define ADC_AWSRH 	(*(volatile uint8_t *)0x540C)
#define ADC_AWSRL 	(*(volatile uint8_t *)0x540D)
#define ADC_AWCRH 	(*(volatile uint8_t *)0x540E)
#define ADC_AWCRL 	(*(volatile uint8_t *)0x540F)

#endif