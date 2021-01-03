#include "i2c.h"

//--------------------------------------------------------------------------------------------------------------------//
_Bool I2cInit(void){

  GPIOB->CRL |= (GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
  GPIOB->CRL |= (GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
  
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  I2C1->CR1 = 0x00;
  I2C1->CR2 = 0x00;
  I2C1->CR2 |= 0x36;
  
  
  CLEAR_BIT(I2C1->OAR2, I2C_OAR2_ENDUAL);
  //Disable General Call
  CLEAR_BIT(I2C1->CR1, I2C_CR1_ENGC);
  //Enable Clock stretching
  CLEAR_BIT(I2C1->CR1, I2C_CR1_NOSTRETCH);
  //Disable I2C peripheral
  CLEAR_BIT(I2C1->CR1, I2C_CR1_PE);
  //ClockSpeed
  MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ, 36);
  //MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, 36 + 1); //100
  MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, 11); //400
  //MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 180); //100
  MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 0x0000801E); //400
  //Set the Own Address1
//  MODIFY_REG(I2C1->OAR1, I2C_OAR1_ADD0 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD8_9 | I2C_OAR1_ADDMODE, I2C_OWNADDRESS1_7BIT);
  //Configure I2C1 peripheral mode with parameter : I2C
//  MODIFY_REG(I2C1->CR1, I2C_CR1_SMBUS | I2C_CR1_SMBTYPE | I2C_CR1_ENARP, I2C_MODE_I2C);
  //Enable I2C peripheral
  SET_BIT(I2C1->CR1, I2C_CR1_PE);
  //TypeAcknowledge
  MODIFY_REG(I2C1->CR1, I2C_CR1_ACK, I2C_CR1_ACK);
  //Set the 7bits Own Address2
  MODIFY_REG(I2C1->OAR2, I2C_OAR2_ADD2, 0);

  return false;
}
