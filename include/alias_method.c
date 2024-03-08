#include "alias_method.h"
#include "stdio.h"

// 初始化一个bin
result alias_init_bin(data_set * set,bin * bins,int lef,int rig){
    int n = rig - lef + 1;
    bins->n = n;
    bins->avg_weight = BN_CTX_get(CTX);
    // 计算平均值
    if(compute_avg_weight(bins->avg_weight,set,lef,rig) != SUCCESS)
        return ERROR;
    // 开辟bin的空间
    bins->units = (bin_unit **) malloc(n * sizeof (bin_unit));
    if(bins->units == NULL)
        return ERROR;
    for(int i = 0 ; i < n ; ++i){
        bins->units[i] = (bin_unit *) malloc(sizeof (bin_unit));
        if(bins->units[i] == NULL)
            return ERROR;
        // 为bin里面的pair初始化
        bins->units[i]->x1->val = BN_CTX_get(CTX);

        bins->units[i]->x1->weight = BN_CTX_get(CTX);

        bins->units[i]->x2->val = BN_CTX_get(CTX);

        bins->units[i]->x2->weight = BN_CTX_get(CTX);
    }

    return SUCCESS;

}
// 将一个data_set直接分割为L1和L2两部分
result alias_divide_data2_list(data_set * set,list * l1,list * l2,int lef,int rig){
    // 按照平均值分
    BIGNUM  *avg = set->avg_weight;
    int n = rig - lef + 1;
    list * tmp;
    for(int i = lef ; i <= rig ; ++i){
        list_node * next = (list_node * ) malloc(sizeof (list_node));
        if(next == NULL)
            return ERROR;
        next->data = (pair *) malloc(sizeof (pair));
        if(next->data == NULL)
            return ERROR;
        next->next = NULL;
        next->data->val = BN_CTX_get(CTX);
        next->data->weight = BN_CTX_get(CTX);

        // 复制过去值
        copy_pair(next->data,set->data[i]);
        if(set->data[i]->weight <= avg){
            tmp = l1;
        }else{
            tmp = l2;
        }
        if(tmp->root == NULL){
            tmp->root = tmp->now = next;
        }else{
            tmp->now->next = next;
            tmp->now = next;
        }
    }
    return SUCCESS;
}
// 将LIST数据填充到bin之中去
result alias_list_fill_bin(bin * bins,list * l1,list * l2){
    // 大小插入
    int n = bins->n;
    BIGNUM  * avg = bins->avg_weight;
    BIGNUM *tmp = BN_new();
    int res;
    list_node * x1;
    list_node * x2;
    for(int i = 0 ; i < n ; ++i){
        x1 = l1->root;
        x2 = l2->root;
        // 传递x1的值
        bins->units[i]->x1 = x1->data;
        // TODO 由于我们将平均值向下取整了一部分，所以这里后面是绝对会出问题的，目前暂时不管逻辑
        // if(BN_cmp(x2->data->weight,avg) == -1)
        BN_sub(tmp,avg,x1->data->weight);

        BN_sub(x2->data->weight,x2->data->weight,tmp);

        pair * new = (pair * ) malloc(sizeof (pair));
        new->val = BN_CTX_get(CTX);
        new->weight = BN_CTX_get(CTX);

        BN_copy(new->val,x1->data->val);
        BN_copy(new->val,tmp);
        bins->units[i]->x2 = new;

        list_node * next = (list_node *) malloc(sizeof (list_node));
        next->next = NULL;
        next->data = x2->data;
        // 判断大小加入链表的尾部
        if((res = BN_cmp(x2->data->weight,avg) )== 0 || res == -1){
            l1->now->next = next;
            l1->now = l1->now->next;
        }else{
            l2->now->next = next;
            l2->now = l2->now->next;
        }

        // 销毁这两个节点
        free(x1);
        free(x2);
        // 跳到下一个
        l1->root = l1->root->next;
        l2->root = l2->root->next;
    }

    BN_free(tmp);

    return SUCCESS;

}
// 销毁list数据结构
result alias_destroy_list_node(list_node * node){
    BN_clear(node->data->val);
    BN_clear(node->data->weight);

    free(node->data);
    node->next = NULL;
    free(node);
    return SUCCESS;
}
// 别名采样方法，进行一个概率的采样
result alias_sampling(BIGNUM *res ,bin * anode){
    int n = anode->n;
    BIGNUM * r = BN_CTX_get(CTX);
    // 获取一个在[0,n - 1]
    int randomN = rand() % n;
    BIGNUM  * tt = BN_CTX_get(CTX);
    BN_add(tt,anode->avg_weight,ONE);
    BN_rand_range(r,tt);
    // 得到[0,avg)
    BN_sub_word(r,1);
    if(BN_cmp(anode->units[randomN]->x1->weight,r) == 1)
        BN_copy(res,anode->units[randomN]->x1->val);
    else
        BN_copy(res,anode->units[randomN]->x2->val);
    BN_clear(r);
    BN_clear(tt);

    return SUCCESS;
}

