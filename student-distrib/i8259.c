/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"


/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */
// 0xFF & 0xFB , 0xFF
/* Initialize the 8259 PIC */
void i8259_init(void) {

    outb(ICW1, MASTER_8259_PORT ); // initialize Primary PIC - sends command ICW1
    outb(ICW2_MASTER, MASTER_8259_PORT+1); // define Primary pic's vector offset
    outb(ICW3_MASTER, MASTER_8259_PORT+1); // inform the primary pic about secondary pic
    outb(ICW4, MASTER_8259_PORT+1); // set pics to 8086 mode
    //outb(master_mask, MASTER_8259_PORT); 

    outb(ICW1, SLAVE_8259_PORT); // initialize Secondary PIC - sends command ICW1 
    outb(ICW2_SLAVE, SLAVE_8259_PORT+1); // define secondary pic's vector offset
    outb(ICW3_SLAVE, SLAVE_8259_PORT+1); // set the secondary pic's cascade
    outb(ICW4, SLAVE_8259_PORT+1); // set pics to 8086 mode
    master_mask = 0xFF; // initialize all ports except secondary pin to be off
    slave_mask = 0xFF; // initalize all ports on secondary pic are off

    outb(master_mask, MASTER_8259_PORT+1);
    outb(slave_mask, SLAVE_8259_PORT+1 );
    enable_irq(2);
}

// uint8_t master_mask; /* IRQs 0-7  */
// uint8_t slave_mask;  /* IRQs 8-15 */

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    //uint8_t val;
    if(irq_num < 8){ // 8 ports per pic, if the irq number is less than 8 then use pic 1
        //val = ~(1 << irq_num) & inb(MASTER_8259_PORT+1); // sets the irq number in the master mask
        //printf("irg_num shiftinvert: %d", ~(1<<irq_num));
        master_mask = ~(1<<irq_num) & master_mask; // sets the irq number in the master mask

        outb(master_mask, MASTER_8259_PORT+1); // send the enabled mask to the primary pic
    }

    else{
        irq_num -= 8; // 8 ports per pic, subtracting 8 determines which port on the secondary pic is notified
       // val =  ~(1 << irq_num) & inb(SLAVE_8259_PORT+1); // sets the irq number in the slave mask
        slave_mask = ~( 1<<irq_num) & slave_mask; // sets the irq number in the master mask
        outb(slave_mask, SLAVE_8259_PORT+1); // send the enabled mask to the secondary pic
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    //uint8_t val;
   if(irq_num < 8){ // 8 ports per pic, if the irq number is less than 8 then use pic 1
        //val = ( 1<<irq_num) | inb(MASTER_8259_PORT+1); // sets the irq number in the master mask
       master_mask = ( 1<<irq_num) | master_mask; // sets the irq number in the master mask
        outb(master_mask, MASTER_8259_PORT+1); // send the enabled mask to the primary pic
    }
    else{
        irq_num -= 8; // 8 ports per pic, subtracting 8 determines which port on the secondary pic is notified
        //val =  (1 << irq_num) | inb(SLAVE_8259_PORT+1); // sets the irq number in the slave mask
        slave_mask = ( 1<<irq_num) | slave_mask; // sets the irq number in the master mask
        outb(slave_mask, SLAVE_8259_PORT+1); // send the enabled mask to the secondary pic
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num >= 8){
        outb(EOI | (irq_num-8), SLAVE_8259_PORT); // send EOI to secondary PIC
        outb(EOI| 2,MASTER_8259_PORT); // send EOI to irq number of secondary pic to primary pic
        // 2: secondary pic is connected to the second pin.

    }
    else{
        outb(EOI | irq_num,MASTER_8259_PORT); // send EOI to primary PIC
    }
}
