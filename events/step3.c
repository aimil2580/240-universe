//
// Created by aimil on 11/6/26.
//

#define GALAXIES_CAP_N 1024
#define OCLUSTERS_CAP_M 256
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct galaxy galaxy_t;
typedef struct solar solar_t;
typedef struct planet planet_t;
typedef struct ocluster ocluster_t;


struct planet {
  int pid;
  int distance;
  planet_t *previous;
  planet_t *next;
};

struct solar {
  int sid;
  planet_t *planets;
  solar_t *next;
};


planet_t* DL_Search(int pid,solar_t* solar) {
  if (solar -> planets == NULL) return NULL;
  planet_t * toBeFound = solar->planets;
  int f =0;
  while (toBeFound!=NULL && toBeFound->pid !=pid) {
    toBeFound=toBeFound->next;
  }
    return toBeFound;
};




planet_t* DL_Insert(int pid,int distancen,solar_t* solar) {
  if (solar == NULL) return NULL;
  planet_t * head = solar->planets;
  planet_t * p = solar->planets;
  planet_t * toBeAdded =  (planet_t* ) malloc(sizeof(planet_t));
  toBeAdded->pid = pid;
  toBeAdded->distance = distancen;
  toBeAdded->next=NULL;
  toBeAdded->previous=NULL;

  //an to ptr gia toys planhtes tou solar einai null tote ua einai o protos planhths
  if (p == NULL) {
    // an einai teleios adeia
    head = toBeAdded;
  } else {
    //an den einai o monos sth lista
    int distSum=0;

    //vrisko swsto distance
    while (p!=NULL && distSum>distancen) {
      distSum+= p->distance;
      p=p->next;
    }
    //an mpei kapoy anamesa
    if (p->next!=NULL && p->previous != NULL) {
      toBeAdded->distance = distancen-(distSum - (p->previous->distance));
      p->distance =distSum -distancen;
      p->previous->next=toBeAdded;
      p->previous = toBeAdded;
    }else if (p->next ==NULL && p->previous != NULL) {
      //an einai na mpei sto telos
      p->next = toBeAdded;
      toBeAdded->distance = distancen-distSum;
    } else if (p->next!=NULL && p->previous == NULL) {
      head->distance = p->distance - distancen;
      toBeAdded->next = head;
      head = toBeAdded;
    }
  }

  return head;
};




planet_t* DL_remove(){};
void EventPlanetCreation(int pid, int distance, int sid) {};

