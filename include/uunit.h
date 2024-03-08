/**
 *
 * @author heqi
 * @date 2024/03/07
 * @desc 这个文件是基本头文件，记录一些通用的数据结构以及基本的根据方法
 * */

/*-----------------------头文件------------------------*/
#include "stdio.h"
#include "openssl/bn.h"
/*------------------------常量----------------------*/
// 管理全局BIGNUM的上下文
#define TRUE 1
#define FALSE 0
#define SUCCESS 2346546
#define ERROR 1654567
#define result int
extern BN_CTX * CTX;
extern BIGNUM * ZERO;
extern BIGNUM * ONE;
/*-----------------------结构定义------------------------*/
typedef struct pair{
    BIGNUM * val;
    BIGNUM * weight;
}pair;

typedef struct data_set{
    pair ** data;
    // 代表数量
    int N;
    BIGNUM * avg_weight;
} data_set;


/*-----------------------方法定义------------------------*/
// 初始化一个data_set
result init_data_set(data_set * set,int n);
// copy pair数据结构
result copy_pair(pair * to,pair *from);
// 初始化常量
inline result init_constant();
// free常量
result free_constant();
// 计算权值平均值
result compute_avg_weight(BIGNUM *res, data_set * set,int lef,int rig);

// 计算值的平均值
result compute_avg_val(BIGNUM *res, data_set * set,int lef,int rig,BIGNUM * lower,BIGNUM * upper);