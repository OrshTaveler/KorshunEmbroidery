#define PIN_HIGH(GPIO,X) GPIO->BSRR = (1<<X)
#define PIN_LOW(GPIO,X) GPIO->BRR = (1<<X)
