#include <stdbool.h>
#include "stm32f1xx.h"

#define queue_lenght 10
#define PWM_DUTY 50

extern int pwm_duty;
extern int step_counter;
extern int command_count;

enum direction{
	X_LEFT,
	X_RIGHT,
	Y_DOWN,
	Y_UP
};

struct command{
	bool abort_flag; // Если истина выключается питание, очищается очередь
	bool pause_flag; // Если истина, то выполнение останавливается до прихода команды с флагом resume_flag
	bool resume_flag;
	int step_count; // Количество шагов мотора
	enum direction direction; // Направление
};

static struct command command_queue[queue_lenght];


void change_direction(enum direction);

void add_command_in_queue(struct command cmd);

void perfom_command();
