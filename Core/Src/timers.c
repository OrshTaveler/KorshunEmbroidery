#include "timers.h"
#include "rccconfig.h"

int ticks = 0;
// Настраиваем TIM2, для ШИМа
void configTIM2(){
	//Настройка таймера
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Включаем тактирование таймера TIM2

	AFIO->MAPR&=~AFIO_MAPR_TIM2_REMAP;

	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); // Включаем тактирование генераторов

	CLEAR_BIT(TIM2->CR1, TIM_CR1_UDIS);// Включаем генерацию событий Update

	SET_BIT(TIM2->CR1, TIM_CR1_ARPE); // Включаем буферизацию (без понятия что это, включил чтоб было)

	CLEAR_BIT(TIM2->CR1, TIM_CR1_URS);// Генерируем прерывания при переполнении

	SET_BIT(TIM2->CR1, TIM_CR1_ARPE); // Начинаем считать заново когда досчитали

	CLEAR_REG(TIM2->DIER);
	SET_BIT(TIM2->DIER, TIM_DIER_CC1IE); // Включаем прерывания по Update
	SET_BIT(TIM2->DIER, TIM_DIER_UIE);


	TIM2->PSC = 8000; // прескалер делит на 8000
	TIM2->ARR = 1000 - 1; // авто-релоуд когда досчитали до 8000
	// Таким образом прерывания по переполнению должны возникать раз в секунду (8МГц / 8000 * 8000 = 1Гц)
	// То есть ШИМ будет тоже с частотой 1Гц

	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_CC1S_Msk, 0b00 << TIM_CCMR1_CC1S_Pos); // Настраиваем первый канал на выход
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_OC1M_Msk, 0b110 << TIM_CCMR1_OC1M_Pos); // Настраиваем таймер в ШИМ мод 1 т.е. в считающий от 0 до CCR1


	// Настраиваем ножку A0 на выход ШИМА с максимальной частотой 10МГц
	GPIOA->CRL|=GPIO_CRL_MODE0;
	GPIOA->CRL|=(GPIO_CRL_CNF0_1);
	GPIOA->CRL&=~(GPIO_CRL_CNF0_0);

	//Настройка ШИМ
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_CC1S_Msk, 0b00 << TIM_CCMR1_CC1S_Pos); // настраиваем первый канал как выход
	SET_BIT(TIM2->CCMR1, TIM_CCMR1_OC1PE); // Включил потому что в доке написано, что это надо для работы ШИМа
	MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_OC1M_Msk, 0b110 << TIM_CCMR1_OC1M_Pos); //PWM MODE 1

	CLEAR_REG(TIM2->CCER);
	SET_BIT(TIM2->CCER, TIM_CCER_CC1E); //On - OC1 signal is output on the corresponding output pin.
	CLEAR_BIT(TIM2->CCER, TIM_CCER_CC1P);

	TIM2->CCR1 = 800;

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);

	SET_BIT(TIM2->CR1, TIM_CR1_CEN); // Включаем таймер TIM2

}

void TIM2_IRQHandler(void) {
	if (TIM2->SR & TIM_SR_CC1IF) {
		TIM2->SR &= ~TIM_SR_CC1IF;  //Сбросим флаг прерывания
		PortSetHi();
		delayMs(100);
		PortSetLow();
	}
}


// Настраиваем TIM3 для функции delay
void configTIM3(){
	TIM3->PSC = 0;
	TIM3->ARR = 1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Включаем тактирование таймера TIM3
	SET_BIT(TIM3->CR1, TIM_CR1_URS);// Генерируем прерывания при переполнении
	SET_BIT(TIM3->EGR,TIM_EGR_UG);
    // Включаем прерывания по Update
	SET_BIT(TIM3->DIER, TIM_DIER_UIE);

	NVIC_EnableIRQ(TIM3_IRQn);


}

void TIM3_IRQHandler(void) {
	    ticks++;
		TIM3->SR &= ~TIM_SR_UIF;  //Сбросим флаг прерывания
}

void delayMs(int delay){
	ticks = 0;
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	while (ticks < delay){}
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
}
