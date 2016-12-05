#ifndef CONSUMERPRODUCER_H
#define CONSUMERPRODUCER_H

void mainProdCons(); 
void * producer(void * ctx);
void * consumer(void * ctx);
void control();
#endif