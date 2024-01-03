#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cctype>
#include <iterator>
#include <regex>

using namespace std;

// Pasalinami skyrybos zenklai, skaiciai, zodziai padaromi mazosiomis raidemis
string zodziu_tvarkymas(const string& zodis) {
    string ats;
    for (char r : zodis) {
        if (std::isalpha(r)) ats += std::tolower(r);
    }
    return ats;
}

// Funkcija randanti URL adresus
vector<string> URLs_gavimas(const string& tekstas)
{
    vector<string> urls;
    regex urlRegex(R"((https:\/\/www\.|http:\/\/www\.|https:\/\/|http:\/\/)?[a-zA-Z0-9]{2,}(\.[a-zA-Z0-9]{2,})(\.[a-zA-Z0-9]{2,})?)");
    sregex_iterator it(tekstas.begin(), tekstas.end(), urlRegex);
    sregex_iterator end;

    while (it != end)
    {
        urls.push_back(it->str());
        ++it;
    }
    return urls;
}

// Funkcija, randanti zodzius su 'ape'
bool zodzio_dalis(string zodis) {
    return (zodis.find("ape") != string::npos);
}

int main() {

    //ZODZIU IR JU SKAICIU RADIMAS
    ifstream failas("testinis.txt");
    if (!failas.is_open()) {
        cerr << "Failo atidarymo klaida." << endl;
        return 1;}

    // Pasikartojusiu zodziu skaiciavimas
    map<string, int> kiekis;
    map<string, vector<int>> nuorodos;
    vector<string> zodziai;
    string eil;
    int nr = 0;
    while (getline(failas, eil)) {
        nr++;
        istringstream iss(eil);
        string tikrinamas;
        while (iss >> tikrinamas) {
            string zodis = zodziu_tvarkymas(tikrinamas);
            if (zodzio_dalis(zodis)==true) zodziai.push_back(zodis);
            if (!zodis.empty()) {
                kiekis[zodis]++;
                nuorodos[zodis].push_back(nr);
            }
        }
    }
    failas.close();

    // Rezultatas naujame faile
    ofstream rezultatai("rezultatai.txt");
    if (!rezultatai.is_open()) {
        cerr << "Failo atidarymo klaida." << endl;
        return 1;}

    for (const auto& [zodis, count] : kiekis) {
        if (count > 1) {
            rezultatai << zodis << ": " << count << " kartai - eilutese: ";

            const vector<int>& nuoroda = nuorodos[zodis];
            if (!nuoroda.empty()) {
                copy(nuoroda.begin(), nuoroda.end() - 1, std::ostream_iterator<int>(rezultatai, ", "));
                rezultatai << nuoroda.back();
            }
            rezultatai << endl;
        }
    }
    rezultatai.close();

    ofstream outputFile("gauti_zodziai.txt");
    for (const auto& z : zodziai) {
        outputFile << z << std::endl;
    }
    outputFile.close();

    // URL GAVIMAS
    ifstream failas2("testinisURL.txt");
    if (!failas2.is_open()) {
        cerr << "Failo atidarymo klaida." << endl;
        return 1;}

    //Nuskaitomas failas i string
    stringstream b;
    b<<failas2.rdbuf();
    string tekstas = b.str();
    //Gaunami URL
    vector<string> urls = URLs_gavimas(tekstas);

    //Isvedimas i faila
    ofstream rezultataiURL("rezultataiURL.txt");
    if (!rezultataiURL.is_open()) {
        cerr << "Failo atidarymo klaida." << endl;
        return 1;}

    for (size_t i = 0; i < urls.size(); ++i) {
        rezultataiURL << "URL #" << i + 1 << ": " << urls[i] << endl;
    }
    rezultataiURL.close();

    cout << "Rezultatai isvesti faile 'rezultatai.txt'. ir 'rezultataiURL.txt'\n" << endl;
    system("pause");
    return 0;
}



