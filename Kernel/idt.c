#include <teclado.h>
#include <idt.h>
#include <lib.h>
#include <video.h>
#include <screensaver.h>
#include <RTCkernelspace.h>
#include <sound.h>
#include <condVar.h>
#include <naiveConsole.h>
#include <mutex.h>
#include <msgqueues.h>
#include <scheduler.h>

int timertick = 0;
int t = 0;


void setup_idt()
{
   idt_entry * idt;
   idt = 0;

   idt_set_entry(idt, 0x21, (unsigned long)&int_21_hand , 0x08, 0x8E);
   idt_set_entry(idt, 0x20, (unsigned long)&int_20_hand , 0x08, 0x8E);
   idt_set_entry(idt, 0x80, (unsigned long)&int_80_hand , 0x08, 0x8E);

   mascaraPIC1(0xFC);  //toma teclado y timertick
   mascaraPIC2(0xFF);

}

/*unicOS sebikul*/
void pit_setup(uint32_t msecs) {
  uint32_t count = 1193182 * msecs;
  count /= 1000;

  outb(0x36, 0x43);
  outb(count, 0x40);
  outb(count >> 8, 0x40);
}

/* setea una entrada de la IDT */
void idt_set_entry(idt_entry * idt_entries, unsigned char index,
				unsigned long base, unsigned short sel, unsigned char flags)
{
   idt_entries[index].offset_low = base & 0xFFFF;
   idt_entries[index].offset_mid = (base >> 16) & 0xFFFF;
   idt_entries[index].offset_high = (base >> 32) & 0xFFFF;
   idt_entries[index].selector = sel;
   idt_entries[index].always0 = 0;
   idt_entries[index].always0_2 = 0;
   idt_entries[index].flags = flags;
}

/* maneja los interrupts del teclado, puede ser que tambien pare el sonido debido a una tecla */
void keyboard_interrupt(unsigned short entrada)
{
	unsigned char valor;

	if(suppress_screensaver() == SS_WAS_ON)
		return ;

	valor = translate_keyboard_input(entrada);

	if(valor == 'z' && suppress_sound() == S_WAS_ON)
		return ;

	if(valor != IGNORE)
		to_input_buffer(valor);

	return ;
}

/* maneja los interrupts del PIT */
void timer_interrupt()
{
	soundtimer();

	// screensavertimer();

	timertick = (timertick+1)%2;

	return;
}

/* sys call 0xA */
void timer_tick(char * str)
{
	*str = timertick;
}

/* sys call 0xB */
void play_music_idt()
{
	play_music();
}

/* sys call 0xC */
void play_beep_idt(uint64_t freq, uint64_t time)
{
	play_beep(freq, time);
}

/* sys call 0x1 */
int create_process(void * entryPoint, char * name, int isBackground)
{
  int pid;
  userToKernel();
  pid = addProcess(entryPoint, name, isBackground);
  kernelToUser();
  if(!isBackground){
    scheduleNow();
  }
  Process * process = getCurrentProcess();
  Process * processAux = process;
  // ncPrintDec(process->foreground);
  // ncPrint(" - ");
  while(process->next->PID != processAux->PID){
    // ncPrintDec(process->next->foreground);
    // ncPrint(" - ");
    process = process->next;
  }
  // ncPrint("PID PLS: ");
  // ncPrintDec(pid);
  // ncNewline();
  return pid;
}

/* sys call 0x2 */
void kill_process(int PID)
{
  removeProcess(PID);
}

/* sys call 0xD */
void list_processes(char ** states, int * vec, char ** names)
{
  Process * process = getCurrentProcess();
  Process * current = process;
  int i = 0;
  vec[i] = process->PID;
  names[i] = process->name;
  states[i] = defineStringState(process->state);
  i++;
  process = process->next;
  while(process != current){
    vec[i] = process->PID;
    names[i] = process->name;
    states[i] = defineStringState(process->state);
    process = process->next;
    i++;
  }
  while(i < 16){
    vec[i] = 0;
    i++;
  }
}

char * defineStringState(int state)
{
  switch(state){
    case 0: return "RUNNING"; break;
    case 1: return "BLOCKED"; break;
    case 2: return "READY"; break;
    case 3: return "DYING"; break;
    case 4: return "SLEEPING"; break;
    case 5: return "NIL"; break;
  }
  return "UNDEFINED";
}

/* sys call 0xE */
void getActivePID(int * PID)
{
  Process * process = getCurrentProcess();
  PID[0] = process->PID;
}


// void initCondVarU(cond_t * condVar, int pid) {

// }

// void waitCondVarU(cond_t * condVar, int mutex) {

// }

// void signalCondVarU(cond_t * condVar) {

// }

void sleepProcess(long milis){
  Process * process = getCurrentProcess();
  int * waitingProcess = getWaitingProcess();
  int * waitingMilis = getWaitingMilis();
  int flag = 0;
  int i = 0;
  for(; i < 16; i++){
    if(waitingProcess[i] == -1){
      flag = 1;
      break;
    }
  }
  if(flag){
    waitingProcess[i] = process->PID;
    waitingMilis[i] = milis;
    changeProcessState(waitingProcess[i], SLEEPING);
  }
}

void listIPCs(char ** ipcs, char * names, int * values)
{
  int i = getOpenedMessageQs(names);
  int j = 0;
  while(j < i){
    char * val = "msgQueue"
    values[j] = val;
    j++;
  }
  ipcs[i] = 0;
}

/* maneja los system calls */
int syscall_handler(uint64_t arg_3, uint64_t arg_2, uint64_t arg_1, uint64_t syscall)
{
  int pid = 0;
  Process * process = getCurrentProcess();
	switch(syscall)
	{
    case 0x1: pid = create_process((void *) arg_2, (char *) arg_1, (int) arg_3);break;
    case 0x2: kill_process((int) arg_1); break;
		case 0x3: if(process->foreground){sys_readKeyboard((char *)arg_2);}else{scheduleNow();} break;
		case 0x4: if(process->foreground){sys_displayWrite((char *)arg_2, arg_1);}else{scheduleNow();} break;
		case 0x5: read_rtc_time((char *) arg_2, arg_1);scheduleNow(); break;
		case 0x6: set_rtc_time((char *) arg_2, arg_1);scheduleNow(); break;
		case 0x7: sys_changeColor(arg_1);scheduleNow(); break;
		case 0x8: if(process->foreground){clearscreen();}else{scheduleNow();} break;
		case 0x9: set_ss_timeout(arg_1);scheduleNow();break;
		case 0xA: timer_tick((char *)arg_2);scheduleNow(); break;
		case 0xB: play_music_idt();scheduleNow(); break;
		case 0xC: play_beep_idt(arg_2, arg_1);scheduleNow(); break;
   	case 0xD: list_processes((char **) arg_3, (int *) arg_2, (char **) arg_1);break;
    case 0xE: getActivePID((int *) arg_1);break;
    case 0xF: mutexLockK(arg_1);break;
    case 0x10: mutexUnlockK(arg_1);break;
    case 0x11: initCondVarK((cond_t*) arg_1);break;
    case 0x12: ncPrintDec(((int *)arg_1)[0]); waitCondVarK((cond_t*) arg_1, (int *)arg_2);break;
    case 0x13: signalCondVarK((cond_t*) arg_1);break;
    case 0x14: openMessageQ(arg_1);break;
    case 0x15: break;//getMessageQ(arg_1);break;
    case 0x16: closeMessageQ(arg_1);break;
    case 0x17: sendMessageQ(arg_1, arg_2);break;
    case 0x18: receiveMessageQ(arg_1, arg_2);break;
    case 0x19: getOpenedMessageQs(arg_1);break;
    case 0x20: sleepProcess((long) arg_1);break;
    case 0x21: listIPCs((char **) arg_1,(char *) arg_2, (int *) arg_3);break;
	}
	return pid;
}
