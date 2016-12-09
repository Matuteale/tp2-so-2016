//Basado en Soundblasteros
#ifndef MSGQUEUES_H
#define MSGQUEUES_H

typedef struct MsgAux Msg;
struct MsgAux{
  char msg[20];
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
void closeMessageQ(char * name);
void sendMessageQ(char * name, char * msg);
void receiveMessageQ(char * name, char * ret);
int getOpenedMessageQs(char ** openedQueues);
MessageQ * findMessageQ(char * name);

#endif
