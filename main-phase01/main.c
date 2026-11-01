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

// Helper functions go here

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
}
void EventGalaxyCreation(int gid) {
  DebugPrint("G %d\n", gid);
  // TODO: Student Implementation
}
void EventStarBirth(int sid, int gid) {
  DebugPrint("S %d %d\n", sid, gid);
  // TODO: Student Implementation
}
void EventPlanetCreation(int pid, int distance, int sid) {
  DebugPrint("P %d %d %d\n", pid, distance, sid);
  // TODO: Student Implementation
}
void EventStarDeath(int sid, int distance) {
  DebugPrint("D %d %d\n", sid, distance);
  // TODO: Student Implementation
}
void EventPlanetOrphanCrash(int oid, int pid) {
  DebugPrint("O %d %d\n", oid, pid);
  // TODO: Student Implementation
}
void EventGalaxyMerge(int gid1, int gid2) {
  DebugPrint("M %d %d\n", gid1, gid2);
  // TODO: Student Implementation
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
}
void EventPrintOrphanCluster(int cid) {
  DebugPrint("I %d\n", cid);
  // TODO: Student Implementation
}
void EventPrintGalaxy(int gid) {
  DebugPrint("J %d\n", gid);
  // TODO: Student Implementation
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
