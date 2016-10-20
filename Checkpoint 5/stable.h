#ifndef stable_h
#define stable_h

struct symbol_table_entry {
  char *name;
  int address;
  int kind;
  int type;
  int size;
};

struct symbol_table {
  struct symbol_table_entry table[100];
  int count;
  int memorySize;
};

void insert(struct symbol_table_entry entry);
int isPresent(char *name);
struct symbol_table_entry retrieve(char *name);
struct symbol_table_entry createEntry(char *name, int address, int kind, int type, int size);
extern struct symbol_table *table;

#endif /** stable_h */
