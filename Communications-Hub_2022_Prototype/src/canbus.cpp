#include "canbus.hpp"
#include "string.h"
#include "debug.hpp"
#include "kinetis_flexcan.h"
#define FLEXCANb_MBn_CS(b, n)             (*(vuint32_t*)(b+0x80+n*0x10))

CanBus::CanBus(FlexCAN* fc, canbus_handler Callback, uint16_t SendAddress)
{
	this->fc = fc;
	this->Callback = Callback;
	memset(Incoming, 0, sizeof(Incoming));
	this->SendAddress = SendAddress;
}

bool CanBus::Send(Message msg)
{
	return this->Outgoing.Push(msg);
}

void CanBus::HandleInput(void)
{
	while(fc->available())
	{
		CAN_message_t rx;
		fc->read(rx);
		CanBusMessage* cb = NULL;
		for(int i = 0; i < CANBUS_BOARD_BUFFER_COUNT; i++)
		{
			//check if there is a buffered message in progress
			if(this->Incoming[i].Enabled && this->Incoming[i].SenderID == rx.id)
			{
				cb = this->Incoming + i;
				break;
			}
		}
		if(cb == NULL)
		{
			//no in-progress buffer from this CAN source address
			for(int i = 0; i < CANBUS_BOARD_BUFFER_COUNT; i++)
			{
				if(!(this->Incoming[i].Enabled))
				{
					cb = (this->Incoming) + i;
					cb->Enabled = true;
					cb->SenderID = rx.id;
					cb->MessageIndex = 0;
					break;
				}
			}
		}
		if(cb == NULL)
		{
			DebugMsg("CAN Too many simultaneous CAN transmitters"); DebugLn();
			return;
		}


		for(int j = 0; j < rx.len; j++)
		{
			char newbyte = rx.buf[j];
			if(newbyte == '\r')
				continue;
			if(newbyte >= 'a' && newbyte <= 'z')
				newbyte &= 0xDF;
			
			if(cb->MessageIndex >= MSG_SIZE)
			{
				cb->MessageIndex = 0;
				memset(cb->MessageBuffer.str, 0, MSG_SIZE);
				DebugMsg("CAN Message too long"); DebugLn();
			}
			if(newbyte != '\n')
			{
				cb->MessageBuffer[cb->MessageIndex++] = newbyte;
				continue;
			}
			cb->MessageBuffer[cb->MessageIndex] = '\0';
			DebugMsg("CAN RX "); DebugMsg(cb->MessageBuffer.str); DebugLn();
			this->Callback(this, cb->MessageBuffer.str);

			cb->MessageIndex = 0;
			memset(cb->MessageBuffer.str, 0, MSG_SIZE);
			if(j == rx.len - 1)
			{
				cb->Enabled = false;
			}
		}
					
	}
}

void CanBus::HandleOutput(void)
{
	while(this->Outgoing.Count() || this->SplitOutgoing.Count())
	{
		while(this->SplitOutgoing.Count())
		{
			if(this->fc->write(this->SplitOutgoing.Peek()))
			{
				//message on bus successfully
				this->SplitOutgoing.Pop();
			}
			else
			{
				//bus is busy
				return;
			}
		}
		if(this->Outgoing.Count())
		{
			Message msg = this->Outgoing.Pop();
			CAN_message_t tx = {0};
			tx.id = this->SendAddress;
			for(int i = 0; i < MSG_SIZE; i++)
			{
				uint8_t cm_index = i % 8;
				tx.buf[cm_index] = msg[i];
				if(msg[i] == '\0')
				{
					tx.buf[cm_index] = '\n';
					tx.len = cm_index + 1;
					this->SplitOutgoing.Push(tx);
					break;
				}
				if(cm_index == 7)
				{
					tx.len = 8;
					this->SplitOutgoing.Push(tx);
					continue;
				}
			}
		}
	}
}

bool CanBus::HandleLockup(void)
{
	uint32_t limiter_timestamp = 0;
	if(this->Outgoing.Count() == CANBUS_OUTPUT_BUFFER_COUNT)
	{
		if (millis() - limiter_timestamp > 200)
		{
			limiter_timestamp = millis();
			FLEXCANb_MBn_CS(FLEXCAN0_BASE, 15) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ABORT);
			delay(1);
			FLEXCANb_MBn_CS(FLEXCAN0_BASE, 15) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);
			return true;
		}
	}
	return false;
}
