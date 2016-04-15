#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>

#define TRACK_SOURCE "track.txt"
#define RANGE_SOURCE "range.txt"
#define ACC_SOURCE "acc.txt"

using namespace std;

struct Card{
	string cardNumber;
	string expDate;
	string seciurityCode;
};

enum authorization{
	PIN = 0,
	SIGNATURE,
	NONE
};

vector <string> loadFile(const char *source)
{
	vector <string> output;
    fstream plik;
    plik.open(source, ios::in );
    if( plik.good() )
    {
        string napis;
        cout << "Wczytana zawartosc z pliku "<< source << endl;
        while( !plik.eof() )
        {
            getline( plik, napis );
            cout << napis << endl;
            if (!napis.empty())
				output.push_back(napis);
        }
        plik.close();
    } 
	else 
		cout << "Error! Nie udalo sie otworzyc pliku!" << endl;
   
	return output;
}

string getCardNumber(string track)
{
	string cardNumber;
	
	int i = 0;
	char c = '=';
	
	while(track.at(i) != '=' || i >= 19)
	{
		i++;
	}
	
	cardNumber = track.substr(1, i-1);
	
	return cardNumber;
}

string getExpDate(string track)
{
	string expDate;
	
	int i = 0;
	char c = '=';
	
	while(track.at(i) != '=' || i >= 19)
	{
		i++;
	}
	
	expDate = track.substr(i+1, 4);
	
	return expDate;
}

string getSeciurityCode(string track)
{
	string seciurityCode;
	
	int i = 0;
	char c = '=';
	
	while(track.at(i) != '=' || i >= 19)
	{
		i++;
	}
	
	seciurityCode = track.substr(i+5, 3);
	
	return seciurityCode;
}

bool isCardDateValid(string expDate)
{
	bool isValid = false;
	time_t czas;
	int year;
	int month;
	int yymm;
	int yymmCard;

    struct tm * ptr;
    time( & czas );
    ptr = localtime( & czas );
    
    year = ptr->tm_year-100;
    month = ptr->tm_mon+1;
	
    yymmCard = atoi(expDate.c_str());
    yymm = year * 100 + month;
    
    cout << "Obecny rok i miesiac w formacie YYMM = " << yymm << endl;
	cout << "Rok i miesiac waznosci karty w formacie YYMM = " << yymmCard <<endl;
    
    if (yymmCard >= yymm)
		isValid = true;
	
	return isValid;
}

// Luhn algorithm is used for validate card number
bool isCardNumberValid(string number)
{
    int sum = 0;
    bool alt = false;
    for(int i = number.size() - 1; i >= 0; i--)
    {
      int temp = number.at(i) - '0'; // convert character to numeric value
      if(alt)
      {  
        temp *= 2;
        if(temp > 9)
        {
          temp -= 9; 
        }
      }
      sum += temp;
      alt = !alt;
    }
    
    return sum % 10 == 0;
}

void checkAuthorizationMethod(string code)
{	
	int value = -1;
	
	value = code.at(2) - '0'; // convert character to numeric value
	
	if (value == 0 || value == 3|| value == 5 || value == 6 || value == 7)
		cout << "Wymagany PIN" << endl;
	else if (value == 1 || value == 2)
		cout << "Wymagany Podpis" << endl;
	else
		cout << "Brak weryfikacji karty "<< endl;
			
	return ;
}

void taskForTrack()
{
	vector <string> track;
	vector <Card> cards;
	
	cout << "Zadania dla pliku " << TRACK_SOURCE << endl;
	cout << "******************" << endl << endl;
	
	track = loadFile(TRACK_SOURCE);
		
	for( int i = 0; i < track.size(); i++)
	{
		Card card;
		
		if(!track.at(i).empty())
		{
			card.cardNumber = getCardNumber(track.at(i));  
			card.expDate = getExpDate(track.at(i));
			card.seciurityCode = getSeciurityCode(track.at(i)); 
		}
		cards.push_back(card);
	}
	
	for( int i = 0; i < cards.size(); i++)
	{
		if (isCardNumberValid(cards.at(i).cardNumber))
		{
			cout<< "Karta o numerze " << cards.at(i).cardNumber << " jest prawdziwa" << endl;
		}
		else
			cout<< "Karta o numerze " << cards.at(i).cardNumber << " jest falszywa" << endl;
		
		if (isCardDateValid(cards.at(i).expDate))
			cout<< "Karta o numerze " << cards.at(i).cardNumber << " jest wazna" << endl;
		else
			cout<< "Karta o numerze " << cards.at(i).cardNumber << " nie jest juz wazna" << endl;	
			
		checkAuthorizationMethod(cards.at(i).seciurityCode);
			
		cout <<endl << "-------------------" << endl;
	}
	
	cout << "Koniec zadan dla pliku " << TRACK_SOURCE << endl;
	cout << "*************************" << endl;
}

/*
//TODO
void taskForRange()
{
	vector <string> range;
	range = loadFile(RANGE_SOURCE);
}

//TODO
void taskForAcc()
{
	vector <string> acc;
	acc = loadFile(ACC_SOURCE);
}
*/

int main()
{
	taskForTrack();
	//taskForRange();
	//taskForAcc();
			
    return 0;
}
