#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <unordered_map>

# define VERBOSE true
using namespace std;

/*
 * Base structure of Price book
 */
struct PriceBook
{
    string id;         // Order Id 
    string operation;  // GFD or IDF
    int price;        
    int quantity;     
};

/*
 * Match machine is responsible for
 * 1. Storing the Buy and Sell orders
 * 2. Check the matched trades
 * 3. Output the sorted Book
 */
class MatchMachine
{
    
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    bool addBuyOrder(vector<string>& op)
    {
        bool res = true;

        PriceBook temp;
        temp.id = op[4];

        // first check the id is unique
        for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
        {
            if(it->id == temp.id)
            {
                res = false;
                if(VERBOSE) cout<<"Duplicated Buy order id!!"<<endl;
                return res;
            }
        }

        temp.operation = op[1];
        temp.price = std::stoi(op[2]);
        temp.quantity = std::stoi(op[3]);
        if(VERBOSE)
        {
            cout<<"add buy order: "<<endl;
            cout<<"Operation="<< temp.operation <<endl;
            cout<<"Price="<<temp.price<<endl;
            cout<<"Quantity="<<temp.quantity<<endl;
            cout<<"Id="<<temp.id<<endl;
        }
        m_buy.push_back(temp);
        return res;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    bool addSellOrder(vector<string>& op)
    {
        bool res = true;

        PriceBook temp;
        temp.id = op[4];

        // first check the id is unique
        for(auto it = m_sell.begin(); it != m_sell.end(); ++it)
        {
            if(it->id == temp.id)
            {
                res = false;
                if(VERBOSE) cout<<"Duplicated SELL order id!!"<<endl;
                return res;
            }
        }

        temp.operation = op[1];
        temp.price = stoi(op[2]);
        temp.quantity = stoi(op[3]);
        if(VERBOSE)
        {
            cout<<"add buy order: "<<endl;
            cout<<"Operation="<< temp.operation <<endl;
            cout<<"Price="<<temp.price<<endl;
            cout<<"Quantity="<<temp.quantity<<endl;
            cout<<"Id="<<temp.id<<endl;
        }
        m_buy.push_back(temp);
        return res;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void cancelOrder(const vector<string>& op)
    {
        string cancelId = op[1]; // second col is the order id
        // search buy pricebook
        for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
        {
            if(it->id == cancelId)
            {
                m_buy.erase(it);
                if(VERBOSE) cout<<"cancel id in buy pricebook"<<endl;
                return; // save time
            }
        }
        
        // search sell pricebook
        for(auto it_ = m_sell.begin(); it_ != m_sell.end(); ++it_)
        {
            if(it_->id == cancelId)
            {
                m_sell.erase(it_);
                if(VERBOSE) cout<<"cancel id in sell pricebook"<<endl;
                return;
            }
        }
        
        // No return before!
        if(VERBOSE) cout<<"[Warning]: no matched id is found! invalid cancel id "<<endl;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void modifyOrder(vector<string>& op)
    {
        string modifiedId = op[1]; // second col is the wanted modified id
        string to = op[2]; // MODIFY order1 SELL 1000 10, the op[2] reveals where to search
        int price = stoi(op[3]);
        int quantity = stoi(op[4]);
        string operation; 
        
        
        // search in buy priceBook
        if(to == "SELL")
        {
            if(VERBOSE) cout<< "modify from BUY to SELL"<<endl;
            for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
            {
                if(it->id == modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    m_buy.erase(it);
                    if(VERBOSE) cout<<"erase from BUY pricebook"<<endl;
                }
            }
            
            // add to sell book
            PriceBook temp;
            temp.id = modifiedId;
            temp.operation = operation;
            temp.price = price;
            temp.quantity = quantity;
            m_sell.push_back(temp);
            if(VERBOSE)
            {
                cout<<"add to SELL price book"<<endl;
                cout<<"id ="<<temp.id<<endl;
                cout<<"operation="<<temp.operation<<endl;
                cout<<"price="<<temp.price<<endl;
                cout<<"quantity="<<temp.quantity<<endl;
            }
            
            return;
            
        }
        
        // search in the sell priceBook
        if(to == "BUY")
        {
            if(VERBOSE) cout<< "modify from SELL to BUY"<<endl;
            for(auto it = m_sell.begin(); it != m_sell.end(); ++it)
            {
                if(it->id == modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    m_sell.erase(it);
                    if(VERBOSE) cout<<"erase from SELL pricebook"<<endl;
                }
            }
            
            // add to Buy book
            PriceBook temp;
            temp.id = modifiedId;
            temp.operation = operation;
            temp.price = price;
            temp.quantity = quantity;
            m_buy.push_back(temp);
            if(VERBOSE)
            {
                cout<<"add to buy price book"<<endl;
                cout<< "id ="<<temp.id<<endl;
                cout<<"operation="<<temp.operation<<endl;
                cout<<"price="<<temp.price<<endl;
                cout<<"quantity="<<temp.quantity<<endl;
            }
            return;
        }
        
        // No return before
        if(VERBOSE) cout<<"[Warning]: invalid modification operation!"<<endl;
        return;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void checkMatches()
    {
        if(isTrade())
            printMatches();
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void print()
    {
        printedSortedBooks();
    }
    
protected:
    /*
     * Check the matched trades in buy and sell pricebooks
     */
    bool isTrade()
    {
        bool res = false;
        // Boundary condition check
        if(m_buy.size() == 0 || m_sell.size() == 0) return res;

        


        return res;
    }
    
    /*
     * Print out the matched trades
     */
    void printMatches()
    {
        
    }
    /*
     * Print out the sorted price books
     */
    void printedSortedBooks()
    {
        typedef int price;
        typedef int quantity;
        
        // Use map to sorted & merged the result
        map<price, quantity> sell;
        map<price, quantity> buy;

        
        for(auto it = m_sell.begin(); it != m_sell.end(); ++it)
        {
            sell.insert(std::pair<price, quantity>(it->price, it->quantity));
        }

        
        for(auto it_ = m_buy.begin(); it_ != m_buy.end(); ++it_)
        {

            buy.insert(std::pair<price, quantity>(it_->price, it_->quantity));
        }

        // The requirement want to bring the descending order!
        cout<<"SELL:"<<endl;
        
        for(auto it_sell = sell.rbegin(); it_sell != sell.rend(); ++it_sell)
        {
            cout<<it_sell->first<<" "<<it_sell->second<<endl;
        }

        cout<<"BUY:"<<endl;
        
        for(auto it_buy = buy.rbegin(); it_buy != buy.rend(); ++it_buy)
        {
            cout<<it_buy->first<<" "<<it_buy->second<<endl;
        }

        return;
    }
    
private:

    vector<PriceBook> m_sell;
    vector<PriceBook> m_buy;
};



int main() {
    
    
    MatchMachine jarvis;
    string line;
    while(std::getline(cin, line))
    {
        // Read the first col and switch by the cases.
        istringstream iss(line);
        vector<string> op{std::istream_iterator<string>(iss), {}};
        if(op[0] == "BUY")
        {
            if(jarvis.addBuyOrder(op))
            {
                jarvis.checkMatches();
            }
            else
            {
                if(VERBOSE) cout<<"[Warning]: non-unique order id for BUY pricebook"<<endl;
            }
        }

        if(op[0] == "SELL")
        {
            if(jarvis.addSellOrder(op))
            {
                jarvis.checkMatches();
            }
            else
            {
                if(VERBOSE) cout<<"[Warning]: non-unique order id for SELL pricebook"<<endl;
            }
        }

        if(op[0] == "CANCEL")
        {
            jarvis.cancelOrder(op);
        }

        if(op[0] == "MODIFY")
        {
            jarvis.modifyOrder(op);
            jarvis.checkMatches();
        }

        if(op[0] == "PRINT")
        {
            jarvis.print();
        }
    }

   
    return 0;
    
    
}