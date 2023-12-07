#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <queue>

using namespace std;

class Hand {
    public:
        const static int CARD_COUNT = 5;

        enum Type {
            HIGH_CARD,
            ONE_PAIR,
            TWO_PAIR,
            THREE_OF_A_KIND,
            FULL_HOUSE,
            FOUR_OF_A_KIND,
            FIVE_OF_A_KIND,
        };

        int cards[CARD_COUNT];
        Type type = Type::HIGH_CARD;
        int bid = 0;

        Hand(string &line_hand, int &bid) {
            int i = 0;

            for (char &c : line_hand) {
                cards[i] = card_value(c);
                i++;

                if (c == 'J') {
                    jokers++;
                } else if (counts.find(c) == counts.end()) {
                    counts.insert(make_pair(c, 1));
                } else {
                    counts.at(c) += 1;
                }
            }

            add_jokers();
            evaluate_type();

            this->bid = bid;
        }

        int compare(Hand* &other) {
            if (other->type != type) {
                if (other->type > type) {
                    return 1;
                } else {
                    return -1;
                }
            }

            for (int i = 0; i < CARD_COUNT; i++) {
                if (other->cards[i] > cards[i]) {
                    return 1;
                } else if (other->cards[i] < cards[i]) {
                    return -1;
                }
            }

            return 0;
        }
    
    private:
        unordered_map<char, int> counts;
        int jokers = 0;

        static int card_value(char &c) {
            switch (c) {
                case 'A':
                    return 12;
                case 'K':
                    return 11;
                case 'Q':
                    return 10;
                case 'J':
                    return -1;
                case 'T':
                    return 8;
                case '9':
                    return 7;
                case '8':
                    return 6;
                case '7':
                    return 5;
                case '6':
                    return 4;
                case '5':
                    return 3;
                case '4':
                    return 2;
                case '3':
                    return 1;
                case '2':
                    return 0;
            }

            return -2;
        }

        void add_jokers() {
            if (counts.empty()) {
                counts.insert(make_pair('J', jokers));
                return;
            }

            char most_seen_card = counts.begin()->first;
            
            for (auto iter = counts.begin(); iter != counts.end(); ++iter) {
                char c = iter->first;
                int count = counts.at(c);

                if (count > counts.at(most_seen_card)) {
                    most_seen_card = c;
                }
            }

            counts.at(most_seen_card) += jokers;
        }

        void evaluate_type() {
            int seen[5] = { 0, 0, 0, 0, 0 };

            for (auto iter = counts.begin(); iter != counts.end(); ++iter) {
                char c = iter->first;
                int count = counts.at(c);
                seen[count - 1]++;
            }

            if (seen[4] == 1) {
                type = Type::FIVE_OF_A_KIND;
            } else if (seen[3] == 1) {
                type = Type::FOUR_OF_A_KIND;
            } else if (seen[2] == 1 && seen[1] == 1) {
                type = Type::FULL_HOUSE;
            } else if (seen[2] == 1) {
                type = Type::THREE_OF_A_KIND;
            } else if (seen[1] == 2) {
                type = Type::TWO_PAIR;
            } else if (seen[1] == 1) {
                type = Type::ONE_PAIR;
            } else {
                type = Type::HIGH_CARD;
            }
        }
};

string get_line(fstream &file) {
    char raw[16];
    file.getline(raw, 16);
    return string(raw);
}

Hand* construct_hand(string &line) {
    string line_hand = line.substr(0, 5);
    int bid = stoi(line.substr(6));
    return new Hand(line_hand, bid);
}

int calculate_total(vector<Hand*> &ranking) {
    int total = 0;
    for (int i = 0; i < ranking.size(); i++) {
        int rank = ranking.size() - (ranking.size() - 1 - i);
        total += ranking[i]->bid * rank;
    }
    return total;
}

void binary_insert(Hand* &hand, vector<Hand*> &hands) {
    int left = 0;
    int right = hands.size() - 1;

    if (hands.empty()) {
        hands.push_back(hand);
        return;
    }

    while (left <= right) {
        int middle = left + (right - left) / 2;

        Hand* other = hands[middle];

        int compare = other->compare(hand);
        if (compare == 1) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    hands.insert(hands.begin() + left, hand);
}

int main() {
    fstream file("input.txt");

    vector<Hand*> ranking = {};

    while (file.good()) {
        string line = get_line(file);
        Hand* hand = construct_hand(line);
        binary_insert(hand, ranking);
    }

    cout << "Total: " << calculate_total(ranking) << endl;
    return 0;
}