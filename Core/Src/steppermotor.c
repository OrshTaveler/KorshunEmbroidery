#include "steppermotor.h"
#include "GPIO.h"

int pwm_duty = 0;
int step_counter = 0;
int command_count = 0;

void change_direction(enum direction dir){
    switch (dir) {
	case X_LEFT:
    	PIN_HIGH(GPIOB,10);
    	PIN_HIGH(GPIOB,11);
    break;
	case X_RIGHT:
    	PIN_LOW(GPIOB,11);
    	PIN_LOW(GPIOB,10);
    break;
    case Y_DOWN:
    	PIN_HIGH(GPIOB,11);
    	PIN_LOW(GPIOB,10);
    break;
    case Y_UP:
    	PIN_HIGH(GPIOB,10);
    	PIN_LOW(GPIOB,11);
    break;
	}
}

void add_command_in_queue(struct command cmd){
	if (command_count >= queue_lenght) return; // later change to call usb function
	command_queue[command_count] = cmd;
	command_count++;
}
void move_commands(){
	for (int i = 1; i < command_count; i++){
		command_queue[i -1] = command_queue[i];
	}
}
void perfom_command(){
	PIN_LOW(GPIOC,13);
	// Проверяем есть ли команды в очереди
	if (command_count == 0) {
	        pwm_duty = 0;
	        step_counter = 0;

	        // Тестово добавляю команды чтобы пройтись по квадрату

	        add_command_in_queue((struct command){0,0,0,500,X_RIGHT});
	        add_command_in_queue((struct command){0,0,0,500,Y_UP});
	        add_command_in_queue((struct command){0,0,0,500,X_LEFT});
	        add_command_in_queue((struct command){0,0,0,500,Y_DOWN});

	        return;
	    }
	// Проверяем не ноль ли заполнение ШИМа и если ноль, то меняем направление и делаем его не нулём
	if (pwm_duty == 0){
	    	 	change_direction(command_queue[0].direction);
	    	    pwm_duty = PWM_DUTY;
	    	    delayMs(10);
	}
	// Считаем шаги
	step_counter++;
	// Как только сделали нужное количество шагов делаем заполнение ШИМа 0 и вот это вот всё
	if (step_counter >= command_queue[0].step_count){
	    	move_commands();
	    	command_count--;
	    	pwm_duty = 0;
	    	step_counter = 0;
	    	PIN_HIGH(GPIOC,13);
	    	delayMs(100);
	}
}
