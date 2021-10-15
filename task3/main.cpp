#include <iostream>
#include "SmartPointer.h"
using namespace std;

class A {
public:
	int a;
	A(int v) { a = v; }
};

int main() { // тестируем функциональность
	SmartPointer<int> p1; // объявляем пустой указатель
	cout << p1.ptr << " " << *p1.ref_count << endl; // и выводим значения его полей
	int* checking_pointer = new int[1]{ 1 }; // создаем простой указатель для проверки существования массива
	SmartPointer<int> p2 = checking_pointer; // создаем из простого умный указатель на тот же массив
	p1 = p2; // присваиваем одному УУ другой
	cout << p1.ptr << " " << *p1.ref_count << endl; // проверяем на равенство поля двух указателя на один объект
	cout << p2.ptr << " " << *p2.ref_count << endl; // и правильность счетчика ссылок = 2
	p2 = new int[1]{ 2 }; // переставляем один УУ
	SmartPointer<A> p3 = new A(1);
	cout << p1.ptr << " " << *p1.ref_count << endl; // проверяем, что у первого УУ счетчик ссылок стал 1
	cout << *p1 << endl; // проверяем операцию разыменования
	cout << p3->a << endl; // проверяем доступ к полям класса по стрелке
	cout << *checking_pointer << endl; // проверяем, что первый массив на месте
	p1 = new int[1]{ 3 }; // переставляем единственный указатель на первый массив
	cout << *checking_pointer << endl; // проверяем, что первого массива больше нет
	if (true) { // имитируем область видимости
		cout << "Entering the new scope" << endl;
		auto a2 = new A(2);
		p3 = a2; // инициализируем УУ объектом
		SmartPointer<int> p4 = checking_pointer = new int[1]{ 4 }; // создаем новый массив и УУ на него
		cout << *checking_pointer << endl; // чтобы проверить, что по выходу из области видимости он сам удалится
		cout << "Exiting the scope" << endl;
	}
	cout << *checking_pointer << endl; // массив и правда удалился
	cout << "Exiting main" << endl;
	return 0;
}
