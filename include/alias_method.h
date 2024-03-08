/**
 *
 * @author heqi
 * @date 2024/03/07
 * @desc 这个文件主要是书写alias Method和alias sampling 以及alias structure等
 * */
#include <stdint.h>
#include <stdlib.h>
#include "uunit.h"
#include <stdio.h>
#include <openssl/bn.h>

/*------------------------常量----------------------*/

/*-----------------------结构定义------------------------*/
typedef struct BIN_unit{
    // 每个bin里面存储两个数据
    pair * x1;
    pair *x2;
}bin_unit;
typedef struct BIN{
    bin_unit ** units;
    int n;
    // 平均的值
    BIGNUM *avg_weight;
}bin;
// L1 和 L2使用链表表示
typedef struct List_node{
    pair * data;
    struct List_node * next;
}list_node;
typedef struct list{
    list_node * root;
    list_node * now;
}list;
/*------------------------方法定义----------------------*/
// 初始化一个bin
result alias_init_bin(data_set * set,bin * bins,int lef,int rig);
// 将一个data_set直接分割为L1和L2两部分
result alias_divide_data2_list(data_set * data,list * l1,list * l2,int lef,int rig);
// 将LIST数据填充到bin之中去
result alias_list_fill_bin(bin * bins,list * l1,list * l2);
// 销毁list数据结构
result alias_destroy_list_node(list_node * node);
// 别名采样方法，进行一个概率的采样
result alias_sampling(BIGNUM *res ,bin * anode);

