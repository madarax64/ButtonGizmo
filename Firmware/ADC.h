/* Header file for ADC Driver */


#ifndef ADC_H_
#define ADC_H_
#include <inttypes.h>

void ADC_Init();
uint16_t ADC_Read(int);
uint16_t ADC_Read_Raw(int);



#endif /* ADC_H_ */