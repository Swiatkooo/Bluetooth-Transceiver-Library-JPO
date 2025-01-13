#pragma once
#include "stm32h5xx_hal.h"
#include "stm32h5xx_nucleo.h"
#include <string>

namespace js
{
	class BLE_Transceiver
	{
		private:
			UART_HandleTypeDef * uartHandle;
			GPIO_TypeDef * statePinPort;
			uint16_t statePinPortNumber;
			uint8_t * rxBuffer;
			uint16_t rxBufferSize;
			uint16_t rxTimeout;
		public:
			BLE_Transceiver();
			~BLE_Transceiver();
			void init(UART_HandleTypeDef *, GPIO_TypeDef *, uint16_t);
			bool isConnectedToRemoteDevice();
			std::string receive();
			void transmit(std::string);
			void transmit(uint8_t *, uint16_t);
			void changeRxBufferSize(uint16_t);
			void changeRxTimeout(uint16_t);
			bool isConnectedToTransceiver();
			void setModuleName(std::string);
			void resetTransceiver();
			void resetToFactoryState();
			bool setPinCode(uint32_t);
			bool setPinCode(std::string);
			bool isDataRegisterNotEmpty();
	};
}