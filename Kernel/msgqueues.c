#include <msgqueues.h>
#include <process.h>
#include <naiveConsole.h>

#define MAX_QUEUES 50

static MessageQ * queue[MAX_QUEUES];
static char * msgQNames[MAX_QUEUES];

void destroyMessageQ(MessageQ * msgQ);

char ** getOpenedMessageQs(){
  char * openedMsgQs[MAX_QUEUES];
  int j = 0;
  for (int i = 0; i < MAX_QUEUES; ++i)
  {
    if(msgQNames[i] != 0){
      openedMsgQs[j] = msgQNames[i];
      j++;
    }
  }
  return openedMsgQs;
}

void destroyMessageQ(MessageQ * msgQ){
  ncPrint("Destroying messageQ: ");
  ncPrint(msgQ->name);
  ncNewline();
  queue[msgQ->id] = 0;
  msgQNames[msgQ->id] = 0;
}

void receiveMessageQ(MessageQ * msgQ, char * ret){
    if(msgQ->first != 0){
      Msg * node = msgQ->first;
      msgQ->first = msgQ->first->next;
      if(msgQ->dead == 1 && msgQ->first == 0){
        destroyMessageQ(msgQ);
      }
      *ret = node->msg;
      return;
    }
  return;
}

void sendMessageQ(MessageQ * msgQ, char msg){
  Msg * newMsg = malloc(sizeof(Msg));

  newMsg->msg = msg;
  newMsg->next = 0;

  if(msgQ->first == 0){
    msgQ->first = newMsg;
    msgQ->last = newMsg;
  }
  else{
    msgQ->last->next = newMsg;
    msgQ->last = newMsg;
  }
}