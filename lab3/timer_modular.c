#include "timer_modular.h"

int hook_id_timer = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq) {
    
        // TIMER_FREQ/freq must be written in 16 bits, so freq can't be lower than 19
    
        if (freq < 19){
            print_location();
            print_dbg("Invalid frequency.\n");
            return 1;
        }
    
        if(test_valid_timer(timer)) return 1;
    
        unsigned char config;
    
        if(timer_get_conf(timer, &config)) return 1;
    
        unsigned long ctrl_word = (BIT(3) | BIT(2) | BIT(1) | BIT(0)) & config; // Bitwise AND preserving only the 4 lsb's.
    
        // Assigning corresponding bit according to timer
        if (timer != 0){
    
            unsigned long n = timer + 5;
            ctrl_word = ctrl_word | BIT(n);
    
        }
    
        ctrl_word = ctrl_word | TIMER_LSB_MSB; //Building control word
    
        if(sys_outb(TIMER_CTRL, ctrl_word)){
            print_location();
            print_dbg("Error writing control word 0x%x to the control register.\n", ctrl_word);
            return 1;
    
        }
    
        unsigned long new_frequency = TIMER_FREQ / freq;
        unsigned char lsb = (unsigned char) new_frequency; //Truncate last 8 bits
        unsigned char msb = (unsigned char ) (new_frequency >> 8); //Shift and truncate first 8 bits
        unsigned long cTimer = get_timer_address(timer);
    
        if (sys_outb(cTimer, lsb)){
            print_location();
            print_dbg("Error loading timer %u with LSB for frequency configuration.\n");
            return 1;
    
        }
    
        if (sys_outb(cTimer, msb)){
            print_location();
            print_dbg("Error loading timer %u with MSB for frequency configuration.\n");
            return 1;
    
        }
    
        return 0;
    
    }


    int timer_subscribe() {
        
            if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != OK) {
                print_location();
                print_dbg("Error calling sys_irqsetpolicy function\n");
                return -1;
            } else if(sys_irqenable(&hook_id_timer) != OK) {
                print_location();
                print_dbg("Error calling sys_irqenable function\n");
                return -1;
            }
        
            return BIT(hook_id_timer);
        }
        
        int timer_unsubscribe() {
        
            if(sys_irqdisable(&hook_id_timer) != OK) {
                print_location();
                print_dbg("Error calling sys_irqdisble\n");
                return 1;
            } else if(sys_irqrmpolicy(&hook_id_timer) != OK) {
                print_location();
                print_dbg("Error calling sys_irqrmpolicy\n");
                return 1;
            }
        
            return 0;
        }

        unsigned long get_timer_address(unsigned char timer){
            
                if (timer == 0){
                    return TIMER_0;
                } else if (timer == 1){
                    return TIMER_1;
                } else {
                    return TIMER_2;
                }
            
            }
            
            int test_valid_timer(unsigned char timer){
            
                if (timer == 0 || timer == 1 || timer == 2) return 0;
                else {
                    printf("Timer %u does not exist.\n", timer);
                    return 1;
                }
            
            }

            int timer_get_conf(unsigned char timer, unsigned char *st) {
                
                    if(test_valid_timer(timer))
                        return 1;
                
                    unsigned long cmd = TIMER_RB_CMD | TIMER_RB_SEL(timer);
                
                    if (sys_outb(TIMER_CTRL, cmd)){
                        print_location();
                        print_dbg("Error writing read-back command to the control register.\n");
                        return 1;
                    }
                
                    unsigned long config;
                    unsigned long cTimer; //Selected timer address
                
                    cTimer = get_timer_address(timer);
                
                    if (sys_inb(cTimer, &config)){
                        print_location();
                        print_dbg("Error reading configuration from timer %u.\n", timer);
                        return 1;
                    } else {
                
                        *st = (unsigned char) config;
                        return 0;
                    }
                
                }