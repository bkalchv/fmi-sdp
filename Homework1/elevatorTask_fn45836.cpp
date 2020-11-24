// F - floors count
// L - first elev. capacity
// S - second elev. capacity
// R - amount of requests for the day
// Elevators' speed - 5 seconds per 1 stage

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <list>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

int F; // Floors amount
int L; // Elevator L's capacity
int S; // Elevator S's capacity
int R; // Requests amount for the day
const double ELEVATORS_SPEED{0.2}; // Floors per second
enum ELDIRECTION {up, down, onhold};
unsigned int currentTime{0}; // In seconds

class Output;
vector <Output> outputLog;

class Output {
  private:
    string elevatorName;
    ELDIRECTION direction;
    int floorNr;
    unsigned int time;

  public:
    // No "natural" default value -> no default constructor
    // Also: initialisation of Output objects happen without it directly being accessed by the user 

    Output(string _elevatorName, ELDIRECTION _direction, int _floorNr, unsigned int _time): elevatorName{_elevatorName}, direction{_direction}, floorNr{_floorNr}, time{_time} {
    }

    Output(const Output& other): elevatorName{other.elevatorName}, direction{other.direction}, floorNr{other.floorNr}, time{other.time} {
    }

    Output& operator=(const Output& rhsOutput) {
      if (this == &rhsOutput) return *this;

      elevatorName = rhsOutput.elevatorName;
      direction = rhsOutput.direction;
      floorNr = rhsOutput.floorNr;
      time = rhsOutput.time;

      return *this;
    }

    unsigned int getOutputTime() const {
      return time;
    }
    
    friend ostream& operator<<(ostream& os, const Output& output);
}; 

class Elevator;

class Person {
  private:
    string direction;
    int fromFloorNr;
    int toFloorNr;
    unsigned int timeOfCall; // in seconds
    bool inElevator;

  public:
    // No "natural" default value -> no default constructor

    Person(string _direction, int _fromFloorNr, int _toFloorNr, unsigned int _timeOfCall) {

      if ( (_fromFloorNr < 1 && _fromFloorNr > F) || (_toFloorNr < 1 && _toFloorNr > F) ) {
        throw new invalid_argument("Floor's number is exceeding the floors of the building.");
      }

      setDirection(_direction);
      setFromFloorNr(_fromFloorNr);
      setToFloorNr(_toFloorNr);
      setTimeOfCall(_timeOfCall);
      inElevator = false;
    }

    Person(const Person& other): direction{other.direction}, fromFloorNr{other.fromFloorNr}, toFloorNr{other.toFloorNr}, timeOfCall{other.timeOfCall}, inElevator{other.inElevator} { 
    }

    Person& operator=(const Person& rhsPerson) {
      if (this == &rhsPerson) return *this;

      direction = rhsPerson.direction;
      fromFloorNr = rhsPerson.fromFloorNr;
      toFloorNr = rhsPerson.toFloorNr;
      timeOfCall = rhsPerson.timeOfCall;
      inElevator = rhsPerson.inElevator;

      return *this;
    }

    bool operator==(const Person& other) {
      return direction == other.direction && fromFloorNr == other.fromFloorNr && toFloorNr == other.toFloorNr && timeOfCall == other.timeOfCall;
    }

    bool operator!=(const Person& other) {
      return !(*this == other);
    }

    string getDirection() const {
      return direction;
    }

    void setDirection(string _direction) {
    
      if (_direction != "UP" && _direction != "DOWN") throw new invalid_argument("Directions different than 'up' or 'down'!");

      this->direction = _direction;
    }

    int getFromFloor() {
      return fromFloorNr;
    }

    void setFromFloorNr(int _fromFloorNr) {
      this->fromFloorNr = _fromFloorNr;
    }

    int getToFloorNr() const {
      return toFloorNr;
    }

    void setToFloorNr(int _toFloorNr) {
      this->toFloorNr = _toFloorNr;
    }

    unsigned int getTimeOfCall() {
      return timeOfCall;
    }

    void setTimeOfCall(unsigned int _timeOfCall) {

      if (_timeOfCall < 0) throw new invalid_argument("Time can't be less than 0.");

      this->timeOfCall = _timeOfCall;
    }

    bool getInElevator() {
      return inElevator;
    }

    void setInElevator(bool _inElevator) {
      this->inElevator = _inElevator;
    }

    friend ostream& operator<<(ostream& os, const Person& p);

    void call(Elevator&);
    void go(Elevator&);
    Elevator& pickAnElevator(Elevator&, Elevator&);
};

class Floor {
  private:
    int floorNr;
    queue<Person> queueOfPeopleAtFloor;

  public:

    Floor(int _floorNr): queueOfPeopleAtFloor{queue<Person>()} {
      setFloorNr(_floorNr);
    }

    Floor(const Floor& other): floorNr{other.floorNr}, queueOfPeopleAtFloor{other.queueOfPeopleAtFloor} {

    }

    Floor& operator=(const Floor& rhsFloor) {
      if (this == &rhsFloor) return *this;

      floorNr = rhsFloor.floorNr;
      queueOfPeopleAtFloor = rhsFloor.queueOfPeopleAtFloor;

      return *this;
    }

    int getFloorNr() const {
      return floorNr;
    }

    void setFloorNr(int floorNr) {
      if (floorNr < 1 || floorNr > F) throw new invalid_argument("Floor's number is exceeding the floors of the building.");

      this->floorNr = floorNr;
    }

    queue<Person> getQueueOfPeopleAtFloor() const {
      return queueOfPeopleAtFloor;
    }

    queue<Person>& getQueueOfPeopleAtFloor() {
      return queueOfPeopleAtFloor;
    }

    void setQueueOfPeopleAtFloor(queue<Person> _queueOfPeople) {
      this->queueOfPeopleAtFloor = _queueOfPeople;
    }

    void enquePerson(const Person& person) {
      queueOfPeopleAtFloor.push(person);
    }

    void dequeuePerson() {
      if (!queueOfPeopleAtFloor.empty()) {
        queueOfPeopleAtFloor.pop();
      }
    }

    void printQueueAtFloor() {
      queue<Person> copyQueue = queueOfPeopleAtFloor;
      while(!copyQueue.empty()) {
        cout << copyQueue.front() << " ";
        copyQueue.pop();
      }
    }

    bool containsPerson(const Person& person) const {
      bool contains{false};
      queue<Person> copyQueue = queueOfPeopleAtFloor;
      while (!copyQueue.empty()) {
        if (copyQueue.front() == person) {
          contains = true;
          break;
        }
        copyQueue.pop();
      }

      return contains;
    }
};

class Elevator {

  private:
    string name;
    int capacity;
    int atFloorNr;
    ELDIRECTION elDirection;
    vector<Floor>* floors; // vector due to no deletion, but fast access thanks to operator[]; pointer, because both Elevators should be operating on the same vector of floors
    list<Person> peopleIn; // list, because of often deletion
    double speed;
    unsigned int timeSinceStartOfSimulation; // the time that's went by since the start of the simulation
  
  public:

    Elevator(int _capacity, string _name): peopleIn{list<Person>()}, timeSinceStartOfSimulation{0} {
      setName(_name);
      setCapacity(_capacity);
      setAtFloorNr(1);
      setElDirection(ELDIRECTION::onhold);
      setSpeed(ELEVATORS_SPEED);
    }

    Elevator(const Elevator& other) {
      name = other.name;
      capacity = other.capacity;
      atFloorNr = other.atFloorNr;
      elDirection = other.elDirection;
      floors = other.floors;
      peopleIn = other.peopleIn;
      speed = other.speed;
      timeSinceStartOfSimulation = other.timeSinceStartOfSimulation;
    }

    Elevator& operator=(const Elevator& rhsElevator) {
      if (this == &rhsElevator) return *this;

      name = rhsElevator.name;
      capacity = rhsElevator.capacity;
      atFloorNr = rhsElevator.atFloorNr;
      elDirection = rhsElevator.elDirection;
      floors = rhsElevator.floors;
      peopleIn = rhsElevator.peopleIn;
      speed = rhsElevator.speed;
      timeSinceStartOfSimulation = rhsElevator.timeSinceStartOfSimulation;

      return *this;      
    }

    string getName() const {
      return name;
    }
    
    void setName(string _name) {
      if (_name.size() == 0) throw new invalid_argument("Name can't be an empty string!");
      this->name = _name;
    }

    int getCapacity() const {
      return capacity;
    }

    void setCapacity(int _capacity) {
      if (_capacity <= 0) throw new invalid_argument("An elevator's capacity can't be less than or equal to 0!"); 
      this->capacity = _capacity;
    }

    int getAtFloorNr() const {
      return atFloorNr;
    }

    void setAtFloorNr(int _atFloorNr) { // No need for exception throwing, because when initializing an Elevator it'll always be on floor 1.
      this->atFloorNr = _atFloorNr;
    }

    ELDIRECTION getElDirection() const {
      return elDirection;
    }

    void setElDirection(const ELDIRECTION& direction) {
      this->elDirection = direction;
    }

    vector<Floor>* getFloors() {
      return floors;
    }

    void setFloors(vector<Floor>* _floors) {
      if ((*_floors).size() <= 1) throw new invalid_argument("Floors amount less than 1. It makes no sense for a building to have less than 1 floor and an elevator!");
      this->floors = _floors;
    }

    list<Person> getPeopleIn() const {
      return peopleIn;
    }

    double getSpeed() const {
      return speed;
    }

    void setSpeed(double _speed) {

      if (_speed != ELEVATORS_SPEED) throw new invalid_argument("Invalid speed!");

      this->speed = _speed;
    }

    int getPeopleInAmount() const {
      return peopleIn.size();
    }

    unsigned int getTimeSinceStartOfSimulation() const {
      return timeSinceStartOfSimulation;
    }

    bool isFull() {
      return (getPeopleInAmount() == capacity) ?  true : false;
    }

    static string elDirectionToString(const ELDIRECTION& enumDirection) {
      if (enumDirection == ELDIRECTION::up) {
        return "UP";
      } else if (enumDirection == ELDIRECTION::down) {
        return "DOWN";
      } else {
        return "ONHOLD";
      }
    }  

    // Elevator takes passangers from the given Floor fromFloor
    // Returns a bool - whether a passanger got in an elevator or not.
    bool takePassangersIn(Floor& fromFloor, unsigned int timeOfCall) {

      bool tookPassangersIn = false;

      queue<Person>& queueAtFloor = fromFloor.getQueueOfPeopleAtFloor();
      queue<Person> queueAtFloorCopy = queueAtFloor;
      
      while (!queueAtFloorCopy.empty() && !isFull()) {
        Person currentPassanger = queueAtFloor.front();

        if ( (elDirection == ELDIRECTION::onhold || Elevator::elDirectionToString(elDirection) == currentPassanger.getDirection()) 
              && ( abs((int) timeOfCall - (int) currentPassanger.getTimeOfCall()) <= abs(fromFloor.getFloorNr() - atFloorNr) / speed || currentPassanger.getTimeOfCall() < timeSinceStartOfSimulation ) ) {
                   
          peopleIn.push_back(currentPassanger);
          currentPassanger.setInElevator(true); 
          //cout << currentPassanger << " got in." << endl;
          tookPassangersIn = true;
          queueAtFloor.pop();
          queueAtFloorCopy.pop();
          
        } else {
          queueAtFloorCopy.pop();
          continue;
        }
      }

      return tookPassangersIn;

    }

    // Elevator goes to Floor targetFloor and executes other requests if possible
    void goToFloor(const Floor& targetFloor, const string& direction, const unsigned int& timeOfCall) {

      int targetFloorNr = targetFloor.getFloorNr();

      if (atFloorNr == targetFloorNr && peopleIn.empty()) { // base of edge case's recursion 
        this->setElDirection(ELDIRECTION::onhold);
        return;
      }

      unsigned int timeInSecondsForCurrentGo = timeOfCall;

      if (targetFloorNr > atFloorNr) {
        this->setElDirection(ELDIRECTION::up);
      } else if (targetFloorNr < atFloorNr) {
        this->setElDirection(ELDIRECTION::down);
      }

      for (int currentAtFloorNr{atFloorNr}; currentAtFloorNr != targetFloorNr;) {
        
        if (currentAtFloorNr != atFloorNr) {
          timeInSecondsForCurrentGo += 1/speed;
        }

        if (!(*floors)[currentAtFloorNr-1].getQueueOfPeopleAtFloor().empty()) {

          if (currentAtFloorNr != atFloorNr) { // Otherwise there's no need for a stop - the elevator is already at the desired floor
            currentTime = timeSinceStartOfSimulation + timeInSecondsForCurrentGo;
            //cout << "***Elevator stops*** on " << currentAtFloorNr << "fl. at "  << currentTime << endl;  
            outputLog.push_back(Output(name, elDirection, currentAtFloorNr, currentTime));
          }

          takePassangersIn((*floors)[currentAtFloorNr-1], timeOfCall);
        } 
        
        if (!peopleIn.empty()) { // There are people in the elevator at currentFloorNr, whose toFloorNr may be equal to currentFloorNr, so they should get off the elevator
          vector<list<Person>::iterator> leavingPeoplePositions;
          for (list<Person>::iterator it {peopleIn.begin()}; it != peopleIn.end(); ++it) {
            if (it->getToFloorNr() == currentAtFloorNr) {
              leavingPeoplePositions.push_back(it);
            }
          }

          if (!leavingPeoplePositions.empty()) {
            bool alreadyStoppedAtFloor{false};
            for (const list<Person>::iterator& position : leavingPeoplePositions) {
            currentTime =  timeSinceStartOfSimulation + timeInSecondsForCurrentGo;
            if (!alreadyStoppedAtFloor) {
                //cout << "***Elevator stops*** on " << currentAtFloorNr << "fl. at " << currentTime << endl;
                outputLog.push_back(Output(name, elDirection, currentAtFloorNr, currentTime));
                alreadyStoppedAtFloor = true;
            }
            //cout << *position << " got off." << endl;
            peopleIn.erase(position);
            }
            leavingPeoplePositions.clear();
          }

        }
                
        if (targetFloorNr > atFloorNr) {
          this->setElDirection(ELDIRECTION::up);
          ++currentAtFloorNr;
        } else if (targetFloorNr < atFloorNr) {
          this->setElDirection(ELDIRECTION::down);
          --currentAtFloorNr;
        }
      }
      
      setAtFloorNr(targetFloorNr);
      timeInSecondsForCurrentGo += 1/speed;
      currentTime = timeSinceStartOfSimulation + timeInSecondsForCurrentGo;
      //cout << "***Elevator stops***" << name << " on " << atFloorNr << "fl. at " << currentTime << endl;
      outputLog.push_back(Output(name, elDirection, atFloorNr, currentTime));
      
      if (!peopleIn.empty() && atFloorNr == targetFloorNr) { //There are people in the elevator at targetFloorNr, whose toFloorNr may be equal to targetFloorNr, so they should get off the elevator
        
        vector<list<Person>::iterator> leavingPeoplePositions;
        for (list<Person>::iterator it {peopleIn.begin()}; it != peopleIn.end(); ++it) {
          if (it->getToFloorNr() == atFloorNr) {
            leavingPeoplePositions.push_back(it);
          }
        }
        for (const list<Person>::iterator& position : leavingPeoplePositions) {
          //cout << *position << " got off." << endl;
          peopleIn.erase(position);
        }
      }

      if (!peopleIn.empty() && Elevator::elDirectionToString(elDirection) == direction) { // There are poeple in the elevator at targetFloorNr, whose toFloorNr is another floor, but they've gotten onto the elevator, therefore the elevator should get them to their toFloorNr(s) and then it'll be ::onhold::

        int highestFloorDifference {0};
        Person personWithHighestFloorDifference(peopleIn.front());
        for (const Person& p : peopleIn) {
          if (highestFloorDifference < abs(atFloorNr - p.getToFloorNr())) {
            highestFloorDifference = abs(atFloorNr - p.getToFloorNr());
            personWithHighestFloorDifference = p;
          }
        }

        int goToFloorIdx;

        if (atFloorNr < personWithHighestFloorDifference.getToFloorNr()) {
          goToFloorIdx  = atFloorNr + highestFloorDifference - 1;
        } else if (atFloorNr > personWithHighestFloorDifference.getToFloorNr()) {
          goToFloorIdx = atFloorNr - highestFloorDifference - 1;
        } else {
          goToFloorIdx = atFloorNr - 1;
        }

        if (atFloorNr != (goToFloorIdx+1)) {
          //cout << "- RECURSION CALLED -" << endl;
          //cout << personWithHighestFloorDifference << endl;
          goToFloor((*floors)[goToFloorIdx], personWithHighestFloorDifference.getDirection(), currentTime);
        }

      }

      if (timeSinceStartOfSimulation == 0) {
        timeSinceStartOfSimulation = timeInSecondsForCurrentGo;
      } else {
        timeSinceStartOfSimulation += timeInSecondsForCurrentGo;
      }

      this->setElDirection(ELDIRECTION::onhold);
    }

    friend ostream& operator<<(ostream& os, const Elevator& el);
};

void Person::call(Elevator& elevator) {
  
  Floor& floorOfCall = elevator.getFloors()->at(fromFloorNr - 1);

  if (fromFloorNr != elevator.getAtFloorNr()) {
    elevator.goToFloor(floorOfCall, direction, timeOfCall);
  }

  elevator.takePassangersIn(floorOfCall, timeOfCall);
}

void Person::go(Elevator& elevator) {
  Floor& floorToGoTo = elevator.getFloors()->at(toFloorNr - 1);
  elevator.goToFloor(floorToGoTo, direction, timeOfCall);
}

// Picks an elevator for the given person/ request, based on the logic in the assignment.
Elevator& Person::pickAnElevator(Elevator& elL, Elevator& elS) {
  int floorDifferenceElevatorL = abs(this->fromFloorNr - elL.getAtFloorNr());
  int floorDifferenceElevatorS = abs(this->fromFloorNr - elS.getAtFloorNr());
  int floorDifferenceOfRequest = abs(this->fromFloorNr - this->toFloorNr);
  unsigned int timeForCallRequestToBeExecutedByL = floorDifferenceElevatorL / ELEVATORS_SPEED;
  unsigned int timeForCallRequestToBeExecutedByS = floorDifferenceElevatorS / ELEVATORS_SPEED;
  unsigned int timeForGoRequestToBeExecuted = floorDifferenceOfRequest / ELEVATORS_SPEED;

  if ( (elL.getElDirection() == ELDIRECTION::onhold && elS.getElDirection() == ELDIRECTION::onhold) || ( Elevator::elDirectionToString(elL.getElDirection()) == this->getDirection() &&  Elevator::elDirectionToString(elS.getElDirection()) == this->getDirection()) ) {
    
    if(elL.getTimeSinceStartOfSimulation() != elS.getTimeSinceStartOfSimulation()) {
      if (elL.getTimeSinceStartOfSimulation() + (timeForCallRequestToBeExecutedByL + timeForGoRequestToBeExecuted) < elS.getTimeSinceStartOfSimulation() + (timeForCallRequestToBeExecutedByS + timeForGoRequestToBeExecuted)) {
        return elL;
      } else if (elL.getTimeSinceStartOfSimulation() + (timeForCallRequestToBeExecutedByL + timeForGoRequestToBeExecuted) > elS.getTimeSinceStartOfSimulation() + (timeForCallRequestToBeExecutedByL + timeForGoRequestToBeExecuted)) {
        return elS;
      }
    }  

    if (floorDifferenceElevatorL < floorDifferenceElevatorS) {
    return elL;
    } else if (floorDifferenceElevatorL > floorDifferenceElevatorS) {
    return elS;
    } else {
    return elL;
    }
    
    
  } else if ( Elevator::elDirectionToString(elL.getElDirection()) == this->getDirection() && ( Elevator::elDirectionToString(elS.getElDirection()) != this->getDirection())) {
    return elL;
  } else if ((Elevator::elDirectionToString(elL.getElDirection()) != this->getDirection()) && Elevator::elDirectionToString(elS.getElDirection()) == this->getDirection()) {
    return elS;
  } else {
    throw new std::logic_error("Unhandled case!");
  }

}

ostream& operator<<(ostream& os, const Person& p) {
  os << " [Going " << p.direction << ", From: " << p.fromFloorNr << " fl., To: " << p.toFloorNr << " fl., At: " << p.timeOfCall << "s.]";
  return os; 
}

ostream& operator<<(ostream& os, const Elevator& el) {
  os << endl << "ELEVATOR: " << el.name << " TimeWentBy: " << el.timeSinceStartOfSimulation << endl;
  os << "| At: " << el.atFloorNr << " fl. |" << endl;
  os << "| Direction: ";
  if (el.getElDirection() == ELDIRECTION::up) {
    os << "UP |" << endl;
  } else if (el.getElDirection() == ELDIRECTION::down) {
    os << "DOWN |" << endl;
  } else {
    os << "ONHOLD |" << endl;
  }
  os << "| ";
  
  if (!el.getPeopleIn().empty()) {
    for (const Person& p : el.peopleIn) {
    os << p << " ";
    }
    os << " are in | ";
  } else {
    os << "*blank* |";
  }

  os << endl;
  
  return os;
}

ostream& operator<<(ostream& os, const Output& output) {
  os << output.elevatorName << " " << Elevator::elDirectionToString(output.direction) << " " << output.floorNr << " " << output.time;
  return os; 
}

// Finds all the substrings in a string and puts each of them in a vector.
// Returns a vector of substrings.
vector<string> stringToSubstringsVector(const string& str) {
  vector<string> result;
  istringstream iss (str);

  do {
    string sub;
    iss >> sub;
    result.push_back(sub);
  } while (iss);

  return result;
}

// Finds all the integers in a string and puts each of them in a vector.
// Returns a vector of integers.
vector<int> stringToIntVector(const string& s) {
  vector<int> result;  
  stringstream str_strm;
  str_strm << s; //convert the string s into stringstream
  string temp_str;
  int temp_int;
  while(!str_strm.eof()) {
    str_strm >> temp_str; //take words into temp_str one by one
    if(stringstream(temp_str) >> temp_int) { //try to convert string to int
      result.push_back(temp_int);
    }
    temp_str = ""; //clear temp string
  }
  return result;
}

// Parses a text file in the format given in the description's task to a queue of Person objects
// Each Person object has the fields: Direction, fromFloor, ToFloor, TimeOfCallRequest
void parseTextFileContentToQueueOfPeople(const string& filesName, queue<Person>& log) {

  ifstream fileLogsStream(filesName);

  string line;
  string twoLines;
  int lineNr{1};
  while (getline (fileLogsStream, line)) {

    if (lineNr == 1) {
      
      vector<int> firstLineIntegers = stringToIntVector(line);

      if (firstLineIntegers.size() == 4) {
        F = firstLineIntegers[0];
        L = firstLineIntegers[1];
        S = firstLineIntegers[2];
        R = firstLineIntegers[3];
      } else {
        throw new runtime_error("First line of the input file is lacking arguments!");
      }

    } else {
    
      if ( lineNr % 2 == 0) {
        twoLines += line;
      } else {

        if (line.empty()) {
          throw new runtime_error("Uneven amount of Calls and Gos!");
        }

        twoLines = twoLines + " " + line;

        vector<string> objectArgumentsVector = stringToSubstringsVector(twoLines);

        string objectDirection = objectArgumentsVector[1];
        int objectFromFloor = stoi(objectArgumentsVector[2]);
        int objectTimeSinceStart = stoi(objectArgumentsVector[3]);
        int objectToFloor = stoi(objectArgumentsVector[5]);

        log.push( Person(objectDirection, objectFromFloor, objectToFloor, objectTimeSinceStart) );

        twoLines = "";
      }

    }
    
    ++lineNr;
  }

}

// Initializes Floor objects for each of the given amount of floors
void initFloors(int floorsAmount, vector<Floor>& floors) {
  for (int i{1}; i <= floorsAmount; ++i) {
    Floor newFloor = Floor(i);
    floors.push_back(newFloor);
  }
}

// Initializes a 'building', a.k.a binds both elevators to a vector of floors, on which both would be working.
void initBuilding(vector<Floor>& floors, Elevator& elevatorL, Elevator& elevatorS) {
  elevatorL.setFloors(&floors);
  elevatorS.setFloors(&floors);
}

// Allocates people/requests to a floor's queue accordingly.
void allocatePeopleToFloors(queue<Person> logOfPeopleCopy, vector<Floor>& floors) {
  while(!logOfPeopleCopy.empty()) {
    Person currentFront = logOfPeopleCopy.front();
    int currentFrontFrom = currentFront.getFromFloor(); // Actual floor
    floors[currentFrontFrom - 1].enquePerson(currentFront); // index
    logOfPeopleCopy.pop();
  }
}

// Displays the building visually in the console
void displayBuilding(vector<Floor>& floors) {
  cout << "The building looks like:" << endl;

  for(size_t i {floors.size()} ; i > 0; --i) { // [5 - 1]
    cout << "--------" << endl;
    cout << "| " << i << "fl. |" ;
    floors[i-1].printQueueAtFloor();
    cout << endl;
    cout << "--------" << endl;
  }
}

// Custom sorter
bool output_sorter(const Output& output1, const Output& output2) {
  if ( output1.getOutputTime() != output2.getOutputTime() ) {
    return  output1.getOutputTime() < output2.getOutputTime();
  }
  return false;
}

// Sorts outputs by time, using std::sort from the <algorithm> library and output_sorter.
void sortOutputs(vector<Output>& outputLogToSort) {
  std::sort(outputLogToSort.begin(), outputLogToSort.end(), &output_sorter);
}

// Starts the simulation of the day. 
// Returns a sorted vector of Output objects by time.
vector<Output> startSimulation(queue<Person>& logOfPeopleForTheDay, vector<Floor>& floors, Elevator& elevatorL, Elevator& elevatorS) {
  
  while(!logOfPeopleForTheDay.empty()) {
    Person currentPerson = logOfPeopleForTheDay.front();
    //cout << endl << "--- CURRENTLY WORKING ON PERSON: " << currentPerson << endl;
    if ( floors[currentPerson.getFromFloor() - 1].getQueueOfPeopleAtFloor().empty() || !(floors[currentPerson.getFromFloor() - 1].containsPerson(currentPerson)) ) {
      //cout << "REQUEST ALREADY HANDLED!" << endl;
      logOfPeopleForTheDay.pop();
      continue;
    }
    
    Elevator& elevatorToRespond = currentPerson.pickAnElevator(elevatorL, elevatorS);
    //cout << "Elevator responding: " << elevatorToRespond.getName() << endl;
    currentPerson.call(elevatorToRespond);
    currentPerson.go(elevatorToRespond);
    //cout << "--- DONE WORKING ON PERSON: " << currentPerson << endl;
    logOfPeopleForTheDay.pop();
  }

  sortOutputs(outputLog);

  return outputLog;
}

int main() {
  try {
    queue<Person> logOfPeople;

    string fileLogsName;
    cout << "Please enter the name of the file with the log of requests for the day!" << endl;
    cout << "***The file with the log of requests for the day should be in the same folder***" << endl;
    cout << "File's name: ";
    cin >>  fileLogsName;

    parseTextFileContentToQueueOfPeople(fileLogsName, logOfPeople);

    Elevator elevatorL = Elevator(L, "L");
    Elevator elevatorS = Elevator(S, "S");

    vector<Floor> floors;
    initFloors(F, floors);
    initBuilding(floors, elevatorL, elevatorS);
    allocatePeopleToFloors(logOfPeople, floors);
    //displayBuilding(floors);

    vector<Output> outputs = startSimulation(logOfPeople, floors, elevatorL, elevatorS);

    cout << endl << "Output:" << endl;

    ofstream myfile ("output.txt");
    if (myfile.is_open()) {
      for (const Output& outputLine : outputs) {
      cout << outputLine << endl;
      myfile << outputLine << endl;
      }
    } else cout << "Unable to open file";
    myfile.close();

  } catch (const exception& e) {
    cout << e.what() << endl;
  }

  return 0;
}