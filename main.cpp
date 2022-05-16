#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
***TREŚĆ ZADANIA***

1. Utworzyć listę z danych jak w zadaniu p1, wczytywanych z pliku o nazwie podanej przez użytkownika. Do tworzenia
listy należy wykorzystać funkcję dołączania jednej osoby o jakichś danych na końcu listy zaczynającej się pod jakimś
adresem (parametrem funkcji dołączania jest tylko adres początku listy oraz imię, nazwisko oraz wiek osoby).

2. Napisać jedną funkcję, która
    a.  usuwa z listy wszystkie osoby o imionach zawierających jakąś literę i jednocześnie starsze od pierwszej
        osoby na liście.
    b.  wczytuje jedną nową osobę o jakichś danych oraz wstawia tę osobę przed co jakąś osobę na liście, która
        jest starsza niż ostatnia osoba na liście.
    c.  przed i po każdej z powyższych operacji drukuje z jakimś nagłówkiem zawartość listy zaczynającej się pod
        jakimś adresem, wykorzystując osobną funkcję „drukuj”.

3. Na końcu usunąć listę z pamięci.

*/

struct Tperson {

    string name;
    string surname;
    int age;

    Tperson *next;
};

void addPerson(Tperson *&address, string name, string surname, int age) {

    Tperson *current = new Tperson;

    current->name = name;
    current->surname = surname;
    current->age = age;

    if (address == NULL) {
        address = current;
        address->next = NULL;
    } else if (address->next == NULL) {
        address->next = current;
        current->next = NULL;
    } else {
        addPerson(address->next, name, surname, age);
    }
}

void printList(Tperson *address, string title) {

    cout << "-----" << title << "-----" << endl;

    while (address != NULL) {
        cout << address->name << " " << address->surname << ", " << address->age << endl;
        address = address->next;
    }

    for (int i=0; i < (title.length() + 10); i++) cout << "-";
    cout << endl << endl;
}

void foo(Tperson *&address, char letter, int every, string name, string surname, int age) {

    // operacje niemozliwe dla pustej listy

    if (address != NULL) {

        printList(address, "PRZED USUWANIEM");

        int headAge = address->age; // wiek pierwszej osoby na liscie

        Tperson *previous, *temporary, *current;
        previous = address;
        current = previous->next;

        while (current != NULL) {

            if ((current->age > headAge) && current->name.find(letter) != string::npos) {
                previous->next = current->next;
                temporary = current;
                current = current->next;
                delete temporary;
            } else {
                previous = current;
                current = current->next;
            }
        }
        printList(address, "PO USUWANIU");

        if (every < 1) {
            cout << "Nie mozna wykonac operacji dodawania, nie mozna wstawic czegos co zerowa osobe (lub co -1 itd)."<< endl;
        } else {
            printList(address, "PRZED DODAWANIEM");
            // wiek ostatniej osoby na liscie
            int tailAge;
            current = address;
            while (current->next != NULL) {
                current = current->next;
            }
            tailAge = current->age;

            current = address; // wracam na start listy
            previous = NULL;
            int counter = 0;
            while (current != NULL) {
                if (current->age > tailAge) {
                    counter++;
                    if (counter % every == 0) {
                        // nowa osoba
                        Tperson *newPerson;
                        newPerson = new Tperson;
                        newPerson->name = name;
                        newPerson->surname = surname;
                        newPerson->age = age;
                        // nowe polaczenie
                        if (previous == NULL) {
                            // aktualizacja glowy
                            address = newPerson;
                            address->next = current;
                        } else {
                            previous->next = newPerson;
                            newPerson->next = current;
                        }

                    }
                }
                // move on to the next element
                previous = current;
                current = current->next;
            }

            printList(address, "PO DODAWANIU");
        }

    } else {
        cout << "Lista jest pusta - operacje tej funkcji na pustej liscie nie sa mozliwe." << endl;
    }

}


int main()
{
    // 1
    // pytam o nazwe pliku i sprawdzam, czy mozna otworzyc

    ifstream inFile;
    string fName;

    cout << "Podaj nazwe pliku: ";
    getline(cin, fName);

    inFile.open(fName.c_str());

    if (!inFile.is_open()) {
        cout << "Blad otwarcia pliku.\nSprawdz, czy plik istnieje.\nSprawdz, czy podajesz prawidlowa nazwe pliku." << endl;
        return -1;
    }

    // odczytuje dane z pliku

    Tperson *head, *current, *previous;
    current = NULL;
    head = NULL;

    string name;
    string surname;
    int age;

    while (inFile.peek() != EOF) {

        inFile >> name;
        inFile >> surname;
        inFile >> age;

        addPerson(head, name, surname, age);
    }

    // 2
    foo(head, 'a', 2, "Scott", "Holmes", 30);

    //3
    while (head != NULL) {
        previous = head;
        head = head->next;
        delete previous;
    }

    inFile.close();

    return 0;
}
