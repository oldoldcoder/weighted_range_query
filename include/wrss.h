/**
 *
 * @author heqi
 * @date 2024/03/07
 * @desc 这个文件书写WRSS算法
 * */

#include "stdio.h"
#include "BST.h"


/*--------------------方法定义-------------------*/
//  通过给定的范围range以及按照我们的tree树，抽样得到一个res
result weight_range_set_sampling(BIGNUM * res , pair * range,bst_tree * tree);