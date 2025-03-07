#include "spi.h"

void SPI2_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(LCD_CS_GPIO_CLK | LCD_SPI_MOSI_GPIO_CLK |
                         LCD_SPI_SCK_GPIO_CLK | LCD_RES_GPIO_CLK | 
												 LCD_DC_GPIO_CLK | LCD_BL_GPIO_CLK, ENABLE);
  LCD_SPI_CLK_CMD(LCD_SPI_CLK, ENABLE);
  
  /*!< Configure LCD_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure LCD_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
  GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure LCD_CS_PIN pin: LCD Card CS pin */
  GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);
	
	 /*!< Configure LCD_RES_PIN pin: LCD Card RES pin */
  GPIO_InitStructure.GPIO_Pin = LCD_RES_PIN;
  GPIO_Init(LCD_RES_GPIO_PORT, &GPIO_InitStructure);

	 /*!< Configure LCD_DC_PIN pin: LCD Card DC pin */
  GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN;
  GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LCD_BL_PIN;
  GPIO_Init(LCD_BL_GPIO_PORT, &GPIO_InitStructure);

	LCD_CS_CMD(1);
  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(LCD_SPI, &SPI_InitStructure);
  SPI_Cmd(LCD_SPI, ENABLE);
}




//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI2_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)	return 0;
	}			  
	SPI_I2S_SendData(LCD_SPI, TxData); //ͨ������SPIx����һ������
	retry=0;
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)	return 0;
	}	  						    
	return SPI_I2S_ReceiveData(LCD_SPI); //����ͨ��SPIx������յ�����					    
}



