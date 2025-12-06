#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 4
#define MAX_COLS 20000

typedef struct {
  long add;
  long multiply;
  char action; // set in part1, not in processFile
} Values;

void processFile(FILE *f, Values *values, int *valuesLength) {
  char buffer[4096];

  for (int i = 0; i < 2048; i++) {
    values[i].add = 0;
    values[i].multiply = 1;
    values[i].action = 0;
  }

  int maxColumns = 0;

  while (fgets(buffer, sizeof(buffer), f)) {

    int col = 0;
    char *p = buffer;

    while (*p) {
      while (*p && isspace((unsigned char)*p))
        p++;

      if (!*p)
        break;

      if (!isdigit((unsigned char)*p))
        break;

      char *end;
      long v = strtol(p, &end, 10);
      p = end;

      // store into column bucket
      if (values[col].multiply == 1 && values[col].add == 0) {
        // first time this column is used
        values[col].add = 0;
        values[col].multiply = 1;
      }

      values[col].add += v;
      values[col].multiply *= v;

      col++;
    }

    if (col > maxColumns)
      maxColumns = col;
  }

  *valuesLength = maxColumns;
}

long part1(Values *values, int valuesLength, const char *opLine) {
  long result = 0;

  // assign actions from operator line
  int vi = 0;
  for (int i = 0; opLine[i] && vi < valuesLength; i++) {
    if (!isspace((unsigned char)opLine[i])) {
      values[vi].action = opLine[i];
      vi++;
    }
  }

  // compute final sum
  for (int i = 0; i < valuesLength; i++) {
    if (values[i].action == '*')
      result += values[i].multiply;
    else
      result += values[i].add;
  }
  return result;
}

void processFileP2(const char *filename, char lines[][4096], size_t *rows) {

  *rows = 0;

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("open");
    exit(1);
  }

  while (fgets(lines[*rows], 4096, fp)) {
    size_t len = strlen(lines[*rows]);
    if (len > 0 && lines[*rows][len - 1] == '\n') {
      lines[*rows][len - 1] = '\0';
    }
    (*rows)++;
  }

  fclose(fp);
}

long part2(char lines[][4096], size_t rows) {
  long nums[10];
  size_t num_idx = 0;
  long total = 0;

  size_t width = strlen(lines[0]);

  for (int col = width - 1; col >= 0; col--) {

    long n = 0;
    bool digit_found = false;

    for (size_t r = 0; r < rows; r++) {
      char ch = lines[r][col];

      if (ch >= '0' && ch <= '9') {
        n = n * 10 + (ch - '0');
        digit_found = true;
      } else if (ch == '*') {
        long prod = n;
        for (size_t j = 0; j < num_idx; j++)
          prod *= nums[j];
        total += prod;
      } else if (ch == '+') {
        long sum = n;
        for (size_t j = 0; j < num_idx; j++)
          sum += nums[j];
        total += sum;
      }
    }

    if (digit_found) {
      nums[num_idx++] = n;
    } else {
      num_idx = 0;
    }
  }

  return total;
}
// this is so messy
int main(void) {
  FILE *f = fopen("./input.txt", "r");
  if (!f) {
    perror("fopen");
    return 1;
  }

  Values values[2048];
  int valuesLength = 0;

  // read all but last operator line
  long opLinePos = 0;
  char line[4096];

  // find operator line by reading the whole file once
  while (fgets(line, sizeof(line), f)) {
    opLinePos = ftell(f);
  }

  // operator line is last line read into `line`
  char operatorLine[4096];
  strcpy(operatorLine, line);

  rewind(f);
  processFile(f, values, &valuesLength);
  fclose(f);

  long answer = part1(values, valuesLength, operatorLine);
  printf("%ld\n", answer);
  char lines[10][4096];
  size_t rows;

  processFileP2("./input.txt", lines, &rows);

  long result = part2(lines, rows);
  printf("%ld\n", result);
}