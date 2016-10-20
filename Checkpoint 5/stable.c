#include "stable.h"
#include "stdio.h"

struct symbol_table *table;
void insert(struct symbol_table_entry entry) {
  table->table[table->count] = entry;
  table->count++;
}
int isPresent(char *name) {
  int i;
  for(i = 0; i < 100; i++) {
    if(table->table[i].name == name) {
      return i;
    }
  }
  return -1;
}
struct symbol_table_entry retrieve(char *name) {
  return table->table[isPresent(name)];
}
int main() {
  return 0;
}
