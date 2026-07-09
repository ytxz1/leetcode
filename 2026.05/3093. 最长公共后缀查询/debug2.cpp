#include <iostream>
#include <vector>
#include <string>
using namespace std;

int getCommonSuffixLength(const string& s1, const string& s2) {
    int i = s1.size() - 1, j = s2.size() - 1;
    int count = 0;
    while (i >= 0 && j >= 0 && s1[i] == s2[j]) {
        count++;
        i--;
        j--;
    }
    return count;
}

int main() {
    vector<string> container = {"abcdefgh", "poiuygh", "ghghgh"};
    vector<string> queries = {"gh", "acbfgh", "acbfegh"};
    
    for (const string& query : queries) {
        cout << "Query: " << query << endl;
        int bestIdx = 0;
        int bestSuffixLen = -1;
        int bestWordLen = container[0].size();
        
        for (int i = 0; i < container.size(); i++) {
            const string& word = container[i];
            int suffixLen = getCommonSuffixLength(query, word);
            
            cout << "  vs " << word << ": suffixLen=" << suffixLen;
            
            bool update = false;
            if (suffixLen > bestSuffixLen) {
                update = true;
                cout << " (longer suffix)";
            } else if (suffixLen == bestSuffixLen && word.size() < bestWordLen) {
                update = true;
                cout << " (shorter word)";
            } else if (suffixLen == bestSuffixLen && word.size() == bestWordLen && i < bestIdx) {
                update = true;
                cout << " (earlier index)";
            }
            
            if (update) {
                bestIdx = i;
                bestSuffixLen = suffixLen;
                bestWordLen = word.size();
                cout << " -> UPDATE bestIdx=" << i;
            }
            cout << endl;
        }
        cout << "Result: " << bestIdx << "\n" << endl;
    }
    return 0;
}
