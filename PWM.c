
#include "lib/include.h"



extern void Configura_Reg_PWM0(uint16_t freq)
{
    //EN ESTA LINEA SE HABILITA EL CLOCK DEL MODULO PWM0. RECORDAR QUE LA TIVA GRANDE SOLO TIENE 1, PAG 398 
    SYSCTL->RCGCPWM |= (1<<0); 
    
    //AQUÍ HABILITAMOS EL RELJO DEL GPIO EN EL PORT G Y F; PG0 PF1 PF2 / P.383
    SYSCTL->RCGCGPIO |= (1<<6) | (1<<5); 
    
    //AQUÍ SE HABILITA LA FUNCIPON ALTERNATIVA CON EL AFSEL PARA PIN PG0 P.770  
    GPIOG_AHB->AFSEL |= (1<<0); /*Control de registros ya sea por GPIO o Otros Pag 672*/
   //         (ENABLE << PIN)
   
   //AQUÍ HABILITAMOS COMO PIN DE SALIDA AL PG0, PF1, PF2 , P.760
    GPIOG_AHB->DIR = (1<<0);
    //                 PG0
    GPIOF_AHB->AFSEL |= (1<<1) | (1<<2);
    //                    PF1     PF2
    //(ENTRADA / SALIDA<< PIN)

    //AQUI LE DECIMOS QUE LOS PINES DEL PWM1 FUNCIONARAN COMO SALIDA
    GPIOG_AHB->DIR = (1<<0);
    GPIOF_AHB->DIR = (1<<1) | (1<<2);

    //FUNCION ALTERNATIVA 
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006;//PORT G
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF00F) | 0x00000660;// PORT F
    
    
    //HABILITAMOS NUESTROS PINES COMO DIGITALES P.781
    GPIOG_AHB->DEN |= (1<<0); 
    GPIOF_AHB->DEN |= (1<<1) | (1<<2); 
    // (DIGITAL/ANALOG<<PIN)  1->DIGITAL
    //                        0->ANALOG   

    //ACTIVACIÓN DEL DIVISOR 
    PWM0->CC =  (1<<8) | (0<<2) | (1<<1) | (0<<0);;  /*Enable o Disable Divisor  Pag 1747*/
    
    //________________________________________________PWM SETTING
    
    //BLOQUE Y DESBLOQUEO DEL GENERADOR, EN ESTE CASO, BLOQUEO
    PWM0->_0_CTL = (0<<0);
    PWM0->_1_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);
    
    /*USAREMOS  EL COMAPARADOR A, EL PWM-GENA LANZA UN DISPARO EN EL PWM
    CUANDO LOS VALORES DE COMPRARACIÓN COINCIDEN */
    PWM0->_0_GENB |= (0X2<<2)|(0X3<<10)|(0x0<<0);   
    PWM0->_1_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0);  
    PWM0->_2_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0); 

    //ASIGNAMOS EL VALOR DE CUENTAS
/*            CUENTAS=F.CLK /F.PWM
            EJ. CON 50 kHz 
            CUENTAS=50 X10^6/ 10 000 = 50000 CUENTAS*/
    PWM0->_0_LOAD = 50000;
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;

    //ASIGNACIÓN DEL CICLO DE TRABAJO-> ESTA A 100% DE LAS 50000,
    // 50000 EN ALTO. EQUIIVALENTES A 2 MILISEGUNDOS
    PWM0->_0_CMPB = 5000;
    PWM0->_1_CMPA = 5000;           
    PWM0->_2_CMPA = 5000;

    // DESBLOQUEAMOS GEN
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    //HABILITO; RECORDAR LA LOGICA DE TRABAJO DE LA TIVA, APAGO, CONFIGURO Y LUEGO ENCIENDO.
    PWM0->ENABLE = (1<<1) | (1<<2) |(1<<4); 

}
