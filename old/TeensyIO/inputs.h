/*
 * inputs.h
 */ 


#ifndef INPUTS_H_
#define INPUTS_H_

#define INPUT_COUNT 3

extern uint8_t input_values[INPUT_COUNT];

extern void inputs_read();
extern void inputs_init(void);

#endif /* INPUTS_H_ */