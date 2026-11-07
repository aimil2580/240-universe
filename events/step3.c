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


planet_t* DL_Search(planet_t * head,int pid) {
  if (head == NULL) return NULL;
  planet_t * toBeFound = head;
  int f =0;
  while (toBeFound!=NULL && toBeFound->pid !=pid) {
    toBeFound=toBeFound->next;
  }
    return toBeFound;
};



planet_t * DL_Insert(planet_t* head,int pid,int distance){
  planet_t* p=head;
  planet_t* prevOfP = NULL;
  planet_t* toBeAdded = (planet_t*) malloc(sizeof(planet_t));
  //init new planet   toBeAdded->pid =pid;
  toBeAdded->distance =distance;
  toBeAdded->next =NULL;
  toBeAdded->previous =NULL;
  toBeAdded->pid = pid;

  if(p==NULL)return toBeAdded;

  if (distance < head->distance) {
    toBeAdded->next = head;
    head->previous = toBeAdded;
    head->distance = head->distance - distance;
    return toBeAdded;
  }


  int distSum = 0;
  while(p!=NULL && (distSum+ p->distance )<=distance){
    distSum += p->distance;
    prevOfP = p;
    p=p->next;
  }
  //somewhere in between
  if(p!=NULL){
    toBeAdded->next = p;
    toBeAdded->previous = p->previous;
    p->previous->next = toBeAdded;
    p->previous = toBeAdded;

    toBeAdded->distance=distance-distSum;
    p->distance= p->distance - toBeAdded->distance;
  } else{
    //in the end
    prevOfP ->next = toBeAdded;
    toBeAdded->previous =  prevOfP;
    toBeAdded->distance = distance - prevOfP->distance;
  }

  return head;

}




void DL_Print(planet_t* h) {
  planet_t* p = h;
  while (p!=NULL) {
    printf("pid :%d  dist: %d\n",p->pid,p->distance);
    p=p->next;
  }
}


planet_t* DL_Remove(planet_t* head,int pid) {
  if (head ==NULL) return NULL;
  planet_t* toBeRemoved = DL_Search(head,pid);
  if (toBeRemoved ==NULL) return NULL;

  //an einai to mono sth lista
  if (toBeRemoved->previous ==NULL && toBeRemoved->next == NULL) {
    toBeRemoved->pid=-1;
    return NULL;
  }

  //an einai to proto
  if (toBeRemoved -> previous ==NULL) {
    toBeRemoved->next->distance = (toBeRemoved->distance)+toBeRemoved->next->distance;
    toBeRemoved->next->previous=NULL;
    toBeRemoved->pid=-1;
    return toBeRemoved->next;
  }

  //an einai to telefteo
  if (toBeRemoved->next==NULL) {
    toBeRemoved->previous->next =NULL;
    toBeRemoved->pid=-1;
    return head;
  }

  //an einai kapoy anamesa
  if (toBeRemoved->next!=NULL && toBeRemoved->previous!=NULL) {
    toBeRemoved->pid=-1;
    toBeRemoved->previous->next= toBeRemoved->next;
    toBeRemoved->next->previous= toBeRemoved->previous;
    toBeRemoved->next->distance=toBeRemoved->next->distance+toBeRemoved->distance;
    return head;
  }
};



int main() {
  planet_t *L = NULL; /* Δείκτης στην αρχή της λίστας */

  printf("=== Testing Step 3: Doubly-Linked Planet List ===\n\n");

  /* 1. Εισαγωγή 1ου πλανήτη (PID 100, Απόσταση 30) */
  printf("[+] Insert PID 100 at dist 30\n");
  L = DL_Insert(L, 1, 3);
  DL_Print(L);

  /* 2. Εισαγωγή στο τέλος (PID 101, Απόσταση 70) */
  printf("[+] Insert PID 101 at dist 70\n");
  L = DL_Insert(L, 2, 4);
  DL_Print(L);

  /* 3. Εισαγωγή στην αρχή (PID 102, Απόσταση 10) */
  printf("[+] Insert PID 102 at dist 10\n");
  L = DL_Insert(L, 3, 9);
  DL_Print(L);

  /* 4. Εισαγωγή ενδιάμεσα (PID 103, Απόσταση 50) */
  printf("[+] Insert PID 103 at dist 50\n");
  L = DL_Insert(L, 4, 15);
  DL_Print(L);

  printf("_______________________________\n");
  L = DL_Remove(L,1);
  DL_Print(L);

  /* 5. Έλεγχος Search */
  planet_t *s = DL_Search(L, 4);
  if (s != NULL) {
    printf("[*] Search PID 3: FOUND (Rel Dist = %d)\n\n", s->distance);
  } else {
    printf("[*] Search PID 3: NOT FOUND\n\n");
  }

  /* 6. Έλεγχος Remove ενδιάμεσου (PID 100, dist 30) */
}

