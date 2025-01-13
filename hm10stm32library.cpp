#include "hm10stm32library.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_nucleo.h"
#include <string>
#include <algorithm>

js::BLE_Transceiver::BLE_Transceiver()
{
		rxBufferSize = 200;
		rxTimeout = 100;
		rxBuffer = new uint8_t[rxBufferSize];
}

js::BLE_Transceiver::BLE_Transceiver(UART_HandleTypeDef * a, GPIO_TypeDef * b, uint16_t c)
{
		rxBufferSize = 200;
		rxTimeout = 100;
		rxBuffer = new uint8_t[rxBufferSize];
		init(a,b,c);
}

js::BLE_Transceiver::~BLE_Transceiver()
{
		delete[] rxBuffer;
}

void js::BLE_Transceiver::init(UART_HandleTypeDef * a, GPIO_TypeDef * b, uint16_t c)
{
		uartHandle = a;
		statePinPort = b;
		statePinPortNumber = c;
}

bool js::BLE_Transceiver::isConnectedToRemoteDevice()
{
    return HAL_GPIO_ReadPin(statePinPort, statePinPortNumber) == GPIO_PIN_SET;
}

std::string js::BLE_Transceiver::receive()
{
		std::string recv;
		std::fill(rxBuffer, rxBuffer+rxBufferSize, '\0');
	
		HAL_UART_Receive(uartHandle, rxBuffer, rxBufferSize, rxTimeout);
	
		for(uint16_t i = 0; i<rxBufferSize; i++)
			{
					if(rxBuffer[i] == '\0')
					{
							break;
					}
				
					recv.push_back(rxBuffer[i]);
			}
	
    return recv;
}

void js::BLE_Transceiver::transmit(std::string txBuffer)
{
		HAL_UART_Transmit(uartHandle, (uint8_t*)txBuffer.c_str(), txBuffer.size(), HAL_MAX_DELAY);
}

void js::BLE_Transceiver::transmit(uint8_t * txBuffer, uint16_t txBufferSize)
{
		HAL_UART_Transmit(uartHandle, txBuffer, txBufferSize, HAL_MAX_DELAY);
}

void js::BLE_Transceiver::changeRxBufferSize(uint16_t newSize)
{
		rxBufferSize = newSize;
		delete[] rxBuffer;
		rxBuffer = new uint8_t[rxBufferSize];
}

void js::BLE_Transceiver::changeRxTimeout(uint16_t a)
{
		rxTimeout = a;
}

bool js::BLE_Transceiver::isConnectedToTransceiver()
{
		std::string buff = "AT";
		transmit(buff);
		uint16_t a = rxTimeout;
		rxTimeout = 100;
		buff = receive();
		rxTimeout = a;
		
		if(buff[0]=='O')
		{
				return true;
		}
		else
		{
				return false;
		}
}

void js::BLE_Transceiver::setModuleName(std::string name)
{
		std::string buff = "AT+NAME" + name;
		transmit(buff);
}

void js::BLE_Transceiver::resetTransceiver()
{
		std::string buff = "AT+RESET";
		transmit(buff);
}

void js::BLE_Transceiver::resetToFactoryState()
{
		std::string buff = "AT+RENEW";
		transmit(buff);
}

bool js::BLE_Transceiver::setPinCode(uint32_t a)
{
		std::string buff = "AT+PIN";
	
		if(a<10)
		{
				buff += "00000";
				buff += std::to_string(a);
		}
		else if(a<100)
		{
				buff += "0000";
				buff += std::to_string(a);
		}
		else if(a<1000)
		{
				buff += "000";
				buff += std::to_string(a);
		}
		else if(a<10000)
		{
				buff += "00";
				buff += std::to_string(a);
		}
		else if(a<100000)
		{
				buff += "0";
				buff += std::to_string(a);
		}
		else if(a<1000000)
		{
				buff += std::to_string(a);
		}
		else
		{
				return false;
		}
			
		transmit(buff);
		return true;
}

bool js::BLE_Transceiver::setPinCode(std::string a)
{
		if(std::all_of(a.begin(), a.end(), ::isdigit) && a.size() == 6)
		{
				std::string buff = "AT+PIN" + a;
				transmit(buff);
				return true;
		}
		
		return false;
}

bool js::BLE_Transceiver::isDataRegisterNotEmpty()
{
	return __HAL_UART_GET_FLAG(uartHandle, UART_FLAG_RXNE) == SET;
}
