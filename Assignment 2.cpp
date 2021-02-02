#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <queue>
#include <map>


using namespace std;

int matrixSize;
int* heuristics;
int** matrixGraph;
string result;
string resTemp;
int startIndex, endIndex;


string getNextLine(ifstream& readFile) {
	string lineRead;
	std::getline(readFile, lineRead);

	while (lineRead.empty() || lineRead.at(0) == '/') {
		std::getline(readFile, lineRead);
	}
	return lineRead;
}


void populateMatrix(ifstream& readFile) {
	string lineRead = getNextLine(readFile);


	while (lineRead.at(0) != '/') {
		istringstream ss(lineRead);
		string temp;

		int index1, index2, heuristic;

		std::getline(ss, temp, ' ');
		index1 = temp.at(0) - 'A';

		std::getline(ss, temp, ' ');
		index2 = temp.at(0) - 'A';

		std::getline(ss, temp, ' ');
		heuristic = stoi(temp);
		
		matrixGraph[index1][index2] = heuristic;
		matrixGraph[index2][index1] = heuristic;
		
		std::getline(readFile, lineRead);
	}



}

void DFS(int start, int end) {
	stack<int> stk;
	stack<int> stkCosts;
	bool* visited = new bool[matrixSize];

	for (int i = 0; i < matrixSize; i++) {
		visited[i] = false;
	}



	ostringstream ss;
	ss << "\nDFS: ";
	
	int curIndex = start;
	int costs = 0;

	stk.push(curIndex);
	stkCosts.push(costs);


	while (!stk.empty()) {
		curIndex = stk.top();
		costs = stkCosts.top();
		stk.pop();
		stkCosts.pop();


		if (visited[curIndex]) {
			continue;
		}
		visited[curIndex] = true;
		
		char temp = 'A' + curIndex;
		ss << temp << "(" << costs << ") ";

		if (curIndex == end) {
			break;
		}

		for (int i = matrixSize-1; i >= 0; i--) {
			if (!visited[i] && matrixGraph[curIndex][i] > 0) {
				stk.push(i);
				stkCosts.push(costs + matrixGraph[curIndex][i]);
			}
		}
		

	}

	resTemp += ss.str()+'\n';

	


}

void BFS(int start, int end) {
	queue<int> Q;
	queue<int> Qcosts;

	bool* visited = new bool[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visited[i] = false;
	}

	int* prevIndex = new int[matrixSize];
	int costs = 0;

	Q.push(start);
	Qcosts.push(costs);


	ostringstream ss;
	ss << "BFS: ";

	while (!Q.empty()) {
		int curIndex = Q.front();
		costs = Qcosts.front();
		Qcosts.pop();
		Q.pop();

		if (visited[curIndex]) {
			continue;
		}

		char temp = 'A' + curIndex;
		ss << temp << "(" << costs << ") ";


		if (curIndex == end) {
			break;
		}

		visited[curIndex] = true;

		for (int i = 0; i < matrixSize; i++) {
			if (!visited[i] && matrixGraph[curIndex][i] > 0) {
				Q.push(i);
				Qcosts.push(costs + matrixGraph[curIndex][i]);
				prevIndex[i] = curIndex;
			}
		}

	}

	resTemp += ss.str() + "\n";
	
}

struct priorityNode {
	int index;
	int cost;

	bool operator<(const priorityNode& p) const{
		return std::tie(cost, index) > std::tie(p.cost, p.index);
	}

};

bool isPresent(priorityNode nodeToFind, vector<priorityNode> &pQ) {
	
	for (auto itr = pQ.begin(); itr != pQ.end(); ++itr) {

		if (itr->index == nodeToFind.index) {
			if (itr->cost > nodeToFind.cost) {

				itr->cost = nodeToFind.cost;
			}

			return true;
		}

	}
	return false;

}

void UCS(int start, int end) {
	
	bool* visited = new bool[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visited[i] = false;
	}


	vector<priorityNode> pQ;
	priorityNode pN = { start,0 };

	pQ.push_back(pN);
	make_heap(pQ.begin(), pQ.end());

	ostringstream ss;
	ss << "UCS: ";

	while (!pQ.empty()) {
		push_heap(pQ.begin(), pQ.end());

		pN = pQ.front();
		int curIndex = pN.index;
		int cost = pN.cost;

		visited[curIndex] = true;

		pop_heap(pQ.begin(), pQ.end());
		pQ.pop_back();

		char temp = 'A' + curIndex;
		ss << temp << "(" << cost << ") ";

		if (curIndex == end) {
			break;
		}

		for (int i = 0; i < matrixSize; i++) {
			if (!visited[i] && matrixGraph[curIndex][i] > 0) {

				priorityNode tempN = { i, (cost + matrixGraph[curIndex][i]) };

				if (isPresent(tempN, pQ)) {
					continue;
				}

				pQ.push_back(tempN);
			}
		}


	}
	resTemp += ss.str() + "\n";
}

struct heuristicNode {
	int index;
	int cost;
	int heuristic;

	bool operator<(const heuristicNode& p) const {
		return std::tie(heuristic, index) > std::tie(p.heuristic, p.index);
	}

};

void bestFirstSearch(int start, int end) {
	bool* visited = new bool[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visited[i] = false;
	}

	priority_queue<heuristicNode> pQ;
	heuristicNode pN = { start,0, heuristics[start] };

	pQ.push(pN);

	ostringstream ss;
	ss << "BestFirstSearch: ";

	while (!pQ.empty()) {
		pN = pQ.top();
		pQ.pop();

		int curIndex = pN.index;
		int cost = pN.cost;

		visited[curIndex] = true;

		char temp = 'A' + curIndex;
		ss << temp << "(" << cost << ") ";

		if (curIndex == end) {
			break;
		}

		for (int i = 0; i < matrixSize; i++) {
			if (!visited[i] && matrixGraph[curIndex][i] > 0) {

				heuristicNode tempN = { i, (cost + matrixGraph[curIndex][i]), heuristics[i] };

				pQ.push(tempN);
			}
		}

	}

	resTemp += ss.str() + "\n";

}

bool isPresentHnode(heuristicNode nodeToFind, vector<heuristicNode>& pQ) {

	for (auto itr = pQ.begin(); itr != pQ.end(); ++itr) {

		if (itr->index == nodeToFind.index) {
			if (itr->heuristic > nodeToFind.heuristic) {

				itr->heuristic = nodeToFind.heuristic;
				itr->cost = nodeToFind.cost;
			}

			return true;
		}

	}
	return false;

}

void Astar (int start, int end) {

	map<int, int> closedList;


	vector<heuristicNode> pQ;
	heuristicNode pN = { start,0, heuristics[start] };

	pQ.push_back(pN);
	make_heap(pQ.begin(), pQ.end());

	ostringstream ss;
	ss << "A*: ";

	while (!pQ.empty()) {
		

		pN = pQ.front();
		int curIndex = pN.index;
		int cost = pN.cost;


		pop_heap(pQ.begin(), pQ.end());
		pQ.pop_back();
		closedList.insert(make_pair(curIndex, pN.heuristic));

		char temp = 'A' + curIndex;
		ss << temp << "(" << cost << ") ";

		if (curIndex == end) {
			break;
		}

		for (int i = 0; i < matrixSize; i++) {
			if (matrixGraph[curIndex][i] > 0) {

				heuristicNode tempN = { i, (cost + matrixGraph[curIndex][i]),cost+ matrixGraph[curIndex][i]+heuristics[i] };

				if (isPresentHnode(tempN, pQ)) {
					continue;
				}

				if (closedList.find(i) != closedList.end()) {
					if (closedList.find(i)->second > cost + heuristics[i])
						closedList.erase(i);
					else
						continue;
				}

				pQ.push_back(tempN);
				push_heap(pQ.begin(), pQ.end());
			}
		}


	}
	resTemp += ss.str() + "\n\n";
}

void setUpGraph(ifstream& readFile) {
	string lineRead;
	lineRead = getNextLine(readFile);

	matrixSize = stoi(lineRead);
	heuristics = new int[matrixSize];

	lineRead = getNextLine(readFile);

	istringstream ss(lineRead);
	string temp;

	int count = 0;
	while (std::getline(ss, temp, ' ')) {
		heuristics[count] = stoi(temp);
		count++;
	}



	matrixGraph = new int* [matrixSize];
	for (int i = 0; i < matrixSize; ++i) {
		matrixGraph[i] = new int[matrixSize];
		for (int j = 0; j < matrixSize; j++) {
			matrixGraph[i][j] = 0;
		}
	}

	populateMatrix(readFile);
}

int main()
{
	result = "Traversals\n\n";
	ifstream readFile;

	string lineRead;

	readFile.open("Assignment2_DataFile.txt");
	int c = 0;
	while (c++ != 3) {
		
		setUpGraph(readFile);

		
		lineRead = getNextLine(readFile);

		

		istringstream ss(lineRead);
		string temp;
		ss.clear();
		ss.str(lineRead);

		std::getline(ss, temp, ' ');
		startIndex = temp.at(0) - 'A';

		std::getline(ss, temp, ' ');
		endIndex = temp.at(0) - 'A';

		DFS(startIndex, endIndex);
		BFS(startIndex, endIndex);
		UCS(startIndex, endIndex);
		bestFirstSearch(startIndex, endIndex);
		Astar(startIndex, endIndex);

		result += ("Graph " + to_string(c) + ": \n" +resTemp);
		resTemp = "";
	}

	ofstream outFile;
	outFile.open("result.txt", ios::out, ios::trunc);

	outFile << result;
	
	outFile.close();


}


