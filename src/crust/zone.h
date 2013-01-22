#ifndef ZONE_H_
#define ZONE_H_

extern void zone_init(void);
extern void zone_read(void);
extern void zone_set(void);
extern void zone_force(uint8_t zone, uint8_t state);

extern uint8_t zone_force[8];
extern uint8_t zone_demand[8];

#endif
