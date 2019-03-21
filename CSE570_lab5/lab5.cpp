#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <iomanip>

using namespace std;

//read terminal and productions from file
void readterminals(vector<char> & t, vector<string> & p, int inputSize = 0, string filename="myG"){
  ifstream fileopen;
  fileopen.open(filename);//change this to make file read any
  string read = "";
  if(inputSize==2){cout << inputSize << endl;return;}//if user input not taken
  if(!fileopen.is_open()){
    cout << "not open\n";
    fileopen.close();
    return;
  }cout << "open\n\n";
  fileopen >> read;
  while (read!="$"){//stop symbol
    cout << read << "\n";
    t.push_back(read[0]);
    fileopen >> read;
  }
  // cout << "$\n";
  getline(fileopen,read);
  while (read!="$"){//stop symbol
    cout << read << "\n";
    if(read!=""){
      p.push_back(read);
    }
    getline(fileopen,read);
  }
  cout << "\n";
  fileopen.close();
}
// cout << "$\n";

//read nonterminals from production
void readnonterminals(set<char> & nt, vector<string> & p){
  set<char>::iterator iter;
  for(int i=0;i<p.size();i++){
    nt.insert(p.at(i)[0]);
  }
  // for(iter=nt.begin();iter!=nt.end();++iter){
  //   cout << *iter << endl;
  // }
  // cout << "end of nonterminals\n";
}

void readfirsts(map<char,set<char>> & mfi, vector<string> & p, set<char> & nt, vector<char> & t){
  //cout << "check\n";
  for(set<char>::iterator scitr=nt.begin();scitr!=nt.end();scitr++){//put all nonterminals in firsts
    set<char> emptysetter;
    if(mfi.find(*scitr)==mfi.end()){
      // cout << "new firsts map entry\n";
      // cout << *scitr << "\n";
      mfi.insert(pair<char,set<char>>(*scitr,emptysetter));
    }
  }
  for(int i=0;i<p.size();i++){
    // cout << "check first for loop\n";
    if(mfi.find(p.at(i)[0])!=mfi.end()){//check if lhs of productions is in firsts
      //make if statements
      for(int ii=i;ii<p.size();ii++){
        if(p[ii][0]==p[i][0]){//if lhs of production equals current nonterminal
          for(int j=1;j<p.at(ii).size();j++){
            //read productions and find firsts of lhs of productions
            if(p.at(ii)[j]=='-'){}//skip '-'
            else if(p.at(ii)[j]=='>'){}//skip '>'
            else if(p.at(ii)[j]==' '){}//skip ' '
            else if(p.at(ii)[j]>='A' && p.at(ii)[j]<='Z'){//nonterminal
              // cout << p.at(ii)[j] << endl;
              if(p.at(i)[0] != p.at(ii)[j]){
                mfi[p.at(i)[0]].insert(p.at(ii)[j]);
              }
              break;
            }else{//terminal
              mfi[p.at(i)[0]].insert(p.at(ii)[j]);
              break;
            }
          }
        }
      }
    }else{cout << "not found\n";}
  }
  // cout << "display firsts map and sets\n";
  // for(map<char,set<char>>::iterator mitr=mfi.begin();mitr!=mfi.end();mitr++){//go through firsts can correct nonterminal
  //   cout << "FIRST(" << mitr->first << ") = { ";
  //   for(set<char>::iterator scitr=mitr->second.begin();scitr!=mitr->second.end();scitr++){
  //     cout << *scitr << ", ";
  //   }
  //   cout << "}\n";
  // }
  // cout << "first half way done\n";
  //add terminals firsts to themselves
  for(int i=0;i<t.size();i++){//put all terminals into firsts
    set<char> setitself;
    setitself.insert(t.at(i));
    if(mfi.find(t.at(i))==mfi.end()){
      // cout << "new firsts map entry\n";
      mfi.insert(pair<char,set<char>>(t.at(i),setitself));
    }
  }
  // cout << "setting terminal firsts done\n";
  bool check = false;
  while (check!=true){//if there are still nonterminals in first sets
    check=true;
    for(map<char,set<char>>::iterator mitr=mfi.begin();mitr!=mfi.end();mitr++){//go through firsts can correct nonterminal
      for(set<char>::iterator scitr=(mitr->second.begin());scitr!=mitr->second.end();scitr++){
        //cout << (*scitr) << endl;
        if(*scitr>='A' && *scitr<='Z'){
          // cout << "terminal correction " << mitr->first << " first is " << *scitr << "\n";
          check=false;//there is still a nonterminal extra check
          mitr->second.insert(mfi.at(*scitr).begin(),mfi.at(*scitr).end());
          mitr->second.erase(*scitr);
          //cout << "terminal corrected " << *(scitr) << "\n";
          scitr = mitr->second.begin();//BIG FIX FIXED!!!
        }
        //cout << *scitr << endl;
      }
      //cout << "one loop map of first\n";
    }
  }
  // cout << "firsts done\n";
  // cout << "display firsts map and sets\n";
  // for(map<char,set<char>>::iterator mitr=mfi.begin();mitr!=mfi.end();mitr++){//go through firsts can correct nonterminal
  //   cout << "FIRST(" << mitr->first << ") = { ";
  //   for(set<char>::iterator scitr=mitr->second.begin();scitr!=mitr->second.end();scitr++){
  //     cout << *scitr << ", ";
  //   }
  //   cout << "}\n";
  // }
  //note: display code was here
  //cout << "end check\n";
}

void readfollows(map<char,set<char>> & mfo, vector<string> & p, set<char> & nt, map<char,set<char>> & fst){
  //cout << "check\n";
  for(set<char>::iterator scitr=nt.begin();scitr!=nt.end();scitr++){//put all nonterminals in follows
    set<char> emptysetter;
    if(mfo.find(*scitr)==mfo.end()){
      //cout << "new follows map entry\n";
      mfo.insert(pair<char,set<char>>(*scitr,emptysetter));
    }
  }//cout << "check productions\n";
  for(int i=0;i<p.size();i++){//go through productions
    //cout << "check for loop i=" << i << endl;
    for(int j=1;j<p.at(i).size();j++){
      //cout << "check for loop j=" << j << endl;
      if(p.at(i).at(j)>='A'&&p.at(i).at(j)<='Z'){
        //cout << "nonterminal found\n";
        if((j+1)==p.at(i).size()){//check for endofstring
          //push lhs nonterminal to follows map
          //cout << "end of line of production\n";
          mfo.at(p.at(i).at(j)).insert(p.at(i).at(0));
        }else if(p.at(i).at(j+1)>='A'&&p.at(i).at(j+1)<='Z'){
          //if follows of current nonterminal is nonterminal
          //'for now' push nonterminal to follows of that nonterminal
          //cout << "nonterminal follow found\n";
          //check the firsts of nonterminal with 'e'
          for(int k=(j+1);k<(p.at(i).size());k++){//turn all rhs nonterminals into first sets
            //if follow of nonterminal is nonterminal check for 'e'
            //cout << "in for loop\n";
            if(p.at(i).at(k)>='A' && p.at(i).at(k)<='Z'){
              //cout << "the follow is a nonterminal\n";
              //if 'e' is found append firsts to follows and continue k
      				if ((fst.at(p.at(i).at(k)).find('e')) != fst.at(p.at(i).at(k)).end()){//push firsts
                //cout << "the first of nonterminal contains 'e'\n";
                mfo.at(p.at(i).at(j)).insert(fst.at(p.at(i).at(k)).begin(),fst.at(p.at(i).at(k)).end());
                if((k)==(p.at(i).size()-1)){//if at end of line and firsts contain 'e' insert lhs nonterminal not firsts
                  //cout << "nonterminal before end of line contains 'e' insert lhs production and break\n";
                  mfo.at(p.at(i).at(j)).insert(p.at(i).at(0));
                  break;
                }//if not at last nonterminal with first 'e' continue
              }else{//if no 'e' found, push firsts, break
                //cout << "nonterminal has no 'e', break\n";
                mfo.at(p.at(i).at(j)).insert(fst.at(p.at(i).at(k)).begin(),fst.at(p.at(i).at(k)).end());
                break;
              }
            }else{//if next is not nonterminal, break
              //cout << "not nonterminal\n";
              mfo.at(p.at(i).at(j)).insert(p.at(i).at(k));
              break;
            }
          }
        //mfo.at(p.at(i).at(j)).insert(fst.at(p.at(i).at(j+1)).begin(),fst.at(p.at(i).at(j+1)).end());
        }else{//if follows of current nonterminal is terminal
          //push terminal to follows of that nonterminal
          //cout << "terminal follow found\n";
          mfo.at(p.at(i).at(j)).insert(p.at(i).at(j+1));
        }
      }
    }
  }
  //nonterminals in follows sets are now appened follows to their set
  //cout << "lhs productions follow sets\n";
  bool check=false;
  while(check!=true){
    check=true;
    for(map<char,set<char>>::iterator mitr=mfo.begin();mitr!=mfo.end();mitr++){//breakpoint
      if((mitr->second.find('e'))==mitr->second.end()){//'e' is not in follow sets
        mitr->second.erase('e');
      }
      for(set<char>::iterator scitr=(mitr->second).begin();scitr!=(mitr->second).end();scitr++){
        if((*scitr)>='A' && (*scitr)<='Z'){
          check=false;
          mitr->second.insert(mfo.at(*scitr).begin(),mfo.at(*scitr).end());
          mitr->second.erase(*scitr);
		      scitr = mitr->second.begin();
        }
      }
    }
  }
  //note: display code was here
}
void printFirstsnFollows(map<char,set<char>> & mfi, map<char,set<char>> & mfo){
  cout << "display firsts map and sets\n";
  for(map<char,set<char>>::iterator mitr=mfi.begin();mitr!=mfi.end();mitr++){//go through firsts can correct nonterminal
    cout << "FIRST(" << mitr->first << ") = { ";
    for(set<char>::iterator scitr=mitr->second.begin();scitr!=mitr->second.end();scitr++){
      cout << *scitr << ", ";
    }
    cout << "}\n";
  }
  //display follows map current state
  cout << "display follows map and sets\n";
  for(map<char,set<char>>::iterator mitr=mfo.begin();mitr!=mfo.end();mitr++){//go through firsts can correct nonterminal
    cout << "FOLLOW(" << mitr->first << ") = { ";
    for(set<char>::iterator scitr=mitr->second.begin();scitr!=mitr->second.end();scitr++){
      cout << *scitr << ", ";
    }
    cout << "}\n";
  }
  cout << "\n";
}
//------------------lab3----------------------
//initialize pproductions with the dot productions
void makePProductions(vector<string> & pp, vector<string> & p){
  pp.insert(pp.begin(),p.begin(),p.end());
  try{
    for(vector<string>::iterator i = pp.begin(); i != pp.end(); i++){
      if(((*i).find('.'))==string::npos){
        //assuming '>' is always there
        (*i).insert(((*i).find('>'))+1,1,'.');
      }
      //pp.push_back(i);
    }
  }catch(...){
    cout << "PProductions cannot be made\n";
  }
  // for(vector<string>::iterator i = pp.begin(); i != pp.end(); i++){
  //   cout << *i << endl;
  // }
}

/*
void testing(vector<string> & t){
  cout << t.at(0) << endl;
}
//can a set keep unique vectors of strings?
//can a set of vectors of strings be modified?
void test(vector<string> & t){
  testing(t);
  set<vector<string>> testSofVofStr;
  vector<string> testVofStr;
  vector<string> testVofStr2;
  vector<string> testVofStr3;
  testVofStr.push_back("GoodAhello1");
  testVofStr.push_back("GoodAhello2");
  testVofStr.push_back("GoodAhello3");
  testVofStr2.push_back("GoodBye1");
  testVofStr2.push_back("GoodBye2");
  testVofStr3 = testVofStr;
  testSofVofStr.insert(testVofStr);
  testSofVofStr.insert(testVofStr2);
  testSofVofStr.insert(testVofStr);
  testSofVofStr.insert(testVofStr3);
  testVofStr3 = testVofStr2;
  testSofVofStr.insert(testVofStr3);
  testSofVofStr.insert(testVofStr2);

  // testSofVofStr.push_back(testVofStr);
  // testSofVofStr.push_back(testVofStr2);
  // testSofVofStr.push_back(testVofStr);
  // testSofVofStr.push_back(testVofStr3);
  // testVofStr3 = testVofStr2;
  // testSofVofStr.push_back(testVofStr3);
  // testSofVofStr.push_back(testVofStr2);
  int count = 0;
  for(set<vector<string>>::iterator i = testSofVofStr.begin(); i != testSofVofStr.end(); i++){
    cout << "item #" << count++ << "\n";
    // (*i) has become const
    //vector<string> jbegin = i->begin();
    //vector<string> jend = i->end();
    for(vector<string>::const_iterator j = (*i).begin(); j != (*i).end(); j++){
      //*j = *j + " test";
      cout << *j << endl;
    }
  }
}
//yes, yes it can keep unique vector of strings.
//no, no it cannot be modified.
*/

//this kinda became the closure, so closure can call this
void findDotFollow(vector<string> & item, vector<string> & pp, char nt){
  //cout << "find dot follow\n";
  for(vector<string>::iterator i = pp.begin(); i != pp.end(); i++){
    if((*i).at(0)==nt){//check lhs of pproductions
      //check for uniques
      if(find(item.begin(), item.end(), *i) == item.end()){
        item.push_back(*i);
        //cout << "items push_back " << *i << endl;
      }
    }
  }
  // for(auto i: item){
  //   cout << i << endl;
  // }
  //after pushing follow of nt from pproductions into item
  //look into item's production and set next nt if
  //nonterinal follows the '.'
  char next;
  //cout << "get next nonterminal after dot\n";
  for(int i = 0; i < item.size(); i++){
    // cout << item.at(i).at(0) << endl;
    if(item.at(i).at(0) == nt){//find follow of '.' of production at all nt
      next = item.at(i).at(item.at(i).find('.')+1);
      //cout << next << " ";
      if(next != nt){
        //cout << next << endl;
        if(next >= 'A' && next <= 'Z'){//if nonterminal follow '.'
          // cout << "nonterminal after dot\n";
          findDotFollow(item, pp, next);//then find follows of it
          //return;
          // for(auto j: item){
          //   cout << j << endl;
          // }
        }else{
          // cout << "terminal after dot\n";
          return;
        }
        // cout << "check\n";
      }
      // cout << "check\n";
    }
    // cout << "check\n";
    // cout << item.at(i) << endl;
  }
  //cout << "find dot follow done\n";
}

//closure starts with I(0) already initialized
//needs condition for no more items
//return value will next set of items which is a vector of strings
vector<string> closure(vector<string> newitem, vector<string> &pp){//vector<string>
  //find the '.' and call goto if '.' is not at end
  //cout << "closure\n";
  vector<string> J = newitem;
  vector<string> addToJ;
  for(vector<string>::iterator i = newitem.begin(); i != newitem.end();i++){
    // string findthis = "";
    // findthis = findthis + '.' +
    //cout << *i << endl;
    if(((*i).find('.')+1) != (*i).size()){//end of string, end set
      //terminal after '.' has no follow, so end
      //cout << "check\n";
      if((*i).at((*i).find('.')+1) >= 'A' && (*i).at((*i).find('.')+1) <= 'Z'){
        char c = (*i).at((*i).find('.')+1);
        //cout << "find " << c << " in pproductions\n";
        findDotFollow(addToJ, pp, c);
        //return here?
      }
      //cout << "check\n";
    }
    //cout << "check\n";
  }
  J.insert(J.end(), addToJ.begin(), addToJ.end());
  //GOTOfunc();
  //cout << "done closure\n";
  return J;//vector<string>
}

//GOTO should be called before closure
//needs to set up and call the next closure.
//BIG CHANGE keep pproductions add item from items
vector<string> GOTOfunc(vector<string> &pp, vector<string> &item, char c){//vector<vector<string>>,grammar
  //call closure and check for repeating patterns or conflicts
  //pass a vector f strings that follow '.'
  //cout << "GOTO function\n";
  //cout << "GOTO(" << c << ")\n";
  vector<string> newSetofItem;
  for(vector<string>::iterator i = item.begin(); i != item.end(); i++){//BIG CHANE ++ to no ++
    //cout << *i << endl;
    string findthis = "";
    findthis = findthis + '.' + c;
    //cout << "find dot\n";
    if((*i).find(findthis) != string::npos){
      string moveDot = *i;
      moveDot.insert(moveDot.find(findthis)+2,1,'.');
      //cout << moveDot << "  =>>  ";
      moveDot.erase(moveDot.find('.'),1);
      //cout << moveDot << "\n";
      newSetofItem.push_back(moveDot);
    }
  }
  //cout << "GOTO to closure\n";
  return closure(newSetofItem, pp);
}

//items has to be built, item has first set, pp has dot productions, i can be used for index of items
void filloutItems(vector<vector<string>> & items, vector<string> item, vector<string> & pp, int i = 0, bool check = false){
  check = true;
  //cout << "fill out items\n";
  //cout << *item.begin() << endl;
  // for(auto i:item){
  //   cout << i << endl;
  // }
  for(vector<string>::iterator vi = item.begin(); vi != item.end(); vi++){
    //cout << "for loop\n";
    //cout << *vi << endl;
    //PUT HERE CHECKER FOR REPEATING GRAMMAR THEN INVALID GRAMMAR
    if((*vi).find('.') < ((*vi).size()-1)){//check if '.' is at end of line
      //cout << "dot is not at end of line\n";
      char c = (*vi).at((*vi).find('.')+1);
    //  cout << c << endl;
      vector<string> appitem = GOTOfunc(pp, item, c);
      if(find(items.begin(), items.end(), appitem) == items.end()){//
        //cout << "pushing to items\n";
        if(!appitem.empty()){
          //cout << "new set not empty\n";
          items.push_back(appitem);
          check = false;
        }
      }
    }
  }
  //checker here
  ++i;
  //cout << "index of items at " << i << " and size of items " << items.size() << endl;
  if(items.size()>i){
    //cout << "recurse filloutItems\n";
    filloutItems(items,items.at(i),pp,i);
  }

  //filloutItems(items, item, pp, ++i);
  //if()
}

// bool checker(vector<vector<string>> & items){
//   // if(!!items.empty()){
//   //   return false;
//   // }
//   for(vector<vector<string>>::iterator vvi = items.begin(); vvi != items.end(); vvi++){
//     for(vector<string>::iterator vi = (*vvi).begin(); vi != (*vvi).end(); vi++){
//       cout << *vi << endl;
//       for(string::iterator si = (*vi).begin(); si != (*vi).end(); si++){
//         cout << *si << endl;
//         if(*si=='.'){
//           cout << "found dot\n";
//           if(si==(*vi).end()){
//             cout << "dot is at end of the line\n";
//             return true;
//           }else{
//             return false;
//           }
//         }
//       }
//     }
//   }
//   return false;
// }

//initialize all items in itemsOfProductions
//do vector of vector to keep track of state
void initializeItems(vector<vector<string>> &items, vector<string> &pp, vector<vector<string>> &lr0){
  //cout << "check\n";
  int count = 0;
  int loop = 0;
  for(vector<string>::iterator i = pp.begin(); i != pp.end(); i++){//for LR(0): i != pp.begin()+2;
    //cout << "item from pproductions " << count++ << "\n";
    vector<string> item;
    //cout << "check for loop1\n";
    //assuming all pp have a '.' in the string
    char c = (*i).at((*i).find('.')+1);
    //cout << "check for loop2\n";
    if(loop == 1){count = items.size();}
    item.push_back(*i);
    //cout << "check for loop3\n";
    findDotFollow(item, pp, c);
    //cout << "check for loop4\n";
    //checker for keeping unique vector<strings>
    bool unique = true;
    //cout << "unique check\n";
    for(vector<vector<string>>::iterator j = items.begin(); j != items.end();j++){
      if(item == *j){
        unique = false;
      }
    }
    //if new item is not in items add it
    if(unique){
      items.push_back(item);
      //cout << "unique is " << c << endl;
      filloutItems(items,item,pp);
      /*code below could work not trying, leaving bad code for refernce for bad code*/
      // char nt;
      // bool check = false;
      // int countingItems = 0;
      // while(check != true){
      //   //vector<vector<string>> appitems;
      //   check = true;
      //   for(vector<vector<string>>::iterator ii = items.begin()+2; ii != items.end(); ii++){
      //     filloutItems();
      //     for(vector<string>::iterator j = ++item.begin(); j != item.end();j++){
      //       nt = (*j).at((*j).find('.')+1);
      //       cout << "production " << *j << endl;
      //       cout << "a grammar " << nt << endl;
      //       vector<string> appitem;
      //       appitem = GOTOfunc(pp,nt);
      //       // if(search(item.begin(), item.end(), appitem.begin(), appitem.end())==item.end()){
      //       //   item.insert(item.end(), appitem.begin(), appitem.end());
      //       // }
      //       if(find(items.begin(), items.end(), appitem) == items.end()){
      //         //if(find(appitems.begin(), appitems.end(), appitem) == appitems.end()){
      //           items.push_back(appitem);//with I(0) set find rest
      //           check = false;
      //           cout << "pushed to items\n";
      //         //}
      //       }
      //     }
      //     cout << "get next item\n";
      //     item = *ii;
      //     cout << "check\n";
      //   }
      // }

    }
    if(loop == 0){
      lr0 = items;
    }
    loop++;
  }/*
  cout << "\ndisplay Items";
  for(int i = 0; i < items.size(); i++){
    if(i == count){cout << "\nEnd of LR(0) set\n";}
    cout << "\nI(" << i << "):\n";
    for(int j = 0; j < items.at(i).size(); j++){
      cout << items.at(i).at(j) << endl;
    }
  }*/
}
//------------------lab5----------------------
int findShiftState(vector<vector<string>> &lr0, string &substr){//assuming that
  for(int i = 0; i < lr0.size(); i++){
    for(int j = 0; j < lr0.at(i).size(); j++){
      size_t found = lr0.at(i).at(j).find(substr);
      if(found!=string::npos){
        return i;
      }
    }
  }
  return NULL;
}

int findReduceState(vector<string> &pp, string &dprod){
  //find production to reduce state
  for(int i = 0; i < pp.size(); i++){
    if(pp.at(i)==dprod){
      //cout << "reduce index " << i << endl;
      return i;
    }
  }
  return NULL;
}

vector<vector<string>> makeTable(vector<vector<string>> &lr0, vector<char> &t, set<char> &nt, vector<string> &pp, map<char,set<char>> mfo){
  vector<vector<string>> table;
  vector<string> row;
  //display LR0
  cout << "LR0 set\n";
  for(int i = 0; i < lr0.size(); i++){
    cout << "I(" << i << ")\n";
    for(int j = 0; j < lr0.at(i).size(); j++){
      cout << lr0.at(i).at(j) << endl;
    }
    cout << endl;
  }
  //cout << "make table\n";
  for(int i = 0; i < t.size(); i++){
    string s(1, t.at(i));
    row.push_back(s);
  }
  row.push_back("$");
  for(set<char>::iterator i = nt.begin(); i != nt.end(); i++){
    string s;
    s += *i;
    row.push_back(s);
  }
  for (int i = 0; i < lr0.size(); i++){//initialize table
    table.push_back(row);
  }
  /* Must treat lr0 as 3d container and table as 2d container */
  //lr0set - table
  for(int i = 0; i < lr0.size(); i++){
    //go through: items - states - I(#)
    for(int j = 0; j < lr0.at(i).size(); j++){
      //go through: productions with dot
      string::iterator findDot = find(lr0.at(i).at(j).begin(), lr0.at(i).at(j).end(), '.');//char
      string::iterator prodend = lr0.at(i).at(j).end();//char
      string pproduction = lr0.at(i).at(j);
      // cout << pproduction << endl;
      if(findDot != prodend){
        //dot exists in current production
        string s = "";
        if(next(findDot) != prodend){
          //dot is not endline shift insert to table
          string::iterator ppitr = find(pproduction.begin(), pproduction.end(), '.');
          pproduction.insert(ppitr+2,'.');
          pproduction.erase(ppitr);
          //cout << "shift dot production: " << pproduction << endl;
          s = "S";
          string charToStr;
          charToStr += *next(findDot);
          vector<string>::iterator insertToTable = find(table.at(i).begin(),table.at(i).end(),charToStr);//string
          int state = findShiftState(lr0, pproduction);//find state to shift to
          s = s + to_string(state);
          //cout << s << endl;
          // for(int j = 0; j < table.at(i).size(); j++){
          //   cout << table.at(i).at(j) << ", ";
          // }cout << endl;
          //this tells us if we have a conflict: shift-reduce
          if(insertToTable != table.at(i).end()){
            //this condition is if we do not have a conflict
            *insertToTable = s;
          }else if(find(table.at(i).begin(),table.at(i).end(),s) != table.at(i).end()){
            //not shift reduce conflict
            //SLR allows shift-reduce, ignore error
            // cout << "State I(" << i << ") has same state " << s << " reached no conflict.\n";
          }else{//this condition is if conflict exists
            //SLR allows shift-reduce, ignore error
            // if(find(pproduction.begin(), pproduction.end(), '.') != pproduction.end()-1){//when dot is not shift-shift(not an error)
            //   cout << "Grammar is not SLR, s-r/s-s conflict exists at state: I(" << state << "), action/goto: " << charToStr << "\n";
            //   //cout << " When writing from state I(" << i << ")" << endl;
            // }
            //return vector<vector<string>>();
          }
          //*insertToTable =
        }else if(next(findDot) == prodend){
          //dot is endline reduce insert to table
          string::iterator ppitr = find(pproduction.begin(), pproduction.end(), '.');
          pproduction.erase(ppitr);
          ppitr = find(pproduction.begin(),pproduction.end(), '>')+1;
          pproduction.insert(ppitr,'.');
          //cout << "reduce dot production: " << pproduction << endl;
          s = "R";
          int state = findReduceState(pp, pproduction);
          s = s + to_string(state);
          //get lhs of production for its follows
          vector<char> lhsFollows(mfo.at(pproduction.at(0)).begin(),mfo.at(pproduction.at(0)).end());
          //cout << Str << endl;
          lhsFollows.push_back('$');
          for(auto &Str:lhsFollows){
            //default insert '$' always
            string charToStr;
            charToStr += Str;
            vector<string>::iterator insertToTable = find(table.at(i).begin(),table.at(i).end(),charToStr);//string
            //this tells us if we have a conflict: reduce-reduce
            if(insertToTable != table.at(i).end()){
              //this condition is if we do not have a conflict
              //cout << s << endl;
              if(s == "R0"){//change condition to if "S->S" or "S'"
                //cout << s << endl;
                //cout << charToStr << endl;
                if(charToStr == "$"){
                  s = "acc";
                }else{s=" ";}
                //cout << s << endl;
              }
              //cout << s << endl;
              *insertToTable = s;
            }else{
              cout << "Grammar is not SLR, reduce-reduce conflict exists state: I(" << i << "), action/goto: " << Str << "\n";
              cout << " When writing state " << s << endl;
              //return vector<vector<string>>();
            }
          }
        }
      }
    }
  }
  // for(int i = 0; i < errorvec.size(); i++){
  // }
  //clean up table
  for(int i = 0; i < table.size(); i++){
    for(int j = 0; j < table.at(i).size(); j++){
      if(table.at(i).at(j) == row.at(j)){
        //clean up terminals and nonterminals
        table.at(i).at(j) = " ";
      }
    }
  }
  return table;
}

void displayTable(vector<vector<string>> &LRtable, vector<string> &p, vector<char> &t, set<char> &nt){
  //display the reduce states: productions
  //display the shift states: items
  //display table
  //col: action & Gotos : terminals and nonterminals
  //row: states: items
  vector<string> row;
  for(int i = 0; i < t.size(); i++){
    string s(1, t.at(i));
    row.push_back(s);
  }
  row.push_back("$");
  for(set<char>::iterator i = nt.begin(); i != nt.end(); i++){
    string s;
    s += *i;
    row.push_back(s);
  }
  int count = 0;
  cout << "SLR Parsing Table\n";
  cout << setw(3) << "I" << setw(t.size()*2+9) << "action" << setw((row.size())*2+5) << "Gotos\n|";
  cout << setw(5) << "|" << setw(3);
  count = count + 8;
  for (vector<char>::iterator i = t.begin(); i != t.end(); i++){
    cout << (*i) << setw(4);
    count = count + 4;
  }
  cout << "$" << setw(4);
  cout << "|" << setw(3);
  count = count + 7;
  for (set<char>::iterator i = nt.begin(); i != nt.end(); i++){
    cout << (*i) << setw(4);
    count = count + 4;
  }
  cout << "|" << endl << "|";
  for (int i = 0; i < count-1; i++){
    cout << "-";
  }
  cout << "|" << endl;
  for (int i = 0; i != LRtable.size(); i++){
    cout << "|" << setw(3) << i << setw(2) << "|" << setw(4);
    int counterRow = 0;
    for(vector<string>::iterator j = LRtable.at(i).begin(); j != LRtable.at(i).end(); j++){
      if(*j == "acc" || row.at(counterRow) == "$"){
        cout << *j << setw(3) << "|" << setw(4);
      }else{
        cout << *j << setw(4);
      }
      counterRow++;
    }
    cout << "|\n";
  }
  //output of table:
  //   Parsing Table
  //need to add action and Goto above here
  //   |  I |  0  1  e  S  T  U  |
  //   |-------------------------|
}

int main(){
  /*
    IMPORTANT NOTE OPTIMIZE LATER
  */
  //need all five variables below are important to complete lab2
  string input="";//not used
  vector<char> terminals;
  set<char> nonterminals;
  vector<string> productions;//lhs is index 0 and "->" is rhs
  vector<string> pproductions;//dot productions
  map<char,set<char>> mfirsts;
  map<char,set<char>> mfollows;
  vector<vector<string>> itemsOfProductions;//sets do not keep track of states
  vector<vector<string>> LR0set;
  vector<vector<string>> LR0parsingtable;
  /*  --------------------Possible errors/fixes:----------------------------
      e:first production must contain augment symbol not used throughout
        the grammar so S is not the only augment or start symbol
      f:do not use augment symbol throughout the grammar only for S->S
        and S->(nonterminal of the lhs of following production)
      e:S is has to be first production
      f:implement S' when productions are created put S' add to input
        productions so user does not have to add it
        e:if user adds S' themselves then 2 S' productions
        f:delete added S' production, recognize S' production and do nothing
      e:S shows up in table as nonterminal
      f:delete S from table
      f:implement S' and recognize as augment symbol and search for it
        and identify as accepting state and not final state,
        so that S can stay as nonterminal
      e:
  --------------------------------------------------------------------------
  */
  cout << "Enter terminals and grammar (S-> is augment do not treat like nonterminal): \n";
  int c = 0;
  while( !(input == "no" || c == 2) ){//"no" input or input(exit on second dollar symbol)
    cin >> input;
    if(input == "$"){
      c++;
      cout << c << "\n";
      if(c==1){
        //productions.push_back("");
      }
    }
    else if(input == "no"){//erase any possible input to terminals and productions
      vector<string> emptyVS;
      vector<char> emptyVC;
      productions = emptyVS;
      terminals = emptyVC;
      cout << "exit on " << input << "\n";
    }//pass through "no"
    else{
      if(c==1){//productions
        //question: why does the offset of one line ruin output?
        cout << "push production\n";
        productions.push_back(input);
      }else{//terminals only push first char of input. can space char ruin input for terminals
        cout << "push terminal\n";
        terminals.push_back(input.at(0));
      }
    }
  }

  // for(int i = 0; i < terminals.size(); i++){cout << terminals.at(i) << endl;}
  // for(int i = 0; i < productions.size(); i++){cout << productions.at(i) << endl;}
  // cout << "done " << c << "\n";
  readterminals(terminals, productions, c);//create/fill terminals/productions
  // cout << "done terminals\n";
  readnonterminals(nonterminals, productions);
  // cout << "done nonterminals\n";
  readfirsts(mfirsts, productions, nonterminals, terminals);
  // cout << "done firsts\n";
  readfollows(mfollows, productions, nonterminals, mfirsts);
  // cout << "done second\n";
  printFirstsnFollows(mfirsts,mfollows);
  makePProductions(pproductions, productions);//make dot productions
  // cout << "done dot productions\n";
  try{
    //use dot productions to make items
    initializeItems(itemsOfProductions,pproductions,LR0set);
  }catch(...){
    cout << "Possible invalid grammar.\nPossible missing \"S->S\" as the start/first production.\nCannot have loop or repeat.\nS->A\nA->S\nOr need S\'->S == \"S->S\", cannot take \"S\'\".";
  }
  LR0parsingtable = makeTable(LR0set, terminals, nonterminals, pproductions, mfollows);
  if(LR0parsingtable != vector<vector<string>>()){
    displayTable(LR0parsingtable,productions,terminals,nonterminals);
  }else{
    cout << "No table\n";
  }
  // vector<string> test1;
  // test1.push_back("hello test testing");
  // test(test1);
}
