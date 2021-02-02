#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <queue>

using namespace std;

map<string, string> parents;

string move(string state, char frog, int size) {
	int tempSize = size;
	int emptyPos = -1;
	int frogPos = -1;
	for (int i = 0; i < state.size(); ++i) {
		if (state.at(i) == '_') {
			emptyPos = i;
		}
		else if (state.at(i) == frog) {
			frogPos = i;
		}
	}
	string st = "flag";
	if (frog < 'A'+ tempSize) {
		if (emptyPos < frogPos || emptyPos - frogPos > 2) {
			return st;
		}
		st = "";
		for (int i = 0; i < state.size(); ++i) {
			if (state.at(i) != '_' && state.at(i) != frog) {
				st = st + state.at(i);
			}
			else if (state.at(i) == '_') {
				st = st + frog;
			}
			else {
				st = st + '_';
			}

		}
		return st;

	}
	else {
		if (emptyPos > frogPos || frogPos - emptyPos > 2) {
			return st;
		}
		st = "";
		for (int i = 0; i < state.size(); ++i) {
			if (state.at(i) != '_' && state.at(i) != frog) {
				st = st + state.at(i);
			}
			else if (state.at(i) == '_') {
				st = st + frog;
			}
			else {
				st = st + '_';
			}

		}
		return st;
	}
}

void printStk(stack<string>& stk) {
	if (stk.empty()) {
		return;
	}

	string temp = stk.top();
	stk.pop();
	printStk(stk);
	cout << temp << "->";

}

int depth = 0;
bool DFSUtil(string curState, string goalState, map<string, bool> &visited, stack<string> &stk, map<string, vector<string>> graph) {
	depth++;
	visited.find(curState)->second = true;

	if (curState.compare(goalState) == 0) {
		stk.push(curState);
		cout << "The DFS path is: ";
		printStk(stk);
		cout << "\b\b. \n";
		return true;
	}
	else {
		stk.push(curState);

		vector<string> lst = graph.find(curState)->second;

		for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
			if (!visited.find(*itr)->second) {
				bool chk = DFSUtil(*itr, goalState, visited, stk, graph);
				if (chk)
					return true;
			}
		}

		stk.pop();
		return false;

	}

}

void DFS(string startCity, string endCity, map<string, vector<string>> graph) {
	map<string, bool> visited;
	for (auto itr = graph.begin(); itr != graph.end(); ++itr) {
		visited.insert(make_pair(itr->first, false));
	}

	stack<string> stk;

	DFSUtil(startCity, endCity, visited, stk, graph);

}

void BFS(string startState, string goalState, map<string,vector<string>> graph) {
	map<string, bool> visited;
	for (auto itr = graph.begin(); itr != graph.end(); ++itr) {
		visited.insert(make_pair(itr->first, false));
	}

	queue<string> Q;
	stack<string> stk;
	map<string, string> parent;

	Q.push(startState);


	while (!Q.empty()) {
		string curCity = Q.front();
		visited.find(curCity)->second = true;
		if (curCity.compare(goalState) == 0) {

			string temp = goalState;
			while (temp.compare(startState) != 0) {
				stk.push(temp);
				temp = parent.find(temp)->second;
			}
			cout << "The BFS path is: " << startState << " -> ";
			while (!stk.empty()) {
				cout << stk.top() << " -> ";
				stk.pop();
			}
			cout << "\b\b\b\b.  " << endl;

			return;
		}
		vector<string> &lst = graph.find(curCity)->second;
		for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
			if (!visited.find(*itr)->second) {
				parent.insert(make_pair(*itr, curCity));
				Q.push(*itr);
			}
		}
		Q.pop();

	}


}


bool IDSUtil(string curState, string goalState, int lvl, map<string, vector<string>> graph) {
	if (curState.compare(goalState) == 0) {
		return true;
	}
	else if (lvl < 0) {
		return false;
	}
	vector<string> lst = graph.find(curState)->second;
	for (auto itr = lst.begin(); itr != lst.end(); ++itr) {
		if (parents.find(*itr) != parents.end()) {
			continue;
		}
		parents.insert(make_pair(*itr, curState));
		if (IDSUtil(*itr, goalState, lvl - 1, graph)) {
			return true;
		}
		parents.erase(*itr);
	}

	return false;

}

void IDS(string startState, string goalState, int max_depth, map<string, vector<string>> graph) {
	parents.clear();
	for (int i = 0; i < max_depth; i++)
	{
		if (IDSUtil(startState, goalState, i, graph)) {
			string temp = goalState;
			stack<string> stk;
			while (temp.compare(startState) != 0) {
				stk.push(temp);
				temp = parents.find(temp)->second;
			}
			cout << "The IDS path is: " << startState << "->";
			while (!stk.empty()) {
				cout << stk.top() << "->";
				stk.pop();
			}
			cout << "\b\b. " << endl;

			return;
		}
	}
}


void initGraph(map<string, vector<string>> &graph, int size) {
	graph.clear();
	vector<char> frogs;
	int tempSize = size;
	for (int i = 0; i < size*2; i++){
		char startOne = 'A';
		char startTwo = 'Z'-size+1;
		if (i < size) {
			frogs.push_back(startOne + i);
		}
		else {
			frogs.push_back(startTwo + (i - size));
		}
	}

	string space(frogs.begin(), frogs.end());
	space.insert(tempSize, "_");

	queue<string> Q;
	Q.push(space);

	while (!Q.empty()) {
		string curState = Q.front();
		Q.pop();
		if (graph.find(curState) == graph.end()) {
			vector<string> adjList;
			graph.insert(make_pair(curState, adjList));
		}
		vector<string> &lst = graph.find(curState)->second;
		for (int i = 0; i < frogs.size(); ++i) {
			string temp = move(curState, frogs[i], tempSize);
			if (temp.compare("flag") != 0) {
				lst.push_back(temp);
				Q.push(temp);
			}
		}
	}
}

vector<string> getStates(int Tsize) {
	int size = Tsize;

	string start;
	string goal;
	for (int i = 0; i < size * 2; i++) {
		char startOne = 'A';
		char startTwo = 'Z' - size + 1;
		if (i < size) {
			start += (startOne + i);
			goal += (startTwo + i);

		}
		else {
			start += (startTwo + (i - size));
			goal += (startOne + (i - size));
		}
	}

	start.insert(size, "_");
	goal.insert(size, "_");

	vector<string> states;
	states.push_back(start);
	states.push_back(goal);

	return states;
}

int main() {
	
	map<string, vector<string>> graph;
	int size;


	int menu;


	while (true) {
		cout << "1) Depth First Search (faster, solves till 7 frogs in reasonable time)" << endl;
		cout << "2) Breadth First Search (solves till 4 frogs in reasonable time)" << endl;
		cout << "Enter Algorithm Number (1-2) or 0 to exit: ";
		cin >> menu;

		if (menu < 1 || menu > 2) {
			return 0;
		}
		
		cout << "Enter number of frogs: ";
		cin >> size;
		initGraph(graph, size);

		string start;
		string goal;

		vector<string> states = getStates(size);

		system("CLS");

		switch (menu)
		{
		case 1: DFS(states[0], states[1], graph); break;
		case 2: BFS(states[0], states[1], graph);break;
		default:
			return 0;
			break;
		}
		cout << "\n\n";

	}
	
	return 0;
}