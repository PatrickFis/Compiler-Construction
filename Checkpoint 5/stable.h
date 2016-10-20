struct symbol_table_entry {
  int address;
  int kind;
  int type;
  int size;
};

struct symbol_table {
  symbol_table_entry table[100];
  int count;
  int memorySize;
}
