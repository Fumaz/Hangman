#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>

using namespace std;

const string STATUSES[] = {
        (
                "  +---+\n"
                "  |   |\n"
                "      |\n"
                "      |\n"
                "      |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                "      |\n"
                "      |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                "  |   |\n"
                "      |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                " /|   |\n"
                "      |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                " /|\\  |\n"
                "      |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                " /|\\  |\n"
                " /    |\n"
                "      |\n"
                "========="
        ),
        (
                "  +---+\n"
                "  |   |\n"
                "  O   |\n"
                " /|\\  |\n"
                " / \\  |\n"
                "      |\n"
                "========="
        )
};

const int MAX_ERRORS = 6;
vector<string> WORDS;

string toLowerCase(string string) {
    transform(string.begin(), string.end(), string.begin(),
              [](unsigned char c) { return tolower(c); });

    return string;
}

enum Status {
    PLAYING,
    WON,
    LOST
};

enum ActionResult {
    NONE,
    ALREADY_GUESSED,
    LETTER_CORRECT,
    LETTER_INCORRECT,
    WORD_CORRECT,
    WORD_INCORRECT,
    GAME_ENDED
};

class Game {
public:
    explicit Game(const string &word) {
        this->word = word;
        this->status = Status::PLAYING;
    }

    ActionResult guess(const string &guess) {
        if (guess.length() == 1) {
            return guessLetter(guess[0]);
        }

        return guessWord(guess);
    }

    string getFormattedWord() {
        string formatted;

        for (char c : word) {
            char l = tolower(c);

            if (guessed.count(string(1, l)) || l == ' ') {
                formatted += c;
            } else {
                formatted += "_";
            }
        }

        return formatted;
    }

    string createMessage(ActionResult result = ActionResult::NONE) {
        string message = "Impiccato\n\n";
        message += STATUSES[errors.size()] + "\n\n";

        status = checkStatus();

        if (status == PLAYING) {
            message += "Puoi fare ancora " + to_string(MAX_ERRORS - errors.size()) + " errori.\n";

            if (result == LETTER_CORRECT || result == WORD_CORRECT) {
                message += "Giusto! Continua cosí!\n";
            } else if (result == LETTER_INCORRECT || result == WORD_INCORRECT) {
                message += "Sbagliato! Riprova.\n";
            }

            message += "\nLa parola é: " + getFormattedWord();
        } else {
            message += "La parola era: " + word + "\n";

            if (status == Status::WON) {
                message += "Avete vinto!";
            } else {
                message += "Avete perso!";
            }
        }

        return message;
    }

    bool isCorrect() {
        return toLowerCase(getFormattedWord()) == toLowerCase(word);
    }

    bool hasLost() {
        return errors.size() >= MAX_ERRORS;
    }

    ActionResult takeInput() {
        if (status != PLAYING) {
            return NONE;
        }

        cout << "\n\nInserisci la lettera/parola: ";

        string word;
        getline(cin >> ws, word);

        return guess(word);
    }

    void start() {
        printMessage();

        while (status == PLAYING) {
            printMessage(takeInput());
        }
    }

    void printMessage(ActionResult result = NONE) {
        cout << createMessage(result);
    }

private:
    string word;
    set<string> errors;
    set<string> guessed;
    Status status;

    ActionResult guessWord(string word) {
        word = toLowerCase(word);

        if (errors.count(word)) {
            return ActionResult::ALREADY_GUESSED;
        }

        if (word == toLowerCase(this->word)) {
            status = Status::WON;

            for (char c : word) {
                guessed.insert(string(1, c));
            }

            return ActionResult::WORD_CORRECT;
        }

        errors.insert(word);
        return ActionResult::WORD_INCORRECT;
    }

    ActionResult guessLetter(char letter) {
        letter = tolower(letter);

        if (errors.count(string(1, letter)) || guessed.count(string(1, letter))) {
            return ActionResult::ALREADY_GUESSED;
        }

        if (toLowerCase(word).find(letter) == string::npos) {
            errors.insert(string(1, letter));

            return ActionResult::LETTER_INCORRECT;
        }

        guessed.insert(string(1, letter));
        return ActionResult::LETTER_CORRECT;
    }

    Status checkStatus() {
        if (isCorrect()) {
            return Status::WON;
        }

        if (hasLost()) {
            return Status::LOST;
        }

        return status;
    }
};

void loadWords() {
    ifstream file("words.txt");
    string word;

    while (getline(file, word)) {
        WORDS.push_back(word);
    }
}

string getRandomWord() {
    srand(time(nullptr));

    return WORDS[rand() % WORDS.size()];
}

int main() {
    loadWords();

    Game game(getRandomWord());
    game.start();

    return 0;
}
