###  循环队列

```cpp
#include<iostream>
#include<assert.h>

using namespace std;

template<class T>
class loopQueue {
private:
    class Node {
    private:
        Node *next;
        T e;
    public:
        friend class loopQueue;

        Node() { next = NULL; }

        Node(T e) {
            next = NULL;
            this->e = e;
        }

        Node(T e, Node *next) {
            this->next = next;
            this->e = e;
        }

        ~Node() { next = NULL; }
    };

    Node *head;
    Node *tail;
    int length;//队列长度
public:
    loopQueue() {
        tail = new Node();
        head = new Node();
        length = 0;
    }

    ~loopQueue() {
        Node *cur = tail->next;
        for (int i = 0; i < length; i++) {
            Node *del = cur;
            cur = cur->next;
            del->next = NULL;
            delete del;
        }
    }

    bool isEmpty() {
        return length == 0;
    }

    int getSize() {
        return length;
    }

    //入队
    void push(T e) {
        if (isEmpty()) {
            tail->next = head->next = new Node(e);
            length++;
            return;
        }
        tail->next = new Node(e, tail->next);
        head->next->next = tail->next;
        length++;
    }

    //弹出队首元素并返回
    T pop() {
        assert(!isEmpty());
        T res = head->next->e;
        Node *prev = tail;
        for (int i = 0; i < length - 1; i++) {
            prev = prev->next;
        }
        Node *del = head->next;
        prev->next = tail->next->next;
        del->next = NULL;
        head->next = prev;
        delete del;
        length--;
        return res;
    }

    //返回队首元素
    T Front() {
        assert(!isEmpty());
        return head->next->e;
    }

    //返回队尾元素
    T Tail() {
        assert(!isEmpty());
        return tail->next->e;
    }

    void toString() {
        cout << "loopQueue [tail->";
        Node *cur = tail->next;
        for (int i = 0; i < length; i++) {
            cout << cur->e << "->";
            cur = cur->next;
        }
        cout << " head]" << endl;
    }
};
```



### 稀疏矩阵的压缩存储

#### （1）对称矩阵                                            ![1](Review-plan\_media\1.png)



![2](Review-plan\_media\2.png)

下三角存储：
1																 0 
2	4					-------->						  1	2
3	5	6													3	4	 5
	用一位数组存储，第一列下标为 0、1、3、6、10、15、21······ 
	可以看出第一列下标规律为 index = i * ( i - 1 ) / 2
	所以所有位置下标规律为： index = i * ( i - 1 ) / 2 + ( j - 1 )
	
上三角存储：
与下三角类似，下标规律为 index = j * ( j -  1 ) / 2 + ( i - 1 )




#### (2)稀疏矩阵  ![3](Review-plan\_media\3.png)



方式1：三元顺序表

* 只储存非零元素的行、列和值三个信息
```cpp
struct number{
	int i,j;//行、列
	int e;	//值
};

struct matrix{
	number arr[m];//m为非零元素个数
	int m,n;//矩阵行列数
};
```





方式2：行逻辑链接的顺序表

* 在三元顺序表储存非零元素的行、列和值的基础上，增加一个数组储存每行首个非零元素在一维数组中的存储位置，在遍历的时候可以跳过每行首个非零元素前面的位置，提高了遍历效率
```cpp
struct number{
	int i,j;//行、列
	int e;	//值
};

struct matrix{
	number arr[m];//m为非零元素个数
	int m,n;//矩阵行列数
	int* rpos;//储存每行首个非零元素在一维数组中的位置
};
```





方式3：十字链表

* 链表和数组结合，每行每列的非零元素都是以链表形式连接，通过两个数组，保存每行每列的首元素

  ![4](Review-plan\_media\4.png)

```cpp
struct Node {
    int i, j, e;//元素行、列和值
    struct Node *right, *down;//右指针 下指针
};

struct CrossList{
    Node* rhead,* chead;//每行首元素指针、每列首元素指针
    int row_n,col_n;//行、列数
    int size;//元素个数
};
```




### 广义表
```cpp
#include <iostream>
#include <string>
using namespace std;

typedef char T;

struct GLNode {
    int tag;//标志位  0：原子   1：表
    union {
        T atom;//原子值
        struct GLNode *head;
    };
    struct GLNode *tail;
};

//创建表 递归
void createList(GLNode *&list, string str) {
    list = new GLNode();
    list->tag = 1;
    list->head = NULL;
    list->tail = NULL;
    for (int i = 1; i < str.length(); i++) {
        if (str[i] == ')') {
            return;
        } else if (str[i] == '(') {//元素为广义表 递归创建
            int index = str.find(')', i + 1);
            GLNode *prev = list->head;
            if (!prev) {
                createList(list->head, str.substr(i, index - i + 1));
            } else {
                while (prev->tail != NULL) prev = prev->tail;
                createList(prev->tail, str.substr(i, index - i + 1));
            }
            i = index;
        } else if (str[i] >= 'a' && str[i] <= 'z') {
            GLNode *temp = list->head;
            if (temp == NULL) temp = list->head = new GLNode();//第一个元素
            else {//同一层次用tail相连
                while (temp->tail != NULL) temp = temp->tail;
                temp = temp->tail = new GLNode();
            }
            temp->tag = 0;
            temp->atom = str[i];
            temp->tail = NULL;
        }
    }
}

//销毁广义表 递归
void deleteList(GLNode *&list) {
    if (list == NULL) return;
    GLNode *cur = list->head;
    while (cur != NULL) {
        GLNode *temp = cur->tail;
        if (cur->tag == 0) {
            delete cur;
        } else {
            deleteList(cur);
        }
        cur = temp;
    }
    delete list;
}

//打印广义表 递归
void toString(GLNode *&list) {
    if (list == NULL) return;
    cout << "(";
    GLNode *temp = list->head;
    while (temp != NULL) {
        if (temp->tag == 0) {
            cout << temp->atom;
        } else {
            toString(temp);
        }
        temp = temp->tail;
        if (temp != NULL) cout << ",";
    }
    cout << ")";
}

//获取表头
GLNode *getHead(GLNode *&list) {
    if (!list) return NULL;
    return list->head;
}

//获取表尾
GLNode *getTail(GLNode *&list) {
    if (!list) return NULL;
    return list->head->tail;
}

//以表头表尾建立广义表
void merge_create(GLNode *&list, GLNode *&head, GLNode *&tail) {
    list = new GLNode();
    list->tag = 1;
    list->head = head;
    list->tail = NULL;
    list->head->tail = tail;
}

//求广义表深度
int getDepth(GLNode *&list) {
    if (!list) return 1;//空表
    if (list->tag == 0) return 0;
    int max_depth = 1;
    GLNode *temp = list->head;
    while (temp) {
        if (temp->tag == 1) {
            int temp_depth = 1 + getDepth(temp);
            if (temp_depth > max_depth) max_depth = temp_depth;
        }
        temp = temp->tail;
    }
    return max_depth;
}

//复制广义表
void copyGlist(GLNode *&list, GLNode *&new_list) {
    if (!list) {
        new_list = NULL;
        return;
    }
    new_list = new GLNode();
    new_list->tag = list->tag;
    if (list->tag == 0) {
        new_list->atom = list->atom;
    } else {
        copyGlist(list->head, new_list->head);
    }
    copyGlist(list->tail, new_list->tail);
}
```
