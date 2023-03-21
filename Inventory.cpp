// Inventory Managing the storage of food can be tricky, especially in commercial environments.
// Fridges and freezers will have many items with “use by”, “frozen on”, etc. 
// 
// The aim of this project is to develop a program that would be useful for both commercial and home
// users, to keep track of a food inventory. 
// 
// Allow the ability to store various kinds of items, and warn users of food that needs to be used in the near future or food that is past it’s
// suitable date and should be thrown out. 
// 
// You could also think of this as something like a commercial warehouse or distributor, where they may have multiple instances of a food
// item and should prioritise using the older items first, suggesting the shelf and item with the oldest date on a users’ request.
// 
 
// FoodItem
//  - Name
//  - Category
//  - Quantity
//  - Expiry Date
//  - isExpired()
//  - isExpiringSoon()

# include <iostream>
# include <string>
# include <ctime>
# include <vector>
# include <algorithm>
# include <iomanip>
# include <sstream>
# include <fstream>

using namespace std;

class FoodItem{
	public:
		string name;
		string category;
		int quantity;
		string barcode;
		time_t expiryDate;
		
		FoodItem(string name, string category, int quantity, string barcode ,time_t expiryDate){
			this->name = name;
			this->category = category;
			this->quantity = quantity;
			this->barcode = barcode;
			this->expiryDate = expiryDate;
		}
		
		// This will check expiry date and will tell us whether our food item is expired or not
		bool isExpired(){
			time_t now = time(NULL); // Time() will return our Current Time
			return expiryDate < now; // if the item is expired it will return True and if it is not expired it will return False	
		}
		
		// isExpiring Function will tell us whether our Food Item will expire in our defined time or not
		// date parameter is our limit in which we want to check our food to be expiring or not
		bool isExpiring(int date=1){
			time_t now = time(NULL); // Time() will return our Current Time 
			time_t threshold = now + date * 24 * 60 * 60;// Add up our current time with the limit we want to check for expiring
			return expiryDate < threshold;
			// Threshold = 08/03/2023 + 1 Day
			// Our Threshold is 09/03/2023
			// 09/03/2023 < 09/03/2023  
		}
};

// Add Food Item | Done
// Edit Food Item | Done
// Delete Food Item | Done
// Search Food Item | Done
// Prioritise | Done
// DisplayWarnings | Done
// Display Inventory | Done
// Saving the data to CSV  | 
// Using a function to get Date in specific order | Done
// Extracting Data from CSV
// Removing Seconds from ExpiryDate display

class Inventory{
	private:
		vector<FoodItem> items;
	public:
		void addFoodItems(string name, string category, int quantity, string barcode, time_t expiryDate){
			items.emplace_back(name, category, quantity, barcode, expiryDate);
		}
		
		void editFoodItems(int index, string name, string category, int quantity, string barcode, time_t expiryDate){
			if(index < items.size()){
				items[index].name = name;
				items[index].category = category;
				items[index].quantity = quantity;
				items[index].barcode = barcode;
				items[index].expiryDate = expiryDate;
				cout<<"Successfully Edited the Values at defined index\n";
			}else{
				cout<<"Index is out of Range\n";
			}
		}
		
		void deleteFoodItems(int index){
			if(index < items.size() && index >=0){
				items.erase(items.begin() + index);
				cout<<"Successfully Deleted Food Item\n";
			}else{
				cout<<"Index is out of Range\n";
			}
		}
		
		void searchItems(string Barcode){
			cout<<setw(20)<<left<<"Name"<<setw(20)<<left<<"Category"<<setw(20)<<left<<"Barcode"<<setw(20)<<left<<"ExpiryDate"<<endl;
			for(auto item: items){
				//   #1234 | #1234
				//             | 18446744073709551615
				if( item.barcode.find(Barcode) != string::npos){
					cout<<setw(20)<<left<<item.name<<setw(20)<<left<<item.category<<setw(20)<<left<<item.barcode<<setw(20)<<left<<asctime(localtime(&item.expiryDate))<<endl;
				}
			}
		}
		
		void prioritize(){
			// sort( first_index, last_index, function)
			// {f1, f2, f3, f4}
			// {b, a
			sort(items.begin(), items.end(), [](const FoodItem& a, const FoodItem& b){	
				return a.expiryDate < b.expiryDate;
			});
		}
		
		void DisplayWarnings(){
			cout<<"Warning! These Food items are expired or expiring Soon..."<<endl;	
			cout<<setw(20)<<left<<"Index"<<setw(20)<<left<<"Name"<<setw(20)<<left<<"Barcode"<<setw(20)<<left<<"Status"<<setw(20)<<left<<"Expiry Date"<<endl;
			int index = 0;
			for(auto item: items){
				if(item.isExpired()){
					cout<<setw(20)<<left<<index<<setw(20)<<left<<item.name<<setw(20)<<left<<item.barcode
						<<setw(20)<<left<<"Expired"<<setw(20)<<left<<asctime(localtime(&item.expiryDate))<<endl;
				}else
				if(item.isExpiring()){
					cout<<setw(20)<<left<<index<<setw(20)<<left<<item.name<<setw(20)<<left<<item.barcode
						<<setw(20)<<left<<"Expiring"<<setw(20)<<left<<asctime(localtime(&item.expiryDate))<<endl;
				}
				index++;
			}
		}
		
		
		void Display(){
			cout<<"Our Inventory\n";
			cout<<setw(20)<<left<<"Name"<<setw(20)<<left<<"Category"<<setw(20)<<left<<"Qunatity"<<setw(20)<<left<<"Barcode"<<setw(20)<<left<<"Expiry Date"<<endl;
			for(auto item:items){
				cout<<setw(20)<<left<<item.name<<setw(20)<<left<<item.category<<setw(20)<<left<<item.quantity
				    <<setw(20)<<left<<item.barcode<<setw(20)<<left<<asctime(localtime(&item.expiryDate))<<endl;
			}
		}
		
		void writeCSV(){
			ofstream file("inventory.csv");
			
			for(auto item: items){
				file << item.name <<","<<item.category<<","<<item.quantity<<","<<item.barcode<<","<<asctime(localtime(&item.expiryDate));
			}
			
			file.close();
		}
};

time_t convertStringtotime(string datestring){
	// YYYY-MM-DD
	istringstream datestream(datestring);
	
	int year, month, day;
	char seperator;
	// YYYY - MM - DD
	datestream >> year >> seperator >> month >> seperator  >> day;
	
	tm expiryDateTm = {0}; 
	// 2023 -1900 = 123 
	expiryDateTm.tm_year = year - 1900;
	// 0-11
	expiryDateTm.tm_mon = month - 1;
	expiryDateTm.tm_mday = day;
	
	time_t expiryDate = mktime(&expiryDateTm);
	
	return expiryDate;
}

int main(){
	Inventory inventory;	
	inventory.addFoodItems("Mangoo", "Fruit", 3, "#b1234" ,time(NULL) + 5*24*60*60);
	inventory.addFoodItems("Milk", "Diary", 2, "#b1245" , time(NULL) - 1*24*60*60);
	inventory.addFoodItems("Banana", "Fruit", 5, "#b1256" , time(NULL) + 7*24*60*60);
	
	bool isExit = false;
	
	while( ! isExit ){
		system("cls");
		
		cout<<"Menu\n";
		cout<<" 1) Add Food Items\n";
		cout<<" 2) Edit Food Items\n";
		cout<<" 3) Delete Food Items\n";
		cout<<" 4) Display Warnings\n";
		cout<<" 5) Display Inventory\n";
		cout<<" 6) Search Food Item \n";
		cout<<" 7) Prioritize Inventory\n";
		cout<<" 0) Exit\n";
		cout<<" Choice : ";
		
		int choice;
		cin>>choice;
		
		string name, category, barcode, expiryDate;
		int quantity, index;
		switch(choice){
			case 1:
				cout<<"Add Food Items\n";
				cout<<"Enter Name : ";     cin>>name;
				cout<<"Enter Category : "; cin>>category;
				cout<<"Enter Barcode : ";
				cin>>barcode;
				cout<<"Enter Quantity : ";
				cin>>quantity;
				cout<<"Enter ExpiryDate (YYYY-MM-DD) : "; 
				cin>>expiryDate;
				inventory.addFoodItems(name, category, quantity, barcode, convertStringtotime(expiryDate));
				system("pause");
				break;
			case 2:
				cout<<"Edit Food Items\n";
				cout<<"Enter Index : ";    cin>>index;
				cout<<"Enter Name : ";     cin>>name;
				cout<<"Enter Category : "; cin>>category;
				cout<<"Enter Barcode : ";  cin>>barcode;
				cout<<"Enter Quantity : "; cin>>quantity;
				cout<<"Enter ExpiryDate (YYYY-MM-DD) : ";
				cin>>expiryDate;
				inventory.editFoodItems(index, name, category, quantity, barcode, convertStringtotime(expiryDate));
				system("pause");
				break;	
			case 3:
			   	cout<<"Deleting Food Item\n";
			   	cout<<"Enter Index of Food Item to Delete : ";
			   	cin>>index;
			   	inventory.deleteFoodItems(index);
			   	system("pause");
			   	break;
			case 4:
				inventory.DisplayWarnings();
				system("pause");
				break;
			case 5:
				inventory.Display();
				system("pause");
				break;
			case 6:
				cout<<"Searching Food Item\n";
				cout<<"Enter Barcode of FoodItem";
				cin>>barcode;
				inventory.searchItems(barcode);
				system("pause");
				break;
			case 7:
				inventory.prioritize();
				system("pause");
				break;
			case 0:
				isExit = true;
				break;
			default:
				cout<<"Invalid Option\n";
		}
		inventory.writeCSV();
	}

	
//	inventory.addFoodItems("Apple", "Fruit", 5, "#b1234" , time(NULL) + 0*24*60*60);
//	inventory.addFoodItems("Orange", "Fruit", 5, "#b1234" , time(NULL) + 3*24*60*60);
////	inventory.Display();
//	cout<<endl;
//	inventory.DisplayWarnings();
//	cout<<endl;
//	int index;
//	cout<<"Enter index to delete Food Item\n";
//	cin>>index;
//	inventory.deleteFoodItems(index);
//	inventory.Display();
}
