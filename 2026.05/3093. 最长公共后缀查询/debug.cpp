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
    string query = "acbfgh";
    
    cout << "Query: " << query << endl;
    for (int i = 0; i < container.size(); i++) {
        int len = getCommonSuffixLength(query, container[i]);
        cout << "vs " << container[i] << ": suffix len = " << len << endl;
    }
    
    // Simulate the logic
    int bestIdx = 0;
    int bestSuffixLen = -1;
    int bestWordLen = container[0].size();
    
    for (int i = 0; i < container.size(); i++) {
        const string& word = container[i];
        int suffixLen = getCommonSuffixLength(query, word);
        
        cout << "\ni=" << i << ", word=" << word << ", suffixLen=" << suffixLen;
        cout << ", bestSuffixLen=" << bestSuffixLen << ", bestWordLen=" << bestWordLen;
        
        if (suffixLen > bestSuffixLen) {
            cout << " -> Case 1: suffixLen > bestSuffixLen";
            bestIdx = i;
            bestSuffixLen = suffixLen;
            bestWordLen = word.size();
        } else if (suffixLen == bestSuffixLen && word.size() < bestWordLen) {
            cout << " -> Case 2: same suffix, shorter word";
            bestIdx = i;
            bestWordLen = word.size();
        } else if (suffixLen == bestSuffixLen && word.size() == bestWordLen && i < bestIdx) {
            cout << " -> Case 3: same suffix, same len, earlier index";
            bestIdx = i;
        } else {
            cout << " -> No update";
        }
        cout << ", bestIdx=" << bestIdx << endl;
    }
    
    cout << "\nFinal bestIdx: " << bestIdx << endl;
    return 0;
}
