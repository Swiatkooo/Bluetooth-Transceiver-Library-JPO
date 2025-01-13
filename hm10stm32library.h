#pragma once
#include "stm32h5xx_hal.h"
#include "stm32h5xx_nucleo.h"
#include <string>

namespace js
{
	class BLE_Transceiver
	{
		private:
			UART_HandleTypeDef * uartHandle; //used to access UART interface
			GPIO_TypeDef * statePinPort; //used to access STATE port
			uint16_t statePinPortNumber; //used to access STATE port
			uint8_t * rxBuffer; //used to store received data
			uint16_t rxBufferSize; //size of rxBuffer
			uint16_t rxTimeout; //time in ms before receive() function ends if it did not fill the rxBuffer before
		public:
			BLE_Transceiver(); //creates object and reserves memory for rxBuffer
			BLE_Transceiver(UART_HandleTypeDef *, GPIO_TypeDef *, uint16_t); //creates objects and reserves memory also calls init() function using its parameters
			~BLE_Transceiver(); //deallocates rxBuffer
			void init(UART_HandleTypeDef *, GPIO_TypeDef *, uint16_t); //gets pointers used to access the device
			bool isConnectedToRemoteDevice(); //checks STATE port
			std::string receive(); //is used to receive data from uart interface, the process lasts till the buffer is full or the function times out
			void transmit(std::string); //sends string to remote device
			void transmit(uint8_t *, uint16_t); //sends raw string to remote device
			void changeRxBufferSize(uint16_t); //changes rxBufferSize and reallocates the memory
			void changeRxTimeout(uint16_t); //changes receive function timeout
			bool isDataRegisterNotEmpty(); //checks if there is any data in a buffer that can be received
			//the functions below can only be used if no remote device is connected to the transceiver
			bool isConnectedToTransceiver(); //used to check the uart connection between microcontroller and a transceiver
			void setModuleName(std::string); //sets module name
			void resetTransceiver(); //reboots the transceiver
			void resetToFactoryState(); //resets settings
			bool setPinCode(uint32_t); //sets pin code
			bool setPinCode(std::string); //sets pin code
	};
}
