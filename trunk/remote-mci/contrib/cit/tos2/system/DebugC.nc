
configuration DebugC{  

 provides interface Debug; 
 provides interface StdControl;
} 	

 
implementation
{
  components ConsoleDebugM, ConsoleM, HPLDBUART;

  Debug = ConsoleDebugM.Debug;
  StdControl = ConsoleM.StdControl;
  ConsoleDebugM.ConsoleOut -> ConsoleM.ConsoleOut;
  ConsoleM.Uart -> HPLDBUART.HPLUART;
}
