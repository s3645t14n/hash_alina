#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <Windows.h>

using namespace std;

//������� ����� �����
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

//����� ������ �����
class Int_number : public virtual Number<int> {
public:
	Int_number *next; // ��� ������������� �������� �������� ����� �������� � ����������� ������. ����� ��������� ������������, ��� ������� ������, �� � �� ������� ��� ����������

	//����������� � ����������
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

class HashTable // ���-�������, �������������� � ���� ������� ��������� (������� � ���� ������� ������������ ������).
{
	Int_number** table;//������� �������
	int quan; //����� �������
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

	//�������� ���������� ��� ����� �����
	int hash(int a) {
		a = (a ^ 61) ^ (a >> 16);
		a = a + (a << 3);
		a = a ^ (a >> 4);
		a = a * 0x27d4eb2d;
		a = a ^ (a >> 15);
		return a % quan; //��-�� ����� % ������� ���������� ���� ����, ���� ����� ����� �� ����� �������
	}

	// ��������� ������� � �������
	void push(int value) {
		if (!find(value)) { //���� ������ �������� ��� (������� ����������)
			int hashNumber = hash(value); //�� ��������� ���
			Int_number* pers = new Int_number(value);
			Int_number* place = table[hashNumber]; //����� ����� � ���� ����� �� ������������ �������
			if (place == NULL) {
				table[hashNumber] = pers; //���� ��� ������, �������� ������� � ������ �����
				return;
			}
			while (place->next != NULL)
				place = place->next;
			place->next = pers; //���� �� ������, ����������� ������ �� ������� �����
		}
	}

	// ������� ������� �� ������� (� ����, ��� ������)
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
			}//��������� ����� �������� � �����
			cout << "n=" << n << endl;
			Int_number *buffer = table[hashNumber];
			int *temp = new int[n];//������� ��������� ��������� ��� ��������� ������� ���� ���������

			while (i < n) { //��������� �� �� �������� �������
				if (buffer->get_set_value() != value) {
					temp[i] = buffer->get_set_value();
					cout << temp[i] << " ";
					i++;
				}
				buffer = buffer->next;	
			}

			table[hashNumber] = NULL; //�������� ������ �����
			for (i = 0; i < n; i++) push(temp[i]); //�������� ������ �������� �������, ��������� ��������� �������
		}
	}

	// �������� ������� �� ������� �� ��� �����.
	Int_number* find(int value)
	{
		int hashNumber = hash(value); //���������� ���������� ����
		Int_number *result = table[hashNumber];
		if (!result) return NULL;
		while (result->get_set_value() != value) {
			if (!result->next)	
				return NULL;
			result = result->next;
		}
		return result;
	}

	void print() { //������ �������. ������� ��� ��������� ����� � ��� �� ��������
		Int_number *result;
		int ctr = 1;
		cout << endl << setw(4) << "���" << setw(10) << "�����" << setw(11) << "��������" << endl;
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

	void search(int v) { //����� ��������
		Int_number* search = find(v);
		if (search)
		{
			cout << endl << setw(4) << "���" << setw(10) << "�����" << endl;
			cout << "=========================================" << endl;
			cout << setw(3) << hash(v) << setw(10) << v << setw(10) << endl;
		}
		else {
			cout << "������� �� ������." << endl;
		};
	}

};

int Hashit(int a, int x) //���������� ���� (�������� �� ��������� � ������, ��������� �������)
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
	cout << "������� ����� �������: ";
	cin >> x;
	HashTable newTable(x);

	cout << "����������:\n1. �������������\n2. �������\n";
	cin >> select;
	for (i = 0; i < x; i++) {
		if (select == 1) newTable.push(rand() % 65536);
		else {
			cout << "������� ����� �" << i + 1 << ": ";
			cin >> v;
			newTable.push(v);
		};
	}

	newTable.print();

	do {
		cout << endl << "1. ����� ��������\n2. ���������� ��������\n3. �������� ��������\n4. ���������� �������\n0. �����\n";
		cin >> select;
		switch (select) {
		case 1:
			cout << endl << "������� ����� ��� ������: ";
			cin >> v;
			newTable.search(v);
			break;
		case 2:
			cout << endl << "������� ����� ��� ����������: ";
			cin >> v;
			newTable.push(v);
			newTable.print();
			break;
		case 3:
			cout << endl << "������� ����� ��� ��������: ";
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