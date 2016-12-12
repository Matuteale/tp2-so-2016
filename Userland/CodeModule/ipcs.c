#include "ipcs.h"
#include "stdio.h"
#include "string.h"

/*Muestra una lista de estructuras creadas*/
void ipcs()
{
  char * ipcs[64];
  char names[704];
  char values[704];
  int i = 0;
  while(i < 64){
    ipcs[i] = &(names[i*11]);
    i++;
  }
  printString("IPC | Value \n");
  ipcs_sys(ipcs, names, values);
  i = 0;
  while(ipcs[i] != 0){
    printString(&names[i*11]);
    printString(" - ");
    printString(&values[i*11]);
    printString("\n");
    i++;
  }
  int activePID;
  sys_getActivePID(&activePID);
  sys_killProcess(activePID);
  while(1);
  return;
}