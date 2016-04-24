#ifndef _LINUX_PRAM_H
#define _LINUX_PRAM_H

#define DRAM_NODE 0
#define PRAM_NODE 1
enum inactive_type {
  PRAM_ANON,
  PRAM_FILE
};
struct pram_migrate {
 int node;
 enum inactive_type type;
}; 

#endif //__LINUX_PRAM_H
