#include <fstream> //for file IO
#include <string>  //for string usage
#include <sstream> //for dissecting data in the files
#include <iostream>//for basic IO operations
#include <vector>  //default container
#include <iomanip> //for setw()

#define structure struct
//#define DEBUG

using namespace std;


struct StockPair {
  string name;
  int quantity;

};


struct BuildingType {
    char type;
    vector<int> consumption;
};


void fileOpennerStock(ifstream &file);
void fileOpennerConsumption(ifstream &file);
void fileOpennerColony(ifstream &file);
vector<StockPair> loadResources2vector(ifstream &file);
void printResources(const vector<StockPair> &resources);
void DEBUGprintConsumption(const vector<BuildingType> &consumption);
void printConsumption(const vector<BuildingType> &consumption);
vector<BuildingType> loadConsumption2vector(ifstream &file);
void printColony(const vector<vector<char>>& colony);
bool canFormColony(ifstream &fileColony, const vector<StockPair>& stock, const vector<BuildingType>& consumption);
vector<vector<char>> formColonyMatrix (ifstream &fileColony);
void deductBuildingCosts(vector<vector<char>>& colony, vector<StockPair>& stock, const vector<BuildingType>& consumption);
void printResourceConsumption(const vector<StockPair>& stock, const vector<BuildingType>& consumption, const vector<vector<char>>& colony);


int main()
{

    cout << "Welcome to the colony management system" << endl;

    ifstream fileStock;

    fileOpennerStock(fileStock); //stock türü txt i aç

    vector<StockPair> vectorStock = loadResources2vector(fileStock); //dosyadan al StockPair türünden bir vektöre yükle

    printResources(vectorStock);

    ifstream fileConsumption; //ifstream objesi2

    fileOpennerConsumption(fileConsumption); //filestream objesine yükledik dosyayı

    vector<BuildingType> vectorConsumption = loadConsumption2vector(fileConsumption);

    printConsumption(vectorConsumption);

    ifstream fileColony;

    fileOpennerColony(fileColony);

    bool proceed = canFormColony(fileColony, vectorStock, vectorConsumption);

        if (proceed == 0){

           cout<<"Not enough resources to build this colony."<<endl;
           cout<<"Thank you for using the colony management system. The program will terminate."<<endl;
           cout<<"Goodbye!"<<endl;

           fileStock.close();
           fileConsumption.close();
           fileColony.close();

           return 1;
        }


    vector<vector<char>> matrixColony = formColonyMatrix(fileColony);

    printColony(matrixColony);

    cout << "Resources stock after loading the colony:" << endl;

    deductBuildingCosts(matrixColony, vectorStock, vectorConsumption);

    printResources(vectorStock);

    printResourceConsumption(vectorStock, vectorConsumption, matrixColony);



    cout << "Please enter an option number:"<< endl;

    cout << "1. Construct a new building on the colony."<< endl;

    cout << "2. Destruct/Disassemble a building from the colony."<< endl;

    cout << "3. Print the colony."<< endl;

    cout << "4. Print the consumption of all resources by each building in the colony."<< endl;

    cout << "5. Print the consumption of a specific resource by each building in the colony."<< endl;

    cout << "6. Print the resources stock."<< endl;

    cout << "7. Exit the program."<< endl;

    string resource_name;
    int resource_index;

    while (true) {

        int answer;
        cin >> answer;

        switch (answer) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                printColony(matrixColony);
                break;
            case 4:
                printResourceConsumption(vectorStock, vectorConsumption, matrixColony);
                break;
            case 5:
                cout << "Please enter the type of the resource: ";
                cin >> resource_name;


                resource_index = -1;
                for (int i = 0; i < vectorStock.size(); i++) { //corresponding indexi stock vektorunde yakala
                    if (vectorStock[i].name == resource_name) {
                        resource_index = i;
                        break;
                    }
                }

                //Olumsuz dönütü
                if (resource_index == -1) {
                    cout << "Resource not found." << endl;
                    break;
                }

                //Consumption & buildingi matrix kurarar coutla
                cout << "Consumption of resource " << resource_name << " by each building in the colony:" << endl;
                for (int i = 0; i < matrixColony.size(); i++) {//traverse the "matrixColony"
                    for (int j = 0; j < matrixColony[i].size(); j++) {

                        for (const BuildingType& building : vectorConsumption) { //vektordeki element basina:
                            if (building.type == matrixColony[i][j]) {//matchse:

                                cout << setw(5) << left << building.consumption[resource_index];
                            }
                        }
                    }
                    cout << endl;
                }
                break;
            case 6:
                printResources(vectorStock);
                break;
            case 7:

                cout << "Thank you for using the colony management system. The program will terminate." << endl;
                cout << "Goodbye!"<<endl;

                return 0;

            default:
                cout << "Invalid option. Please enter a number between 1 and 7."<<endl;
                break;
        }
        cout << "Please enter an option number:" <<endl;
    }


    return 0;
}

//IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

void printResourceConsumption(const vector<StockPair>& stock, const vector<BuildingType>& consumption, const vector<vector<char>>& colony) {

    for (int resourceIndex = 0; resourceIndex < stock.size(); resourceIndex++) {

        StockPair resource = stock[resourceIndex];

        cout << "Consumption of resource "<< resource.name << " by each building in the colony:" << endl;

        for (int rowIndex = 0; rowIndex < colony.size(); rowIndex++) {

            vector<char> row = colony[rowIndex];

            for (int columnIndex = 0; columnIndex < row.size(); columnIndex++) {

                char buildingType = row[columnIndex];

                for (int consumptionIndex = 0; consumptionIndex < consumption.size(); consumptionIndex++) { //searching for BuildingType

                    BuildingType building = consumption[consumptionIndex];

                    if (building.type == buildingType) {

                        for (int stockIndex = 0; stockIndex < stock.size(); stockIndex++) { //find the consumption of iterated resource

                            if (stock[stockIndex].name == resource.name) {
                                cout << setw(5) << left << building.consumption[stockIndex];//coutting + formatting output
                            }
                        }
                    }
                }
            }
             cout << endl;
        }
    }
}


//*********************************

void deductBuildingCosts(vector<vector<char>>& colony, vector<StockPair>& stock, const vector<BuildingType>& consumption) {
    // colony traverse
    for (int rowIndex = 0; rowIndex < colony.size(); rowIndex++) {

        vector<char>& row = colony[rowIndex];

        for (int columnIndex = 0; columnIndex < row.size(); columnIndex++) {

            char buildingType = row[columnIndex];

            if (buildingType != '-') {// bosları salla
                // finding the corresponding BuildingType in consumption
                for (int consumptionIndex = 0; consumptionIndex < consumption.size(); consumptionIndex++) {

                    BuildingType building = consumption[consumptionIndex];

                    if (building.type == buildingType) {
                        // tuketilen resource basına corresponding structu bul stockta ve islemi yap
                        for (int i = 0; i < building.consumption.size(); i++) {
                            for (int stockIndex = 0; stockIndex < stock.size(); stockIndex++) {

                                StockPair& resource = stock[stockIndex];

                                if (resource.name == stock[i].name) {
                                    resource.quantity -= building.consumption[i];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//*********************************

void fileOpennerStock(ifstream &file){
    string filename;
    cout<<"Please enter file name for resources stock:"<<endl;//io ops
    cin >> filename;
    file.open(filename.c_str());
    while( file.fail() ){
        cout<< "Unable to open the file " << filename << " for reading."<<endl;
        cout<< "Please enter the correct file name:" << endl;
        cin >> filename;
        file.open(filename.c_str());
    }
    cout<<"Available resources loaded from " << filename <<endl;//reply
}

//*********************************

void fileOpennerConsumption(ifstream &file){
    string filename;
    cout<<"Please enter file name for resource consumption per building type: "<<endl;
    cin >> filename;
    file.open(filename.c_str());
    while( file.fail() ){
        cout<< "Unable to open the file " << filename << " for reading."<<endl;
        cout<< "Please enter the correct file name:" << endl;
        cin >> filename;
        file.open(filename.c_str());
    }
    cout<<"Resources consumption per building type loaded from " << filename <<endl;
}

//*********************************

void fileOpennerColony(ifstream &file){
    string filename;
    cout<<"Please enter file name for colony: "<<endl;
    cin >> filename;
    file.open(filename.c_str());
    while( file.fail() ){
        cout<< "Unable to open the file " << filename << " for reading."<<endl;
        cout<< "Please enter the correct file name:" << endl;
        cin >> filename;
        file.open(filename.c_str());
    }
    cout<<"Colony loaded from " << filename <<endl;
}

//*********************************

vector<StockPair> loadResources2vector(ifstream &file) {

    vector<StockPair> resources; //stockpair objesine sahip bir vektor, bunu return ediyoruz zaten

    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        string name;
        int quantity;

        ss >> name >> quantity; //line den gelecek ilk eleman name ye atanıyor, ikinci eleman ise quantity e atılıyor sonra da bir bunlardan oluşan bir
                                //obje yaratılıp, yaratıldığı gibi vektöre atılıyor, bütün lineler bitince de return ediliyor bu vektör...
        resources.push_back( {name, quantity} );
    }

    return resources;
}

//*********************************

void printResources(const vector<StockPair> &resources) {

    cout << "Resource stock:" << endl;

    for (int i = 0; i < resources.size(); i++) {
        cout << resources[i].name << " " << resources[i].quantity << endl;//fieldleri ver
    }
}

//*********************************

vector<BuildingType> loadConsumption2vector(ifstream &file) {

    vector<BuildingType> consumption;
    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        char buildingType;
        ss >> buildingType;

        vector<int> buildingConsumption;

        int quantity;
        while (ss >> quantity) {
            buildingConsumption.push_back(quantity);
        }

        consumption.push_back({buildingType, buildingConsumption});//birlestirip vektore yolla
    }

    return consumption;
}

//*********************************

void printConsumption(const vector<BuildingType> &consumption) {

    cout << "Resources consumption per building type:" << endl;
    for (int i = 0; i < consumption.size(); i++) {
        cout << consumption[i].type << " ";

        for (int j = 0; j < consumption[i].consumption.size(); j++) {
            cout << consumption[i].consumption[j] << " ";
        }
        cout << endl;
    }
}

//*********************************

void printColony(const vector<vector<char>>& mat) {

    cout << "Colony:" << endl;

    for (int i=0; i < mat.size(); i++){

      for (int j=0; j < mat[0].size(); j++){

        cout << setw(5) << left << mat[i][j];//formatlayarak elemanları coutla

      }
      cout << endl;
    }
}

//*********************************

bool canFormColony(ifstream &fileColony, const vector<StockPair>& stock, const vector<BuildingType>& consumption) {

    vector<StockPair> remainingStock = stock;

    vector<int> buildingCounts(26, 0);  // harfe ozel vektorden cek adedi

    char buildingType;

    while (fileColony >> buildingType) {

        if (buildingType != '\n') {
            buildingCounts[buildingType - 'A']++;
        }
    }

    for (int consumptionIndex = 0; consumptionIndex < consumption.size(); consumptionIndex++) {

        BuildingType building = consumption[consumptionIndex];

        int count = buildingCounts[building.type - 'A'];

        for (int i = 0; i < building.consumption.size(); i++) {

            for (int stockIndex = 0; stockIndex < remainingStock.size(); stockIndex++) {

                StockPair& resource = remainingStock[stockIndex];

                if (resource.name == stock[i].name) {

                    int totalConsumption = count * building.consumption[i];

                    if (resource.quantity < totalConsumption) {//impossible case.

                        cout << "Not enough " << resource.name << " to build this building." << endl;
                        return false;
                    } else {
                        resource.quantity -= totalConsumption; //deducting
                    }
                }
            }
        }
    }

    return true; //all good
}

//*********************************

vector<vector<char>> formColonyMatrix (ifstream &fileColony){

    fileColony.clear(); //reset fpointer
    fileColony.seekg(0);

    vector<vector<char>> toBeReturned;

    string line;
    while(getline(fileColony,line)){//dissecting

        vector<char> temp;

        for (char c : line){ //for every character in line string:
            temp.push_back(c);
        }

        toBeReturned.push_back(temp);
    }
    return toBeReturned;
}
