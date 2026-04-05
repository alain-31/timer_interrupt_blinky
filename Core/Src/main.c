#include "stm32f411xe.h"

static void gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << (5U * 2U));
    GPIOA->MODER |=  (1U << (5U * 2U));
}

static void tim2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* 16 MHz / 16000 = 1 kHz -> 1 ms tick */
    TIM2->PSC = 16000U - 1U;

    /* 500 ms period */
    TIM2->ARR = 500U - 1U;

    /* Load prescaler immediately */
    TIM2->EGR = TIM_EGR_UG;

    /* Clear update flag */
    TIM2->SR &= ~TIM_SR_UIF;

    /* Enable update interrupt */
    TIM2->DIER |= TIM_DIER_UIE;

    /* Start timer */
    TIM2->CR1 |= TIM_CR1_CEN;
}

static void nvic_init(void)
{
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        GPIOA->ODR ^= (1U << 5U);
    }
}

int main(void)
{
    gpio_init();
    tim2_init();
    nvic_init();

    while (1)
    {
        /* Nothing to do: blinking is interrupt-driven */
    }
}