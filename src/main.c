#include <stdint.h>

#define __IO volatile
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN16 16

#define WIDTH2 2
#define WIDTH4 4

#define RESET2 3
#define RESET4 0xF

typedef struct SYST_t{
    __IO uint32_t SYST_CSR;
    __IO uint32_t SYST_RVR;
    __IO uint32_t SYST_CVR;
    __IO uint32_t SYST_CALIB;

}SYST_t;

typedef struct RCC_t{
	__IO uint32_t RCC_CR;
	__IO uint32_t RCC_PLLCFGT;
	__IO uint32_t RCC_CFGR;
	__IO uint32_t RCC_CIR;
	__IO uint32_t RCC_AHB1RSTR;
	__IO uint32_t RCC_AHB2RSTR;
	__IO uint32_t res1[2];
	__IO uint32_t RCC_APB1RSTR;
	__IO uint32_t RCC_APB2RSTR;
	__IO uint32_t res2[2];
	__IO uint32_t RCC_AHB1ENR;
	__IO uint32_t RCC_AHB2ENR;
	__IO uint32_t res3[2];
	__IO uint32_t RCC_APB1ENR;
	__IO uint32_t RCC_APB2ENR;
	__IO uint32_t res4[2];
	__IO uint32_t RCC_AHB1LPENR;
	__IO uint32_t RCC_AHB2LPENR;
	__IO uint32_t res5[2];
	__IO uint32_t RCC_APB1LPENR;
	__IO uint32_t RCC_APB2LPENR;
	__IO uint32_t res6[2];
	__IO uint32_t RCC_BDCR;
	__IO uint32_t RCC_CSR;
	__IO uint32_t res7[2];
	__IO uint32_t RCC_SSCGR;
	__IO uint32_t RCC_PLLI2SCFGR;
	__IO uint32_t res8;
	__IO uint32_t RCC_DCKCFGR;
}RCC_t;

typedef struct GPIOx_t{
	__IO uint32_t GPIOx_MODER; 		
	__IO uint32_t GPIOx_OTYPER;	
	__IO uint32_t GPIOx_OSPEEDER;	
	__IO uint32_t GPIOx_PUPDR;	
	__IO uint32_t GPIOx_IDR;
	__IO uint32_t GPIOx_ODR;	
	__IO uint32_t GPIOx_BSRR;	
	__IO uint32_t GPIOx_LCKR;
	__IO uint32_t GPIOx_AFRL;
	__IO uint32_t GPIOx_AFRH;
}GPIOx_t;

void wait(int time){
    for(int i = 0; i < time; i++){
        for(int j = 0; j < 1600; j++);
    }
}

RCC_t   * const RCC   = (RCC_t *)   0x40023800;
GPIOx_t * const GPIOA = (GPIOx_t *) 0x40020000;
SYST_t  * const SYST  = (SYST_t *)  0xE000E010;

int main(void){

    // Enable GPIOA
    RCC->RCC_AHB1ENR |= 1;

    // Set PIN5 as output
    GPIOA->GPIOx_MODER &= ~(RESET2 << (PIN5 * WIDTH2));
    GPIOA->GPIOx_MODER |=  (1      << (PIN5 * WIDTH2));

    // Load Reload Register with maximum value
    SYST->SYST_RVR = 2666666 - 1;
    // Disable Interrupt SYST
    SYST->SYST_CSR &= ~(1 << PIN1);
    // Set internal Clock as source and start timer
    SYST->SYST_CSR |=  ((1 << PIN2) | (1 << PIN0));

    for(;;){
        // Poll Status Register of 
        if(SYST->SYST_CSR & (1 << PIN16)){
            GPIOA->GPIOx_ODR ^= (1 << PIN5);
        }
    }
}
