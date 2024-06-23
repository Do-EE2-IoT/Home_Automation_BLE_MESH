
#ifndef __PERIPHERAL_H_
#define __PERIPHERAL_H_
#include "esp_err.h"
#include "hal/gpio_types.h"
#include "driver/i2c.h"


/*-------------------------------------------Input Library--------------------------------------------*/
typedef void (*input_callback_t )(int,uint64_t);

typedef enum {
    GPIO_INTR_rising= 1,     /*!< GPIO interrupt type : rising edge                  */
    GPIO_INTR_falling = 2,     /*!< GPIO interrupt type : falling edge                 */
    GPIO_INTR_ANY = 3,     /*!< GPIO interrupt type : both rising and falling edge */
} type_interrupt_e;

void input_io_create(gpio_num_t gpio_num, type_interrupt_e type);
void input_get_level(gpio_num_t gpio_num );
void input_callback_register(void *cb);

/**---------------------------------------Output Library-----------------------------------------------*/

typedef enum{
    LOW = 0,
    HIGH = 1,
}type_output_e;

void output_io_create(gpio_num_t gpio_num);
void output_set_level(gpio_num_t gpio_num, type_output_e type );
void output_toggle_pin(gpio_num_t gpio_num);



/** -------------------------------I2C Library--------------------------------------------------*/

void i2c_master_write_to_slave(i2c_port_t i2c_num,uint8_t device_address,uint8_t* write_buffer,int write_leng,int time_out);
void i2c_master_read_from_slave(i2c_port_t i2c_num,uint8_t device_address,uint8_t* read_buffer,int read_leng,int time_out);
#endif