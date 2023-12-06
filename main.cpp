#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <fstream>
using namespace std;

enum class RegistrationStatus
{
    Paid,
    Unpaid
};

struct Category
{
    string name;
    int minAge;
    int maxAge;

    Category(string n, int minA, int maxA) : name(n), minAge(minA), maxAge(maxA) {}
};

Category U6("U6", 0, 5);
Category U8("U8", 6, 7);
Category U10("U10", 8, 9);
Category U12("U12", 10, 11);
Category U14("U14", 12, 13);
Category U17("U17", 14, 16);

struct Player
{
    string firstName;
    string lastName;
    int yearOfBirth;
    Category category;
    RegistrationStatus registrationStatus;

    // Constructor for easy initialization
    Player(string fn, string ln, int yob, Category cat, RegistrationStatus regStatus)
        : firstName(move(fn)), lastName(move(ln)), yearOfBirth(yob), category(cat), registrationStatus(regStatus) {}
};

class SoccerRegistrationSystem
{
private:
    vector<Player> players;
    vector<Player> searchResults;
    int currentYear;
    string filename;
    int searchIndex = 0;

public:
    SoccerRegistrationSystem(int year)
        : currentYear(year) {}

    void saveToFile(const std::string &filename)
    {
        std::ofstream file(filename);
        if (!file)
        {
            std::cerr << "Error opening file for writing.\n";
            return;
        }
        file << currentYear << "\n";
        for (const auto &player : players)
        {
            file << player.firstName << "," << player.lastName << "," << player.yearOfBirth << ","
                 << (player.category.name) << ","
                 << registrationStatusToString(player.registrationStatus) << "\n";
        }

        file.close();
        std::cout << "Data saved to " << filename << "\n";
    }
    Category determineCategory(int yearOfBirth, int currentYear)
    {
        int age = currentYear - yearOfBirth;
        if (age <= U6.maxAge)
        {
            return U6;
            cout << "Player added successfully.\n";
        }
        else if (age <= U8.maxAge)
        {
            return U8;
            cout << "Player added successfully.\n";
        }
        else if (age <= U10.maxAge)
        {
            return U10;
            cout << "Player added successfully.\n";
        }
        else if (age <= U12.maxAge)
        {
            return U12;
            cout << "Player added successfully.\n";
        }
        else if (age <= U14.maxAge)
        {
            return U14;
            cout << "Player added successfully.\n";
        }
        else if (age <= U17.maxAge)
        {
            return U17;
            cout << "Player added successfully.\n";
        }
        else
        {
            cout << "Invalid age. Please try again.\n";
            return Category("Invalid", -1, -1);
        }
    }

    void addPlayer()
    {
        string firstName, lastName;
        int yearOfBirth;
        string paymentStatus;
        RegistrationStatus registrationStatus;

        cout << "Enter player's name: ";
        cin >> firstName >> lastName;
        cout << "Enter year of birth: ";
        cin >> yearOfBirth;
        cin.ignore();

        cout << "Has the player paid? (yes/no): ";
        getline(cin, paymentStatus);
        registrationStatus = (paymentStatus == "yes") ? RegistrationStatus::Paid : RegistrationStatus::Unpaid;

        Category category = determineCategory(yearOfBirth, currentYear);
        players.emplace_back(firstName, lastName, yearOfBirth, category, registrationStatus);
        saveToFile("players_data.txt");
    }

    void searchPlayer(const string &firstName, const string &lastName, const string &keyword,
                      int yearOfBirth, RegistrationStatus regStatus, const string &categoryName,
                      bool considerRegStatus = true)
    {
        searchResults.clear();
        searchIndex = 0;

        for (const auto &player : players)
        {
            if ((!firstName.empty() && player.firstName != firstName) ||
                (!lastName.empty() && player.lastName != lastName) ||
                (!keyword.empty() && player.firstName.find(keyword) == string::npos && player.lastName.find(keyword) == string::npos) ||
                (yearOfBirth != -1 && player.yearOfBirth != yearOfBirth) ||
                (!categoryName.empty() && player.category.name != categoryName) ||
                (considerRegStatus && player.registrationStatus != regStatus))
            {
                continue;
            }
            searchResults.push_back(player);
        }

        if (!searchResults.empty())
        {
            displaySearchResult();
        }
        else
        {
            cout << "No players found.\n";
        }
    }

    void displaySearchResult()
    {
        if (searchResults.empty())
            return;
        const Player &player = searchResults[searchIndex];
        cout << "Player " << (searchIndex + 1) << " of " << searchResults.size() << ":\n";
        cout << left << setw(10) << player.firstName << left << setw(10) << player.lastName << right << setw(6) << player.yearOfBirth << " " << 
        setw(5) << left << player.category.name << setw(5) << left << registrationStatusToString(player.registrationStatus) << "\n";
    }

    void displayAllPlayers()
    {
        cout << "All Players:\n";
        cout << left << setw(18) << "Name" << right << setw(10) << "Year " << setw(9) << right << "  Category " << setw(10) << right << "Status "
             << "\n";
        cout << "----------------------------------------------\n";
        for (const auto &player : players)
        {
            cout << left << setw(10) << player.firstName << left << setw(10) << player.lastName << "|" << right << setw(6) << player.yearOfBirth << " " 
            << "| " << setw(9) << left << player.category.name << "| " << setw(6) << right << registrationStatusToString(player.registrationStatus) << "\n";
        }
    }

    void nextPlayer()
    {
        if (searchResults.empty())
            return;
        searchIndex = (searchIndex + 1) % searchResults.size();
        displaySearchResult();
    }

    void previousPlayer()
    {
        if (searchResults.empty())
            return;
        if (searchIndex == 0)
            searchIndex = searchResults.size();
        searchIndex = (searchIndex - 1) % searchResults.size();
        displaySearchResult();
    }

    bool isSearchEmpty()
    {
        return searchResults.empty();
    }

    string registrationStatusToString(RegistrationStatus status)
    {
        switch (status)
        {
        case RegistrationStatus::Paid:
            return "Paid";
        case RegistrationStatus::Unpaid:
            return "Unpaid";
        default:
            return "Unknown";
        }
    }

    void loadFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file)
        {
            std::cerr << "Error opening file for reading.\n";
            return;
        }
        file >> currentYear;
        file.get();
        string line;
        while (getline(file, line))
        {
            std::istringstream iss(line);
            string firstName, lastName, categoryName, regStatusStr;
            int yearOfBirth;
            char delim;

            getline(iss, firstName, ',');
            getline(iss, lastName, ',');
            iss >> yearOfBirth >> delim;
            getline(iss, categoryName, ',');
            getline(iss, regStatusStr);

            RegistrationStatus regStatus = (regStatusStr == "Paid") ? RegistrationStatus::Paid : RegistrationStatus::Unpaid;
            Category category = getCategoryByName(categoryName);

            players.emplace_back(firstName, lastName, yearOfBirth, category, regStatus);
        }

        file.close();
    }

    Category getCategoryByName(const string &name)
    {
        if (name == "U6")
            return U6;
        else if (name == "U8")
            return U8;
        else if (name == "U10")
            return U10;
        else if (name == "U12")
            return U12;
        else if (name == "U14")
            return U14;
        else if (name == "U17")
            return U17;
        else
            return Category("Invalid", -1, -1); // Handle invalid category
    }
    void updateYearAndRecalculateCategory(int newYear)
    {
        currentYear = newYear;
        for (auto &player : players)
        {
            player.category = determineCategory(player.yearOfBirth, currentYear);
        }
    }
};

void clearScreen()
{
    // Clear the screen. ("cls" for Windows, "clear" for Unix/Linux)
    system("clear");
}

int main()
{
    int currentYear, none;
    clearScreen();
    char choice;
    bool inSearchView = false;

    SoccerRegistrationSystem system(currentYear);
    system.loadFromFile("players_data.txt");

    do
    {
        clearScreen();

        if (!inSearchView)
        {
            cout << "Soccer Registration System\n";
            cout << "A. Add Player\nS. Search Player\nD. Display All Players\nC. Change Year\nE. Exit\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            string searchName;

            switch (choice)
            {
            case 'A':
            case 'a':
                clearScreen();
                system.addPlayer();
                break;
            case 'S':
            case 's':
                clearScreen();
                {
                    string firstName, lastName, keyword, categoryName;
                    int yearOfBirth = -1; // Using -1 as a default value to indicate 'any year'
                    RegistrationStatus regStatus;
                    string regStatusInput;
                    bool considerRegStatus = true;

                    cout << "Enter first name (or leave blank): ";
                    getline(cin, firstName);
                    cout << "Enter last name (or leave blank): ";
                    getline(cin, lastName);
                    cout << "Enter keyword (or leave blank): ";
                    getline(cin, keyword);
                    cout << "Enter year of birth (or leave blank): ";
                    {
                        string yobInput;
                        getline(cin, yobInput);
                        if (!yobInput.empty())
                        {
                            yearOfBirth = stoi(yobInput);
                        }
                    }
                    cout << "Enter registration status (Paid/NotPaid/Any or leave blank): ";
                    getline(cin, regStatusInput);
                    if (regStatusInput.empty() || regStatusInput == "Any")
                    {
                        considerRegStatus = false;
                    }
                    else
                    {
                        regStatus = (regStatusInput == "Paid") ? RegistrationStatus::Paid : RegistrationStatus::Unpaid;
                    }
                    cout << "Enter category (or leave blank): ";
                    getline(cin, categoryName);

                    system.searchPlayer(firstName, lastName, keyword, yearOfBirth, regStatus, categoryName, considerRegStatus);
                    inSearchView = !system.isSearchEmpty();
                }

                break;
            case 'D':
            case 'd':
                clearScreen();
                system.displayAllPlayers();
                break;
            case 'C':
            case 'c':
                cout << "Enter the current year: ";
                cin >> currentYear;
                cin.ignore();
                system.updateYearAndRecalculateCategory(currentYear);
                break;
            case 'E':
            case 'e':
                clearScreen();
                system.saveToFile("players_data.txt");
                break;
            default:
                clearScreen();
                cout << "Invalid choice. Please try again.\n";
                cout << "Press Enter to continue...";
                cin.get(); // Wait for user input before clearing the screen
            }
        }
        else
        {
            system.displaySearchResult();
            cout << "N. Next Player\nP. Previous Player\nE. Exit to Main View\nEnter choice: ";
            cin >> choice;
            clearScreen();

            switch (choice)
            {
            case 'N':
            case 'n':
                system.nextPlayer();
                break;
            case 'P':
            case 'p':
                system.previousPlayer();
                break;
            case 'E':
            case 'e':
                inSearchView = false;
                break;
            }
        }

        cout << "Press Enter to continue...";
        cin.get();
    } while (choice != 'e' || choice != 'e');
    system.saveToFile("players_data.txt");
    return 0;
}