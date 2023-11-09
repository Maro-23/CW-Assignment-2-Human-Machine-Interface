
#define Set_Bit(reg, bit)    (reg |= (1<<bit)) //for setting 1 bit in register to 1

#define Clear_Bit(reg, bit)  (reg &= ~(1<<bit))//for setting 1 bit in register to 0

#define Get_Bit(reg, bit)    ((reg >> bit) & 1)//for getting the value of 1 bit from the register