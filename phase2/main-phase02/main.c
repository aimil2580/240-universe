/*
 * ============================================
 * file: main.c
 * @Author Nikos Metaxakis (csdp1437@csd.uoc.gr)
 * @Version 23/11/2026
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 2,
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

typedef struct galaxy galaxy_t;
typedef struct solar solar_t;
typedef struct planet planet_t;

struct galaxy {
  int gid;
  solar_t *solars;
  solar_t *ssentinel;
  planet_t *orphans;
};

struct solar {
  int sid;
  planet_t *planets;
  planet_t *psentinel;
  solar_t *next;
};

struct planet {
  int pid;
  int distance;
  planet_t *lc;
  planet_t *rc;
  planet_t *parent;
};

galaxy_t Galaxies[GALAXIES_CAP_N];

// Globals for Bonus 2 (Universal Hashing)
int max_id_g = 0;
int primes_g[160] = {
    5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,  53,  59,
    61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
    223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
    307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389,
    397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
    487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587,
    593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
    677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773,
    787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
    883, 887, 907, 911, 919, 929, 937, 941, 947, 953};

// Helper functions go here

void EventBigBang(void);
void EventGalaxyCreation(int gid);
void EventStarBirth(int sid, int gid);
void EventPlanetCreation(int pid, int distance, int sid);
void EventStarDeath(int sid, int distance);
void EventPlanetOrphanCrash(int oid, int pid);
void EventLookupPlanet(int pid);
void EventLookupOrphanPlanet(int oid);
void EventPrintSolar(int sid);
void EventPrintOrphans(int gid);
void EventPrintGalaxy(int gid);
void EventPrintUniverse(void);
void EventEndOfWorld(void);
void EventBinaryStarCreation(int sid1, int distance, int sid2, int sid3);

void EventBigBang(void) {
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
void EventLookupPlanet(int pid) {
  DebugPrint("L %d\n", pid);
  // TODO: Student Implementation
}
void EventLookupOrphanPlanet(int oid) {
  DebugPrint("K %d\n", oid);
  // TODO: Student Implementation
}
void EventPrintSolar(int sid) {
  DebugPrint("H %d\n", sid);
  // TODO: Student Implementation
}
void EventPrintOrphans(int gid) {
  DebugPrint("I %d\n", gid);
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
void EventEndOfWorld(void) {
  DebugPrint("E\n");
  // TODO: Student Implementation
}
void EventBinaryStarCreation(int sid1, int distance, int sid2, int sid3) {
  DebugPrint("C %d %d %d %d\n", sid1, distance, sid2, sid3);
  // TODO: Student Implementation (Bonus 1)
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
  memmove(line, start, strlen(start) + 1);
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
    if (line[0] == '3') { // MAX_ID (for Bonus 2)
      const char *args = &(line[1]);
      int count = sscanf(args, "%d", &max_id_g);
      ReportInvalidParse(line, lineIdx, count, 1);
      ReportOutOfBounds(line, lineIdx, max_id_g, 0, INT_MAX);
      DebugPrint("3 %d\n", max_id_g);
      continue;
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
    if (line[0] == 'L') {
      const char *args = &(line[1]);
      int pid;
      int count = sscanf(args, "%d", &pid);
      ReportInvalidParse(line, lineIdx, count, 1);
      ReportOutOfBounds(line, lineIdx, pid, 0, INT_MAX);
      EventLookupPlanet(pid);
      continue;
    }
    if (line[0] == 'K') {
      const char *args = &(line[1]);
      int oid;
      int count = sscanf(args, "%d", &oid);
      ReportInvalidParse(line, lineIdx, count, 1);
      ReportOutOfBounds(line, lineIdx, oid, 0, INT_MAX);
      EventLookupOrphanPlanet(oid);
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
      int gid;
      int count = sscanf(args, "%d", &gid);
      ReportInvalidParse(line, lineIdx, count, 1);
      ReportOutOfBounds(line, lineIdx, gid, 0, INT_MAX);
      EventPrintOrphans(gid);
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
    if (line[0] == 'E') {
      EventEndOfWorld();
      continue;
    }
    if (line[0] == 'C') {
      const char *args = &(line[1]);
      int sid1, distance, sid2, sid3;
      int count = sscanf(args, "%d %d %d %d", &sid1, &distance, &sid2, &sid3);
      ReportInvalidParse(line, lineIdx, count, 4);
      ReportOutOfBounds(line, lineIdx, sid1, 0, INT_MAX);
      ReportOutOfBounds(line, lineIdx, distance, 0, INT_MAX);
      ReportOutOfBounds(line, lineIdx, sid2, 0, INT_MAX);
      ReportOutOfBounds(line, lineIdx, sid3, 0, INT_MAX);
      EventBinaryStarCreation(sid1, distance, sid2, sid3);
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
  return EXIT_SUCCESS;
}