#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  long start;
  long end;
} FreshRange;

int part1(long *ids, int idLength, FreshRange *validRanges,
          int validRangesLength) {
  int result = 0;
  for (size_t i = 0; i < idLength; i++) {
    long id = ids[i];
    for (size_t j = 0; j < validRangesLength; j++) {
      if (validRanges[j].start == -1 || validRanges[j].end == -1)
        continue;
      if (id >= validRanges[j].start && id <= validRanges[j].end) {
        // printf("%ld\n", id);
        result += 1;
        break;
      }
    }
  }
  return result;
}

long part2(FreshRange *validRanges, int validRangesLength) {
  long result = 0;
  for (size_t i = 0; i < validRangesLength; i++) {
    if (validRanges[i].start == -1 || validRanges[i].end == -1)
      continue;
    result += validRanges[i].end - validRanges[i].start + 1;
  }
  return result;
}

void processFile(FILE *f, long *ids, int *idLength, FreshRange *validRanges,
                 int *validRangesLength) {

  int i = 0;
  long start, end;
  char buffer[256];

  while (fgets(buffer, sizeof(buffer), f)) {
    if (buffer[0] == '\n') {
      break;
    }
    if (sscanf(buffer, "%ld-%ld", &start, &end) == 2) {
      validRanges[i].start = start;
      validRanges[i].end = end;
      i++;
    }
  }

  *validRangesLength = i;
  i = 0;
  while (fgets(buffer, sizeof(buffer), f) != NULL) {
    long value = atol(buffer);

    ids[i] = value;
    i++;
  }

  *idLength = i;
  fclose(f);
}

void condenseRanges(FreshRange *ranges, int validRangesLength) {
  bool reduced = true;
  while (reduced) {
    reduced = false;
    for (size_t i = 0; i < validRangesLength; i++) {
      if (ranges[i].start == -1)
        continue;
      for (size_t j = 0; j < validRangesLength; j++) {

        if (i == j)
          continue;
        if (ranges[j].start == -1)
          continue;
        if (ranges[j].start <= ranges[i].start &&
            ranges[i].end <= ranges[j].end) {
          ranges[i].start = ranges[j].start;
          ranges[i].end = ranges[j].end;
          reduced = true;
          ranges[j].start = -1;
          ranges[j].end = -1;
          continue;
        }
        if (ranges[i].start <= ranges[j].start &&
            ranges[j].start <= ranges[i].end &&
            ranges[j].end >= ranges[i].end) {
          ranges[i].end = ranges[j].end;
          reduced = true;
          ranges[j].start = -1;
          ranges[j].end = -1;
          continue;
        }
        if (ranges[i].start <= ranges[j].end &&
            ranges[j].end <= ranges[i].end &&
            ranges[j].start <= ranges[i].start) {
          ranges[i].start = ranges[j].start;
          reduced = true;
          ranges[j].start = -1;
          ranges[j].end = -1;
          continue;
        }
      }
    }
  }
}

int main() {
  FILE *f = fopen("./input.txt", "r");
  long ids[1024];
  FreshRange validRanges[1024];
  int idLength, validRangesLength;
  clock_t start = clock();
  processFile(f, ids, &idLength, validRanges, &validRangesLength);
  condenseRanges(validRanges, validRangesLength);

  int p1 = part1(ids, idLength, validRanges, validRangesLength);
  printf("p1 %d\n", p1);

  long p2 = part2(validRanges, validRangesLength);
  clock_t end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("time %f\n", time_spent);
  printf("p2 %ld\n", p2);
}