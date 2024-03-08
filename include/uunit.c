#include "uunit.h"
#include "openssl/bn.h"
#include <stdlib.h>
/*---------------------------------------------------------*/
BN_CTX * CTX;
BIGNUM * ZERO;
BIGNUM * ONE;
// 初始化常量
result init_constant(){
    CTX = BN_CTX_new();
    BN_CTX_start(CTX);
    ZERO = BN_CTX_get(CTX);
    ONE = BN_CTX_get(CTX);

    BN_set_word(ZERO,0);
    BN_set_word(ONE,1);
    return SUCCESS;
}
// 释放常量
result free_constant(){
    BN_free(ZERO);
    BN_free(ONE);
    BN_CTX_end(CTX);
    BN_CTX_free(CTX);
    return SUCCESS;
}


inline result copy_pair(pair * to,pair *from){
    BN_copy(to->val,from->val);
    BN_copy(to->weight,from->weight);
    return SUCCESS;
}
// 计算平均值,我们的平均值最后取的小值，然后在最后l1，l2往内部放的时候，我们应该去判断
result compute_avg_weight(BIGNUM *res,struct data_set * set){
    int n = set->N;
    BIGNUM * tmp = BN_CTX_get(CTX);
    BIGNUM * nn = BN_CTX_get(CTX);
    BN_set_word(nn,n);
    BN_set_word(tmp,0);
    for(int i = 0 ; i < n ; ++i){
        BN_add(tmp,tmp,set->data[i]->weight);
    }
    // 除法计算,给余数位置上了null值
    if(BN_div(res,NULL,tmp,nn,CTX) != 1)
        return ERROR;
    BN_clear(tmp);
    BN_clear(nn);
    return SUCCESS;
}
// 初始化data_set
result init_data_set(data_set * set,int n){
    set->avg_weight = BN_CTX_get(CTX);
    set->N = n;
    set->data = (pair **) malloc(n * sizeof (pair *));
    if(set->data == NULL)
        return ERROR;
    // 开辟pair空间
    for(int i = 0 ; i < n ; ++i){
        set->data[i] = (pair *) malloc(sizeof (pair));
        if(set->data[i] == NULL)
            return ERROR;
    }

    return SUCCESS;
}