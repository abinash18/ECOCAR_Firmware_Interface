#include "client.hpp"
#include "debug.hpp"


/*All of this SerPort junk is here because USBSerial and HardwareSerial
do not share a base class. They share the stream class, but that doesn't
have availableforwrite(). So we have to have a class to do this all 
manually. */

#define BASE_FUNCTION(var, fcn) \
do {                            \
  switch(this->type)            \
  {                             \
  case 'H':                     \
	return HW->fcn(var);        \
  case 'U':                     \
	return USB->fcn(var);       \
  }                             \
} while(0)

	
SerPort::SerPort(HardwareSerial* HWS)
{
	this->HW = HWS;
	this->type = 'H';
}

SerPort::SerPort(usb_serial_class* USB)
{
	this->USB = USB;
	this->type = 'U';
}

int SerPort::available(void)
{
	BASE_FUNCTION(, available);
	return 0;
}

int SerPort::read(void)
{
	BASE_FUNCTION(, read);
	return -1;
}

void SerPort::begin(uint32_t baud)
{
	BASE_FUNCTION(baud, begin);
}

int SerPort::availableForWrite(void)
{
	BASE_FUNCTION(, availableForWrite);
	return 0;
}

size_t SerPort::write(uint8_t data)
{
	BASE_FUNCTION(data, write);
	return 0;
}

size_t SerPort::print(int32_t number)
{
	BASE_FUNCTION(number, print);
	return 0;
}

size_t SerPort::print(const char* data)
{
	BASE_FUNCTION(data, print);
	return 0;
}

size_t SerPort::print(float data)
{
	BASE_FUNCTION(data, print);
	return 0;
}

serial_client::serial_client(SerPort port, serial_handler callback)
 : Port(port), Callback(callback)
{
	/*Make sure that the caches are initialzied to zero*/
	memset(this->ValueCache, 0, sizeof(this->ValueCache));
	memset(this->IncomingFilter, 0, sizeof(this->IncomingFilter));
}

/*The board input and output functions
The HandleOutput sends out each message from the message
buffer in order, and the HandleInput receives bytes from 
the serial port, turns them into a string, and sends
that string to the handler/callback function*/

void serial_client::HandleInput(void)
{
	while(this->Port.available())
	{
		uint8_t newbyte = this->Port.read(); //read one byte at a time
		if(newbyte == '\r')
			continue; //ignore CR, only process LF
		if(newbyte >= 'a' && newbyte <= 'z') //make lowercase letters uppercase
			newbyte &= 0xDF;
		//DebugMsg("Got byte "); DebugMsgHex(newbyte); DebugLn();

		if(this->input_index >= INPUT_BUF_SIZE)
		{
			//discard input if it's too long
			this->input_index = 0;
			memset(this->Incoming, 0, INPUT_BUF_SIZE);
			DebugMsg("Input Too Long\n");
		}
		if(newbyte != '\n')
		{
			this->Incoming[this->input_index++] = newbyte;
			//DebugMsg(this->Incoming); DebugLn();
			continue;
		}
		this->Incoming[this->input_index++] = '\0';
		DebugMsg("Received string\n"); DebugMsg(this->Incoming); DebugLn();
		this->Callback(this, this->Incoming);
		this->input_index = 0;
		memset(this->Incoming, 0, INPUT_BUF_SIZE);
	}
}

void serial_client::HandleOutput(void)
{
	if(this->Outgoing.Count() > 0)
	{
		//DebugMsg("Outgoing Message\n");
		int writelen = this->Port.availableForWrite();
		for(int i = 0; i < writelen; i++)
		{
			uint8_t byte_to_write;
			if(this->output_index < MSG_SIZE)
			{					
				byte_to_write = this->Outgoing.Peek().str[this->output_index++];
				//DebugMsg("Writing ");
				//DebugMsgHex(byte_to_write);
				//DebugLn();
				if(byte_to_write != '\0')
				{
					this->Port.write(byte_to_write);
					continue;
				}
			}
			//Message is too long or we are at the end of the string
			//DebugMsg("End of string\n");
			this->Port.write('\n');
			this->output_index = 0;
			this->Outgoing.Pop();
			break;
		}
	}
}

bool serial_client::Send(Message msg)
{
	return this->Outgoing.Push(msg);
}