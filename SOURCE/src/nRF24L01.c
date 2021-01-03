#include "nRF24L01.h"

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01Spi(uint8_t byte){
  while(!(NRF24L01_SPI->SR & SPI_SR_TXE));
  NRF24L01_SPI->DR = byte;
  while(!(NRF24L01_SPI->SR & SPI_SR_RXNE));
  return NRF24L01_SPI->DR;
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01ReadReg(uint8_t reg){// Читает значение однобайтового регистра reg (от 0 до 31) и возвращает его
  NRF24L01_CS_LOW;
  Nrf24l01Spi((reg & 0x1F) | NRF24_R_REGISTER);
  uint8_t byte = Nrf24l01Spi(0xFF);
  NRF24L01_CS_HIGHT;
  return byte;
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01WriteReg(uint8_t reg, uint8_t val){// Записывает значение однобайтового регистра reg (от 0 до 31), возвращает регистр статуса
  NRF24L01_CS_LOW;
  uint8_t status = Nrf24l01Spi((reg & 0x1F) | NRF24_W_REGISTER);
  Nrf24l01Spi(val);
  NRF24L01_CS_HIGHT;
  return status;
}

//--------------------------------------------------------------------------------------------------------------------//
uint32_t Nrf24ReadAdr(uint8_t reg, uint8_t count){// Выполняет команду cmd, и читает count байт ответа, помещая их в буфер buf, возвращает регистр статуса
  NRF24L01_CS_LOW;
  uint32_t adr = 0x00;
  uint8_t i = 0x00;
  Nrf24l01Spi((reg & 0x1F) | NRF24_R_REGISTER);
  while (count--){
    adr <<= (i * 0x08);
    adr |= Nrf24l01Spi(0xFF);
    i++;
  }
  NRF24L01_CS_HIGHT;
  return adr;
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01WriteAdr(uint8_t reg, uint32_t adr, uint8_t count){// Выполняет команду cmd, и передаёт count байт параметров из буфера buf, возвращает регистр статуса
  NRF24L01_CS_LOW;
  uint8_t status = Nrf24l01Spi((reg & 0x1F) | NRF24_W_REGISTER);
  while(count--){
    Nrf24l01Spi(adr);
    adr >>= 0x08;
  }
  NRF24L01_CS_HIGHT;
  return status;
}

/*
//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01ReadRegBuff(uint8_t reg, uint8_t *buff, uint8_t count){// Читает count байт многобайтового регистра reg (от 0 до 31) и сохраняет его в буфер buf, возвращает регистр статуса
  return Nrf24l01ReadBuff((reg & 0x1F) | NRF24_R_REGISTER, buff, count);
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01WriteRegBuff(uint8_t reg, uint8_t *buff, uint8_t count){// Записывает count байт из буфера buf в многобайтовый регистр reg (от 0 до 31), возвращает регистр статуса
  return Nrf24l01WriteBuff((reg & 0x1F) | NRF24_W_REGISTER, buff, count);
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01ReadRxPayloadWidth(void){// Возвращает размер данных в начале FIFO очереди приёмника
  NRF24L01_CS_LOW;
  Nrf24l01Spi(NRF24_R_RX_PL_WID);
  uint8_t answ = Nrf24l01Spi(0xFF);
  NRF24L01_CS_HIGHT;
  return answ;
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24l01Cmd(uint8_t cmd){// Выполняет команду. Возвращает регистр статуса
  NRF24L01_CS_LOW;
  uint8_t status = Nrf24l01Spi(cmd);
  NRF24L01_CS_HIGHT;
  return status;
}

//--------------------------------------------------------------------------------------------------------------------//
void Nrf24OnSendError(void){// Вызывается, когда превышено число попыток отправки, а подтверждение так и не было получено.
 // TODO здесь можно описать обработчик неудачной отправки
  
  
  
  
}

//--------------------------------------------------------------------------------------------------------------------//
void Nrf24OnPacket(uint8_t *buff, uint8_t size){// Вызывается при получении нового пакета по каналу 1 от удалённой стороны. // buf - буфер с данными, size - длина данных (от 1 до 32)
 // TODO здесь нужно написать обработчик принятого пакета

 // Если предполагается немедленная отправка ответа, то необходимо обеспечить задержку ,
 // во время которой чип отправит подтверждение о приёме 
 // чтобы с момента приёма пакета до перевода в режим PTX прошло:
 // 130мкс + ((длина_адреса + длина_CRC + длина_данных_подтверждения) * 8 + 17) / скорость_обмена
 // При типичных условиях и частоте МК 8 мГц достаточно дополнительной задержки 100мкс
}

//--------------------------------------------------------------------------------------------------------------------//
uint8_t Nrf24SendData(uint8_t *buff, uint8_t size){// Помещает пакет в очередь отправки. // buff - буфер с данными, size - длина данных (от 1 до 32)
  NRF24L01_CE_LOW; // Если в режиме приёма, то выключаем его 
  uint8_t conf = Nrf24ReadReg(NRF24_CONFIG);
  if(!(conf & NRF24_PWR_UP)) // Если питание по какой-то причине отключено, возвращаемся с ошибкой
    return false; 
  uint8_t status = Nrf24WriteReg(NRF24_CONFIG, conf & ~(NRF24_PRIM_RX)); // Сбрасываем бит PRIM_RX
  if(status & NRF24_TX_FULL_STATUS) // Если очередь передатчика заполнена, возвращаемся с ошибкой
    return false;
  Nrf24WriteBuff(NRF24_W_TX_PAYLOAD, buff, size); // Запись данных на отправку
  NRF24L01_CE_HIGHT; // Импульс на линии CE приведёт к началу передачи
  //DelayMc(0x0F); // Нужно минимум 10мкс, возьмём с запасом
  NRF24L01_CE_LOW;
  return true;
}

//--------------------------------------------------------------------------------------------------------------------//
void Nrf24CheckRadio(void){ //PD13
  uint8_t status = Nrf24Cmd(NRF24_NOP);
  Nrf24WriteReg(NRF24_STATUS, status); // Просто запишем регистр обратно, тем самым сбросив биты прерываний
  if(status & (NRF24_TX_DS | NRF24_MAX_RT)){ // Завершена передача успехом, или нет,
    
    
    
    if(status & NRF24_MAX_RT){ // Если достигнуто максимальное число попыток
      Nrf24Cmd(NRF24_FLUSH_TX); // Удалим последний пакет из очереди
      Nrf24OnSendError(); // Вызовем обработчик
    } 
    if(!(Nrf24ReadReg(NRF24_FIFO_STATUS) & NRF24_TX_EMPTY)){ // Если в очереди передатчика есть что передавать
      NRF24L01_CE_HIGHT; // Импульс на линии CE приведёт к началу передачи
      //DelayMc(0x0F); // Нужно минимум 10мкс, возьмём с запасом
      NRF24L01_CE_LOW;
    }else{
      uint8_t conf = Nrf24ReadReg(NRF24_CONFIG);
      Nrf24WriteReg(NRF24_CONFIG, conf | NRF24_PRIM_RX); // Устанавливаем бит PRIM_RX: приём
      NRF24L01_CE_HIGHT; // Высокий уровень на линии CE переводит радио-чип в режим приёма
    }
  }
  uint8_t protect = 0x04; // В очереди FIFO не должно быть более 3 пакетов. Если больше, значит что-то не так
  while(((status & NRF24_RX_P_NO) != NRF24_RX_P_NO) && protect--){ // Пока в очереди есть принятый пакет
    uint8_t l = Nrf24ReadRxPayloadWidth(); // Узнаём длину пакета
    if(l > 0x20){ // Ошибка. Такой пакет нужно сбросить
      Nrf24Cmd(NRF24_FLUSH_RX); 
    }else{ 
      uint8_t buff[0x20]; // буфер для принятого пакета
      Nrf24ReadBuff(NRF24_R_RX_PAYLOAD, &buff[0x00], l); // начитывается пакет
      if((status & NRF24_RX_P_NO) == NRF24_RX_P1){ // если datapipe 1 
        Nrf24OnPacket(&buff[0x00], l); // вызываем обработчик полученного пакета
      }
    }
    status = Nrf24Cmd(NRF24_NOP);
  }
}
 */
//--------------------------------------------------------------------------------------------------------------------//
_Bool Nrf24Init(void){// Производим первоначальную настройку устройства.
  uint8_t i = 0x32;
  uint8_t status = true;
  GPIOB->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF15);
  GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1;
  GPIOB->CRH |= GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15;
  
  GPIOD->CRH &= ~GPIO_CRH_CNF13_0;
  GPIOD->CRH |= GPIO_CRH_CNF13_1;
  GPIOD->CRH &= ~GPIO_CRH_MODE13;
  GPIOD->BSRR |= GPIO_BSRR_BS13;
  
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  NRF24L01_SPI->CR1 &= ~SPI_CR1_BR; // 40MHz // SPI2->CR1 |= SPI_CR1_BR_0; // 10MHz
  NRF24L01_SPI->CR1 &= ~SPI_CR1_CPOL;
  NRF24L01_SPI->CR1 &= ~SPI_CR1_CPHA;
  NRF24L01_SPI->CR1 &= ~SPI_CR1_DFF;
  NRF24L01_SPI->CR1 &= ~SPI_CR1_LSBFIRST;
  NRF24L01_SPI->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
  NRF24L01_SPI->CR1 |= SPI_CR1_MSTR;
  NRF24L01_SPI->CR1 |= SPI_CR1_SPE;
  
  NRF24L01_CE_LOW;
  do{
    Nrf24l01WriteReg(NRF24_CONFIG, NRF24_EN_CRC | NRF24_CRCO | NRF24_PRIM_RX); // Выключение питания
    DelayMs(0x01);
    if(Nrf24l01ReadReg(NRF24_CONFIG) == (NRF24_EN_CRC | NRF24_CRCO | NRF24_PRIM_RX)){
      Nrf24l01WriteReg(NRF24_EN_AA, NRF24_ENAA_P1 | NRF24_ENAA_P2); // включение автоподтверждения только по каналу 1 и 2
      Nrf24l01WriteReg(NRF24_EN_RXADDR, NRF24_ERX_P0 | NRF24_ERX_P1 | NRF24_ERX_P2); // включение каналов 0, 1 и 2
      Nrf24l01WriteReg(NRF24_SETUP_AW, NRF24_SETUP_AW_3BYTES_ADDRESS); // выбор длины адреса 3 байт
      Nrf24l01WriteReg(NRF24_SETUP_RETR, ((~(DEVICE_TYPE << 0x05)) & 0xE0) | NRF24_SETUP_15_RETRANSMIT);
      Nrf24l01WriteReg(NRF24_CH, (conf.settings.nRF24L01Ch & 0x7F)); // Выбор частотного канала
      Nrf24l01WriteReg(NRF24_SETUP, (((conf.settings.nRF24L01Speed << 0x01) & 0x20) | ((conf.settings.nRF24L01Speed << 0x03) & 0x08) | ((conf.settings.nRF24L01Power << 0x01) & 0x06)));
      Nrf24l01WriteAdr(NRF24_TX_ADDR,    ((conf.settings.nRF24L01Addr << 0x08) | conf.settings.nRF24L01Prim), 0x03);
      Nrf24l01WriteAdr(NRF24_RX_ADDR_P0, ((conf.settings.nRF24L01Addr << 0x08) | conf.settings.nRF24L01Prim), 0x03); // Подтверждения приходят на канал 0
      Nrf24l01WriteAdr(NRF24_RX_ADDR_P1, ((conf.settings.nRF24L01Addr << 0x08) | 0x80), 0x03);
      Nrf24l01WriteReg(NRF24_RX_ADDR_P2, 0xFF);
      Nrf24l01WriteReg(NRF24_RX_PW_P0, 32);
      Nrf24l01WriteReg(NRF24_RX_PW_P1, 32);
      Nrf24l01WriteReg(NRF24_RX_PW_P2, 32);
      Nrf24l01WriteReg(NRF24_DYNPD, NRF24_DPL_P0 | NRF24_DPL_P1 | NRF24_DPL_P2); // включение произвольной длины для каналов 0, 1 и 2
      Nrf24l01WriteReg(NRF24_FEATURE, NRF24_EN_DPL | NRF24_EN_ACK_PAY); // разрешение произвольной длины пакета данных
      Nrf24l01WriteReg(NRF24_CONFIG, NRF24_EN_CRC | NRF24_CRCO | NRF24_PWR_UP | NRF24_PRIM_RX); // Включение питания
      status = false;
      i = false;
    }
  }while(i--);// Если прочитано не то что записано, то значит либо радио-чип ещё инициализируется, либо не работает.
  if(!(status)){
    AFIO->EXTICR[0X03] |= AFIO_EXTICR4_EXTI13_PD;
    EXTI->FTSR |= EXTI_FTSR_TR13;
    EXTI->PR |= EXTI_PR_PR13;
    EXTI->IMR |= EXTI_IMR_MR13;
    NVIC_SetPriority(EXTI15_10_IRQn, PRIORITY_RF24);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
  }
  #if (defined DEBUG_NRF24 || defined INFO)
    if(!(status)){
      printf("< OK >    Initialization nRF24\r\n");
      printf("\t*----------------------------*\r\n");
      printf("\tRF24_TX_Primary:  0x%06X\r\n", Nrf24ReadAdr(NRF24_TX_ADDR, 0x03));
      printf("\tRF24_RX_Primary:  0x%06X\r\n", Nrf24ReadAdr(NRF24_RX_ADDR_P0, 0x03));
      printf("\tRF24_RX_Device:   0x%06X\r\n", Nrf24ReadAdr(NRF24_RX_ADDR_P1, 0x03));
      printf("\tNRF24_CH:                  %d\r\n", Nrf24l01ReadReg(NRF24_CH));
      printf("\tNRF24_CONFIG:           0x%02X\r\n", Nrf24l01ReadReg(NRF24_CONFIG));
      printf("\tNRF24_SETUP:            0x%02X\r\n", Nrf24l01ReadReg(NRF24_SETUP));
      printf("\tNRF24_RETR:             0x%02X\r\n", Nrf24l01ReadReg(NRF24_SETUP_RETR));
      printf("\t*----------------------------*\r\n");
    }else{
      printf("<ERROR>   Initialization nRF24\r\n");
    }
  #endif
  return status;
}
