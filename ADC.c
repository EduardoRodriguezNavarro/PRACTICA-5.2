#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    SYSCTL->RCGCADC = (1<<0);  // SE INICIALIZA MODULO DE CLK DE ADC
  
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<0);//PUERTOS DONDE HABILITAMOS EL RELOJ
    
    GPIOE_AHB->DIR =(0<<1) | (0<<2) | (0<<5); //HABIITAMOS PINES COMO ENTRADAS
    //                PE1      PE2     PE5
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<5);//SE ACTIVA FUNCIÓN ANALOGICA
    GPIOE_AHB->DEN = (0<<1) | (0<<2) | (0<<5 );//APAGAMOS DIGITAL MODE P.781
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F);
    GPIOE_AHB->AMSEL =(1<<1) | (1<<2) | (1<<5);//ACTIVAMOS ANALOG MODE P.786
    ADC0->PC = (0<<2)|(0<<1)|(1<<0);//LE ASIGNAMOS LA VELOCIDAD CON LA QUE CONVERTIRA POR CADA SEGUNDO
    //         =      250ksps
    ADC0->SSPRI = 0x3210;//LE INDICAMOS LA PRIORIDAD DE LOS SECUENCIADORES P.1099
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);//AQUÍ DESACTIVAMOS  LOS SECUENCIADORES P.1077
    ADC0->EMUX  = (0x0000);//AQUÍ LE INDICAMOS EL DISPARADOR(EVENTO QUE LE DICE CUANDO COMENZAR A ACTICVAR)P.1091
    //AQUI ASIGANAMOS ENTRADAS ANALOG EN EL CANAL Y SECUANCIADOR QUE SE ELIGIO 
    ADC0->SSMUX0 = (1<<0); 
    ADC0->SSMUX1 = (2<<0); 
    ADC0->SSMUX2 = (8<<0);

    //INDICAMOS EL BIT QUE CONTROLA EL MUESTREO ASÍ COMO INTERRUOCION P.868
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL2 = (1<<2) | (1<<1);

    ADC0->IM |= (1<<0) | (1<<1) |(1<<2);
    
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0);//REACTIVAMOS SECUENCIADORES
    
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2);
}
extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty){

       ADC0->PSSI = (1<<0) | (1<<1) | (1<<2);;
       while((ADC0->RIS&0x04)==0){}; // CONVERTIDOR ES EL QUE CONTROLA ESTE WHILE
      
    Result[0] = ADC0->SSFIFO0&0xFFF;
                    //FIFO 0 GUARDA MUESTREO DE SECUENCIADOR 0 P. 860 
    duty[0] = (Result[0]*50000)/4096;
     //20MS EN VALOR DE CUENTAS 
    
    Result[1] = ADC0->SSFIFO1&0xFFF;
     //FIFO 1 GUARDA MUESTREO DE SEC. 1 
    duty[1] = (Result[1]*50000)/4096; 
     
    Result[2] = ADC0->SSFIFO2&0xFFF;
    // FIFO 2 GUARDA MUESTREO DE SEC. 2 
    duty[2] = (Result[2]*50000)/4096;
    
    ADC0->ISC = (1<<0) | (1<<1) | (1<<2); 

}


