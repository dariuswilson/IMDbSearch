  /****
  LIBRARIES
  ****/
#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

/*********************************************** Helper functions ***************************************/

//The splitString function takes a line and split it on the delimiter and push the parts into items
void splitString(string line, char delimiter, vector<string>& items ) {
	stringstream s_stream(line);
	while (s_stream.good()) {
		string item;
		getline(s_stream, item, delimiter);
		items.push_back(item);
	}
}

/**********************************************************************************************************/
  /****
  STRUCT
  ****/
struct TitleRecord {
    string tconst;
    string titleType;
    string primaryTitle;
    string originalTitle;
    string isAdult;
    string startYear;
    string endYear;
    string runtimeMinutes;
    string genres;
};

struct NameRecord {
    string nconst;
    string primaryName;
    string birthYear;
    string deathYear;
    string primaryProfession;
    string knownForTitles;
};

struct PrincipalRecord {
    string tconst;
    string ordering;
    string nconst;
    string category;
    string job;
    string characters;
};

  /****
  VECTORS
  ****/

vector<TitleRecord> titleRecord;
vector<NameRecord> nameRecord;
vector<PrincipalRecord> principalRecord;

  /****
  READ FILES
  ****/

void readTitlesFile(string titlesFile) {
    //open a file in read mode
    ifstream infile;
    infile.open (titlesFile);

    string data;
    titleRecord.clear();

    getline (infile, data); // read header

    while (getline (infile, data)) { // read other rows
        vector<string> items;
        splitString(data, '\t', items);

        TitleRecord record;
        record.tconst = items[0];
        record.titleType = items[1];
        record.primaryTitle = items[2];
        record.originalTitle = items[3];
        record.isAdult = items[4];
        record.startYear = items[5];
        record.endYear = items[6];
        record.runtimeMinutes = items[7];
        record.genres = items[8];

        titleRecord.push_back(record);
    }

    infile.close (); //close the opened file
}

void readNamesFile(string namesFile) {
    //open a file in read mode
    ifstream infile;
    infile.open (namesFile);

    string data;
    nameRecord.clear();

    getline (infile, data); // read header

    while (getline (infile, data)) { // read other rows
        vector<string> items;
        splitString(data, '\t', items);

        NameRecord record;
        record.nconst = items[0];
        record.primaryName = items[1];
        record.birthYear = items[2];
        record.deathYear = items[3];
        record.primaryProfession = items[4];
        record.knownForTitles = items[5];

        nameRecord.push_back(record);
    }

    infile.close (); //close the opened file
}

void readPrincipalsFile(string principalsFile) {
    //open a file in read mode
    ifstream infile;
    infile.open (principalsFile);

    string data;
    principalRecord.clear();

    getline (infile, data); // read header

    while (getline (infile, data)) { // read other rows
        vector<string> items;
        splitString(data, '\t', items);

        PrincipalRecord record;
        record.tconst = items[0];
        record.ordering = items[1];
        record.nconst = items[2];
        record.category = items[3];
        record.job = items[4];
        record.characters = items[5];

        principalRecord.push_back(record);
    }

    infile.close (); //close the opened file
}

  /****
  search for movie
  ****/

void searchMovies(string searchPhrase) {
    int choice = -1;

    do {
        string keyword = "";
        // lowercase searchPhrase
        for (int i = 0; i < searchPhrase.size(); i++){
            keyword += tolower(searchPhrase[i]);
        }

        // populate keywords
        vector<string>keywords;
        keywords.clear();

        string temp = "";
        for (int i = 0; i < keyword.size(); i++) {
            if (keyword[i] != '+') temp += keyword[i];
            else {
                keywords.push_back(temp);
                temp = "";
            }
            if (i == keyword.size() - 1) keywords.push_back(temp);
        }

        int matchCounter = 0;
        vector<TitleRecord> results;
        results.clear();
        for (int i = 0; i < titleRecord.size(); i++) {
            string title = "";
            for (int j = 0; j < titleRecord[i].primaryTitle.length(); j++) {
                title += tolower(titleRecord[i].primaryTitle[j]);
            }

            // matched all keywords to the title
            bool matched = false;
            for (int k = 0; k < keywords.size(); k++) {
                if (title.find(keywords[k]) != string::npos) matched = true;
                else {
                    matched = false;
                    break;
                }
            }

            if (matched) {
                if (matchCounter == 0) cout << endl;
                cout << matchCounter << ":" << endl;
                cout << "Title: " << titleRecord[i].primaryTitle << endl;
                cout << "Year: " << titleRecord[i].startYear << endl;
                cout << "Genre: " << titleRecord[i].genres << endl;
                cout << "---------------" << endl;
                results.push_back(titleRecord[i]);
                matchCounter = matchCounter + 1;
            }
        }

        if (matchCounter == 0) cout << "No match found!" << endl << endl;
        else {
            cout << "Select a movie to see its actors/actresses (-1 to go back to the previous menu): ";
            cin >> choice;

            if (cin.fail()) {
                break;
            }

            if (choice > -1 && choice < results.size()) {
                // get movie characters from principals
                for (int i = 0; i < principalRecord.size(); i++) {
                    if (principalRecord[i].tconst == results[choice].tconst && (principalRecord[i].category == "actor" || principalRecord[i].category == "actress")) {

                        // get the names of the actors/actress
                        for (int j = 0; j < nameRecord.size(); j++) {
                            if (principalRecord[i].nconst == nameRecord[j].nconst) {
                                cout << nameRecord[j].primaryName << " " << principalRecord[i].characters << endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
    } while (choice != -1);
}

  /****
  search for actors
  ****/

void searchActorsActressess(string searchPhrase) {
    int choice = -1;

    do {
        string keyword = "";
        // lowercase searchPhrase
        for (int i = 0; i < searchPhrase.size(); i++){
            keyword += tolower(searchPhrase[i]);
        }

        // populate keywords
        vector<string>keywords;
        keywords.clear();

        string temp = "";
        for (int i = 0; i < keyword.size(); i++) {
            if (keyword[i] != '+') temp += keyword[i];
            else {
                keywords.push_back(temp);
                temp = "";
            }
            if (i == keyword.size() - 1) keywords.push_back(temp);
        }

        int matchCounter = 0;
        vector<NameRecord> results;
        results.clear();
        for (int i = 0; i < nameRecord.size(); i++) {
            string name = "";
            for (int j = 0; j < nameRecord[i].primaryName.length(); j++) {
                name += tolower(nameRecord[i].primaryName[j]);
            }

            // matched all keywords to the name
            bool matched = false;
            for (int k = 0; k < keywords.size(); k++) {
                if (name.find(keywords[k]) != string::npos) matched = true;
                else {
                    matched = false;
                    break;
                }
            }

            if (matched) {
                if (matchCounter == 0) cout << endl;
                cout << matchCounter << ":" << endl;
                cout << nameRecord[i].primaryName << endl;
                cout << nameRecord[i].primaryProfession << endl;
                cout << "(" << nameRecord[i].birthYear << "-" << nameRecord[i].deathYear << ")" << endl;
                cout << "---------------" << endl;
                results.push_back(nameRecord[i]);
                matchCounter = matchCounter + 1;
            }
        }

        if (matchCounter == 0) cout << "No match found!" << endl << endl;
        else {
            cout << "Select an actor/actress to see movies (-1 to go back to the previous menu): ";
            cin >> choice;
            
            if (cin.fail()) {
                break;
            } 

            if (choice > -1 && choice < results.size()) {
                // get title codes from principals
                for (int i = 0; i < principalRecord.size(); i++) {
                    if (principalRecord[i].nconst == results[choice].nconst && (principalRecord[i].category == "actor" || principalRecord[i].category == "actress")) {

                        // get the movies of the actors/actress
                        for (int j = 0; j < titleRecord.size(); j++) {
                            if (principalRecord[i].tconst == titleRecord[j].tconst) {
                                cout << titleRecord[j].primaryTitle << " " << principalRecord[i].characters << endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
    } while (choice != -1);
}

int run(string titlesFile, string namesFile, string principalsFile) {
    // since the main function should not be changed, implement the needed processes in the run function

    int choice = 0;
    string searchPhrase;
    do {
        readTitlesFile(titlesFile); // read titlesFile and populate the vector of TitleRecord
        readNamesFile(namesFile); // read namesFile and populate the vector of NameRecord
        readPrincipalsFile(principalsFile); // read principalsFile and populate the vector of PrincipalsRecord

        cout << "Select a menu option:" << endl;
        cout << "1. Search for movies" << endl;
        cout << "2. Search for actors/actresses" << endl;
        cout << "3. Exit" << endl;

        cout << "Your choice --> ";
        cin >> choice;
        if (cin.fail()) {
            break;
        }

        if (choice == 1 || choice == 2) {
            cout << endl << "Enter search phrase: ";
            cin >> searchPhrase;
            if (cin.fail()) {
                break;
            }

            if (choice == 1) {
                searchMovies(searchPhrase);
            } else if (choice == 2) {
                searchActorsActressess(searchPhrase);
            }
        }

    } while (choice != 3);

	return 0;	
}

int main() {
	//Change these addresses according to your local machine
	string titlesFile =	"movie.titles.tsv";
	string namesFile = "movie.names.tsv";
	string principalsFile = "movie.principals.tsv";

	//DO NOT change the main function. The main function will be removed by the autograder
	run(titlesFile, namesFile, principalsFile);
	return 0;
}