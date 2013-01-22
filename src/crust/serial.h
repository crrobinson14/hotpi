#ifndef SERIAL_H_
#define SERIAL_H_

#define SERIAL_BUFFER_SIZE 32

extern void serial_init(void);
extern void serial_send(void);
extern void serial_read(void);

extern uint8_t serial_bytes_out;
extern uint8_t serial_buffer_out[SERIAL_BUFFER_SIZE];

extern uint8_t serial_bytes_in;
extern uint8_t serial_buffer_in[SERIAL_BUFFER_SIZE];

#endif
