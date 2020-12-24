#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <ctime>

using namespace std;
////////////////////////////TAXI CLASS///////////////////////////////////////
class Taxi{
    double longtitude;
    double latitude;

    double distance;

    public:
        Taxi();
        Taxi(int,int);
        void setDistance();
        double getDistance();
        void decreaseDistance();
        void setLongtitude(double);
        double getLongtitude();
        void setLatitude(double);
        double getLatitude();
};

Taxi::Taxi(){
    this->latitude = 0.0;
    this->longtitude = 0.0;
}

Taxi::Taxi(int x, int y){
    this->latitude = x;
    this->latitude = y;
}

void Taxi::setDistance(){
    this->distance = sqrt(this->longtitude * this->longtitude + this->latitude * this->latitude);
}

double Taxi::getDistance(){
    return this->distance;
}

void Taxi::decreaseDistance(){
    this->distance -= 0.01;
}

void Taxi::setLongtitude(double x){
    this->longtitude = x;
}

double Taxi::getLongtitude(){
    return this->longtitude;
}

void Taxi::setLatitude(double y){
    this->latitude = y;
}

double Taxi::getLatitude(){
    return this->latitude;
}
///////////////////////////HEAP TREE CLASS/////////////////////////////////////

class minBHT{
    int totalCapacity;
    int currentSize;
    Taxi* taxiList;

    int parent(int i){
        return(i-1)/2;
    }
    int left(int i){
        return 2*i +1;
    }
    int right(int i){
        return 2*i +2;
    }

    public:
        int getCapacity();
        int getSize(); //to get the current size of the queue on public
        minBHT(int); //constructor
        void insert(Taxi); //adding new taxi to the queue
        void heapify(int); //shaping queue
        void updateByDecrease(int); //updating taxi's distance(only decrease)
        double removeTheMostPrior(); //extract the minimum distance taxi and shape the queue recursively
};

int minBHT::getCapacity(){
    return this->totalCapacity;
}

int minBHT::getSize(){
    return this->currentSize;
}

minBHT::minBHT(int capacity){
    this->totalCapacity = capacity;
    this->currentSize = 0;
    this->taxiList = new Taxi[capacity];
}

void minBHT::insert(Taxi x){
    if(this->currentSize == this->totalCapacity){
        cout<<"Cannot be added element to the heap."<<endl;
        return;
    }

    this->currentSize++;

    int tempPosition = this->currentSize - 1;
    this->taxiList[tempPosition] = x;

    while(taxiList[tempPosition].getDistance() < taxiList[this->parent(tempPosition)].getDistance()  && tempPosition != 0){
        swap(taxiList[tempPosition] , taxiList[ this->parent(tempPosition) ] );
        tempPosition = this->parent(tempPosition);
    }
}

void minBHT::heapify(int i){
    int lft = this->left(i); //left child of the tree
    int rgt = this->right(i); //right child of the tree
    int smalllest = i;

    if(lft < this->currentSize && this->taxiList[lft].getDistance() < this->taxiList[smalllest].getDistance() ){
        smalllest = lft;
    }

    if(rgt < this->currentSize && this->taxiList[rgt].getDistance() < this->taxiList[smalllest].getDistance() ){
        smalllest = rgt;
    }

    if(smalllest != i){
        swap(this->taxiList[i] , this->taxiList[smalllest]);
        heapify(smalllest);
    }
}

double minBHT::removeTheMostPrior(){
    /* if(this->currentSize == 0){
        cout<<"The heap is already empty."<<endl;
        return ;
    } */
    if(this->currentSize == 1){
        this->currentSize = 0;
        return this->taxiList[0].getDistance();
    }
    else{
        Taxi root = this->taxiList[0];
        this->taxiList[0] = this->taxiList[this->currentSize - 1];
        this->currentSize--;
        this->heapify(0);

        return root.getDistance();
    }
}

void minBHT::updateByDecrease(int index){
    this->taxiList[index].decreaseDistance();

    while(index != 0 && this->taxiList[ parent(index)].getDistance() > this->taxiList[ index ].getDistance()){
        swap(this->taxiList[index],this->taxiList[parent(index)]);
        index = parent(index);
    }   
}

////////////////////////////MAIN//////////////////////////

int main(int argc, char* argv[]){
    clock_t startTime= clock(); //starting time of the program
    srand (time(NULL));    

    int m = stoi(argv[1]);
    double p = stod(argv[2]);
    
    int random;
    int percentP = 100 * p;
    double x,y; //temp dis

    Taxi taxi = Taxi(0.0,0.0);
    taxi.setDistance();

    double removedDistance;
    int updateCount = 0;
    int additionCount=0;


    string line;
    string filename = "locations.txt";
    ifstream file;
    file.open(filename);
    
    getline(file,line); //header


    minBHT taxiPriorityQueue = minBHT(m);

    for (int i=1; i<=m; i++){
        random = rand() % 100;
        if(random < percentP && i != 1){ //probolability p
            int randomIndex = rand() % taxiPriorityQueue.getSize();
            taxiPriorityQueue.updateByDecrease(randomIndex);
            updateCount++;
        }
        else{//1-p
            getline(file,line,' '); //longtitude
            x = stod(line);

            getline(file,line,'\n'); //latitude
            y= stod(line);

            taxi.setLongtitude(x);
            taxi.setLatitude(y);
            taxi.setDistance();

            taxiPriorityQueue.insert(taxi);
            additionCount++;
        }

        if(i % 100 ==0){
            removedDistance = taxiPriorityQueue.removeTheMostPrior();
            cout<<"The distance of called taxi is "<<removedDistance<<endl;
        }
    }
    clock_t endTime = clock(); //end time of the program
    clock_t t = difftime(endTime,startTime); //running time of the program

    cout<<"Number of additions: "<<additionCount<<endl;
    cout<<"Number of updates: "<<updateCount<<endl;
    cout<<"The program run in "<<t<<" miliseconds."<<endl;
    
    return EXIT_SUCCESS;
}
