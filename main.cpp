#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <sstream>

#define TRACK_SOURCE "track.txt"
#define RANGE_SOURCE "range.txt"
#define ACC_SOURCE "acc.txt"

using namespace std;

static fstream output;

struct Card{
    string cardNumber;
    string expDate;
    string seciurityCode;
};

struct RangeCard{
    string from;
    string to;
    string name;
};

vector <string> loadFile(const char *source)
{
    vector <string> input;
    fstream plik;
    plik.open(source, ios::in );
    if( plik.good() )
    {
        string napis;
        cout << "Wczytana zawartosc z pliku "<< source << endl;
        output << "Wczytana zawartosc z pliku "<< source << endl;
        while( !plik.eof() )
        {
            getline( plik, napis );
            cout << napis << endl;
            output << napis << endl;
            if (!napis.empty())
                input.push_back(napis);
        }
        plik.close();
    }
    else
    {
        cout << "Error! Nie udalo sie otworzyc pliku!" << endl;
        output << "Error! Nie udalo sie otworzyc pliku!" << endl;
    }

    cout << endl;
    output << endl;
    return input;
}

string getCardNumber(string track)
{
    string cardNumber;

    int i = 0;

    while(track.at(i) != '=' || i == track.size())
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

    while(track.at(i) != '=' || i == track.size())
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

    while(track.at(i) != '=' || i == track.size())
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
    output << "Obecny rok i miesiac w formacie YYMM = " << yymm << endl;
    cout << "Rok i miesiac waznosci karty w formacie YYMM = " << yymmCard <<endl;
    output << "Rok i miesiac waznosci karty w formacie YYMM = " << yymmCard <<endl;

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
    {
        cout << "Wymagany PIN" << endl;
        output << "Wymagany PIN" << endl;
    }
    else if (value == 1 || value == 2)
    {
        cout << "Wymagany Podpis" << endl;
        output << "Wymagany Podpis" << endl;
    }
    else
    {
        cout << "Brak weryfikacji karty "<< endl;
        output << "Brak weryfikacji karty "<< endl;
    }

    return ;
}

void checkCardName(string number, const vector<RangeCard> &rangeCards)
{
    int nrChar = -1;
    int rangeFromChar = -1;
    int rangeToChar = -1;

    for(int i = 0; i < rangeCards.size(); i++)
    {
        for(int j = 0; j < rangeCards.at(i).from.size(); j++)
        {
            nrChar = number.at(j) - '0';
            rangeFromChar = rangeCards.at(i).from.at(j) - '0';
            rangeToChar = rangeCards.at(i).to.at(j) - '0';

            if (nrChar >= rangeFromChar && nrChar <=rangeToChar)
                ;
            else
                break;

            if (j == rangeCards.at(i).from.size() - 1)
            {
                cout << "Nazwa karty o numerze " << number <<" to " << rangeCards.at(i).name << endl;
                output << "Nazwa karty o numerze " << number <<" to " << rangeCards.at(i).name << endl;
            }
        }
    }
}

void taskForTrack(const vector<RangeCard> &rangeCards)
{
    vector <string> track;
    vector <Card> cards;

    cout << "Zadania dla pliku " << TRACK_SOURCE << endl << "- Pobranie numerow kart z pliku" << endl <<
            "- Sprawdzenie czy dana karta jest wazna" << endl << "- Sprawdzenie metody weryfikacji karty" << endl
         << "- Wyciagniecie i wyswietlenie nazwy karty" << endl;
    output << "Zadania dla pliku " << TRACK_SOURCE << endl << "- Pobranie numerow kart z pliku" << endl <<
              "- Sprawdzenie czy dana karta jest wazna" << endl << "- Sprawdzenie metody weryfikacji karty" << endl
           << "- Wyciagniecie i wyswietlenie nazwy karty" << endl;
    cout << "******************" << endl << endl;
    output << "******************" << endl << endl;

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
            output<< "Karta o numerze " << cards.at(i).cardNumber << " jest prawdziwa" << endl;
        }
        else
        {
            cout<< "Karta o numerze " << cards.at(i).cardNumber << " jest falszywa" << endl;
            output<< "Karta o numerze " << cards.at(i).cardNumber << " jest falszywa" << endl;
        }

        if (isCardDateValid(cards.at(i).expDate))
        {
            cout<< "Karta o numerze " << cards.at(i).cardNumber << " jest wazna" << endl;
            output<< "Karta o numerze " << cards.at(i).cardNumber << " jest wazna" << endl;
        }
        else
        {
            cout<< "Karta o numerze " << cards.at(i).cardNumber << " nie jest juz wazna" << endl;
            output<< "Karta o numerze " << cards.at(i).cardNumber << " nie jest juz wazna" << endl;
        }

        checkAuthorizationMethod(cards.at(i).seciurityCode);

        checkCardName(cards.at(i).cardNumber, rangeCards);

        cout <<endl << "-------------------" << endl;
        output <<endl << "-------------------" << endl;
    }

    cout << endl <<  "Koniec zadan dla pliku " << TRACK_SOURCE << endl;
    output << endl <<  "Koniec zadan dla pliku " << TRACK_SOURCE << endl;
    cout << "*************************" << endl << endl;
    output << "*************************" << endl << endl;
}

RangeCard getRangeCardInformation(string range)
{
    RangeCard rCard;

    int i = 0;

    stringstream ss(range);
    string item;
    while (getline(ss, item, '|'))
    {
        if(!item.empty())
        {
            i++;
            if (i == 1)
                rCard.from = item;
            else if(i == 2)
                rCard.to = item;
            else if(i == 3)
                rCard.name = item;
        }
    }

    return rCard;
}

vector<RangeCard> taskForRange()
{
    vector <string> range;
    vector <RangeCard> rangeCards;

    cout << "Zadania dla pliku " << RANGE_SOURCE << endl << "- Pobranie zakresow poczatkowych oraz koncowych numerow kart z pliku" << endl;
    cout << "******************" << endl << endl;
    output << "Zadania dla pliku " << RANGE_SOURCE << endl << "- Pobranie zakresow poczatkowych oraz koncowych numerow kart z pliku" << endl;
    output << "******************" << endl << endl;

    range = loadFile(RANGE_SOURCE);

    for( int i = 2; i < range.size(); i++)
    {
        RangeCard rCard;

        if(!range.at(i).empty())
        {
            rCard = getRangeCardInformation(range.at(i));
        }
        rangeCards.push_back(rCard);
    }

    for( int i = 0; i< rangeCards.size(); i++)
    {
        cout << "Zakres od " << rangeCards.at(i).from << " do " << rangeCards.at(i).to << " to karta " << rangeCards.at(i).name << endl;
        output << "Zakres od " << rangeCards.at(i).from << " do " << rangeCards.at(i).to << " to karta " << rangeCards.at(i).name << endl;
    }

    cout << endl << "Koniec zadan dla pliku " << RANGE_SOURCE << endl;
    cout << "*************************" << endl << endl;
    output << endl << "Koniec zadan dla pliku " << RANGE_SOURCE << endl;
    output << "*************************" << endl << endl;

    return rangeCards;
}

bool isValidAccountNumber(string number)
{
    bool isValid = false;

    int modulo = 0;
    int charNr = 0;

    string znak;
    string temp;

    if (number.size() != 26)
        return false;

    temp = number.substr(0,2);
    number.erase(0,2);
    number += "2521";
    number += temp;

    for (int i = 0; i < number.size(); i+=2)
    {
        znak = number.substr(i,2);
        charNr = atoi(znak.c_str());

        modulo = (100 * modulo + charNr) % 97;
    }

    if (modulo == 1)
        isValid = true;

    return isValid;
}

void validateAccountNumber(string number)
{
    string accN = number;
    while(accN.find(" ") != string::npos)
        accN.erase(accN.find(" "), 1);

    bool isValid = false;

    isValid = isValidAccountNumber(accN);

    if (isValid)
    {
        cout << "Numer Konta = " << accN << " jest poprawny" << endl;
        output << "Numer Konta = " << accN << " jest poprawny" << endl;
    }
    else
    {
        cout << "Numer Konta = " << accN << " jest niepoprawny" << endl;
        output << "Numer Konta = " << accN << " jest niepoprawny" << endl;
    }

    return;
}

void taskForAcc()
{
    vector <string> acc;

    cout << "Zadania dla pliku " << ACC_SOURCE << endl << "- Pobranie numerow kont bankowych z pliku" <<
            endl << "- Sprawdzenie czy dany numer konta jest poprawny" << endl;
    cout << "******************" << endl << endl;
    output << "Zadania dla pliku " << ACC_SOURCE << endl << "- Pobranie numerow kont bankowych z pliku" <<
              endl << "- Sprawdzenie czy dany numer konta jest poprawny" << endl;
    output << "******************" << endl << endl;

    acc = loadFile(ACC_SOURCE);

    for( int i = 2; i < acc.size(); i++)
    {
        validateAccountNumber(acc.at(i));
    }

    cout << endl <<  "Koniec zadan dla pliku " << ACC_SOURCE << endl;
    cout << "*************************" << endl << endl;
    output << endl <<  "Koniec zadan dla pliku " << ACC_SOURCE << endl;
    output << "*************************" << endl << endl;
}

int main()
{
    vector<RangeCard> rCard;

    output.open("output.txt", ios::out );
    if (!output.good())
        cout << "Wystapil problem z zapisem do pliku. Output zostanie wyswietlony tylko na konsoli" << endl;

    rCard = taskForRange();
    taskForTrack(rCard);
    taskForAcc();

    output.close();

    return 0;
}

