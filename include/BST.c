#include "stdio.h"
#include "BST.h"

/*--------------------方法实现-------------------*/
// 初始化一个树
result bst_init_tree(bst_tree * tree,int n,data_set * set){
    tree->n = n;
    tree->root = (tree_node * ) malloc(sizeof (tree_node));
    // 初始化一个树的节点
    bst_init_node(tree->root);
    tree->total = set;
    return SUCCESS;
}
// 初始化一个树节点
result bst_init_node(tree_node * node){
    node->anode = NULL;
    node->right = NULL;
    node->left = NULL;
    node->is_left_node = FALSE;
    // 只有叶子节点需要，所以目前不会去主动的malloc值
    node->leftVal = NULL;
    node->lower = NULL;
    node->upper = NULL;
    node->sum_weight = NULL;
    node->vode = NULL;
    node->N = 0;
    return SUCCESS;
}

// 静态交换pair值的值
static void swap(pair ** arr,int i , int j){
    pair * p = arr[i];
    arr[i] = arr[j];
    arr[j ] = p;
}

// 构建一颗树,TODO 构建树的过程正确性还需验证
result bst_create_tree(tree_node * node,data_set * total,int lef,int rig){
    BIGNUM  * tmp = BN_CTX_get(CTX);
    // 如果现在是一个值的时候，我们就当做叶子节点进行构建了
    if(rig == lef){
        node->N = 1;
        node->is_left_node = TRUE;
        // TODo 目前对于所有的节点，我们是进行复制pair值，而没有直接引用
        node->leftVal = (pair *) malloc(sizeof(pair));
        copy_pair(node->leftVal,total->data[lef]);
    }
    node->N = rig - lef + 1;
    // 先求出vnode分割数据
    node->vode = BN_CTX_get(CTX);
    node->lower = BN_CTX_get(CTX);
    node->upper = BN_CTX_get(CTX);
    compute_avg_val(node->vode,total,lef,rig,node->lower,node->upper);
    // 构建anode的结构，以及获得权值总和
    node->anode = (bin *) malloc(sizeof (bin));

    // 初始化内容
    alias_init_bin(total,node->anode,lef,rig);
    // 创建两个链表
    list * l1 = (list * ) malloc(sizeof (list));
    list * l2 = (list * ) malloc(sizeof (list));
    alias_divide_data2_list(total,l1,l2,lef,rig);
    // list数据填充到bin
    alias_list_fill_bin(node->anode,l1,l2);

    BN_set_word(tmp,node->N);
    BN_mul(node->sum_weight,node->anode->avg_weight,tmp,CTX);

    int l = lef,r = rig;
    // 快排来分割重新排序
    while(l <= r){
        // 先找左侧的点
        while(l <= r && BN_cmp(total->data[l]->val,node->vode) != 1){
            l ++;
        }
        while(l <= r && BN_cmp(total->data[r]->val,node->vode) != -1){
            r--;
        }
        // 判断是否超过界限，同时进行交换数值
        if(l <= r){
            swap(total->data,l,r);
            l ++;
            r --;
        }
    }
    // 排序指针交换完毕
    node->left = (tree_node *) malloc(sizeof (tree_node));
    node->right = (tree_node *) malloc(sizeof (tree_node));
    // 初始化
    bst_init_node(node->left);
    bst_init_node(node->right);
    if(node->left == NULL || node->right == NULL){
        fprintf(stderr,"Memory allocation failed.\n");
        return ERROR;
    }
    // 进行递归查找
    bst_create_tree(node->left,total,lef,r);
    bst_create_tree(node->right,total,l,rig);

    BN_clear(tmp);
    return SUCCESS;
}
// 查询的具体算法
result bst_search(tree_node * node,BIGNUM * lef,BIGNUM *right,c_query * CQ){
    if(node == NULL){
        return ERROR;
    }
    if(node->is_left_node == TRUE){
        BIGNUM * val = node->leftVal->val;
        int ret1 = BN_cmp(val,lef);
        int ret2 = BN_cmp(val,right);
        // TODO 注意这里放入CQ我们没有复制放入
        if((ret1 == 0 || ret1 == 1) && (ret2 == 0 || ret2 == -1)){
            CQ->ele[CQ->idx++] = node;
        }
        return SUCCESS;
    }
    int ret1 = BN_cmp(node->lower,lef);
    int ret2 = BN_cmp(node->upper,right);
    if((ret1 == 1 || ret1 == 0) && (ret2 == 0 || ret2 == -1)){
        CQ->ele[CQ->idx++] = node;
        return SUCCESS;
    }
    // 按照vode进行划分
    ret1 = BN_cmp(node->vode,lef);
    ret2 = BN_cmp(node->vode,right);
    // TODO 这里的返回值进行了忽略
    if(ret1 == 1 || ret1 == 0){
        bst_search(node->left,lef,right,CQ);
    }
    if(ret2 == -1){
        bst_search(node->right,lef,right,CQ);
    }
    return SUCCESS;
}
// 求出CQ的加和，随机一个R值，然后计算满足不等式的i的值
result bst_middle_process(int * i , c_query *cq){
    // 目前的CQ里面存在叶子节点的数据，也存在内部节点的数据
    // 首先计算W的总和
    int n = cq->idx;
    BIGNUM * sum = BN_CTX_get(CTX);
    BN_set_word(sum,0);
    for(int i )
    BN_clear(sum);
}
result bst_init_CQ(c_query * cq){
    cq->idx = 0;
    cq->maxLength = CQ_MAX_LENGTH;
    cq->ele = (tree_node ** ) malloc(cq->maxLength * sizeof (tree_node *));
    if(cq->ele == NULL)
        return ERROR;
    return SUCCESS;
}
