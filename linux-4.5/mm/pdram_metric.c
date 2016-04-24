#include<linux/pdram_metric.h>
#include<linux/kernel.h>

unsigned long d2p_anon_count;
unsigned long d2p_page_count;
unsigned long p2d_anon_count;
unsigned long p2d_page_count;
unsigned long pram_flush_count;
unsigned long dram_flush_count;
void init_counters(){
    d2p_anon_count=0;
    d2p_page_count=0;
}
void inc_dram2pram_anon_count(){
    
    d2p_anon_count++;
    printk("Anon Count increased\n");
}

void clear_pdram_metrics(){
    d2p_anon_count  =0;
    p2d_anon_count  =0;
    d2p_page_count  =0;
    p2d_page_count  =0;
    pram_flush_count=0;

}

void print_pdram_metrics(){
    printk(" D2P_ANON_COUNT :%ld\n",d2p_anon_count);
    printk(" P2D_ANON_COUNT :%ld\n",p2d_anon_count);
    printk(" D2P_PAGE_COUNT :%ld\n",d2p_page_count);
    printk(" P2D_PAGE_COUNT :%ld\n",p2d_page_count);
    printk(" PRAM_FLUSH_COUNT :%ld\n",pram_flush_count);
}
EXPORT_SYMBOL(clear_pdram_metrics);
EXPORT_SYMBOL(print_pdram_metrics);
//////////////////////////////////////////////////
// 0 : D2P_ANON_COUNT
// 1 : P2D_ANON_COUNT
// 2 : D2P_PAGE_COUNT
// 3 : P2D_PAGE_COUNT
// 4 : PRAM_FLUSH_COUNT 
//////////////////////////////////////////////////
void update_pdram_metrics(int metric_type,enum lru_list lru){
    
    int type=99;

    /////////////////////////////////////////
    //
    // 0 -> Added to the inactive list. Represents move to PRAM Here.
    // 4 -> Being freed from the lru list.
    // 1 -> Not sure added to which list. Resolve using lru.
    // 2 -> Being added to the active list.
    // /////////////////////////////////////////

    if(metric_type == 0){
        if(lru == LRU_INACTIVE_ANON)
            type = 0;
        else if (lru == LRU_INACTIVE_FILE)
            type = 1;
    }
    else if(metric_type == 1){
        if (lru == LRU_INACTIVE_ANON)
            type = 0;
        else if(lru == LRU_INACTIVE_FILE)
            type = 1;
        else if(lru == LRU_ACTIVE_ANON)
            type = 2;
        else if(lru == LRU_ACTIVE_FILE)
            type = 3;
    }
    else if(metric_type == 2){
        if(lru == LRU_ACTIVE_ANON)
            type = 2;
        else if(lru == LRU_ACTIVE_FILE)
            type = 3;
    }
    else if(metric_type == 4){
        if (lru == LRU_INACTIVE_ANON || lru == LRU_INACTIVE_FILE)
            type = 4; //Being freed from PRAM
        if (lru == LRU_ACTIVE_ANON || lru == LRU_ACTIVE_FILE)
            type = 5; //Being freed from DRAM
    }


    switch(type){
        
        case 0: d2p_anon_count++;break;
        
        case 1: d2p_page_count++;break;

        case 2: p2d_anon_count++;break;

        case 3: p2d_page_count++;break;

        case 4: pram_flush_count++;break;

        case 5: dram_flush_count++;break;
        default: break;

    }


}
