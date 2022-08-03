
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char valid[] = "+-.,[]><";
static int cells[30000];
static int *dp = cells;

typedef struct bfins {
  char name;
  int pos;
  int call;
  int jump;

} BFins;

int is_valid(char ch) {
  for (int i = 0; i != 8; i++) {
    if (ch == valid[i]) {
      return 1;
    }
  }
  return 0;
}

size_t bfsize(FILE *file) {
  int size = 0;
  char ch;
  while (ch != EOF) {
    ch = fgetc(file);
    if (is_valid(ch)) {
      size++;
    }
  }
  rewind(file);
  return size;
}

char *read_from(char *filename) {
  FILE *file = fopen(filename, "r");
  int size_ = bfsize(file);
  char *bfcode = malloc((size_ + 1));
  char ch;
  int i = 0;
  while (i != size_) {
    ch = fgetc(file);
    if (is_valid(ch)) {
      bfcode[i] = ch;
      i++;
    }
  }
  bfcode[size_] = '\0';
  return bfcode;
}

BFins *compile(char *bfcode) {
  int len = strlen(bfcode);
  BFins *compiled = malloc(len * sizeof(BFins));
  char temp;
  for (int i = 0; i != len; i++) {
    temp = bfcode[i];
    if (!(is_valid(temp))) {
      continue;
    }
    BFins ins;
    ins.name = temp;
    ins.pos = i;
    compiled[i] = ins;
  }
  return compiled;
}

void handle_brackets(BFins *compiled, int size_) {
  BFins temp;
  int depth, rel;
  for (int i = 0; i != size_ + 1; i++) {
    temp = compiled[i];
    if (temp.name == '[') {
      depth = 0;
      for (rel = i + 1; depth != 0 || compiled[rel].name != ']'; rel++) {
        if (compiled[rel].name == '[') {
          depth++;
        }
        if (compiled[rel].name == ']') {
          depth--;
        }
      }
      compiled[temp.pos].jump = rel;
      compiled[rel].call = i;
    }
  }
}

void interpret(BFins *compiled, int size_) {
  BFins temp;
  for (int i = 0; i != size_; i++) {
    temp = compiled[i];
    if (temp.name == '+') {
      (*dp)++;
    }
    if (temp.name == '-') {
      (*dp)--;
    }
    if (temp.name == '>') {
      dp++;
    }
    if (temp.name == '<') {
      dp--;
    }
    if (temp.name == '.') {
      putchar(*dp);
    }
    if (temp.name == ',') {
      *dp = getchar();
    }
    if (temp.name == '[') {
      if (!*dp) {
        i = temp.jump;
      }
    }
    if (temp.name == ']') {
      if (*dp) {
        i = temp.call;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  char *bfcode = read_from(argv[1]);
  int size_ = strlen(bfcode);
  BFins *compiled = compile(bfcode);
  handle_brackets(compiled, size_);
  interpret(compiled, size_);
}