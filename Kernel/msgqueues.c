#include <msgqueues.h>
#include <process.h>
#include <naiveConsole.h>
#include <memManager.h>
#include <lib.h>

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

void getMessageQ(char * name){
  for (int i = 0; i < MAX_QUEUES; ++i){
    if(strcmp(msgQNames[i], name)){
      ncPrint(queue[i]->name);
      return queue[i];
    }
  }
  return 0;
}

void openMessageQ(char * name, MessageQ * msgQ){
  for (int i = 0; i < MAX_QUEUES; ++i){
    if(strcmp(msgQNames[i], name))
      return queue[i];
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
      memcpy(msgQ, auxQueue, sizeof(MessageQ));
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

void receiveMessageQ(MessageQ * msgQ, char * ret){
    ncPrint(msgQ->name);
    if(msgQ->first != 0){
      Msg * node = msgQ->first;
      msgQ->first = msgQ->first->next;
      if(msgQ->dead == 1 && msgQ->first == 0){
        destroyMessageQ(msgQ);
      }
      *ret = node->msg;
      ncPrint("received");
      return;
    }
    ncPrint("receivedRet");
  return;
}

void sendMessageQ(MessageQ * msgQ, char msg){
  Msg * newMsg = myMalloc(sizeof(Msg));

  newMsg->msg = msg;
  newMsg->next = 0;
  if(msgQ->first == 0){
    msgQ->first = newMsg;
    msgQ->last = newMsg;
    ncPrint("sended");
  }
  else{
    msgQ->last->next = newMsg;
    msgQ->last = newMsg;
  }
}

void closeMessageQ(MessageQ * msgQ){
  msgQ->dead = 1;
  if(msgQ->first == 0)
    destroyMessageQ(msgQ);
}