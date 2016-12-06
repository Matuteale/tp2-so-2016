//Basado en Soundblasteros
#ifndef MSGQUEUES_H
#define MSGQUEUES_H

typedef struct Msg{
  char msg;
  Msg * next;
} Msg;

typedef struct MessageQ {
  unsigned int  id;
  char * name;
  int dead;
  Msg* first;
  Msg* last;
} MessageQ;

MessageQ * openMessageQ(char * name);
MessageQ * getMessageQ(char * name);
void closeMessageQ(MessageQ * msgQ);
void sendMessageQ(MessageQ * msgQ, char msg);
void receiveMessageQ(MessageQ * msgQ, char * ret);
char ** getOpenedMessageQs();

#endif
