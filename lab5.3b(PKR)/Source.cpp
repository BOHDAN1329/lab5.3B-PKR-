#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

enum Spec { CS, IT, ME, PhIT, TR };
string strSpec[] = { "����'�����i �����","I����������","���������� �� ������i��"," �i���� �� i����������","������� ��������" };
struct Student
{
	char lname[20];
	int kurs;
	Spec spec;
	int physics;
	int math;
	union {
		int progr;
		int numb;
		int ped;
	};
};

void Create(char* fname)
{
	ofstream file(fname, ios::binary);
	if (!file)
	{
		cerr << "�� ������� ��������! \"" << fname << "\"" << endl << endl;
		return;
	}

	cout << "������ ��������! \"" << fname << "\"" << endl << endl;
}

void fWrite(fstream& f, const size_t i, const Student x)
{
	f.seekp(i * (long)sizeof(Student));
	f.write((char*)&x, sizeof(Student));
}

void Add(char* fname)
{
	ofstream file(fname, ios::binary | ios::_Nocreate);
	if (!file)
	{
		cerr << "Could not create \"" << fname << "\".\n\n";
		return;
	}

	int speciality;
	int i = 0;
	Student worker;
	char choice;
	do
	{
		cout << endl;
		cout << "�������: "; cin.sync();
		cin >> worker.lname;
		cout << "����: "; cin >> worker.kurs;
		cout << "����i����i��� ([0] - ����'�����i �����,[1] -I����������, [2] - ���������� �� ������i��, [3] - �i���� �� i����������, [4] - ������� ��������): ";
		cin >> speciality;
		worker.spec = static_cast<Spec>(speciality);
		cout << "��i��� � �i����: ";
		cin >> worker.physics;
		cout << "��i��� � ����������: ";
		cin >> worker.math;
		if (worker.spec == 0) {
			cout << "��i��� � �������������: ";
			cin >> worker.progr;
		}
		else if (worker.spec == 1) {
			cout << "��i��� � ��������� �����i�: ";
			cin >> worker.numb;
		}
		else {
			cout << "��i��� � �������i��: ";
			cin >> worker.ped;
		}
		if (!file.write((char*)&worker, sizeof(Student)))
		{
			cerr << "������� �������� �����." << endl;
		}
		cout << "����������? (�/�) "; cin >> choice;
	} while (choice == '�' || choice == '�');
}

void Print(char* fname)
{
	int i = 0;
	ifstream f(fname, ios::binary);
	if (!f)
	{
		cerr << "������� �������� ����� '" << fname << "'" << endl;
		return;
	}

	cout <<
		"=========================================================================================================================="
		<< endl;
	cout << "| � |   ��i�����    | ���� |      ����i����i���     | �i���� | ���������� | ������������� | �������i ������ | �������i�� |"
		<< endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------"
		<< endl;

	Student worker;
	while (f.read((char*)&worker, sizeof(Student)))
	{
		cout << "| " << setw(2) << right << i + 1 << "";
		cout << "| " << setw(14) << left << worker.lname
			<< "| " << setw(5) << right << worker.kurs
			<< "| " << setw(23) << left << strSpec[worker.spec]
			<< "| " << setw(7) << worker.physics
			<< "| " << setw(11) << right << worker.math;
		if (worker.spec == 0) {
			cout << "| " << setw(14) << right << worker.progr
				<< "| " << setw(16) << right << " "
				<< "| " << setw(11) << right << " " << "| ";
		}
		else if (worker.spec == 1) {
			cout << "| " << setw(14) << right << " "
				<< "| " << setw(16) << right << worker.numb
				<< "| " << setw(11) << right << " " << "| ";
		}
		else {
			cout << "| " << setw(14) << right << " "
				<< "| " << setw(16) << right << " "
				<< "| " << setw(11) << right << worker.ped << "| ";
		}
		cout << endl;
		++i;
	}
	cout << "=========================================================================================================================="
		<< endl << endl << endl;
}

Student fRead(fstream& f, const size_t i)
{
	Student x;
	f.seekg(i * (long)sizeof(Student));
	f.read((char*)&x, sizeof(Student));
	return x;
}

size_t Size(char* fname)
{
	size_t count = 0;
	ifstream file(fname, ios::binary | ios::in);
	if (!file)
		return 0;

	Student list;
	while (file.read(reinterpret_cast<char*>(&list), sizeof(list)))
		count++;

	return count;
}

double avg(Student s)
{
	int a;
	if (s.spec == 0)
	{
		a = s.progr;
	}
	else if (s.spec == 1)
	{
		a = s.numb;
	}
	else
	{
		a = s.ped;
	}
	double k = s.math + s.physics + a;
	return k / 3;
}

void Sort(char* fname)
{
	Student tmp;
	size_t size = Size(fname);

	fstream file(fname, ios::binary | ios::_Nocreate | ios::out | ios::in);
	if (!file)
	{
		cerr << "������� �������� ����� '" << fname << "'" << endl;
		return;
	}

	for (int i0 = 0; i0 < size - 1; i0++)
	{
		for (int i1 = 0; i1 < size - i0 - 1; i1++)
		{
			if ((avg(fRead(file, i1)) > avg(fRead(file, i1 + 1))) ||
				(avg(fRead(file, i1)) == avg(fRead(file, i1 + 1)) &&
					string(fRead(file, i1).lname) < string(fRead(file, i1 + 1).lname)) ||
				(avg(fRead(file, i1)) == avg(fRead(file, i1 + 1)) &&
					string(fRead(file, i1).lname) == string(fRead(file, i1 + 1).lname) &&
					fRead(file, i1).kurs > fRead(file, i1 + 1).kurs))
			{
				tmp = fRead(file, i1);
				fWrite(file, i1, fRead(file, i1 + 1));
				fWrite(file, i1 + 1, tmp);
			};
		}
	}
}

int fReadi(fstream& f, const size_t i)
{
	int x;
	f.seekg(i * (long)sizeof(int));
	f.read((char*)&x, sizeof(int));
	return x;
}

void fWritei(fstream& f, const size_t i, const int x)
{
	f.seekp(i * (long)sizeof(int));
	f.write((char*)&x, sizeof(int));
}

void IndexSort(char* fname)
{
	size_t size = Size(fname);

	fstream srce(fname, ios::binary | ios::_Nocreate | ios::in);
	if (!srce)
	{
		cerr << "Could not open \"" << fname << "\".\n\n";
		return;
	}

	fstream dest(string(fname) + "index", ios::binary | ios::in | ios::out | ios::trunc);


	for (int i = 0; i < size; i++)
		fWritei(dest, i, i);
	int i, j, value;
	for (i = 1; i < size; i++)
	{
		value = fReadi(dest, i);
		for (j = i - 1;
			j >= 0 && ((avg(fRead(srce, fReadi(dest, j))) > avg(fRead(srce, value))) ||
				(avg(fRead(srce, fReadi(dest, j))) == avg(fRead(srce, value)) &&
					string(fRead(srce, fReadi(dest, j)).lname) < string(fRead(srce, value).lname)) ||
				(avg(fRead(srce, fReadi(dest, j))) == avg(fRead(srce, value)) &&
					string(fRead(srce, fReadi(dest, j)).lname) == string(fRead(srce, value).lname) &&
					fRead(srce, fReadi(dest, j)).kurs > fRead(srce, value).kurs));
			j--)
		{
			fWritei(dest, j + 1, fReadi(dest, j));
		}
		fWritei(dest, j + 1, value);
	}
}

void IndexDisplay(char* filename, size_t size = 0, size_t k = 0)
{
	fstream file(filename, ios::binary | ios::in);
	if (!file)
	{
		cerr << "Could not open \"" << filename << "\".\n\n";
		return;
	}

	fstream index(string(filename) + "index", ios::binary | ios::in);
	if (!index)
	{
		cerr << "Could not open \"" << "index" << "\".\n\n";
		return;
	}

	cout << "=========================================================================================================================="
		<< endl;
	cout << "| � |   ��i�����    | ���� |      ����i����i���     | �i���� | ���������� | ������������� | �������i ������ | �������i�� |"
		<< endl;
	cout << "--------------------------------------------------------------------------------------------------------------------------"
		<< endl;

	if (size == 0)
		size = Size(filename);

	if (size != 0)
		for (size_t i = k; i < size; i++)
		{
			cout << "| " << setw(2) << right << i + 1 << "";
			cout << "| " << setw(14) << left << fRead(file, fReadi(index, i)).lname
				<< "| " << setw(5) << right << fRead(file, fReadi(index, i)).kurs
				<< "| " << setw(23) << left << strSpec[fRead(file, fReadi(index, i)).spec]
				<< "| " << setw(7) << fRead(file, fReadi(index, i)).physics
				<< "| " << setw(11) << right << fRead(file, fReadi(index, i)).math;
			if (fRead(file, fReadi(index, i)).spec == 0) {
				cout << "| " << setw(14) << right << fRead(file, fReadi(index, i)).progr
					<< "| " << setw(16) << right << " "
					<< "| " << setw(11) << right << " " << "| ";
			}
			else if (fRead(file, fReadi(index, i)).spec == 1) {
				cout << "| " << setw(14) << right << " "
					<< "| " << setw(16) << right << fRead(file, fReadi(index, i)).numb
					<< "| " << setw(11) << right << " " << "| ";
			}
			else {
				cout << "| " << setw(14) << right << " "
					<< "| " << setw(16) << right << " "
					<< "| " << setw(11) << right << fRead(file, fReadi(index, i)).ped << "| ";
			}
			cout << endl;;
		}
	cout << "--------------------------------------------------------------------------------------------------------------------------"
		<< endl << endl << endl;
}

int BinSearch(char* filename, char* surname, int k, const unsigned grade)
{
	fstream file(filename, ios::binary | ios::in);
	if (!file)
	{
		cerr << "������� �������� ����� \"" << filename << "\"!.\n\n";
		return -1;
	}

	size_t size = Size(filename);
	int L = 0, R = size - 1, m;
	do {
		m = (L + R) / 2;
		if (string(fRead(file, m).lname) == surname && fRead(file, m).kurs == k &&
			avg(fRead(file, m)) == grade)
			return m;
		if ((string(fRead(file, m).lname) < surname)
			||
			(string(fRead(file, m).lname) == surname &&
				fRead(file, m).kurs > k) || (string(fRead(file, m).lname) == surname && fRead(file, m).kurs > k
					&& avg(fRead(file, m)) > grade))
		{
			L = m + 1;
		}
		else
		{
			R = m - 1;
		}
	} while (L <= R);
	return -1;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int kurs;
	char fname[20];
	char surname[15];
	Spec spec;
	unsigned speciality;
	char choice;
	unsigned grade;

	do {
		cout << "������ ��!" << endl << "[1] - C������� ����" << endl << "[2] - ������ �������� �� �����" << endl << "[3] - ������� ��� �� �����" << endl << "[4] - Գ����� ������������ " << endl << "[5] - �������� �������������" << endl << "[6] - ������� �� ����� �������� ������������� ����" << endl << "[7] - �������� �����" << endl << "[0] - ����� �� ��������" << endl << endl << endl;
		cout << "��� ���� : "; cin >> choice;
		switch (choice)
		{
		case '1':
			cout << "����� ����� : "; cin >> fname;
			Create(fname);
			break;
		case '2':
			cout << "����� ����� : "; cin >> fname;
			Add(fname);
			break;
		case '3':
			cout << "����� ����� : "; cin >> fname;
			Print(fname);
			break;
		case '4':
			cout << "����� ����� : "; cin >> fname;
			Sort(fname);
			break;
		case '5':
			cout << "����� ����� : "; cin >> fname;
			IndexSort(fname);
			break;
		case '6':
			cout << "����� ����� : "; cin >> fname;
			IndexDisplay(fname);
			break;
		case '7':
			cout << "����� ����� : "; cin >> fname;
			cout << "������� : "; cin >> surname;
			cout << "���� : "; cin >> kurs;
			cout << "������� ��� : "; cin >> grade;
			if (BinSearch(fname, surname, kurs, grade) != -1)
				cout << "�������� ��������" << endl << endl << endl;
			else cout << "�� ��������!" << endl << endl << endl;
			break;
		case '0':
			break;
		default:
			cout << "\n�������!\n\n";
			break;
		}
	} while (choice != '0');

	return 0;
}
