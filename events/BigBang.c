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


void EventGalaxyCreation(int gid) {
    if (galaxyPtr >=GALAXIES_CAP_N) return;
    Galaxies[galaxyPtr].gid = gid;
    solar_t* sentinel = (solar_t*) malloc(sizeof(solar_t));
    sentinel->planets =NULL;
    sentinel->sid = -1;
    sentinel->next = sentinel;
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



int main() {
    EventBigBang();
    EventGalaxyCreation(67);
    EventGalaxyCreation(9898);
    return 0;
}
