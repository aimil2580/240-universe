//
// Created by aimil on 11/3/26.
//
//
// Created by aimil on 11/1/26.
#define GALAXIES_CAP_N 1024
#define OCLUSTERS_CAP_M 256
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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

static galaxy_t Galaxies[GALAXIES_CAP_N];
static int galaxyPtr;




void EventGalaxyCreation(int gid) {
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



void EventBigBang() {
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
    printf("B DONE\n");
};



void SL_Remove(galaxy_t* galaxy , int sid) {
    if (galaxy ==NULL || galaxy->ssentinel ==NULL) return;
    solar_t * toBeRemoved = galaxy->solars;
    solar_t * prevOfDeleted;
    while (sid != toBeRemoved->sid && toBeRemoved->sid !=-1) {
        prevOfDeleted =toBeRemoved;
        toBeRemoved = toBeRemoved ->next;
    }
    //ama de to vrei
    if (toBeRemoved->sid==-1) return;

    //an pao na vgalo to ena poy uparxei
    if (toBeRemoved->next == galaxy->ssentinel) {
        toBeRemoved->next =NULL;
        toBeRemoved->planets=NULL;
        toBeRemoved->sid =-2;
        galaxy->solars= galaxy->ssentinel;
        galaxy->ssentinel->next = NULL;
    }else {
        // an otidhpote allo
        prevOfDeleted->next= toBeRemoved->next;
        if (toBeRemoved->next == galaxy->ssentinel){
            galaxy->ssentinel->next = prevOfDeleted;
        }
        toBeRemoved->next=NULL;
        toBeRemoved->planets=NULL;
        toBeRemoved->sid=-2;
    }
}
solar_t* SL_Search(galaxy_t* galaxy, int sid) {
    if (galaxy ==NULL || galaxy->ssentinel ==NULL) return NULL;
    solar_t* toBeFound = galaxy->solars;
    while (sid != toBeFound->sid && toBeFound->sid !=-1) toBeFound=toBeFound->next;
    if (toBeFound == galaxy->ssentinel) return NULL;
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




void EventStarBirth(int sid, int gid) {
    int p=0;
    while (p<galaxyPtr && Galaxies[p].gid != gid) p++;
    // if (p==galaxyPtr) return;
    // to move p to the right spot In Galaxies Table;
    SL_Insert(&Galaxies[p],sid);

    printf("S %d %d \n  Solars = ",sid,gid);
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



void EventPrintSolar(int sid) {
    solar_t * toBePrinted=NULL;
    int p=0;

    while (p<galaxyPtr && toBePrinted ==NULL) {
        toBePrinted = SL_Search(&Galaxies[p],sid);
        p++;
    }

    if (toBePrinted ==NULL) return;

    printf("H %d \n Planets = \n",sid);
    if (toBePrinted->planets != NULL) for (planet_t* j = toBePrinted->planets; j != NULL;j=j->next) printf("%d : %d : %d : %d\n", j->pid,j->distance,(j->previous ==NULL ? 0 : j->previous->pid), (j->next ==NULL ? 0 : j->next->pid));
    printf("DONE\n");
}

void EventPrintGalaxy(int gid) {
    int p=0;
    while (p<galaxyPtr && Galaxies[p].gid != gid) p++;
    if (p==galaxyPtr) return;
    // to move p to the right spot In Galaxies Table;
    printf("J %d\n",gid);
    if (Galaxies[p].solars!=NULL) {
        for (solar_t* i =Galaxies[p].solars;i->sid!=-1;i=i->next) EventPrintSolar(i->sid);
    }else {

    printf("DONE\n");
    }
}
int main() {
    EventBigBang();
    EventGalaxyCreation(122);
    EventGalaxyCreation(127);
    EventGalaxyCreation(123);
    EventGalaxyCreation(124);
    EventGalaxyCreation(10);
    EventStarBirth(2,10);
    EventStarBirth(445,10);
    EventStarBirth(1,10);
    EventStarBirth(97,10);
    printf("%p\n",SL_Search(&Galaxies[4],445));

    planet_t* new1 = (planet_t*) malloc(sizeof(planet_t));
    new1->pid=51;
    new1->distance=21;
    new1->previous=NULL;
    planet_t* new2 = (planet_t*) malloc(sizeof(planet_t));
    new1->next = new2;
    new2->previous= new1;
    new2->pid=52;
    new2->distance=22;
    planet_t* new3 =(planet_t* )malloc(sizeof(planet_t));
    new2->next=new3;

    new3->next=NULL;
    new3->pid=53;
    new3->previous=new2;
    new3->distance=23;

    Galaxies[4].solars->planets =  new1;// sid =2
    // EventPrintSolar(2);

    SL_Remove(&Galaxies[4],51);
    // EventPrintSolar(2);
    EventPrintGalaxy(10);
}

