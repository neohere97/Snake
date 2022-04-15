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

#define PA_ODR   	(*(volatile uint8_t *)0x5000)
#define PA_IDR   	(*(volatile uint8_t *)0x5001)
#define PA_DDR   	(*(volatile uint8_t *)0x5002)
#define PA_CR1   	(*(volatile uint8_t *)0x5003)
#define PA_CR2   	(*(volatile uint8_t *)0x5004)

#define PB_ODR   	(*(volatile uint8_t *)0x5005)
#define PB_IDR   	(*(volatile uint8_t *)0x5006)
#define PB_DDR   	(*(volatile uint8_t *)0x5007)
#define PB_CR1   	(*(volatile uint8_t *)0x5008)
#define PB_CR2   	(*(volatile uint8_t *)0x5009)

#define PC_ODR   	(*(volatile uint8_t *)0x500A)
#define PC_IDR   	(*(volatile uint8_t *)0x500B)
#define PC_DDR   	(*(volatile uint8_t *)0x500C)
#define PC_CR1   	(*(volatile uint8_t *)0x500D)
#define PC_CR2   	(*(volatile uint8_t *)0x500E)

#define PD_ODR   	(*(volatile uint8_t *)0x500F)
#define PD_IDR   	(*(volatile uint8_t *)0x5010)
#define PD_DDR   	(*(volatile uint8_t *)0x5011)
#define PD_CR1   	(*(volatile uint8_t *)0x5012)
#define PD_CR2   	(*(volatile uint8_t *)0x5013)

#define EXTI_CR1   	(*(volatile uint8_t *)0x50A0)
#define EXTI_CR2   	(*(volatile uint8_t *)0x50A1)

#define CCR        	(*(volatile uint8_t *)0x7F0A)
#define CFG_GCR    	(*(volatile uint8_t *)0x7F60)



#endif