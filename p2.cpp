#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>


class Article {
private:
    int code_barre;
    std::string designation;
    float prix;
    std::time_t horaire_scan;

public:
    Article(int code_barre, std::string designation, float prix, std::time_t horaire_scan)
        : code_barre(code_barre), designation(designation), prix(prix), horaire_scan(horaire_scan) {}

    int get_code_barre() const { return code_barre; }
    void set_code_barre(int code_barre) { this->code_barre = code_barre; }

    std::string get_designation() const { return designation; }
    void set_designation(std::string designation) { this->designation = designation; }

    float get_prix() const { return prix; }
    void set_prix(float prix) { this->prix = prix; }

    std::time_t get_horaire_scan() const { return horaire_scan; }
    void set_horaire_scan(std::time_t horaire_scan) { this->horaire_scan = horaire_scan; }
};

class Panier {
private:
    std::vector<Article> articles;

public:
    Panier() {}

    bool est_vide() const { return articles.empty(); }

    void ajouter_article(Article article) {
        articles.push_back(article);
        std::cout << "Article ajoute: " << article.get_designation() << ", Prix: " << article.get_prix() << " Dhs" << std::endl;
        std::cout << "Nombre d'articles dans le panier: " << articles.size() << ", Prix total: " << prix_total() << " Dhs" << std::endl;
    }

    float prix_total() const {
        float total = 0;
        for (const Article& article : articles) {
            total += article.get_prix();
        }
        return total;
    }

    void ticket() const {
        std::cout << "\n------------------- Ticket de Caisse -------------------" << std::endl;
        std::time_t t = std::time(nullptr);
        std::tm* local_time = std::localtime(&t);
        std::cout << "Date et heure de l'impression: " << std::put_time(local_time, "%c") << std::endl;
        for (const Article& article : articles) {
            std::cout << "Article: " << article.get_designation() << ", Prix: " << article.get_prix() << " Dhs" << std::endl;
        }
        std::cout << "Nombre d'articles: " << articles.size() << ", Montant total: " << prix_total() << " Dhs" << std::endl;
    }

    void duree_courses() const {
        if (articles.size() >= 2) {
            std::time_t first_article_scan = articles.front().get_horaire_scan();
            std::time_t last_article_scan = articles.back().get_horaire_scan();
            std::time_t duration = last_article_scan - first_article_scan;
            std::cout << "Durée des achats: " << duration << " secondes." << std::endl;
        } else {
            std::cout << "Il n'y a pas assez d'articles pour calculer la durée des achats." << std::endl;
        }
    }
};

bool article_existe(int code_barre, const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int code;
        iss >> code;
        if (code == code_barre) {
            return true;
        }
    }
    return false;
}

Article recuperer_article(int code_bar, const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int code;
        std::string designation;
        float prix;
        iss >> code;
        iss.ignore(2);
        std::getline(iss, designation, ',');
        iss.ignore(1);
        iss >> prix;
        if (code == code_bar) {
            std::time_t horaire_scan = std::time(nullptr);
            return Article(code, designation, prix, horaire_scan);
        }
    }
    return Article(0, "Article inconnu", 0.0f, std::time(nullptr));
}
int main() {
    std::cout << "\n------------------- Simulation de Courses -------------------" << std::endl;
    Panier panier;

    std::string filename = "articles.txt"; 

    int code_barre;
    std::cout << "\nEntrez le code-barres de l'article (entier): ";
    std::cin >> code_barre;

    if (article_existe(code_barre, filename)) {
        Article article = recuperer_article(code_barre, filename);
        panier.ajouter_article(article);
    } else {
        std::string designation;
        float prix;
        std::cout << "Entrez le nom de l'article: ";
        std::cin >> designation;
        std::cout << "Entrez le prix de l'article: ";
        std::cin >> prix;
        std::time_t horaire_scan = std::time(nullptr);
        Article article(code_barre, designation, prix, horaire_scan);
        panier.ajouter_article(article);
        // Add the new article to the file
        std::ofstream file(filename, std::ios::app);
        file << "\n" << code_barre << " , " << designation << " , " << prix;
    }
    std::string more_articles;
    std::cout << "\nVoulez-vous ajouter d'autres articles au panier? (oui/non): ";
    std::cin >> more_articles;


        while (more_articles == "oui") {
            int code_barre;
            std::cout << "\nEntrez le code-barres de l'article (entier): ";
            if (!(std::cin >> code_barre)) {
                std::cout << "Erreur: entree invalide. Veuillez entrer un nombre entier.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (article_existe(code_barre, filename)) {
                Article article = recuperer_article(code_barre, filename);
                panier.ajouter_article(article);
            } else {
                std::string designation;
                float prix;

                std::cout << "Entrez le nom de l'article: ";
                std::cin >> designation;

                std::cout << "Entrez le prix de l'article: ";
                std::cin >> prix;
                if (std::cin.fail()) {
                    std::cout << "Erreur: entree invalide. Veuillez entrer un nombre reel.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                std::time_t horaire_scan = std::time(nullptr);

                Article article(code_barre, designation, prix, horaire_scan);
                panier.ajouter_article(article);
            }

            std::cout << "\nVoulez-vous ajouter d'autres articles au panier? (oui/non): ";
            std::cin >> more_articles;
        }

 
    panier.ticket();
    panier.duree_courses();

    return 0;
}
