/**
 *
 * @author heqi
 * @date 2024/03/07
 * @desc 这个头文件主要定义的是我们构建的二叉查找树的内容，二叉查找树使用了alias structure结构来构建我们的树
 * */

// (vnode, [lnode, unode], Anode, w(Anode), left, right)
#include "stdio.h"
#include "openssl/bn.h"
#include "alias_method.h"
/*--------------------常量定义--------------------*/

/*--------------------结构定义-------------------*/
typedef struct tree_node{
    // vode是分割的值,分割的时候是用val而不是权值
    BIGNUM * vode;
    // 值的下界
    BIGNUM  * lower;
    // 值的上界
    BIGNUM * upper;
    // alias结构
    bin * anode;
    // 权值的总和
    BIGNUM * sum_weight;
    // 叶子节点的结构
    pair * leftVal;
    // 是否是叶子结点
    int is_left_node;
    // 左右节点
    struct tree_node * left;
    struct tree_node * right;
}tree_node;

// 树的定义
typedef struct bst_tree{
    tree_node *root;
    // 总节点的个数
    int n;
    // 指向总数据的指针
    data_set * total;
}bst_tree;

// CQ集合
typedef struct CQ{
    tree_node ** ele;
}c_query;
/*--------------------方法定义-------------------*/
// 初始化一个树
result bst_init_tree();
// 初始化一个树节点
result bst_init_node();
// 构建一颗树
result bst_create_tree(tree_node * node,data_set * total,int lef,int rig);
// 查询的具体算法
result bst_search(tree_node * node,BIGNUM * lef,BIGNUM *right,c_query * CQ);
// 求出CQ的加和，随机一个R值，然后计算满足不等式的i的值
result bst_middle_process(int * i , c_query *cq);