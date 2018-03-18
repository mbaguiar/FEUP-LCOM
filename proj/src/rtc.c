#include "rtc.h"

int hook_id_rtc = 8;
static Date date;
void (*rtcListener)(Date date);

int rtc_subscribe(int PI, int AI, int UI){

        unsigned long data;


        if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) {
                print_location();
                print_dbg("Error issuing read command\n");
                return 1;
        }

        if (sys_inb(RTC_DATA_REG, &data) != OK) {
                print_location();
                print_dbg("Error reading RTC_REG_B\n");
                return 1;
        }

        data &= CLEAR_INT_SOURCE;

        if (PI) data |= RTC_PIE;
        if (AI) data |= RTC_AIE;
        if (UI) data |= RTC_UIE;

        if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) {
                print_location();
                print_dbg("Error issuing write command\n");
                return 1;
        }

        if (sys_outb(RTC_DATA_REG, data) != OK) {
                print_location();
                print_dbg("Error writing new config\n");
                return 1;
        }

        if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_rtc) != OK) {
                print_location();
                print_dbg("Error calling sys_irqsetpolicy function\n");
                return 1;
        } else if (sys_irqenable(&hook_id_rtc) != OK) {
                print_location();
                print_dbg("Error calling sys_irqenable function\n");
                return 1;
        }
        return 0;

}

int rtc_unsubscribe(){

        unsigned long data;

        if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) {
                print_location();
                print_dbg("Error issuing read command\n");
                return 1;
        }

        if (sys_inb(RTC_DATA_REG, &data) != OK) {
                print_location();
                print_dbg("Error reading RTC_REG_B\n");
                return 1;
        }

        data = data & CLEAR_INT_SOURCE;

        if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) {
                print_location();
                print_dbg("Error issuing write command\n");
                return 1;
        }

        if (sys_outb(RTC_DATA_REG, data) != OK) {
                print_location();
                print_dbg("Error writing new config\n");
                return 1;
        }


        if (sys_irqdisable(&hook_id_rtc) != OK) {
                print_location();
                print_dbg("Error calling sys_irqdisable\n");
                return 1;
        } else if(sys_irqrmpolicy(&hook_id_rtc) != OK) {
                print_location();
                print_dbg("Error calling sys_irqrpolicy\n");
                return 1;
        }

        return 0;

}

void rtc_handler(){
        rtcIntHandlerASM();
        short data = rtcData;

        if( data & RTC_UF )
                handle_update_int();

        rtcListener(date);

}

int binaryMode(){
        
        rtcIntHandlerASM();
        short data = rtcData;

        return (data & RTC_DM);

}

int updateDate(){
        //TODO: test if bcd
        if (sys_outb(RTC_ADDR_REG, RTC_YEAR) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.year) != OK) {return 1;}
        if (sys_outb(RTC_ADDR_REG, RTC_MONTH) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.month) != OK) {return 1;}
        if (sys_outb(RTC_ADDR_REG, RTC_DAY) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.day) != OK) {return 1;}


        if (!binaryMode()) {
                BCDtoBinary(&date.year);
                BCDtoBinary(&date.month);
                BCDtoBinary(&date.day);
        }
        return 0;
}

int updateHour(){
        //TODO: test if bcd and 24/12hr clock
        if (sys_outb(RTC_ADDR_REG, RTC_HOUR) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.hour) != OK) {return 1;}
        if (sys_outb(RTC_ADDR_REG, RTC_MIN) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.minutes) != OK) {return 1;}
        if (sys_outb(RTC_ADDR_REG, RTC_SEC) != OK) {return 1;}
        if (sys_inb(RTC_DATA_REG, &date.seconds) != OK) {return 1;}



        if (!binaryMode()) {
                BCDtoBinary(&date.hour);
                BCDtoBinary(&date.minutes);
                BCDtoBinary(&date.seconds);
        }
        return 0;
}

void handle_update_int(){
        updateDate();
        updateHour();
        return;
}

void BCDtoBinary(unsigned long * data){
        unsigned long fst, scd;
        fst = (*data & 0xF0) >> 4;
        scd = *data & 0x0F;
        *data = fst * 10 + scd;
        return;
}

Date getCurrentDate(){
        handle_update_int();
        return date;
}

void setRTCListener (void (*listener)(Date date)) {
        rtcListener = listener;
}
