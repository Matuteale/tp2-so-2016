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

void openMessageQ(char * name);
void closeMessageQ(MessageQ * msgQ);
void sendMessageQ(char * name, char msg);
void receiveMessageQ(char * name, char * ret);
char ** getOpenedMessageQs();
MessageQ * findMessageQ(char * name);

#endif
