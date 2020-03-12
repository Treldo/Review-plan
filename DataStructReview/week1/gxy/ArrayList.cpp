/**
	顺序表 数组实现
*/
#include <iostream>
#include <assert.h>

using namespace std;

template<class T>
class ArrayList {
private:
    T *arr;
    int size;//容量
    int length;//当前元素个数
public:
    ArrayList() {
        arr = new T[10];
        size = 10;
        length = 0;
    }

    ArrayList(int Capacity) {
        arr = new T[Capacity];
        size = Capacity;
        length = 0;
    }

    ~ArrayList() {
        delete[] arr;
    }

    //清空数组元素
    void ClearList(){
        delete[] arr;
        length=0;
        size=10;
        arr=new int[size];
    }

    //判断数组是否为空
    bool isEmpty() {
        return length == 0;
    }

    //返回数组当前元素个数
    int getSize() {
        return length;
    }

    //扩容函数
    void resize();

    //在下标为index处添加元素e
    void insert(int index, T e);

    //在数组末尾添加元素e
    void addLast(T e);

    //删除下标为index的元素,并返回
    T pop(int index);

    //删除第一个值为e的元素
    void remove(T e);

    //删除所有值为e的元素
    void removeAll(T e);

    //修改下标为index的元素的值
    void set(int index, T e);

    //查询下标为index的元素
    T get(int index);

    //返回第一个值为e的元素的下标
    int findIndex(T e);

    //返回所有值为e的元素的下标 len为返回数组的长度
    int *findAllIndex(T e, int &len);

    //逆置顺序表
    void reverse();

    //对顺序表排序 默认缺省值为true：升序排序   false：降序排序
    void sort(bool flag = true);

    //打印顺序表
    void toString() {
        for (int i = 0; i < length; i++) {
            cout << arr[i] << " ";
            if (i % 5 == 0 && i != 0) cout << endl;
        }
    }
};

template<typename T>
void ArrayList<T>::resize() {
    T *temp = new int[size];//new分配失败时，不会返回空指针，而是抛出异常
    for (int i = 0; i < size; i++) temp[i] = arr[i];
    delete[] arr;
    size = size * 2;
    arr = new int[size];
    for (int i = 0; i < size; i++) arr[i] = temp[i];
    delete[] temp;
}

template<typename T>
void ArrayList<T>::insert(int index, T e) {
    //容量已满 扩容
    if (length == size) resize();
    assert(index >= 0);
    assert(index <= length);
    //从index开始往后挪
    for (int i = length - 1; i >= index; i--) arr[i + 1] = arr[i];
    arr[index] = e;
    length++;
}

template<typename T>
void ArrayList<T>::addLast(T e) {
    insert(length, e);
}

template<typename T>
T ArrayList<T>::pop(int index) {
    assert(index >= 0);
    assert(index < length);
    T res = arr[index];
    for (int i = index; i < length - 1; i++) {
        arr[i] = arr[i + 1];
    }
    length--;
    return res;
}

template<typename T>
void ArrayList<T>::remove(T e) {
    for (int i = 0; i < length; i++) {
        if (e == arr[i]) {
            pop(i);
            return;
        }
    }
}

template<typename T>
void ArrayList<T>::removeAll(T e) {
    for (int i = length - 1; i >= 0; i++) {
        if (e == arr[i]) pop(i);
    }
}

template<typename T>
void ArrayList<T>::set(int index, T e) {
    assert(index >= 0);
    assert(index < length);
    arr[index] = e;
}

template<typename T>
T ArrayList<T>::get(int index) {
    assert(index >= 0);
    assert(index < length);
    return arr[index];
}

template<typename T>
int ArrayList<T>::findIndex(T e) {
    for (int i = 0; i < length; i++) {
        if (e == arr[i]) return i;
    }
    return -1;
}

template<typename T>
int *ArrayList<T>::findAllIndex(T e, int &len) {
    int n = 0;
    for (int i = 0; i < length; i++) {
        if (e == arr[i]) n++;
    }
    if (n == 0) return NULL;
    int *res = new int[n];
    len = n;
    n = 0;
    for (int i = 0; i < length; i++) {
        if (e == arr[i]) {
            res[n++] = i;
            cout << i;
        }
    }
    return res;
}

template<typename T>
void ArrayList<T>::reverse() {
    for (int i = 0; i < length / 2; i++) {
        swap(arr[i], arr[length - 1 - i]);
    }
}

template<typename T>
void ArrayList<T>::sort(bool flag) {
    if (isEmpty()) return;
    for (int i = 0; i < length - 1; i++) {
        for (int k = 0; k < length - 1 - i; k++) {
            if (arr[k] > arr[k + 1]) swap(arr[k], arr[k + 1]);
        }
    }
    if (!flag) reverse();
}

int main() {
    ArrayList<int> *list = new ArrayList<int>(1);
    list->insert(0, 7);
    list->insert(1, 8);
    list->insert(1, 4);
    list->insert(1, 3);
    list->insert(1, 6);
    list->insert(1, 0);
    list->insert(1, 2);
    list->toString();
    list->sort();
    list->toString();
    delete list;
    return 0;
}