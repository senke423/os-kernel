//
// Created by sybilvane on 7/31/24.
//

#ifndef INC_41F_OS_PROJEKAT_LINKEDLIST_HPP
#define INC_41F_OS_PROJEKAT_LINKEDLIST_HPP

#include "../lib/hw.h"

template<typename Type>
class LinkedList {
    struct Element {
        Type data;
        Element* next;
        Element* previous;

        explicit Element(Type data){
            this->data = data;
            next = previous = nullptr;
        }

        Element(){
            next = previous = nullptr;
        }
    };

    Element* head;
    Element* tail;
    Element* cur;
    size_t len;

public:
    LinkedList(){
        head = tail = nullptr;
        len = 0;
        cur = nullptr;
    }

    void push(Type data);
    void cur_set_first();
    void cur_next();
    void remove_cur();
    bool add(Type data, size_t index);
    bool exists(Type data);
    Type remove(Type data);
    Type remove(size_t index);
    Type pop();
    Type get_cur();
    Type peek(size_t index);
    size_t getLen();

    void print();
};

template<typename Type>
void LinkedList<Type>::remove_cur() {
    remove(cur->data);
}

template<typename Type>
Type LinkedList<Type>::get_cur() {
    if (cur)
        return cur->data;
    return nullptr;
}

template<typename Type>
void LinkedList<Type>::cur_next() {
    if (cur)
        cur = cur->next;
}

template<typename Type>
void LinkedList<Type>::cur_set_first() {
    cur = head;
}

template<typename Type>
bool LinkedList<Type>::exists(Type data) {

    Element* iter = head;
    while (iter){
        if (iter->data == data)
            return true;
        iter = iter->next;
    }

    return false;
}

template<typename Type>
Type LinkedList<Type>::remove(Type data) {
    size_t index = 0;
    bool found = false;

    Element* iter = head;
    while (iter){
        if (iter->data == data){
            found = true;
            break;
        }
        index++;
        iter = iter->next;
    }

    if (!found)
        return nullptr;

    return LinkedList<Type>::remove(index);
}

template<typename Type>
size_t LinkedList<Type>::getLen() {
    return len;
}

template<typename Type>
Type LinkedList<Type>::remove(size_t index) {
    if (index >= len || !len)
        return nullptr;

    if (index == 0){
        Type ret = head->data;
        Element* tmp = head;
        head = head->next;

        delete tmp;
        return ret;
    }

    if (index == len - 1)
        return pop();

    Element* iter = head;
    for (size_t i = 0; i < index; i++)
        iter = iter->next;

    iter->previous->next = iter->next;
    iter->next->previous = iter->previous;
    Type ret = iter->data;

    delete iter;
    return ret;
}

template<typename Type>
Type LinkedList<Type>::pop() {
    if (!len)
        return nullptr;

    Element* iter = head;
    while (iter->next != nullptr){
        iter = iter->next;
    }

    if (iter->previous){
        iter->previous->next = nullptr;
    } else {
        head = nullptr;
    }

    Type ret = iter->data;
    delete iter;

    len--;
    return ret;
}

template<typename Type>
Type LinkedList<Type>::peek(size_t index) {
    if (index >= len)
        return nullptr;
    Element* curr = head;
    for (size_t i = 0; i < index; i++){
        curr = curr->next;
    }
    Type ret = curr->data;
    return ret;
}

template<typename Type>
void LinkedList<Type>::print() {

//    Element* curr = head;
//    while (head != nullptr){
//        putc(curr->data);
//        putc(' ');
//    }
}



template<typename Type>
bool LinkedList<Type>::add(Type data, size_t index) {
    if (index > len)
        return false;
    if (index == 0){
        Element* temp = new Element(data);

        if (len){
            head->previous = temp;
            temp->next = head;
        }
        head = temp;

        len++;
        return true;
    }
    if (index == len){
        push(data);
        return true;
    }
    Element* curr = head;
    for (size_t i = 0; i < index; i++){
        curr = curr->next;
    }
    Element* temp = new Element(data);
    curr->previous->next = temp;
    temp->previous = curr->previous;
    temp->next = curr;
    curr->previous = temp;

    len++;
    return true;
}

template<typename Type>
void LinkedList<Type>::push(Type data) {
    Element* temp = new Element(data);

    if (head == nullptr){
        head = tail = temp;
    } else {
        tail->next = temp;
        temp->previous = tail;
        tail = temp;
    }
    len++;
}


#endif //INC_41F_OS_PROJEKAT_LINKEDLIST_HPP
