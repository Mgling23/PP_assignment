#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    time_t start = clock();
    int containerSize = 0;
    string item[26], rule[26];
    int vw[26][2] = { 0 }, rulevw[26][2] = { 0 };
    int amountI = 0, amountR = 0;
    string line;
    ifstream inFile;
    ofstream outFile;
    inFile.open("D:\\User\\Documents\\PP\\KNSK\\problem.txt");
    outFile.open("D:\\User\\Documents\\PP\\KNSK\\output.txt");

    if (outFile.fail()) {
        //cout << "Out file cannot open";
    }
    if (inFile.fail()) {
        //cout << "Unable to open the file! " << endl;
    }
    else {
       // cout << "successfully open" << endl;
        getline(inFile, line);
        inFile >> containerSize;
        inFile.ignore();
        getline(inFile, line);
        while (getline(inFile, line) && line != "Rules:") {
            stringstream ss(line);
            ss >> item[amountI] >> vw[amountI][0] >> vw[amountI][1];
            amountI++;
        }
        while (getline(inFile, line)) {
            stringstream ss(line);
            ss >> rule[amountR] >> rulevw[amountR][0] >> rulevw[amountR][1];
            amountR++;
        }
        // Dynamic programming to solve the knapsack problem
        vector<vector<int>> c(amountI + 1, vector<int>(containerSize + 1, 0)); // c[i][j] will store the maximum value that can be achieved with i items and j capacity
        vector<vector<bool>> chosen(amountI + 1, vector<bool>(containerSize + 1, false)); // Track which items are chosen

        for (int i = 1; i <= amountI; ++i) {
            for (int w = 1; w <= containerSize; ++w) {
                if (w >= vw[i - 1][0]) { // Check if current capacity is enough to include the ith item
                    // Determine if we should include the item based on rules
                    bool canInclude = true;
                    for (int r = 0; r < amountR; ++r) {
                        for (int j = 0; j < i-2; j++) {
                            
                            if (rule[r].find(item[i - 1])<rule[r].length() && rule[r].find(item[j]) < rule[r].length()) {
                                if (w < vw[i - 1][0] + rulevw[r][0] || c[i - 1][w - vw[i - 1][0]] < rulevw[r][1]) {
                                    canInclude = false;
                                    break;
                                }
                                

                            }
                        }

                        
                    }
                    if (canInclude) {
                        c[i][w] = max(c[i - 1][w], c[i - 1][w - vw[i - 1][0]] + vw[i - 1][1]);
                        chosen[i][w] = true; // Mark the item as chosen
                    }
                    else {
                        c[i][w] = c[i - 1][w];
                    }
                }
                else {
                    c[i][w] = c[i - 1][w];
                }
            }
        }

        // Find which items are chosen and their weights
        vector<int> chosenItems;
        int remainingCapacity = containerSize;
        for (int i = amountI; i > 0 && remainingCapacity > 0; --i) {
            if (chosen[i][remainingCapacity]) {
                chosenItems.push_back(i);
                remainingCapacity -= vw[i - 1][0];
            }
        }

        // Output chosen items and their weights
        //cout << "Chosen items: ";
        int r = 0;
        vector<bool> isPrinted(amountR);
        for (int i = chosenItems.size() - 1; i >= 0; --i) {
            //cout << item[chosenItems[i] - 1] << " ";
            for (r = 0; r < amountR; ++r) {
               
                for (int j = chosenItems.size() - 1; j >= 0; --j) {
                    
                    if (i == j)
                        continue;
                    if (rule[r].find(item[chosenItems[j] - 1]) < rule[r].length() && rule[r].find(item[chosenItems[i] - 1]) < rule[r].length()) {
                        if (!isPrinted[r]) {
                            outFile << rule[r] << endl;
                            isPrinted[r] = true;
                            
                        }
                    }
                    //cout << item[chosenItems[i] - 1] << " ";

                }
                if (isPrinted[r])
                    continue;
            }

        
            if (isPrinted[r])
                continue;
            outFile << item[chosenItems[i] - 1] << endl;
        }
        
        
     
       
        //cout << endl;
        //cout << "Total weight of chosen items: " << containerSize - remainingCapacity << endl;
    }

    time_t stop = clock();

    // Pause the program to view the output
    outFile.close();
    inFile.close();
    //cout << "The time is "<< stop - start << "ms";
    //cin.get();

    return 0;
}
