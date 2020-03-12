### 静态链表

```c
#include <stdio.h>
#include <stdlib.h>

#define len 10

int head;

typedef struct {
    char data;
    int cur;
} Component, SLinkList[len];

// 初始化一个空的 List
SLinkList * InitSpace_SL() {
    SLinkList *list = calloc(len, sizeof(Component));
    for (int i = 0; i < len; ++i) {
        list[i]->cur = i + 1;
        list[i]->data = '$';
    }
    list[len - 1]->cur = 0;
    return list;
}

// 遍历检验
void OverLook(SLinkList *list) {
    for (int i = 0; i < len; ++i)
        printf("%c~%d  ", list[i]->data, list[i]->cur);
    printf("\n");
}

// 按链表中的索引顺序打印
void Print(SLinkList *list) {
    if (head == 0) return;
    int p = head;
    while (p != 0) {
        printf("%c~%d  ", list[p]->data, list[p]->cur);
        p = list[p]->cur;
    }
    printf("\n");
}

// 寻找可分配内存的下标地址
int Malloc_SL(SLinkList *list) {
    // 若备用链表为空，返回分配结点的下标，否则返回 0
    int i = list[0]->cur;
    if (i) list[0]->cur = list[i]->cur;
    // 此时头指针指向下一个分配结点，否则指向自身（头结点指针为 0 代表没有足够空间可以分配）
    return i;
}

// 将索引元素回收至备用链表
void Free_SL(SLinkList *list, int k) {
    // 将下标为 k 的空闲结点回收到备用链表（类似于头插法）
    list[k]->cur = list[0]->cur;
    list[k]->data = '$';
    list[0]->cur = k;
}

// 在静态链表中插入一个元素
void Insert_SL(SLinkList *list, int k, char data) {
    int i = Malloc_SL(list);
    if (i == 0) return;       // 当无可用空间供分配时，直接 return 回去
    list[i]->data = data;
    list[i]->cur = 0;         // 保证插在链尾的有穷性
    if (k == 0) {
        list[i]->cur = head;  // 因为 head 初始化为 0 ，所以插入首元素时不会影响链的有穷性
        head = i;
    } else if (k > 0) {       // 当插入位置不在链表头时，需要确定插入位置
        int p = head;
        for (int j = 0; j < k - 1; ++j) p = list[p]->cur;
        // 调整指针指向
        list[i]->cur = list[p]->cur;
        list[p]->cur = i;
    }
}

// 在静态链表中删掉一个元素
void Delete_SL(SLinkList *list, int k) {
    if (head == 0) return;  // 若链表为空，直接 return
    if (k == 0) {
        int temp = head;
        head = list[head]->cur;
        Free_SL(list, temp);
    } else if (k > 0) {
        int p = head;
        for (int i = 0; i < k - 1; ++i) {
            p = list[p]->cur;
            if (p == 0) return;  // 当 p 定位到链尾时，必越界
        }
        int temp = list[p]->cur;
        list[p]->cur = list[temp]->cur;
        Free_SL(list, temp);
    }
}

int main(int argc, char *argv[]) {
    SLinkList *list = InitSpace_SL();
    Insert_SL(list, 0, 'd');
    Insert_SL(list, 0, 'b');
    Delete_SL(list, 1);
    Insert_SL(list, 1, 'c');
    Insert_SL(list, 0, 'a');
    Delete_SL(list, 10);
    OverLook(list);
    Print(list);
}
```

### 一元多项式的运算

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float coef;  // 系数
    int expn;    // 指数
} Data;

typedef struct _Item {
    Data data;
    struct _Item *next;
} Item;

typedef struct {
    int n;
    Item *head, *tail;
} Polynomial;

// 将一组数据写入多项式
void WriteInPolyn(Polynomial *polyn, float coef, int expn) {
    if (coef == 0) return;  // 如果某一项系数为 0 ，则不添加到 res 中
    Item *item = (Item*) malloc(sizeof(Item));
    item->data.coef = coef;
    item->data.expn = expn;
    item->next = NULL;
    if (polyn->n == 0) {
        polyn->head = item;
        polyn->tail = item;
    } else {
        polyn->tail->next = item;
        polyn->tail = item;
    }
    polyn->n++;
}

// 初始化一个空的多项式
Polynomial * InitPolyn() {
    Polynomial *polyn = (Polynomial*) malloc(sizeof(Polynomial));
    polyn->n = 0;
    polyn->head = NULL;
    polyn->tail = NULL;
    return polyn;
}

// 从标准输入初始化一个 n 项式
Polynomial * InitPolynBy(int n) {
    Polynomial *polyn = InitPolyn();
    float coef;
    int expn;
    for (int i = 0; i < n; ++i) {
        scanf("%f %d", &coef, &expn);
        if (coef != 0) WriteInPolyn(polyn, coef, expn);
    }
    return polyn;
}

// 打印 n 项式的内容（取决于数据存储顺序，即输入顺序）
void OrderPolyn(Polynomial *polyn) {
    Item *p = polyn->head;
    while (p) {
        printf("%.1f", p->data.coef);
        if (p->data.expn != 0) {
            printf("x^%d", p->data.expn);
            if (p->next != NULL) printf("  ");
        } else if (p->next != NULL) {
            printf("  ");
        }
        p = p->next;
    }
    printf("\n");
}

// 将两个多项式相加
Polynomial * AddPolyn(Polynomial *x, Polynomial *y) {
    Polynomial *res = InitPolyn();
    Item *p = x->head;
    Item *q = y->head;
    while (p && q) {
        if (p->data.expn < q->data.expn) {
            WriteInPolyn(res, p->data.coef, p->data.expn);
            p = p->next;
        } else if (p->data.expn > q->data.expn) {
            WriteInPolyn(res, q->data.coef, q->data.expn);
            q = q->next;
        } else {
            WriteInPolyn(res, p->data.coef + q->data.coef, p->data.expn);
            p = p->next;
            q = q->next;
        }
    }
    while (p) {
        WriteInPolyn(res, p->data.coef, p->data.expn);
        p = p->next;
    }
    while (q) {
        WriteInPolyn(res, q->data.coef, q->data.expn);
        q = q->next;
    }
    return res;
}

// 将两个多项式相减
Polynomial * SubtractPolyn(Polynomial *x, Polynomial *y) {
    Polynomial *res = InitPolyn();
    Item *p = x->head;
    Item *q = y->head;
    while (p && q) {
        if (p->data.expn < q->data.expn) {
            WriteInPolyn(res, p->data.coef, p->data.expn);
            p = p->next;
        } else if (p->data.expn > q->data.expn) {
            WriteInPolyn(res, q->data.coef, q->data.expn);
            q = q->next;
        } else {
            WriteInPolyn(res, p->data.coef - q->data.coef, p->data.expn);
            p = p->next;
            q = q->next;
        }
    }
    while (p) {
        WriteInPolyn(res, p->data.coef, p->data.expn);
        p = p->next;
    }
    while (q) {
        WriteInPolyn(res, q->data.coef, q->data.expn);
        q = q->next;
    }
    return res;
}

// 供调用的方法，单项式乘多项式
Polynomial * MultiplyIP(Item *item, Polynomial *polyn) {
    Polynomial *res = InitPolyn();
    Item *p = polyn->head;
    while (p) {
        WriteInPolyn(res, item->data.coef * p->data.coef, item->data.expn + p->data.expn);
        p = p->next;
    }
    return res;
}

// 两多项式相乘
Polynomial * MultiplyPolyn(Polynomial *x, Polynomial *y) {
    Polynomial *res = InitPolyn();
    Item *p = x->head;
    Polynomial *temp;
    while (p) {
        temp = MultiplyIP(p, y);
        res = AddPolyn(temp, res);
        p = p->next;
    }
    return res;
}

int main(int argc, char *argv[]) {
    Polynomial *x = InitPolynBy(3);
    OrderPolyn(x);
    Polynomial *y = InitPolynBy(3);
    OrderPolyn(y);
    Polynomial *res = MultiplyPolyn(x, y);
    OrderPolyn(res);
    return 0;
}
```