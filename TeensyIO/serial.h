/*
 * serial.h
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

extern void serial_init(void);
extern void serial_send(void);
extern uint8_t serial_read(void);

extern uint8_t serial_buffer_out[32];

extern uint8_t serial_bytes_in;
extern uint8_t serial_buffer_in[32];


#endif /* SERIAL_H_ */