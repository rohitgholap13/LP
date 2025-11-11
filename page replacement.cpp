#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

// FIFO Page Replacement
int fifo(const vector<int> &pages, int frames)
{
    unordered_set<int> set;
    queue<int> q;
    int page_faults = 0;

    for (int page : pages)
    {
        if (set.find(page) == set.end())
        {

            if ((int)set.size() == frames)
            {
                int front = q.front();
                q.pop();
                set.erase(front);
            }
            set.insert(page);
            q.push(page);
            page_faults++;
        }
    }
    return page_faults;
}

// LRU Page Replacement
int lru(const vector<int> &pages, int frames)
{
    unordered_set<int> set;
    unordered_map<int, int> indexes;
    int page_faults = 0;

    for (int i = 0; i < (int)pages.size(); i++)
    {
        int page = pages[i];
        if (set.find(page) == set.end())
        {

            if ((int)set.size() == frames)
            {

                int lru_page = -1;
                int min_index = i;
                for (int p : set)
                {
                    if (indexes[p] < min_index)
                    {
                        min_index = indexes[p];
                        lru_page = p;
                    }
                }
                set.erase(lru_page);
                indexes.erase(lru_page);
            }
            set.insert(page);
            page_faults++;
        }
        indexes[page] = i;
    }
    return page_faults;
}

// Optimal Page Replacement
int optimal(const vector<int> &pages, int frames)
{
    unordered_set<int> set;
    int page_faults = 0;

    for (int i = 0; i < (int)pages.size(); i++)
    {
        int page = pages[i];
        if (set.find(page) == set.end())
        {
            if ((int)set.size() == frames)
            {

                int index_to_replace = -1;
                int farthest = i;
                for (int p : set)
                {
                    int j;
                    for (j = i + 1; j < (int)pages.size(); j++)
                    {
                        if (pages[j] == p)
                            break;
                    }
                    if (j == (int)pages.size())
                    {
                        index_to_replace = p;
                        break;
                    }
                    if (j > farthest)
                    {
                        farthest = j;
                        index_to_replace = p;
                    }
                }
                set.erase(index_to_replace);
            }
            set.insert(page);
            page_faults++;
        }
    }
    return page_faults;
}

int main()
{
    int frames ;
    cout << "Enter number of frames: ";
    cin >> frames;

    int n ;
    cout << "Enter number of pages in reference string: ";
    cin >> n ;

    vector<int> pages(n);
    cout << "Enter the reference string pages: ";
    for (int i = 0; i < n; i++)
    {
        cin >> pages[i];
    }

    cout << "\nPage Faults using FIFO: " << fifo(pages, frames) << "\n";
    cout << "Page Faults using LRU: " << lru(pages, frames) << "\n";
    cout << "Page Faults using Optimal: " << optimal(pages, frames) << "\n";

    return 0;
}