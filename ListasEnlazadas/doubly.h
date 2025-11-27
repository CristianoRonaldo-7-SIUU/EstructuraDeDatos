//Lista Doblemente Enlazada (Doubly)
#ifndef DOUBLY_H
#define DOUBLY_H

#include <iostream>
#include <stdlib.h>

using namespace std;

namespace Doubly {

    struct node {
        int data;
        struct node *next;
        struct node *prev;
    };

    struct node *head = NULL;

    void beginsert() {
        struct node *ptr;
        int item;
        ptr = (struct node *)malloc(sizeof(struct node));
        if(ptr == NULL) {
            cout << "\nDESBORDAMIENTO";
        } else {
            cout << "\nValor: ";
            cin >> item;
            ptr->data = item;
            ptr->prev = NULL;
            ptr->next = head;
            if(head != NULL) {
                head->prev = ptr;
            }
            head = ptr;
            cout << "\nNodo insertado";
        }
    }

    void lastinsert() {
        struct node *ptr, *temp;
        int item;
        ptr = (struct node *)malloc(sizeof(struct node));
        if(ptr == NULL) {
            cout << "\nDESBORDAMIENTO";
        } else {
            cout << "\nValor: ";
            cin >> item;
            ptr->data = item;
            ptr->next = NULL;
            if(head == NULL) {
                ptr->prev = NULL;
                head = ptr;
            } else {
                temp = head;
                while(temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = ptr;
                ptr->prev = temp;
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
                if(temp == NULL) {
                    cout << "\nNo se puede insertar\n";
                    return;
                }
            }
            ptr->next = temp->next;
            ptr->prev = temp;
            if(temp->next != NULL) {
                temp->next->prev = ptr;
            }
            temp->next = ptr;
            cout << "\nNodo insertado";
        }
    }

    void begin_delete() {
        struct node *ptr;
        if(head == NULL) {
            cout << "\nLista vacia\n";
        } else {
            ptr = head;
            head = ptr->next;
            if(head != NULL) {
                head->prev = NULL;
            }
            free(ptr);
            cout << "\nNodo eliminado del principio\n";
        }
    }

    void last_delete() {
        struct node *ptr;
        if(head == NULL) {
            cout << "\nLista vacia\n";
        } else if(head->next == NULL) {
            free(head);
            head = NULL;
            cout << "\nSolo un nodo eliminado\n";
        } else {
            ptr = head;
            while(ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->prev->next = NULL;
            free(ptr);
            cout << "\nNodo eliminado del final\n";
        }
    }

    void random_delete() {
        struct node *ptr, *temp;
        int loc, i;
        cout << "\nPosicion a eliminar: ";
        cin >> loc;
        ptr = head;
        for(i=0; i<loc; i++) {
            temp = ptr;
            ptr = ptr->next;
            if(ptr == NULL) {
                cout << "\nNo se puede eliminar";
                return;
            }
        }
        if(ptr->next != NULL) {
            ptr->next->prev = ptr->prev;
        }
        if(ptr->prev != NULL) {
            ptr->prev->next = ptr->next;
        } else {
            head = ptr->next;
        }
        free(ptr);
        cout << "\nNodo eliminado en posicion " << loc;
    }

    void search() {
        struct node *ptr;
        int item, i=0, flag=1;
        ptr = head;
        if(ptr == NULL) {
            cout << "\nLista vacia\n";
        } else {
            cout << "\nBuscar elemento: ";
            cin >> item;
            while(ptr != NULL) {
                if(ptr->data == item) {
                    cout << "\nEncontrado en ubicacion " << i+1;
                    flag = 0;
                }
                i++;
                ptr = ptr->next;
            }
            if(flag == 1) cout << "\nNo encontrado\n";
        }
    }

    void display() {
        struct node *ptr;
        ptr = head;
        if(ptr == NULL) {
            cout << "\nNada que imprimir";
        } else {
            cout << "\nImprimiendo valores . . . . .\n";
            while(ptr != NULL) {
                cout << "\n" << ptr->data;
                ptr = ptr->next;
            }
        }
    }

    void runMenu() {
        int choice = 0;
        while(choice != 9) {
            cout << "\n\n*** MENU DOUBLY ***\n";
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
