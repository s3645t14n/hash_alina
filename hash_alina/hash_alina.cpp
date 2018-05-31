#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <Windows.h>

using namespace std;

//это базовый класс числа
template <class T> class Number {
protected:
	T value;
public:
	Number(T v) {
		value = v;
	}

	void get_set_value(T v) {
		value = v;
	}

	T get_set_value() {
		return value;
	}
};

//класс целого числа
class Int_number : public virtual Number<int> {
public:
	Int_number *next; // При возникновении коллизии элементы будут помещены в односвязный список. Здесь нарушение инкапсуляции, это спорный момент, но я не успеваю его переделать

	//конструктор с параметром
	Int_number(int v = 0) : Number<int>(v) {
		this->value = v;
		this->next = NULL;
	};

	~Int_number() {
		if (this->next != NULL) {
			delete this->next;
		}
	}
};

class HashTable // Хеш-таблица, представленная в виде массива элементов (которые в свою очередь представляют список).
{
	Int_number** table;//таблица записей
	int quan; //число записей
public:
	HashTable(int x) {
		quan = x;
		table = new Int_number* [quan];
		for (int i = 0; i < quan; i++) {
			table[i] = NULL;
		}
	}

	~HashTable() {
		for (int i = 0; i < quan; i++) {
			delete table[i];
		}
	}

	//неплохая хэшфункция для целых чисел
	int hash(int a) {
		a = (a ^ 61) ^ (a >> 16);
		a = a + (a << 3);
		a = a ^ (a >> 4);
		a = a * 0x27d4eb2d;
		a = a ^ (a >> 15);
		return a % quan; //из-за этого % функция становится чуть хуже, зато имеет смысл на нашей таблице
	}

	// Вставляет элемент в таблицу
	void push(int value) {
		if (!find(value)) { //если такого элемента нет (никаких дубликатов)
			int hashNumber = hash(value); //то вычисляем хэш
			Int_number* pers = new Int_number(value);
			Int_number* place = table[hashNumber]; //берем ветку с этим хэшем из существующей таблицы
			if (place == NULL) {
				table[hashNumber] = pers; //если она пустая, помещаем элемент в начало ветки
				return;
			}
			while (place->next != NULL)
				place = place->next;
			place->next = pers; //если не пустая, проматываем список до пустого места
		}
	}

	// Удаляет элемент из таблицы (я знаю, это ужасно)
	void pop(int value) {
		Int_number* search = find(value);
		if (search)
		{
			int n = 0, i = 0;
			
			int hashNumber = hash(value);
			Int_number *result = table[hashNumber];
			while (result->next) {
				n++;
				result = result->next;
			}//посчитали число коллизий в ветке
			cout << "n=" << n << endl;
			Int_number *buffer = table[hashNumber];
			int *temp = new int[n];//создали временное хранилище для элементов которые надо сохранить

			while (i < n) { //сохранили их во временно массиве
				if (buffer->get_set_value() != value) {
					temp[i] = buffer->get_set_value();
					cout << temp[i] << " ";
					i++;
				}
				buffer = buffer->next;	
			}

			table[hashNumber] = NULL; //обнуляем старую ветку
			for (i = 0; i < n; i++) push(temp[i]); //загоняем нужные коллизии обратно, пропуская удаленный элемент
		}
	}

	// Получает элемент из таблицы по его имени.
	Int_number* find(int value)
	{
		int hashNumber = hash(value); //аналогично добавлению выше
		Int_number *result = table[hashNumber];
		if (!result) return NULL;
		while (result->get_set_value() != value) {
			if (!result->next)	
				return NULL;
			result = result->next;
		}
		return result;
	}

	void print() { //печать таблицы. находит все ненулевые ветки и все их коллизии
		Int_number *result;
		int ctr = 1;
		cout << endl << setw(4) << "Хэш" << setw(10) << "Число" << setw(11) << "Коллизия" << endl;
		cout << "=========================================" << endl;
		for (int i = 0; i < quan; i++)
			if (table[i]) {
				result = table[i];
				cout << setw(4) << hash(result->get_set_value()) << setw(11) << result->get_set_value() << setw(10) << endl;
				while (result->next) {
					ctr++;
					result = result->next;
					cout << setw(4) << hash(result->get_set_value()) << setw(11) << result->get_set_value() << setw(10) << "." << ctr << endl;
				}
				ctr = 1;
			}
	}

	void search(int v) { //поиск элемента
		Int_number* search = find(v);
		if (search)
		{
			cout << endl << setw(4) << "Хэш" << setw(10) << "Число" << endl;
			cout << "=========================================" << endl;
			cout << setw(3) << hash(v) << setw(10) << v << setw(10) << endl;
		}
		else {
			cout << "Элемент не найден." << endl;
		};
	}

};

int Hashit(int a, int x) //вычисление хэша (вннешнее по отношению к классу, служебная функция)
{
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);
	return abs(a % x);
}

int main()
{
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	int x, i = 0, v, select;
	cout << "Введите число записей: ";
	cin >> x;
	HashTable newTable(x);

	cout << "Заполнение:\n1. Автоматически\n2. Вручную\n";
	cin >> select;
	for (i = 0; i < x; i++) {
		if (select == 1) newTable.push(rand() % 65536);
		else {
			cout << "Введите число №" << i + 1 << ": ";
			cin >> v;
			newTable.push(v);
		};
	}

	newTable.print();

	do {
		cout << endl << "1. Поиск элемента\n2. Добавление элемента\n3. Удаление элемента\n4. Отобразить таблицу\n0. Выход\n";
		cin >> select;
		switch (select) {
		case 1:
			cout << endl << "Введите число для поиска: ";
			cin >> v;
			newTable.search(v);
			break;
		case 2:
			cout << endl << "Введите число для добавления: ";
			cin >> v;
			newTable.push(v);
			newTable.print();
			break;
		case 3:
			cout << endl << "Введите число для удаления: ";
			cin >> v;
			newTable.pop(v);
			newTable.print();
			break;
		case 4:
			newTable.print();
			break;
		default:
			select = 0;
			break;
		}
	} while (select);
	return 0;
}