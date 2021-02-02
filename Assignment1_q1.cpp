
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <queue>

using namespace std;

map<string, vector<string>> cityAdjList;
map<string, string> parents;

void addEdge(map<string, vector<string>> &adjList, string cityOne, string cityTwo) {
	adjList.find(cityOne)->second.push_back(cityTwo);
	adjList.find(cityTwo)->second.push_back(cityOne);
}

void printStk(stack<string> &stk) {
	if (stk.empty()) {
		return;
	}

	string temp = stk.top();
	stk.pop();
	printStk(stk);
	cout << temp << "->";

}

bool DFSUtil(string curCity, string endCity, map < string, bool> &visited, stack<string> &stk) {
	
	visited.find(curCity)->second = true;

	if (curCity.compare(endCity) == 0) {
		stk.push(curCity);
		cout << "The DFS path is: ";
		printStk(stk);
		cout << "\b\b. \n";
		return true;
	}
	else {
		stk.push(curCity);

		vector<string> lst = cityAdjList.find(curCity)->second;

		for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
			if (!visited.find(*itr)->second) {
				bool chk = DFSUtil(*itr, endCity, visited, stk);
				if (chk)
					return true;
			}
		}

		stk.pop();
		return false;

	}

}

void DFS(string startCity, string endCity, string cities[], int noOfCities) {
	map<string, bool> visited;
	for (int i = 0; i < noOfCities; ++i) {
		visited.insert(make_pair(cities[i], false));
	}

	stack<string> stk;

	DFSUtil(startCity, endCity, visited, stk);

}

void BFS(string startCity, string endCity, string cities[], int noOfCities) {
	map<string, bool> visited;
	for (int i = 0; i < noOfCities; ++i) {
		visited.insert(make_pair(cities[i], false));
	}

	queue<string> Q;
	stack<string> stk;
	map<string, string> parent;

	Q.push(startCity);
	

	while (!Q.empty()) {
		string curCity = Q.front();
		visited.find(curCity)->second = true;
		if (curCity.compare(endCity) == 0) {

			string temp = endCity;
			while (temp.compare(startCity) != 0) {
				stk.push(temp);
				temp = parent.find(temp)->second;
			}
			cout << "The BFS path is: " << startCity << "->";
			while (!stk.empty()) {
				cout << stk.top() << "->";
				stk.pop();
			}
			cout << "\b\b. " << endl;

			return;
		}
		vector<string> lst = cityAdjList.find(curCity)->second;
		for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
			if (!visited.find(*itr)->second) {
				parent.insert(make_pair(*itr, curCity));
				Q.push(*itr);
			}
		}
		Q.pop();
		
	}


}

//Took idea from geekforgeeks
bool IDSUtil(string curCity, string endCity, int lvl) {
	if (curCity.compare(endCity) == 0) {
		return true;
	}
	else if (lvl < 0) {
		return false;
	}
	vector<string> lst = cityAdjList.find(curCity)->second;
	for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
		if (parents.find(*itr) != parents.end()) {
			continue;
		}
		parents.insert(make_pair(*itr, curCity));
		if (IDSUtil(*itr, endCity, lvl - 1)) {
			return true;
		}
		parents.erase(*itr);
	}

	return false;

}

void IDS(string startCity, string endCity, int max_depth) {
	parents.clear();
	for (int i = 0; i < max_depth; i++)
	{
		if (IDSUtil(startCity, endCity, i)) {
			string temp = endCity;
			stack<string> stk;
			while (temp.compare(startCity) != 0) {
				stk.push(temp);
				temp = parents.find(temp)->second;
			}
			cout << "The IDS path is: " << startCity << "->";
			while (!stk.empty()) {
				cout << stk.top() << "->";
				stk.pop();
			}
			cout << "\b\b. " << endl;

			return;
			return;
		}
	}
}


int main()
{
	string cities[] = {"Arad", "Zerind","Odarea","Sibiu","Timisoara","Lugoj","Mehadia",
					"Dobreta","Craiova","Rimnieu Vilcea","Pitesti","Farara","Bucharest",
					"Giurgui","Urziceni","Hirsova","Eforie","Vaslui","Iasi","Neamt"};

	
	for (string c : cities) {
		vector<string> lst;
		cityAdjList.insert(make_pair(c, lst));
	}



	addEdge(cityAdjList, "Arad", "Zerind");
	addEdge(cityAdjList, "Arad", "Timisoara");
	addEdge(cityAdjList, "Lugoj", "Timisoara");
	addEdge(cityAdjList, "Lugoj", "Mehadia");
	addEdge(cityAdjList, "Dobreta", "Mehadia");
	addEdge(cityAdjList, "Dobreta", "Craiova");
	addEdge(cityAdjList, "Pitesti", "Craiova");
	addEdge(cityAdjList, "Rimnieu Vilcea", "Craiova");
	addEdge(cityAdjList, "Rimnieu Vilcea", "Sibiu");
	addEdge(cityAdjList, "Rimnieu Vilcea", "Pitesti");
	addEdge(cityAdjList, "Bucharest", "Pitesti");
	addEdge(cityAdjList, "Bucharest", "Farara");
	addEdge(cityAdjList, "Bucharest", "Giurgui");
	addEdge(cityAdjList, "Bucharest", "Urziceni");
	addEdge(cityAdjList, "Hirsova", "Urziceni");
	addEdge(cityAdjList, "Hirsova", "Eforie");
	addEdge(cityAdjList, "Hirsova", "Vaslui");
	addEdge(cityAdjList, "Iasi", "Vaslui");
	addEdge(cityAdjList, "Iasi", "Neamt");
	addEdge(cityAdjList, "Arad", "Sibiu");
	addEdge(cityAdjList, "Zerind", "Odarea");
	addEdge(cityAdjList, "Sibiu", "Odarea");
	addEdge(cityAdjList, "Sibiu", "Farara");

	
	int menu;
	
	
	while (true) {
		cout << "1) Depth First Search" << endl;
		cout << "2) Breadth First Search" << endl;
		cout << "3) Iterative Deepening Search" << endl;
		cout << "Enter Algorithm Number (1-3) or 0 to exit: ";
		cin >> menu;

		system("CLS");

		switch (menu)
		{
		case 1: DFS("Arad", "Bucharest", cities, sizeof(cities) / sizeof(*cities)); break;
		case 2: BFS("Arad", "Bucharest", cities, sizeof(cities) / sizeof(*cities)); break;
		case 3: IDS("Arad", "Bucharest", 20); break;
		default:
			return 0;
			break;
		}
		cout << "\n\n";
		
	}
	return 0;
}




