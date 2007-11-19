

configuration HPLDBUART {
  provides interface HPLUART;

}
implementation
{
  components PlatformSerialC, HPLDBUARTM;
  
  HPLUART = HPLDBUARTM.HPLUART;
  HPLDBUARTM.UartStream->PlatformSerialC.UartStream;
  HPLDBUARTM.StdControl->PlatformSerialC.StdControl;
}
