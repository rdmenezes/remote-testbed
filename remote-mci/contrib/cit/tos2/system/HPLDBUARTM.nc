

module HPLDBUARTM {
  provides interface HPLUART;
  uses interface UartStream;
  uses interface StdControl;
}
implementation
{
  command bool HPLUART.init() {
   	return(call StdControl.start());
  }

  command bool HPLUART.stop() {
	return(call StdControl.stop());
  }

  async command bool HPLUART.put(uint8_t data) {
	return(!( call UartStream.send(&data, 1)));		
  }

  async event void UartStream.sendDone( uint8_t* buf, uint16_t len, error_t error ) {}
  async event void UartStream.receivedByte(uint8_t data) {}
  async event void UartStream.receiveDone( uint8_t* buf, uint16_t len, error_t error ) {}
}
