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