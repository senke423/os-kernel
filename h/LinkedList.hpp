//
// Created by sybilvane on 7/31/24.
//

#ifndef INC_41F_OS_PROJEKAT_LINKEDLIST_HPP
#define INC_41F_OS_PROJEKAT_LINKEDLIST_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../lib/console.h"

template<typename Type>
class LinkedList {
    typedef struct Element {
        Type data;
        Element* next;
        Element* previous;

        Element(Type data){
            this->data = data;
            next = previous = nullptr;
        }

        Element(){
            next = previous = nullptr;
        }
    } Element;

    Element* head;
    Element* tail;
    size_t len;

public:
    LinkedList(){
        head = tail = nullptr;
        len = 0;
    }

    void push(Type data);
    bool add(Type data, size_t index);
    Element remove(size_t index);
    Element pop();
    Element* peek(size_t index);

    void print();
};

template<typename Type>
void LinkedList<Type>::print() {

    Element* curr = head;
    while (head != nullptr){
        __putc(curr->data);
        __putc(' ');
    }
}

template<typename Type>
typename LinkedList<Type>::Element *LinkedList<Type>::peek(size_t index) {
    if (index >= len)
        return nullptr;
    Element* temp = new Element();
    Element* curr = head;
    for (int i = 0; i < index; i++){
        curr = curr->next;
    }
    temp->data = curr->data;
    return temp;
}

template<typename Type>
bool LinkedList<Type>::add(Type data, size_t index) {
    if (index > len)
        return false;
    if (index == 0){
        Element* temp = new Element(data);
        head->previous = temp;
        temp->next = head;
        head = temp;
        len++;
        return true;
    }
    if (index == len){
        push(data);
        return true;
    }
    Element* curr = head;
    for (int i = 0; i < index; i++){
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
