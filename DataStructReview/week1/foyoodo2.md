### 循环队列

```c
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 20

typedef struct {
    char *base;
    int front;
    int rear;
} SqQueue;

SqQueue * InitQueue() {
    SqQueue *queue = (SqQueue*) malloc(sizeof(SqQueue));
    queue->base = calloc(MAXSIZE, sizeof(char));
    queue->front = 0;
    queue->rear = 0;
    return queue;
}

int GetLength(SqQueue *queue) {
    return (MAXSIZE + queue->rear - queue->front) % MAXSIZE;
}

void EnQueue(SqQueue *queue, char c) {
    if ((queue->rear + 1) % MAXSIZE == queue->front) return;
    queue->base[queue->rear] = c;
    queue->rear = (queue->rear + 1) % MAXSIZE;
}

char DeQueue(SqQueue *queue) {
    if (queue->front == queue->rear) return '0';
    queue->front = (queue->front + 1) % MAXSIZE;    // 更新首元素索引
    return queue->base[(queue->front + MAXSIZE - 1) % MAXSIZE];
}

void Print(SqQueue *queue) {
    for (int i = queue->front; i != queue->rear; i = (i + 1) % MAXSIZE)
        printf("%c ", queue->base[i]);
    printf("\n");
}

int main(int argc, char *argv[]) {
    SqQueue *queue = InitQueue();
    EnQueue(queue, 'a');
    EnQueue(queue, 'b');
    EnQueue(queue, 'c');
    DeQueue(queue);
    Print(queue);
    return 0;
}
```

### 广义表

```c
#include <stdio.h>
#include <stdlib.h>

typedef char AtomType;

typedef enum {ATOM, LIST} ElemTag;    // 0 表示原子，1 表示子表

typedef struct _GLNode {
    ElemTag tag;
    union {
        AtomType atom;
        struct {struct _GLNode *hp, *tp;} ptr;
    };
} GLNode;

GLNode *tail;    // tail 用来标志每个表的表尾，方便新加入“原子”结点
                 // 注意：每一个表都共用了 tail ，所以在下一个表创建之后不允许对上一个表进行添加原子的操作

GLNode * InitLists() {
    GLNode *lists = (GLNode*) malloc(sizeof(GLNode));
    lists->tag = 1;
    lists->ptr.hp = NULL;
    lists->ptr.tp = NULL;
    tail = NULL;
    return lists;
}

void InsertAtom(GLNode *lists, AtomType atom) {
    GLNode *node = (GLNode*) malloc(sizeof(GLNode));
    node->tag = 0;
    node->atom = atom;
    GLNode *list = (GLNode*) malloc(sizeof(GLNode));
    list->tag = 1;
    list->ptr.hp = node;
    list->ptr.tp = NULL;
    if (!lists->ptr.hp) lists->ptr.hp = list;
    else tail->ptr.tp = list;
    tail = list;
}

// 这里没有为了方便使用 tail ，因为我需要在表创建完成后进行一次插入子表的操作
void InsertList(GLNode *lists, GLNode *list) {
    if (!lists->ptr.hp) lists->ptr.hp = list;
    else {
        GLNode *p = lists->ptr.hp;
        while (p->ptr.tp) p = p->ptr.tp;
        p->ptr.tp = list;
    }
}

void Print(GLNode *lists) {
    GLNode *p = lists->ptr.hp;
    while (p) {
        if (p->ptr.hp->tag == 0) printf("%c ", p->ptr.hp->atom);
        else {
            printf("{ ");
            Print(p);
            printf("} ");
        }
        p = p->ptr.tp;
    }
}

int main(int argc, char *argv[]) {
    GLNode *lists = InitLists();
    InsertAtom(lists, 'a');
    InsertAtom(lists, 'b');
    GLNode *list = InitLists();
    InsertAtom(list, 'c');
    InsertAtom(list, 'd');
    GLNode *list2 = InitLists();
    InsertAtom(list2, 'e');
    InsertList(lists, list);
    InsertList(list, list2);
    
    Print(lists);
    return 0;
}
```