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
#include <algorithm>
#include <unordered_map>

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
    void addBuyOrder(const string& op)
    {
        PriceBook temp;
        cout<<"add buy order: "<<endl
        temp.operation = op[1];
        cout<<"Operation="<< temp.operation <<endl
        temp.price = stoi(op[2]);
        cout<<"Price="<<temp.price<<endl;
        temp.quantity = stoi(op[3]);
        cout<<"Quantity="<<temp.quantity<<endl;
        temp.id = op[4];
        cout<<"Id="<<temp.id<<endl;
        m_buy.push_back(temp);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void addSellOrder(const string& op)
    {
        PriceBook temp;
        cout<<"add sell order: "<<endl
        temp.operation = op[1];
        cout<<"Operation="<< temp.operation <<endl
        temp.price = stoi(op[2]);
        cout<<"Price="<<temp.price<<endl;
        temp.quantity = stoi(op[3]);
        cout<<"Quantity="<<temp.quantity<<endl;
        temp.id = op[4];
        cout<<"Id="<<temp.id<<endl;
        m_sell.push_back(temp);
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void cancelOrder(const string& op)
    {
        string cancelId = op[1]; // second col is the order id
        // search buy pricebook
        auto it = m_buy.begin();
        for(; it != m_buy.end(); ++it)
        {
            if(it->id == cancelId)
            {
                m_buy.erase(it);
                cout<<"cancel id in buy pricebook"<<endl;
                return; // save time
            }
        }
        
        // search sell pricebook
        auto it_ = m_sell.begin();
        for(; it_ != m_sell.end(); ++it_)
        {
            if(it_->id == cancelId)
            {
                m_sell.erase(it_);
                cout<<"cancel id in sell pricebook"<<endl;
                return;
            }
        }
        
        // No return before!
        cout<<"[Warning]: no matched id is found! invalid cancel id "<<endl;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void modifyOrder(const string& op)
    {
        string modifiedId = op[1]; // second col is the wanted modified id
        string to = op[2]; // MODIFY order1 SELL 1000 10, the op[2] reveals where to search
        int price = stoi(op[3]);
        int quantity = stoi(op[4]);
        string operation; 
        
        
        // search in buy priceBook
        if(to == "SELL")
        {
            cout<< "modify from BUY to SELL"<<endl;
            auto it = m_buy.begin();
            for(; it != m_buy.end(); ++it)
            {
                if(it->id == modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    m_buy.erase(it);
                    cout<<"erase from BUY pricebook"<<endl;
                }
            }
            
            // add to sell book
            PriceBook temp;
            temp.id = modifiedId;
            temp.operation = operation;
            temp.price = price;
            temp.quantity = quantity;
            m_sell.push_back(temp);
            cout<<"add to SELL price book"<<endl;
            cout<< "id ="<<temp.id<<endl;
            cout<<"operation="<<temp.operation<<endl;
            cout<<"price="<<temp.price<<endl;
            cout<<"quantity="<<temp.quantity<<endl;
            
            return;
            
        }
        
        // search in the sell priceBook
        if(to == "BUY")
        {
            cout<< "modify from SELL to BUY"<<endl;
            auto it = m_sell.begin();
            for(; it != m_sell.end(); ++it)
            {
                if(it->id = modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    m_sell.erase(it);
                    cout<<"erase from SELL pricebook"<<endl;
                }
            }
            
            // add to Buy book
            PriceBook temp;
            temp.id = modifiedId;
            temp.operation = operation;
            temp.price = price;
            temp.quantity = quantity;
            m_buy.push_back(temp);
            cout<<"add to buy price book"<<endl;
            cout<< "id ="<<temp.id<<endl;
            cout<<"operation="<<temp.operation<<endl;
            cout<<"price="<<temp.price<<endl;
            cout<<"quantity="<<temp.quantity<<endl;
            
            return;
        }
        
        // No return before
        cout<<"[Warning]: invalid modification operation!"<<endl;
        return;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void print()
    {
        checkMatch();
        printMatch();
        printedSortedBooks();
    }
    
protected:
    /*
     * Check the matched trades in buy and sell pricebooks
     */
    bool checkMatch()
    {
        
    }
    
    /*
     * Print out the matched trades
     */
    bool printMatch()
    {
        
    }
    /*
     * Print out the sorted price books
     */
    bool printedSortedBooks()
    {
        
    }
    
private:

    vector<PriceBook> m_sell;
    vector<PriceBook> m_buy;
    Trader m_trader;  
};



int main() {
    
    
    MatchMachine jarvis;

    
    // Read the first col and switch by the cases.
    string op[5];
    cin << op[0];
    switch(op[0])
    {
        case "BUY":
            cin >> op[1];
            cin >> op[2];
            cin >> op[3];
            cin >> op[4];
            jarvis.addBuyOrder(op);
            break;

        case "SELL":
            cin >> op[1];
            cin >> op[2];
            cin >> op[3];
            cin >> op[4];
            jarvis.addSellOrder(op);
            break;

        case "CANCEL":
            cin >> op[1];
            javis.cancelOrder(op);
            break;

        case "MODIFY":
            cin >> op[1];
            cin >> op[2];
            cin >> op[3];
            cin >> op[4];
            javis.modifyOrder(op);
            break;

        case "PRINT":
            jarvis.print();
            break;

        default:
            cout<<"[Warning]: invalid input"<<endl;
    }
    
    
    
    
    
    
    
    
    
    return 0;
    
    
}