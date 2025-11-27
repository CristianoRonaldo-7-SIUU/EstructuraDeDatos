//Lista Circular Doblemente Enlazada (Circular Doubly)
#ifndef CDOUBLY_H
#define CDOUBLY_H

#include <iostream>
#include <stdlib.h>

using namespace std;

namespace CDoubly {

    struct node {
        int data;
        struct node *next;
        struct node *prev;
    };

    struct node *head = NULL;

    void beginsert() {
        struct node *ptr, *last;
        int item;
        ptr = (struct node *)malloc(sizeof(struct node));
        if(ptr == NULL) {
            cout << "\nDESBORDAMIENTO";
        } else {
            cout << "\nValor: ";
            cin >> item;
            ptr->data = item;
            if(head == NULL) {
                head = ptr;
                ptr->next = head;
                ptr->prev = head;
            } else {
                last = head->prev;
                ptr->next = head;
                ptr->prev = last;
                last->next = ptr;
                head->prev = ptr;
                head = ptr;
            }
            cout << "\nNodo insertado";
        }
    }

    void lastinsert() {
        struct node *ptr, *last;
        int item;
        ptr = (struct node *)malloc(sizeof(struct node));
        if(ptr == NULL) {
            cout << "\nDESBORDAMIENTO";
        } else {
            cout << "\nValor: ";
            cin >> item;
            ptr->data = item;
            if(head == NULL) {
                head = ptr;
                ptr->next = head;
                ptr->prev = head;
            } else {
                last = head->prev;
                ptr->next = head;
                ptr->prev = last;
                last->next = ptr;
                head->prev = ptr;
            }
            cout << "\nNodo insertado";
        }
    }

    void randominsert() {
        struct node *ptr, *temp;
        int item, loc, i;
        ptr = (struct node *)malloc(sizeof(struct node));
        if(ptr == NULL) {
            cout << "\nDESBORDAMIENTO";
        } else {
            cout << "\nValor: ";
            cin >> item;
            ptr->data = item;
            cout << "\nInsertar despues de posicion: ";
            cin >> loc;
            temp = head;
            for(i=0; i<loc; i++) {
                temp = temp->next;
                if(temp == head) {
                    cout << "\nNo se puede insertar";
                    return;
                }
            }
            ptr->next = temp->next;
            ptr->prev = temp;
            temp->next->prev = ptr;
            temp->next = ptr;
            cout << "\nNodo insertado";
        }
    }

    void begin_delete() {
        struct node *temp;
        if(head == NULL) {
            cout << "\nLista vacia";
        } else if(head->next == head) {
            free(head);
            head = NULL;
            cout << "\nNodo eliminado";
        } else {
            temp = head;
            head->prev->next = head->next;
            head->next->prev = head->prev;
            head = head->next;
            free(temp);
            cout << "\nNodo eliminado del principio";
        }
    }

    void last_delete() {
        struct node *temp;
        if(head == NULL) {
            cout << "\nLista vacia";
        } else if(head->next == head) {
            free(head);
            head = NULL;
            cout << "\nNodo eliminado";
        } else {
            temp = head->prev;
            temp->prev->next = head;
            head->prev = temp->prev;
            free(temp);
            cout << "\nNodo eliminado del final";
        }
    }

    void random_delete() {
        struct node *ptr;
        int loc, i;
        cout << "\nPosicion a eliminar: ";
        cin >> loc;
        ptr = head;
        if(head == NULL) return;
        for(i=0; i<loc; i++) {
            ptr = ptr->next;
            if(ptr == head) {
                cout << "\nNo se puede eliminar";
                return;
            }
        }
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        if(ptr == head) head = ptr->next;
        free(ptr);
        cout << "\nNodo eliminado en posicion " << loc;
    }

    void search() {
        struct node *ptr;
        int item, i=0, flag=1;
        ptr = head;
        if(ptr == NULL) {
            cout << "\nLista vacia";
        } else {
            cout << "\nBuscar elemento: ";
            cin >> item;
            do {
                if(ptr->data == item) {
                    cout << "\nEncontrado en ubicacion " << i+1;
                    flag = 0;
                }
                i++;
                ptr = ptr->next;
            } while(ptr != head);
            if(flag) cout << "\nNo encontrado";
        }
    }

    void display() {
        struct node *ptr;
        ptr = head;
        if(head == NULL) {
            cout << "\nNada que imprimir";
        } else {
            cout << "\nImprimiendo valores . . . . .\n";
            do {
                cout << "\n" << ptr->data;
                ptr = ptr->next;
            } while(ptr != head);
        }
    }

    void runMenu() {
        int choice = 0;
        while(choice != 9) {
            cout << "\n\n*** MENU CDOUBLY ***\n";
            cout << "1.Insertar inicio\n2.Insertar final\n3.Insertar aleatorio\n4.Eliminar inicio\n5.Eliminar final\n6.Eliminar aleatorio\n7.Buscar\n8.Mostrar\n9.Volver\n";
            cout << "Opcion: ";
            cin >> choice;
            switch(choice) {
                case 1: beginsert(); break;
                case 2: lastinsert(); break;
                case 3: randominsert(); break;
                case 4: begin_delete(); break;
                case 5: last_delete(); break;
                case 6: random_delete(); break;
                case 7: search(); break;
                case 8: display(); break;
                case 9: break;
                default: cout << "Invalido\n";
            }
        }
    }
}
#endif
