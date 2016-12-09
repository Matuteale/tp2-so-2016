//Basado en Soundblasteros
#ifndef MSGQUEUES_H
#define MSGQUEUES_H

typedef struct MsgAux Msg;
struct MsgAux{
  char msg;
  Msg * next;
};

typedef struct MessageQ {
  unsigned int  id;
  char * name;
  int dead;
  Msg* first;
  Msg* last;
} MessageQ;

void openMessageQ(char * name, MessageQ * msgQ);
void getMessageQ(char * name, MessageQ * msgQ);
void closeMessageQ(MessageQ * msgQ);
void sendMessageQ(MessageQ * msgQ, char msg);
void receiveMessageQ(MessageQ * msgQ, char * ret);
char ** getOpenedMessageQs();

#endif
