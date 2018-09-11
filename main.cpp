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

# define VERBOSE false
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
            cout<<"add BUY order: "<<endl;
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
            cout<<"add SELL order: "<<endl;
            cout<<"Operation="<< temp.operation <<endl;
            cout<<"Price="<<temp.price<<endl;
            cout<<"Quantity="<<temp.quantity<<endl;
            cout<<"Id="<<temp.id<<endl;
        }
        m_sell.push_back(temp);
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
    
    // TBD: What if partially modify the quantity? 
    // 1. Check the valid id
    // 2. Check not IOC
    bool modifyOrder(vector<string>& op)
    {

        string modifiedId = op[1]; // second col is the wanted modified id
        string to = op[2]; // MODIFY order1 SELL 1000 10, the op[2] reveals where to search
        int price = stoi(op[3]);
        int quantity = stoi(op[4]);
        string operation; 
        
        
        // search in both buy & sell priceBook
        if(to == "SELL")
        {
            bool isFound = false;
            if(VERBOSE) cout<< "modify from BUY to SELL"<<endl;
            // search in buy
            for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
            {
                if(it->id == modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    // check IOC
                    if(operation == "IOC")
                    {
                        if(VERBOSE) cout<<"cannot modify IOC operation";
                        return false;
                    }
                    m_buy.erase(it);
                    if(VERBOSE) cout<<"erase from BUY pricebook"<<endl;
                    isFound = true;
                    break;
                }

            }

            if(!isFound)
            {
                for(auto it = m_sell.begin(); it != m_sell.end(); ++it)
                {

                    if(it->id == modifiedId)
                    {
                        operation = it->operation; // buffer the operation
                        if(operation == "IOC")
                        {
                            if(VERBOSE) cout<< "cannot modify IOC operation";
                            return false;
                        }
                        m_sell.erase(it);
                        if(VERBOSE) cout<<"erase from SELL pricebook"<<endl;
                        isFound = true;
                        break;
                    }
                }
            }
            if(isFound)
            {
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

                return true;
            }
            else
            {
                if(VERBOSE) cout<<"cannot find the specified ID to modify!"<<endl;
                return false;
            }

            
        }
        
        // search in both buy and sell priceBook
        if(to == "BUY")
        {
            bool isFound = false;
            if(VERBOSE) cout<< "modify from SELL to BUY"<<endl; 
            // search in sell price book
            for(auto it = m_sell.begin(); it != m_sell.end(); ++it)
            {
                if(it->id == modifiedId)
                {
                    operation = it->operation; // buffer the operation
                    if(operation == "IOC")
                    {
                        if(VERBOSE) cout<<"cannot modify the IOC operation"<<endl;
                        return false;
                    }
                    m_sell.erase(it);
                    isFound = true;
                    if(VERBOSE) cout<<"erase from SELL pricebook"<<endl;
                    break;
                }

            }

            // search in buy pricebook
            if(!isFound)
            {
                for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
                {
                    if(it->id == modifiedId)
                    {
                        operation = it->operation;
                        if(operation == "IOC")
                        {
                            if(VERBOSE) cout<<"cannot modify the IOC operation"<<endl;
                            return false;
                        }
                        m_buy.erase(it);
                        isFound = true;
                        if(VERBOSE) cout<<"erase from BUY pricebook"<<endl;
                        break;
                    }
                }
            }
            
            if(isFound)
            {
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
                return true;
            }
            else
            {
                if(VERBOSE) cout<<"cannot find the specified ID to modify!"<<endl;
                return false;
            }
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void checkMatches(const string& target)
    {
        if(target == "BUY") checkBuy();
        if(target == "SELL") checkSell();
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void print()
    {
        printedSortedBooks();
    }
    
protected:
    /*
     * Check the matched trades in buy pricebooks
     * Log out the trade information
     */
    void checkBuy()
    {
        // boundary condition
        if(m_buy.size() == 0)
        {
            if(m_sell.back().operation == "IOC")
            {
                if(VERBOSE) cout<<"cancel the non-matched IOC query!"<<endl;
                m_sell.pop_back();
            }      
            return;
        }

        auto it = m_buy.begin();
        for(; it != m_buy.end() && !m_sell.empty(); /*empty*/ ) // follow the time-order!
        {
            if(it->price >= m_sell.back().price) // Buy price >= Sell price
            {
                // Log out
                int minQuantity = min(it->quantity, m_sell.back().quantity);

                cout<<"TRADE "<<it->id<<" "<<it->price<<" "<<minQuantity<<" "<<m_sell.back().id<<" "<<m_sell.back().price<<" "<<minQuantity<<endl;

                // Update information
                if(it->quantity == m_sell.back().quantity)
                {
                    if(VERBOSE) cout<<"quantity is equal, erase both"<<endl;
                    m_buy.erase(it);
                    m_sell.pop_back();
                    continue;
                }
                if(it->quantity > m_sell.back().quantity) // Buy amount > Sell amount
                {
                    if(VERBOSE) cout<<"buy amount > sell amount"<<endl;
                    it->quantity = it->quantity - m_sell.back().quantity; // update the quantity
                    if(VERBOSE) cout<<"update buy quantity="<<it->quantity<<endl;
                    m_sell.pop_back(); // erase from sell pricebook
                    if(VERBOSE) cout<<"erase sell pricebook"<<endl;
                    continue;
                }
                else // Sell amount > Buy amount
                {
                    if(m_sell.back().operation == "IOC")  // Req: If IOC is partially matches, cancel the rest.
                    {
                        if(VERBOSE) cout<<"cancel the PARTIALLY non-matched IOC query!"<<endl;
                        m_sell.pop_back();
                    }
                    else
                    {
                        if(VERBOSE) cout<<"sell amount > buy amount"<<endl;
                        m_sell.back().quantity = m_sell.back().quantity - it->quantity; // update the quantity
                        if(VERBOSE) cout<<"update sell quantity="<<m_sell.back().quantity<<endl;
                    }
                    m_buy.erase(it); // erase from buy pricebook
                    if(VERBOSE) cout<<"erase buy pricebook"<<endl;
                    continue;

                }
            }
            else
            {
                if(VERBOSE) cout<<"increment iterator"<<endl;
                ++it;
            }
        }
        // Req: If IOC can not be traded right away -> cancel it
        if(it == m_buy.end() && !m_sell.empty())
        {
            if(m_sell.back().operation == "IOC")
            {
                if(VERBOSE) cout<<"cancel the non-matched IOC query!"<<endl;
                m_sell.pop_back();
            }
        }

    }

    /*
     * Check the matched trades in sell pricebooks
     * Log out the trade information
     */
    void checkSell()
    {
        // boundary condition
        if(m_sell.size() == 0)
        {
            
            if(m_buy.back().operation == "IOC")
            {
                if(VERBOSE) cout<<"cancel the non-matched IOC query!"<<endl;
                m_buy.pop_back();
            }
            return;           
        }

        auto it = m_sell.begin();
        for(; it != m_sell.end() && !m_buy.empty(); /*empty*/ ) // follow the time-order!
        {
            if(it->price <= m_buy.back().price) // Sell price =< Buy price
            {
                int minQuantity = min(it->quantity, m_buy.back().quantity);
                // Log out
                cout<<"TRADE "<<it->id<<" "<<it->price<<" "<<minQuantity<<" "<<m_buy.back().id<<" "<<m_buy.back().price<<" "<<minQuantity<<endl;

                // Update information
                if(it->quantity == m_buy.back().quantity)
                {
                    m_sell.erase(it);
                    m_buy.pop_back();
                    continue;
                }
                if(it->quantity > m_buy.back().quantity) // Sell amount > Buy amount
                {
                    it->quantity = it->quantity - m_buy.back().quantity; // update the quantity
                    m_buy.pop_back(); // erase from buy pricebook
                    continue;
                }
                else // Buy amount > Sell amount
                {
                    if(m_buy.back().operation == "IOC")
                    {
                        if(VERBOSE) cout<<"cancel the PARTIALLY non-matched IOC query!"<<endl;
                        m_buy.pop_back();  // Req: If IOC is partially matches, cancel the rest.
                        
                    }
                    else
                    {
                        m_buy.back().quantity = m_buy.back().quantity - it->quantity; // update the quantity
                    }
                    m_sell.erase(it); // erase from sell pricebook
                    continue;
                }
            }
            else
            {
                ++it;
            }
        }

        // Req: If IOC can not be traded right away -> cancel it
        if(it == m_sell.end() && !m_buy.empty())
        {
        
            if(m_buy.back().operation == "IOC")
            {
                if(VERBOSE) cout<<"cancel the non-matched IOC query!"<<endl;
                m_buy.pop_back();
            }
        }

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
            std::map<price, quantity>::iterator it_; // map iterator
            it_ = sell.find(it->price);
            if( it_ != sell.end()) // the price is there already
            {
                it_->second += it->quantity; // simply add the qantity
                if(VERBOSE) cout<<"Add to the same map due to eqaul price"<<endl;
            }
            else
            {
                sell.insert(std::pair<price, quantity>(it->price, it->quantity));
                if(VERBOSE) cout<<"insert new element into map!"<<endl;
            }
        }

        
        for(auto it = m_buy.begin(); it != m_buy.end(); ++it)
        {
            std::map<price, quantity>::iterator it_; // map iterator
            it_ = buy.find(it->price);
            if( it_ != buy.end())
            {
                it_->second += it->quantity;
                if(VERBOSE) cout<<"Add to the same map due to eqaul price"<<endl;
            }
            else
            {
                buy.insert(std::pair<price, quantity>(it->price, it->quantity));
                if(VERBOSE) cout<<"insert new element into map!"<<endl;
            }

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
            // Boundary check:
            if(!(op[1] == "GFD" || op[1] == "IOC")) continue; // if the operation is invalid
            if(stoi(op[2]) <=0 || stoi(op[3]) <= 0) continue; // if price <=0 or quantity <=0
            if(op[4].empty()) continue; // if invalid id. TBD: what else is invalid id?

            if(jarvis.addBuyOrder(op))
            {
                jarvis.checkMatches("SELL");
            }
            else
            {
                if(VERBOSE) cout<<"[Warning]: non-unique order id for BUY pricebook"<<endl;
            }
        }

        if(op[0] == "SELL")
        {
            // Boundary check:
            if(!(op[1] == "GFD" || op[1] == "IOC")) continue; // if the operation is invalid
            if(stoi(op[2]) <=0 || stoi(op[3]) <= 0) continue; // if price <=0 or quantity <=0
            if(op[4].empty()) continue; // if invalid id. TBD: what else is invalid id?

            if(jarvis.addSellOrder(op))
            {
                jarvis.checkMatches("BUY");
            }
            else
            {
                if(VERBOSE) cout<<"[Warning]: non-unique order id for SELL pricebook"<<endl;
            }
        }

        if(op[0] == "CANCEL")
        {
            // Boundary check:
            if(op[1].empty()) continue; // if invalid id. TBD: what else is invalid id?
            jarvis.cancelOrder(op);
        }

        if(op[0] == "MODIFY")
        {
            // Boundary check:
            if(stoi(op[3]) <=0 || stoi(op[4]) <= 0) continue; // if price <=0 or quantity <=0

            if(jarvis.modifyOrder(op))
            {
                if(op[2] == "BUY") jarvis.checkMatches("SELL");
                if(op[2] == "SELL") jarvis.checkMatches("BUY");
            }
            else
            {
                if(VERBOSE) cout<<"[Warning]: invalid MODIFY operation"<<endl;
            }
 
        }

        if(op[0] == "PRINT")
        {
            jarvis.print();
        }
    }

   
    return 0;
    
    
}