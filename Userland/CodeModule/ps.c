#include "ps.h"
#include "stdio.h"
#include "string.h"
/*Lista de procesos*/
void ps()
{
  unsigned int processPID[16];
  char * names[16];
  char * states[16];
  printString("Name | PID | State \n");
  ps_sys(names, processPID, states);
  int i = 0;
  while(processPID[i] != 0){
    printString(names[i]);
    printString(" - ");
    printDec(processPID[i]);
    printString(" - ");
    printString(states[i]);
    printString("\n");
    i++;
  }
  int activePID;
  sys_getActivePID(&activePID);
  sys_killProcess(activePID);
  while(1);
  return;
}