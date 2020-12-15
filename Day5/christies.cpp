#include <forward_list>
#include <stack>
#include <ctime>
#include <iostream>

struct Friend {
  bool isMale;
  int popularity;

  Friend(bool __isMale, int __popularity): isMale{__isMale}, popularity{__popularity} {

  }
};

using namespace std;

const size_t INITIAL_FREINDS_AMOUNT {30};
const size_t POPULARITY_RANGE {100};
const size_t DELETE_RATIO_IN_PERCENT {25};

void initFriendsList(std::forward_list<Friend>& friends) {

  srand(time(NULL)); /// !!! VERY IMPORTANT FOR THE RANDOMNESS OF rand() !!!

  for (size_t i{0}; i < INITIAL_FREINDS_AMOUNT; ++i) {
    bool _isMale {(rand() % 100) < 50}; // when less than 50, 1 -> male, otherwise 0 -> female
    size_t _popularity {rand() % POPULARITY_RANGE}; // a random number in the range 0 - 100
    friends.push_front(Friend(_isMale, _popularity));
  }

}

ostream& operator<<(ostream& o, const Friend& fri) {
  o << "[";
  if (fri.isMale) {
    o << "m";
  } else {
    o << "w";
  }
  o << ", " << fri.popularity << "]";

  return o;
}

void print(const std::forward_list<Friend>& friends) {
  for (const Friend& fr : friends) {
     cout << fr << "-> ";
  }
  cout << endl;
}

int main() {
  forward_list<Friend> friendsList;
  size_t friendsAmount = INITIAL_FREINDS_AMOUNT;
  initFriendsList(friendsList);
  stack<Friend> deletedFriends;
  size_t algoIterations{0};
  bool deletionImpossible{false};
  
  cout << "Friends list before deletion algorithm: " << endl;
  print(friendsList);

  
  do {

    size_t sizeBeforeAlgo = friendsAmount; 
    size_t sizeDeletedBeforeAlgo = deletedFriends.size();

    cout << algoIterations + 1 << ". iteration:" << endl; 

    for (forward_list<Friend>::iterator it {friendsList.begin()}; it != friendsList.end(); ++it) {
      forward_list<Friend>::iterator copyIt{it};
      Friend firstFriend = *copyIt;

      if (++copyIt == friendsList.end()) {
        break;
      }

      Friend secondFriend = *(copyIt);

      cout << " Pair: " << firstFriend << secondFriend;

      if (firstFriend.popularity > secondFriend.popularity && secondFriend.isMale) { // Delete a Friend given the criteria.
        deletedFriends.push(*it);
        friendsList.erase_after(it);
        --friendsAmount;

      } else if (!deletedFriends.empty() && secondFriend.isMale && deletedFriends.top().popularity > secondFriend.popularity) {
        cout << " At deletedFriends top: " << deletedFriends.top() << " => " << deletedFriends.top() << " swaps " << secondFriend;
        *(copyIt) = deletedFriends.top();
        deletedFriends.pop();

      } else { // Nothing's been deleted. The pair remains -> ++it so that we go to the next pair.
        ++it;
      }

      cout << endl;
    }

    size_t sizeAfterAlgo = friendsAmount;
    size_t sizeDeletedAfterAlgo = deletedFriends.size();

    if (sizeBeforeAlgo == sizeAfterAlgo && sizeDeletedBeforeAlgo == sizeDeletedAfterAlgo) {
      deletionImpossible = true;
    }

    ++algoIterations;
  } while (friendsAmount <= INITIAL_FREINDS_AMOUNT - INITIAL_FREINDS_AMOUNT * DELETE_RATIO_IN_PERCENT && !deletionImpossible); 

  cout << "The algorithm has stopped after " << algoIterations << " iterations." << endl;

  cout << "Friends list after deletion: " << endl;
  print(friendsList);

  cout << endl;
  cout << "| Friends amount before deletion: " << INITIAL_FREINDS_AMOUNT << endl;
  cout << "| Friends amount after deletion: " << friendsAmount << endl;
  cout << "| which is " << 100 * ((float) (INITIAL_FREINDS_AMOUNT - friendsAmount) / INITIAL_FREINDS_AMOUNT) << "% less." << endl;

  if ( 100 * ((float) (INITIAL_FREINDS_AMOUNT - friendsAmount) / INITIAL_FREINDS_AMOUNT) < 25 ) {
    cout << "| Therefore the algotirhm has stopped, because it couldn't delete more friends. " << endl;
  } else {
    cout << "| Therefore the algorithm has stopped, because it deleted 25% or more of the friends." << endl;
  }
  cout << endl;

  return 0;
}