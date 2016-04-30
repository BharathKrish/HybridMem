#include<linux/mmzone.h>

void inc_dram2pram_file_count(void);
void inc_pram2dram_file_count(void);
void inc_dram2pram_anon_count(void);
void inc_pram2dram_anon_count(void);

void init_counters(void);

extern unsigned long d2p_anon_count;
extern unsigned long d2p_page_count;
extern unsigned long p2d_anon_count;
extern unsigned long p2d_page_count;
extern unsigned long pram_flush_count;


//////////////////////////////////////////////////
// 0 : D2P_ANON_COUNT
// 1 : P2D_ANON_COUNT
// 2 : D2P_PAGE_COUNT
// 3 : P2D_PAGE_COUNT
// 4 : PRAM_FLUSH_COUNT 
//////////////////////////////////////////////////

void update_pdram_metrics(int type,enum lru_list lru);
void clear_pdram_metrics(void);
void print_pdram_metrics(void);
