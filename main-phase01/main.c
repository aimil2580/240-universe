/*
 * ============================================
 * file: main.c
 * @Author Myron Tsatsarakis (myrontsa@csd.uoc.gr)
 * @Version 15/10/2026
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2026-2027
 * ============================================
 */

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enable in Makefile
#ifdef DEBUG_PRINTS_ENABLED
#define DebugPrint(...) fprintf(stderr, __VA_ARGS__);
#else
#define DebugPrint(...)
#endif

#define GALAXIES_CAP_N 1024
#define OCLUSTERS_CAP_M 256

typedef struct galaxy galaxy_t;
typedef struct solar solar_t;
typedef struct planet planet_t;
typedef struct ocluster ocluster_t;

struct ocluster {
  int cid;
  planet_t *orphans;
};

struct galaxy {
  int gid;
  solar_t *solars;
  solar_t *ssentinel;
  ocluster_t oclusters[OCLUSTERS_CAP_M];
};

struct solar {
  int sid;
  planet_t *planets;
  solar_t *next;
};

struct planet {
  int pid;
  int distance;
  planet_t *previous;
  planet_t *next;
};

galaxy_t Galaxies[GALAXIES_CAP_N];

static int galaxyPtr;

// Helper functions go here


void SL_Remove(galaxy_t* galaxy , int sid) {
  if (galaxy ==NULL || galaxy->ssentinel ==NULL || galaxy->solars == galaxy->ssentinel) return;
  solar_t * toBeRemoved = galaxy->solars;
  solar_t * prevOfDeleted=NULL;

  while (toBeRemoved != galaxy->ssentinel && toBeRemoved->sid !=sid) {
    prevOfDeleted =toBeRemoved;
    toBeRemoved = toBeRemoved ->next;
  }
  //ama de to vrei
  if (toBeRemoved==galaxy->ssentinel) return;

  //an pao na vgalo to ena poy uparxei
  if (toBeRemoved == galaxy->solars) {
    galaxy->solars=toBeRemoved->next;

    toBeRemoved->sid =-1;

  }else {
    // an otidhpote allo
    prevOfDeleted->next= toBeRemoved->next;
    toBeRemoved->next=NULL;
    }

    if (galaxy->ssentinel->next == toBeRemoved){
      galaxy->ssentinel->next = (toBeRemoved == galaxy->solars) ? NULL : prevOfDeleted;
  }
}

solar_t* SL_Search(galaxy_t* galaxy, int sid) {
  if (galaxy ==NULL || galaxy->ssentinel ==NULL || galaxy->solars ==NULL) return NULL;
  solar_t* toBeFound = galaxy->solars;
  while (toBeFound!= NULL && toBeFound != galaxy->ssentinel && toBeFound->sid !=sid) {
    toBeFound=toBeFound->next;
  }
  if (toBeFound!= NULL && toBeFound == galaxy->ssentinel) return NULL;
  return toBeFound;
}
void SL_Insert(galaxy_t* galaxy,int sid) {
  if (galaxy ==NULL || galaxy->ssentinel ==NULL) return;

  solar_t * newStar = (solar_t *) malloc(sizeof(solar_t));
  newStar->sid=sid;
  newStar->planets=NULL;

  if (galaxy->ssentinel->next ==NULL) {
    newStar->next = galaxy->ssentinel;
    galaxy->solars= newStar;
    galaxy->ssentinel->next= newStar;
  }else {
    newStar->next = galaxy->ssentinel;
    galaxy->ssentinel->next->next= newStar;
    galaxy->ssentinel->next = newStar;
  }
}

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
    toBeAdded->distance = distance - distSum;
  }

  return head;

}
planet_t* DL_Remove(planet_t* head,int pid) {
  if (head ==NULL) return NULL;
  planet_t* toBeRemoved = DL_Search(head,pid);
  if (toBeRemoved ==NULL) return head;

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
}
planet_t* srtById(planet_t* head) {
  if (head ==NULL) return NULL;
  if (head->next ==NULL) return  head;

  planet_t* newHead= NULL;
  planet_t* p=head;

  while (p!=NULL) {
    planet_t* pnext = p->next;
    p->previous=NULL;
    p->next=NULL;

    if (newHead ==NULL || p->pid< newHead->pid) {
      p->next = newHead;
      if (newHead !=NULL) {
        newHead->previous =p;
      }
      newHead = p;
    } else {
      planet_t* temp = newHead;
      while (temp->next != NULL && temp->next->pid<p->pid) temp= temp->next;

      p->next = temp->next;
      if (temp->next !=NULL) temp->next ->previous = p;
      temp->next=p;
      p->previous=temp;
    }
    p=pnext;
  }
  return newHead;
}
planet_t * DL_RemoveOrph(planet_t* head,int pid){
  if(head == NULL || (head->pid != pid && head->next == NULL) )return head;

  planet_t* p = head;

  while (p!=NULL && p->pid!=pid) p=p->next;
  if(p==NULL) return head;// not found!



  //an einai to head poy preppei na vgei kai einai mono toy
  if(p->next == NULL && p==head && p->pid == pid) return NULL;


  //telefteo
  if(p->next==NULL){

    p->previous->next = NULL;
    p->pid=-1;
    return head;
  }

  //proto
  if(p->previous == NULL ){

    head->next->previous= NULL;
    head->pid =-1;
    return head->next;
  }

  //endiamesa
  if(p->previous !=NULL && p->next != NULL ){
    p->next->previous =  p ->previous;
    p->previous ->next = p->next;

    p->next =NULL;
    p->previous=NULL;
    p->pid=-1;

    return head;
  }
}
planet_t* mergeOclust(planet_t* cl1,planet_t* cl2) {
  if (cl1 == NULL) return cl2;
  if (cl2 == NULL) return cl1;

  planet_t* i = cl1;
  planet_t* j =cl2;
  planet_t* newHead=NULL;
  planet_t* newPtr =NULL;

  if (i->pid < j ->pid ) {
    newPtr =i;
    i=i->next;
  } else {
    newPtr=j;
    j=j->next;
  }

  newPtr->next =NULL;
  newPtr->previous=NULL;
  newHead=newPtr;

  while (i!=NULL && j!=NULL) {
    if (i->pid < j ->pid ) {
      newPtr->next = i;
      newPtr->next->previous = newPtr;
      i=i->next;
    } else {
      newPtr->next = j;
      newPtr->next->previous = newPtr;
      j=j->next;
    }
    newPtr=newPtr->next;
  }

  if (i==NULL) {
    newPtr->next = j;
    newPtr->next->previous = newPtr;
  } else if (j==NULL){
    newPtr->next = i;
    newPtr->next->previous = newPtr;
  }

  return newHead;
};
planet_t* RemovePlantes(int dist ,planet_t* head) {
  if (head==NULL) return NULL;

  //na mh figei kaneis
  if (head->distance > dist) return head;

  int distSum=0;
  planet_t* p=head;
  while (head != NULL && head ->distance <=dist) {

    //remove first planet and make start be the second

    //note for debug: otan vgazeis apo thn arxh , to proto tha krataei to smu
    head= DL_Remove(head,head->pid);
  }
  return head;
};

int findBeginning(ocluster_t clusters[OCLUSTERS_CAP_M]) {
  int low = 0, high = OCLUSTERS_CAP_M - 1;
  int result = -1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (clusters[mid].cid == INT_MAX) {
      result = mid;
      high = mid - 1;
    } else {
      low = mid + 1;        }
  }
  return result;
}




void EventBigBang();
void EventGalaxyCreation(int gid);
void EventStarBirth(int sid, int gid);
void EventPlanetCreation(int pid, int distance, int sid);
void EventStarDeath(int sid, int distance);
void EventPlanetOrphanCrash(int oid, int pid);
void EventGalaxyMerge(int gid1, int gid2);
void EventOrphansClusterCrash(int cid1, int cid2, int cid3);
void EventEndOfWorld(void);
void EventFindInRange(int pid, int distance);
void EventPrintSolar(int sid);
void EventPrintOrphanCluster(int cid);
void EventPrintGalaxy(int gid);
void EventPrintUniverse(void);


void EventBigBang() {
  DebugPrint("B\n");
  // TODO: Student Implementation
  for (int i=0; i<GALAXIES_CAP_N; i++) {
    Galaxies[i].gid = INT_MAX;
    Galaxies[i].solars = NULL;
    Galaxies[i].ssentinel =NULL;
    for (int j=0;j<OCLUSTERS_CAP_M;j++) {
      Galaxies[i].oclusters[j].cid =INT_MAX;
      Galaxies[i].oclusters[j].orphans =NULL;
    }
  }

  galaxyPtr=0;
  printf("DONE\n");
}
void EventGalaxyCreation(int gid) {
  DebugPrint("G %d\n", gid);
  // TODO: Student Implementation
  if (galaxyPtr >=GALAXIES_CAP_N) return;
  Galaxies[galaxyPtr].gid = gid;
  solar_t* sentinel = (solar_t*) malloc(sizeof(solar_t));
  sentinel->planets =NULL;
  sentinel->sid = -1;
  sentinel->next = NULL;
  Galaxies[galaxyPtr].ssentinel = sentinel;
  Galaxies[galaxyPtr].solars=sentinel;

  for (int i=0;i<OCLUSTERS_CAP_M;i++) {
    Galaxies[galaxyPtr].oclusters[i].cid=INT_MAX;
    Galaxies[galaxyPtr].oclusters[i].orphans =NULL;
  }
  galaxyPtr++;
  printf("G %d\n",gid);
  printf("Galaxies = ");
  for (int i=0;i<galaxyPtr;i++) printf(" %d ",Galaxies[i].gid);
  printf("\nDONE\n");
}
void EventStarBirth(int sid, int gid) {
  DebugPrint("S %d %d\n", sid, gid);
  // TODO: Student Implementation
  int p=0;
  while (p<galaxyPtr && Galaxies[p].gid != gid) p++;
  if (p==galaxyPtr) return;

  // to move p to the right spot In Galaxies Table;
  SL_Insert(&Galaxies[p],sid);

  printf("S %d %d \n Solars = ",sid,gid);
  for (solar_t* i=Galaxies[p].solars;i->sid!=-1;i=i->next ) {
    printf("%d",i->sid);
    if (i->next->sid!=-1) {
      printf(",");
    } else {
      printf("\n");
    }
  }
  printf("DONE\n");
}
void EventPlanetCreation(int pid, int distance, int sid) {
  DebugPrint("P %d %d %d\n", pid, distance, sid);
  // TODO: Student Implementation


  solar_t* solarForNewPlanet=NULL;
  for (int i=0;i<galaxyPtr;i++) {
    solarForNewPlanet = SL_Search(&Galaxies[i],sid);
    if (solarForNewPlanet!=NULL) break;
  }
  // ama de to vrei
  if (solarForNewPlanet ==NULL) return;

  solarForNewPlanet->planets = DL_Insert(solarForNewPlanet->planets,pid,distance);

  printf("P %d %d %d\n",pid,distance,sid);
  for (planet_t* i= solarForNewPlanet->planets; i!= NULL;i= i->next) {
    printf("%d : %d : %d : %d\n",i->pid,i->distance, i->previous ==NULL ? 0: i->previous->pid, i->next ==NULL ? 0 : i -> next -> pid);
  }
  printf("DONE\n");
}
void EventStarDeath(int sid, int distance) {
  DebugPrint("D %d %d\n", sid, distance);
  // TODO: Student Implementation
  solar_t* toBeRemoved = NULL;
  //gia na bro solar
  int i=0;
  for (;i<galaxyPtr;i++) {
  toBeRemoved = SL_Search(&Galaxies[i],sid);
    if (toBeRemoved!=NULL) break;
  }

  if (toBeRemoved==NULL) return;

  planet_t* remaningPlanets  =RemovePlantes(distance,toBeRemoved->planets);
  SL_Remove(&Galaxies[i],sid);

  if (remaningPlanets == NULL) return;
  remaningPlanets=srtById(remaningPlanets);

  int j=findBeginning(Galaxies[i].oclusters);

  for (planet_t*p=remaningPlanets;p!=NULL;p=p->next) p->distance=0;

  Galaxies[i].oclusters[j].cid = sid;
  Galaxies[i].oclusters[j].orphans = remaningPlanets;


  printf("D %d %d\n",sid,distance);
  printf(" Solars = ");

  solar_t *sptr=Galaxies[i].solars;
  while (sptr!=Galaxies[i].ssentinel) {
    printf(" %d ",sptr->sid);
    sptr=sptr->next;
    }

  printf("\n OrphansC = ");
  for (int k = 0; k < OCLUSTERS_CAP_M && Galaxies[i].oclusters[k].cid !=INT_MAX; ++k) {
    printf(" %d ",Galaxies[i].oclusters[k].cid);
    printf(Galaxies[i].oclusters[k].cid != INT_MAX ? "," : "");
  }
  printf("\n Orphans = ");
  for (planet_t* pptr=Galaxies[i].oclusters[j].orphans;pptr!=NULL;pptr=pptr->next)pptr->next ==NULL ? printf(" %d ",pptr->pid) : printf(" %d,",pptr->pid);
  printf("\nDONE\n");
}
void EventPlanetOrphanCrash(int oid, int pid) {
  DebugPrint("O %d %d\n", oid, pid);
  // TODO: Student Implementation
  planet_t* crashSid=NULL;
  planet_t* crashOid=NULL;

  solar_t* headSol= NULL;
  ocluster_t* headClust =NULL;

for (int i=0;i<galaxyPtr;i++) {

    for (solar_t* j =Galaxies[i].solars; j!=Galaxies[i].ssentinel && crashSid ==NULL;j=j->next) {
      crashSid  = DL_Search(j->planets,pid);
      headSol = j;
    }

    for (int k=0;k<OCLUSTERS_CAP_M && crashOid == NULL;k++) {
      crashOid = DL_Search(Galaxies[i].oclusters[k].orphans,oid);
      headClust = &Galaxies[i].oclusters[k];
    }
    if (crashSid != NULL && crashOid != NULL) break;
  }
  if (crashSid == NULL || crashOid ==NULL) return;
  if (headClust == NULL || headSol == NULL) return;

  headSol->planets = DL_Remove(headSol->planets,pid);
  headClust->orphans =DL_RemoveOrph(headClust->orphans,oid);

  printf("O %d %d\n",oid ,pid);
  printf(" Planets = \n");
  for (planet_t *i = headSol->planets;i!=NULL;i=i->next) printf("  %d : %d : %d : %d\n",i->pid,i->distance, i->previous !=NULL ? i->previous->pid : 0,i->next !=NULL ? i->next->pid : 0 );

  printf(" Orphans = ");
  for (planet_t *i = headClust->orphans;i!=NULL;i=i->next) i->next!= NULL ? printf(" %d, ",i->pid) : printf(" %d\n",i->pid);

  printf("\nDONE\n");



}
void EventGalaxyMerge(int gid1, int gid2) {
  DebugPrint("M %d %d\n", gid1, gid2);

  galaxy_t* gal1=NULL;
  galaxy_t* gal2=NULL;

  int i=0;
  int j=0;
  for(;i<galaxyPtr; i++){
    if(Galaxies[i].gid == gid1) gal1=&Galaxies[i];
    if(Galaxies[i].gid == gid2) {
      gal2=&Galaxies[i];
      j=i;
    }
  }

  if(gal1 ==NULL || gal2==NULL || gal1==gal2) return;

  printf("M %d %d\n",gid1,gid2);
  printf(" Solars1 = ");
  for (solar_t* tmp=gal1->solars;tmp!=gal1->ssentinel;tmp=tmp->next) tmp->next == gal1->ssentinel ? printf(" %d",tmp->sid) : printf(" %d,",tmp->sid);
  printf("\n");
  printf(" OrphansC1 = ");
  for (int tmp=0;tmp<OCLUSTERS_CAP_M && gal1->oclusters[tmp].cid!=INT_MAX;tmp++) tmp > 0 ? printf(", %d ",gal1->oclusters[tmp].cid) : printf("%d",gal1->oclusters[tmp].cid);


  printf("\n");
  printf(" Solars2 = ");
  for (solar_t* tmp=gal2->solars;tmp!=gal2->ssentinel;tmp=tmp->next) tmp->next == gal2->ssentinel ? printf(" %d",tmp->sid) : printf(" %d,",tmp->sid);
  printf("\n");
  printf(" OrphansC2 = ");
  for (int tmp=0;tmp<OCLUSTERS_CAP_M && gal2->oclusters[tmp].cid!=INT_MAX;tmp++) tmp > 0 ? printf(", %d ",gal2->oclusters[tmp].cid) : printf("%d",gal2->oclusters[tmp].cid);

  if(gal2->solars != gal2->ssentinel) {
    if (gal1->solars == gal1->ssentinel) {
      gal1->solars = gal2->solars;
      gal2->ssentinel->next->next = gal1->ssentinel;
      gal1->ssentinel->next = gal2->ssentinel->next;
    } else {
      gal1->ssentinel->next->next = gal2->solars;
      gal2->ssentinel->next->next = gal1->ssentinel;
      gal1->ssentinel->next = gal2->ssentinel->next;
    }
  }
  gal2->solars=gal2->ssentinel;
  gal2->ssentinel->next=NULL;

  int m1 =findBeginning(gal1->oclusters);
  m1 == -1 ? m1= OCLUSTERS_CAP_M : m1;
  int m2 =findBeginning(gal2->oclusters);
  m2 == -1 ? m2= OCLUSTERS_CAP_M : m2;
  int mmin = m1 > m2 ? m2 : m1;

  for (int k=0;k<mmin;k++) {
    gal1->oclusters[k].orphans = mergeOclust(gal1->oclusters[k].orphans,gal2->oclusters[k].orphans);
  }

  if (m2>mmin) {
    for (int k = mmin;k<m2;k++) {
      gal1->oclusters[k] = gal2->oclusters[k];
    }
  }

  Galaxies[j] =Galaxies[galaxyPtr-1];
  Galaxies[galaxyPtr-1].gid=INT_MAX;
  Galaxies[galaxyPtr - 1].solars = NULL;
  Galaxies[galaxyPtr - 1].ssentinel = NULL;
  for (int m = 0; m < OCLUSTERS_CAP_M; m++) {
    Galaxies[galaxyPtr - 1].oclusters[m].cid = INT_MAX;
    Galaxies[galaxyPtr - 1].oclusters[m].orphans = NULL;
  }
  galaxyPtr--;

  printf("\n");
  printf(" Solars3 = ");
  for (solar_t* tmp=gal1->solars;tmp!=gal1->ssentinel;tmp=tmp->next) tmp->next == gal1->ssentinel ? printf(" %d",tmp->sid) : printf(" %d,",tmp->sid);
  printf("\n");
  printf(" OrphansC3 = ");
  for (int tmp=0;tmp<OCLUSTERS_CAP_M && gal1->oclusters[tmp].cid!=INT_MAX;tmp++) tmp > 0 ? printf(", %d ",gal1->oclusters[tmp].cid) : printf("%d",gal1->oclusters[tmp].cid);
  printf("\nDONE\n");


}

void EventOrphansClusterCrash(int cid1, int cid2, int cid3) {
  DebugPrint("C %d %d %d\n", cid1, cid2, cid3);
  // TODO: Student Implementation
}
void EventEndOfWorld(void) {
  DebugPrint("E\n");
  // TODO: Student Implementation
}
void EventFindInRange(int pid, int distance) {
  DebugPrint("F %d %d\n", pid, distance);
  // TODO: Student Implementation
}
void EventPrintSolar(int sid) {
  DebugPrint("H %d\n", sid);
  // TODO: Student Implementation
  solar_t * toBePrinted=NULL;
  int p=0;

  while (p<galaxyPtr && toBePrinted ==NULL) {
    toBePrinted = SL_Search(&Galaxies[p],sid);
    p++;
  }

  if (toBePrinted ==NULL) return;

  printf("H %d \n Planets = \n",sid);
  if (toBePrinted->planets != NULL) for (planet_t* j = toBePrinted->planets; j != NULL;j=j->next)
    printf("%d : %d : %d : %d\n", j->pid,j->distance,(j->previous ==NULL ? 0 : j->previous->pid), (j->next ==NULL ? 0 : j->next->pid));


  printf("DONE\n");
}
void EventPrintOrphanCluster(int cid) {
  DebugPrint("I %d\n", cid);
  // TODO: Student Implementation

  int i=0,j=0;

  for (;i<galaxyPtr;i++) {
    for (j=0;j<OCLUSTERS_CAP_M && Galaxies[i].oclusters[j].cid !=INT_MAX;j++) {
      if (Galaxies[i].oclusters[j].cid == cid) break;
    }
    if (j < OCLUSTERS_CAP_M && Galaxies[i].oclusters[j].cid == cid) break;
  }
    printf("I %d\n",cid);
    printf("Orphans = \n");
    for (planet_t* t=Galaxies[i].oclusters[j].orphans;t!=NULL;t=t->next) {
      printf("%d : %d : %d \n",t->pid, t->previous!=NULL ? t->previous->pid : 0, t->next !=NULL ? t->next->pid: 0);
    }
    printf("\nDONE\n");

}
void EventPrintGalaxy(int gid) {
    DebugPrint("J %d\n", gid);
    // TODO: Student Implementation
    int p=0;
    while (p<galaxyPtr && Galaxies[p].gid != gid) p++;
    if (p==galaxyPtr) return;
    // to move p to the right spot In Galaxies Table;
    printf("J %d\n",gid);


    printf("Solars =\n");
    for (solar_t* i = Galaxies[p].solars; i != NULL && i != Galaxies[p].ssentinel; i = i->next) {
      EventPrintSolar(i->sid);
    }

    printf("\nOrphan planets clusters = \n");
    for (int i=0;i<OCLUSTERS_CAP_M && Galaxies[p].oclusters[i].cid != INT_MAX ;i++) {
      EventPrintOrphanCluster(Galaxies[p].oclusters[i].cid);
    }
    printf("\nDONE\n");
  }
void EventPrintUniverse(void) {
    DebugPrint("U\n");
    // TODO: Student Implementation
  }

  /*** Parsing Utilities ***/

  void TrimLine(char *line) {
    char *start = line;
    // Trim leading whitespace
    while (*start && isspace((unsigned char)*start))
      start += 1;
    if (*start == '\0') { // whitespace-only string case
      *line = '\0';
      return;
    }
    // Trim trailing whitespace
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end))
      end -= 1;
    *(end + 1) = '\0';
    // Copy the trimmed content to the original buffer (memmove used for
    // overlapping content)
    memmove(line, start, strlen(start));
  }

  bool IsEmptyLine(const char *line) {
    if (line == NULL || *line == '\0')
      return true;
    // check for non-whitespace characters
    for (const char *p = line; *p != '\0'; ++p)
      if (!isspace((unsigned char)*p))
        return false;
    return true;
  }

  void ReportOutOfBounds(const char *line, int lineIdx, int arg, int minLimit,
                         int maxLimit) {
    if ((minLimit <= arg) && (arg < maxLimit))
      return;
    fprintf(stderr, "On line %d: %s\n", lineIdx, line);
    fprintf(stderr, "argument %d out of bounds [%d, %d)\n", arg, minLimit,
            maxLimit);
    exit(EXIT_FAILURE);
  }

  void ReportInvalidParse(const char *line, int lineIdx, int actual,
                          int expected) {
    if (actual == expected)
      return;
    fprintf(stderr, "On line %d: %s\n", lineIdx, line);
    fprintf(stderr,
            "sscanf argument assignment error: expected %d, assigned %d\n",
            expected, actual);
    exit(EXIT_FAILURE);
  }

  int main(int argc, char *argv[]) {
    if (argc != 2) {
      fprintf(stderr, "Invalid number of arguments\n");
      exit(EXIT_FAILURE);
    }
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
      fprintf(stderr, "fopen error (Invalid path ?)\n");
      exit(EXIT_FAILURE);
    }
#define LINE_SZ 4096
    char line[LINE_SZ] = {0};
    for (int lineIdx = 1; fgets(line, LINE_SZ, inputFile) != NULL;
         ++lineIdx) { // Read next line from input file
      line[strcspn(line, "\r\n")] = '\0';
      TrimLine(line);
      if (IsEmptyLine(line)) {
        continue; // Skip empty lines
      }
      if (line[0] == '#') {
        continue; // Skip comment lines
      }
      if (line[0] == 'B') {
        EventBigBang();
        continue;
      }
      if (line[0] == 'G') {
        const char *args = &(line[1]);
        int gid;
        int count = sscanf(args, "%d", &gid);
        ReportInvalidParse(line, lineIdx, count, 1);
        ReportOutOfBounds(line, lineIdx, gid, 0, INT_MAX);
        EventGalaxyCreation(gid);
        continue;
      }
      if (line[0] == 'S') {
        const char *args = &(line[1]);
        int sid, gid;
        int count = sscanf(args, "%d %d", &sid, &gid);
        ReportInvalidParse(line, lineIdx, count, 2);
        ReportOutOfBounds(line, lineIdx, sid, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, gid, 0, INT_MAX);
        EventStarBirth(sid, gid);
        continue;
      }
      if (line[0] == 'P') {
        const char *args = &(line[1]);
        int pid, distance, sid;
        int count = sscanf(args, "%d %d %d", &pid, &distance, &sid);
        ReportInvalidParse(line, lineIdx, count, 3);
        ReportOutOfBounds(line, lineIdx, pid, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, distance, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, sid, 0, INT_MAX);
        EventPlanetCreation(pid, distance, sid);
        continue;
      }
      if (line[0] == 'D') {
        const char *args = &(line[1]);
        int sid, distance;
        int count = sscanf(args, "%d %d", &sid, &distance);
        ReportInvalidParse(line, lineIdx, count, 2);
        ReportOutOfBounds(line, lineIdx, sid, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, distance, 0, INT_MAX);
        EventStarDeath(sid, distance);
        continue;
      }
      if (line[0] == 'O') {
        const char *args = &(line[1]);
        int oid, pid;
        int count = sscanf(args, "%d %d", &oid, &pid);
        ReportInvalidParse(line, lineIdx, count, 2);
        ReportOutOfBounds(line, lineIdx, oid, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, pid, 0, INT_MAX);
        EventPlanetOrphanCrash(oid, pid);
        continue;
      }
      if (line[0] == 'M') {
        const char *args = &(line[1]);
        int gid1, gid2;
        int count = sscanf(args, "%d %d", &gid1, &gid2);
        ReportInvalidParse(line, lineIdx, count, 2);
        ReportOutOfBounds(line, lineIdx, gid1, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, gid2, 0, INT_MAX);
        EventGalaxyMerge(gid1, gid2);
        continue;
      }
      if (line[0] == 'C') {
        const char *args = &(line[1]);
        int cid1, cid2, cid3;
        int count = sscanf(args, "%d %d %d", &cid1, &cid2, &cid3);
        ReportInvalidParse(line, lineIdx, count, 3);
        ReportOutOfBounds(line, lineIdx, cid1, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, cid2, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, cid3, 0, INT_MAX);
        EventOrphansClusterCrash(cid1, cid2, cid3);
        continue;
      }
      if (line[0] == 'E') {
        EventEndOfWorld();
        continue;
      }
      if (line[0] == 'F') {
        const char *args = &(line[1]);
        int pid, distance;
        int count = sscanf(args, "%d %d", &pid, &distance);
        ReportInvalidParse(line, lineIdx, count, 2);
        ReportOutOfBounds(line, lineIdx, pid, 0, INT_MAX);
        ReportOutOfBounds(line, lineIdx, distance, 0, INT_MAX);
        EventFindInRange(pid, distance);
        continue;
      }
      if (line[0] == 'H') {
        const char *args = &(line[1]);
        int sid;
        int count = sscanf(args, "%d", &sid);
        ReportInvalidParse(line, lineIdx, count, 1);
        ReportOutOfBounds(line, lineIdx, sid, 0, INT_MAX);
        EventPrintSolar(sid);
        continue;
      }
      if (line[0] == 'I') {
        const char *args = &(line[1]);
        int cid;
        int count = sscanf(args, "%d", &cid);
        ReportInvalidParse(line, lineIdx, count, 1);
        ReportOutOfBounds(line, lineIdx, cid, 0, INT_MAX);
        EventPrintOrphanCluster(cid);
        continue;
      }
      if (line[0] == 'J') {
        const char *args = &(line[1]);
        int gid;
        int count = sscanf(args, "%d", &gid);
        ReportInvalidParse(line, lineIdx, count, 1);
        ReportOutOfBounds(line, lineIdx, gid, 0, INT_MAX);
        EventPrintGalaxy(gid);
        continue;
      }
      if (line[0] == 'U') {
        EventPrintUniverse();
        continue;
      }
      fprintf(stderr, "Invalid line %d: %s\n", lineIdx, line);
      exit(EXIT_FAILURE);
         }
    int err = fclose(inputFile);
    if (err != 0) {
      fprintf(stderr, "fclose error\n");
      exit(EXIT_FAILURE);
    }
  }
