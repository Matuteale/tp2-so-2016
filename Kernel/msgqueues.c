#include <msgqueues.h>
#include <process.h>
#include <naiveConsole.h>
#include <memManager.h>
#include <lib.h>

#define MAX_QUEUES 50

static MessageQ * queue[MAX_QUEUES];
static char * msgQNames[MAX_QUEUES];

void destroyMessageQ(MessageQ * msgQ);

int getOpenedMessageQs(char * names){
  openMessageQ("hola");
  openMessageQ("chau");
  int j = 0;
  for (int i = 0; i < MAX_QUEUES; ++i)
  {
    if(msgQNames[i] != 0){
      int size = 0;
      while(msgQNames[i][size++] != 0);
      memcpy(&(names[j*21]), msgQNames[i], size);
      ncPrint(&(names[j*21]));
      j++;
    }
  }
  return j;
}

void openMessageQ(char * name){
  int size = 0;
  while(name[size++] != 0);
  if(size > 20) return;
  for (int i = 0; i < MAX_QUEUES; ++i){
    if(strcmp(msgQNames[i], name)){
      return;
    }
  }

  for (int i = 0; i < MAX_QUEUES; ++i){
    if(msgQNames[i] == 0){
      MessageQ * auxQueue = myMalloc(sizeof(MessageQ));
      auxQueue->id = i;
      auxQueue->name = name;
      auxQueue->dead = 0;
      auxQueue->first = 0;
      auxQueue->last = 0;
      msgQNames[i] = name;
      queue[i] = auxQueue;
      // ncPrint("Opened: ");
      ncPrint(auxQueue->name);
      return;
    }
  }
  return;
}

void destroyMessageQ(MessageQ * msgQ){
  ncPrint("Destroying messageQ: ");
  ncPrint(msgQ->name);
  ncNewline();
  queue[msgQ->id] = 0;
  msgQNames[msgQ->id] = 0;
}

void receiveMessageQ(char * name, char * ret){
  MessageQ * msgQ = findMessageQ(name);

  if(msgQ != 0 && msgQ->first != 0){
    Msg * node = msgQ->first;
    msgQ->first = msgQ->first->next;
    if(msgQ->dead == 1 && msgQ->first == 0){
      destroyMessageQ(msgQ);
    }
    int size = 0;
    while((node->msg)[size++] != 0);
    memcpy(ret, node->msg, size);
    // ncPrint("received");
    return;
  }
  return;
}

MessageQ * findMessageQ(char * name){
  for (int i = 0; i < MAX_QUEUES; ++i){
    if(strcmp(msgQNames[i], name)){
      return queue[i];
    }
  }
  return 0;
}

void sendMessageQ(char * name, char * msg){
  // ncPrint(" MQ: ");
  // ncPrint(name);
  MessageQ * msgQ = findMessageQ(name);
  if(msgQ == 0) return;

  Msg * newMsg = myMalloc(sizeof(Msg));

  memcpy(newMsg->msg, msg, 20);
  (newMsg->msg)[20] = 0;
  newMsg->next = 0;
  if(msgQ->first == 0){
    msgQ->first = newMsg;
    msgQ->last = newMsg;
    // ncPrint("sended");
    // ncPrintDec(newMsg->next);
  }
  else{
    msgQ->last->next = newMsg;
    msgQ->last = newMsg;
  }
}

void closeMessageQ(char * name){
  MessageQ * msgQ = findMessageQ(name);
  msgQ->dead = 1;
  if(msgQ->first == 0)
    destroyMessageQ(msgQ);
}