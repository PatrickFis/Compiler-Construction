/*
 * stable.h
 * Programmer - Patrick Fischer
 * This is the code for my symbol table.
 *
 */

#include "stable.h"
#include "stdio.h"
#include <string.h>

// Global symbol table
struct symbol_table *table;
// Insert a new symbol_table_entry into table
void insert(struct symbol_table_entry entry) {
  entry.address = table->memorySize;
  table->table[table->count] = entry;
  table->count++;
  table->memorySize += entry.size;
  // printf("name = %s, address = %d, kind = %d, type = %d, size = %d, count = %d\n", entry.name, entry.address, entry.kind, entry.type, entry.size, table->count);
}

// If name is present in table, then return its' array location.
// This method was incorrect... strings cannot be compared with the == operator.
// strcmp must be used instead. Remember that strcmp returns 0 if the strings
// are equal.
int isPresent(char *name) {
  int i;
  // printf("%s\n", name);
  for(i = 0; i < 100; i++) {
    // printf("%s\n", table->table[i].name);
    if(strcmp(table->table[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

// Return the symbol_table_entry given by name
struct symbol_table_entry retrieve(char *name) {
  return table->table[isPresent(name)];
}

struct symbol_table_entry createEntry(char *name, int address, int kind, int type, int size) {
  struct symbol_table_entry entry;
  entry.name = name; // This might need a malloc
  entry.address = address;
  entry.kind = kind;
  entry.type = type;
  entry.size = size;
  return entry;
}
