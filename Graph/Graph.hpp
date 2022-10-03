//ulopoihsh me pinaka geitniasews

#ifndef _GRAPH_HPP_ 
#define _GRAPH_HPP_

#include <list>
#include "NegativeGraphCycle.hpp"
#include "UnionFind.hpp" //den to xrhsimopoiw
#include<fstream>
#include<iostream>
#include<string>
#include <algorithm>
#define INT_MAX 100000000
#define INT_MIN -100000000

using namespace std;

template<typename T>
struct Edge {
    T from;
    T to;
    int dist;
    Edge(T f, T t, int d): from(f), to(t), dist(d) {
    }
    bool operator<(const Edge<T>& e) const;
    bool operator>(const Edge<T>& e) const;
    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const Edge<U>& e);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& e) {
    out << e.from << " -- " << e.to << " (" << e.dist << ")";
    return out;
}

template <typename T>
class Graph {
    int capacity;	//h xwritikothta tou pinaka geitniashs
	int graph_nodes;//o arithmos twn komvwn tou grafou
    int** table; 	//pinakas geitniashs
    T* info_table; 	//vohthitikos pinakas gia antistoixish mias plhroforias T me enan arithmo
    bool directive;	//true an grafos kateuthinomenos, diaforetika false
    
    /*Genikh idea gia thn ulopoihsh mou:
	 * An den uparxei akmh metaksu duo komvwn tou grafou se ayth th thesh tou pinaka
	 * kataxwrw enan megalo thetiko akeraio (INT_MAX).
	 * Tis grammes kai tis sthles tou pinaka pou den antistoixoun se komvous tou grafhmatos
	 * tis symplhrwnw me enan mikro arnhtiko akeraio (INT_MIN). H timh ayth den paizei rolo
	 * sthn ylopoihsh twv algorthmwn kai ginetai mono gia arxikopoihsh tou pinaka geitniasews.*/

	void expand_table();
    void shrink_table();  
  
public:
	T empty();	//epistrefei T() kai to xrhsimopoiw gia arxikopoihsh tou info_table
    Graph(bool isDirectedGraph = true);
    bool contains(const T& info);
    bool addVtx(const T& info);
    bool rmvVtx(const T& info);
    bool addEdg(const T& from, const T& to, int distance);
    bool rmvEdg(const T& from, const T& to);
    list<T> dfs(const T& info) const;
    list<T> dfs_new_element(const T& info, list<T> list) const;
	list<T> bfs_new_element(list<T> list1, list<T> list2) const;
    list<T> bfs(const T& info) const;
    list<Edge<T>> mst();
  
    bool print2DotFile(const char *filename) const;
    list<T> dijkstra(const T& from, const T& to);
    list<T> bellman_ford(const T& from, const T& to);
  
};

template <typename T>
T Graph<T>::empty(){
	return T();
}

template <typename T>
Graph<T>::Graph(bool isDirectiveGraph) {
	int i, j;
	
	capacity = 2;
	graph_nodes = 0;
	table = new int*[capacity];
	for(i=0; i<capacity; i++){
		table[i] = new int[capacity];
	}
	for(i=0; i<capacity; i++){
		for(j=0; j<capacity; j++){
			table[i][j] = INT_MIN; 	//oses grammes kai sthles den antistoixoun se komvo
									//tou grafou tis arxikopoiw me ton mikrotero akeraio arithmo
		}
	}
	info_table = new T[capacity];
	for(i=0; i<capacity; i++){
		info_table[i]= empty();
	}
	directive = isDirectiveGraph;
}

template <typename T>
void Graph<T>::expand_table() {
	int i, j;
	int** temp = table;
	T* temp_info = info_table;

	capacity = 2 * capacity;
	table = new int*[capacity];
	for(i=0; i<capacity; i++){
		table[i] = new int[capacity];
	}
	for(i=0; i<capacity; i++){
		for(j=0; j<capacity; j++){
			table[i][j] = INT_MIN;
		}
	}
	info_table = new T[capacity];
	for(i=0; i<capacity; i++){
		info_table[i]= empty();
	}
	
	//topothetei stous neous pinakes tis times twn paliwn pinakwn
	for (i = 0; i < capacity / 2; i++) {
		for (j = 0; j < capacity / 2; j++) {
			table[i][j] = temp[i][j];
		}
	}

	for (i = 0; i < capacity / 2; i++) {
		info_table[i] = temp_info[i];
	}

	//cout << "Capacity: " << capacity / 2 <<" -> " << capacity << endl;
}

template <typename T>
void Graph<T>::shrink_table() {
	
	int i, j;
	int** temp = table;
	T* temp_info = info_table;

	capacity = capacity/2;
	table = new int*[capacity];
	for(i=0; i<capacity; i++){
		table[i] = new int[capacity];
	}
	for(i=0; i<capacity; i++){
		for(j=0; j<capacity; j++){
			table[i][j] = INT_MIN;
		}
	}
	info_table = new T[capacity];
	for(i=0; i<capacity; i++){
		info_table[i]= empty();
	}
	
	//topothetei stous neous pinakes tis times twn paliwn pinakwn
	for (i = 0; i < graph_nodes; i++) {
		for (j = 0; j < graph_nodes; j++) {
			table[i][j] = temp[i][j];
		}
	}

	for (i = 0; i < graph_nodes; i++) {
		info_table[i] = temp_info[i];
	}

	//cout << "Capacity: " << capacity * 2 <<" -> " << capacity << endl;
}


template <typename T>
bool Graph<T>::contains(const T& info) {
	int i;

	for (i = 0; i < graph_nodes; i++) {
		if (info_table[i] == info) {
			return true;
		}
	}
	return false;
}

template <typename T>
bool Graph<T>::addVtx(const T& info) {
	int i, j;
	
	if (contains(info) == true) {
		return false;
	}

	for (i = 0; i < capacity; i++) {
		if (i==graph_nodes) {
			info_table[i] = info;
			//th grammh kai thn sthlh tou pinaka geitniashs pou antistoixei sto neo 
			//komvo tis gemizei me INT_MAX afou de sundeetai me kanenan allo kamvo
			for(j=0; j<i; j++){
				table[j][i] = INT_MAX;
				table[i][j] = INT_MAX;
			}
			table[i][i] = INT_MAX;
			graph_nodes++;
			return true;
		}
	}
	
	//sthn periptwsh pou o pinakas geitniashs einai gematos
	expand_table();
	info_table[capacity / 2] = info;
	for(j=0; j<i; j++){
		table[j][capacity/2] = INT_MAX;
		table[capacity/2][j] = INT_MAX;
	}
	table[capacity/2][capacity/2] = INT_MAX;
	graph_nodes++;
	return true;
}

template <typename T>
bool Graph<T>::rmvVtx(const T& info) {
	int pos, i, j, temp_distance;
	T temp_info;

	if (contains(info) == false) {
		return false;
	}
	
	//vriskei ton komvo pou theloume na afairesoume ston bohthitiko pinaka
	for (pos = 0; pos < graph_nodes; pos++) {
		if (info_table[pos] == info) {
			info_table[pos]= empty();
			break;
		}
	}
	
	//taksinomei ton pinaka, stelnontas th thesh pou afairesame deksiotera apo olous tous komvous
	for (i = pos+1; i < graph_nodes; i++) {
		temp_info = info_table[i];
		info_table[i] = info_table[i - 1];
		info_table[i - 1] = temp_info;
	}
	
	//elegxei an prepei na upodiplasiasei tous pinakes
	if (graph_nodes-1 == capacity / 4) {
		shrink_table();
	}
	
	/*metatopizei thn grammh pou antistioxei ston afairoumeno komvo katwtera apo oles tis alles grammes
	pou antistoixoun se energous komvous tou grafou*/
	for (i = pos + 1; i < graph_nodes; i++) {
		for (j = 0; j < graph_nodes; j++) {
			temp_distance = table[i][j];
			table[i][j] = table[i - 1][j];
			table[i - 1][j] = temp_distance;
		}
	}
	
	/*metatopizei thn sthlh pou antistioxei ston afairoumeno komvo deksiotera apo oles tis alles sthles
	pou antistoixoun se energous komvous tou grafou*/
	for (i = 0; i < graph_nodes; i++) {
		for (j = pos+1; j < graph_nodes; j++) {
			temp_distance = table[i][j];
			table[i][j] = table[i][j-1];
			table[i][j-1] = temp_distance;
		}
	}
	
	pos = graph_nodes-1;
	
	//sumplhrwnwi th frammh kai th sthlh tou komvou pou afairoume me INT_MIN
	for(j=0; j<pos; j++){
		table[j][pos] = INT_MIN;
		table[pos][j] = INT_MIN;
	}
	table[pos][pos] = INT_MIN;
	graph_nodes--;
	return true;
}


template <typename T>
bool Graph<T>::addEdg(const T& from, const T& to, int cost) {
	int i, j;

	if (contains(from) && contains(to)) {
		for (i = 0; i < graph_nodes; i++) {
			if (info_table[i] == from) {
				break;
			}
		}

		for (j = 0; j < graph_nodes; j++) {
			if (info_table[j] == to) {
				break;
			}
		}

		if (table[i][j] == INT_MAX) {
			table[i][j] = cost;
			//an to grafhma mh kateuthinomeno enhmerwnei kai ton komvo "to" gia ton komvo "from"
			if (!directive) {
				table[j][i] = cost;
			}
			return true;
		}
	}
	return false;
}


template <typename T>
bool Graph<T>::rmvEdg(const T& from, const T& to) {
	int i, j;

	if (contains(from) && contains(to)) {
		for (i = 0; i < graph_nodes; i++) {
			if (info_table[i] == from) {
				break;
			}
		}

		for (j = 0; j < graph_nodes; j++) {
			if (info_table[j] == to) {
				break;
			}
		}

		if (table[i][j] != INT_MAX) {
			table[i][j] = INT_MAX;
			//an to grafhma mh kateuthinomeno enhmerwnei kai ton komvo "to" gia ton komvo "from"
			if (!directive) {
				table[j][i] = INT_MAX;
			}
			return true;
		}
		else{
			//se periptwsh pou h akmh den uparxei metaksu twn duo komvwn
			return false;
		}
	}
	return false;
}

template <typename T>
list<T> Graph<T>::dfs(const T& info) const {
	list<T> dfs_list;
	int i, j;
	
	dfs_list.push_back(info);
	
	for (i = 0; i < graph_nodes; i++) {
		if (info_table[i] == info) {
			break;
		}
	}

	for (j = 0; j < graph_nodes; j++) {
		if (table[i][j] != INT_MAX) {
			//elegxos an o komvos exei hdh prospelastei
			typename list<T>::iterator it;
			for (it = dfs_list.begin(); it != dfs_list.end(); ++it) {
				if (*it == info_table[j]) {
					break;
				}
			}
			if (it == dfs_list.end()) {
				dfs_list.push_back(info_table[j]);
				dfs_list = dfs_new_element(info_table[j], dfs_list);
			}
		}
	}
	
	return dfs_list;
}

template <typename T>
list<T> Graph<T>::dfs_new_element(const T& info, list<T> dfs_list) const{
	int i, j;

	for (i = 0; i < graph_nodes; i++) {
		if (info_table[i] == info) {
			break;
		}
	}

	for (j = 0; j < graph_nodes; j++) {
		if (table[i][j] != INT_MAX) {
			typename list<T>::iterator it;
			for (it = dfs_list.begin(); it != dfs_list.end(); ++it) {
				if (*it == info_table[j]) {
					break;
				}
			}
			if (it == dfs_list.end()) {
				dfs_list.push_back(info_table[j]);
				dfs_list = dfs_new_element(info_table[j], dfs_list);
			}
		}
	}
	return dfs_list;
}


template <typename T>
list<T> Graph<T>::bfs(const T& info) const {
	list<T> curr_list_level, bfs_list;
	int i, j;
	
	bfs_list.push_back(info);

	for (i = 0; i < graph_nodes; i++) {
		if (info_table[i] == info) {
			break;
		}
	}

	for (j = 0; j < graph_nodes; j++) {
		if (table[i][j] != INT_MAX) {
			typename list<T>::const_iterator it;
			for (it = bfs_list.begin(); it != bfs_list.end(); ++it) {
				if (*it == info_table[j]) {
					break;
				}
			}
			if (it == bfs_list.end()) {
				bfs_list.push_back(info_table[j]);
				//pernaw to epomeno epipedo tou dentrou tou bfs se mia lista
				curr_list_level.push_back(info_table[j]);
			}
		}
	}

	return bfs_new_element(curr_list_level, bfs_list);
}

template <typename T>
list<T> Graph<T>::bfs_new_element(list<T> previous_level, list<T> bfs_list) const {
	list<T> curr_list_level;
	int i, j;
	
	//sunthhkh termatismou anadromhs
	if(previous_level.empty()){
		return bfs_list;
	}
	//gia kathe komvo autou tou epipedou vriskw ta paidia tou kai ta prosthetw sth bfs_list
	//parallhla ftiaxnw to epomeno epipedo tou dentrou
	for (typename list<T>::iterator iter = previous_level.begin(); iter != previous_level.end(); ++iter) {
		for (i = 0; i < graph_nodes; i++) {
			if (info_table[i] == *iter) {
				break;
			}
		}

		for (j = 0; j < graph_nodes; j++) {
			if (table[i][j] != INT_MAX) {
				typename list<T>::const_iterator it;
				for (it = bfs_list.begin(); it != bfs_list.end(); ++it) {
					if (*it == info_table[j]) {
						break;
					}
				}
				if (it == bfs_list.end()) {
					bfs_list.push_back(info_table[j]);
					curr_list_level.push_back(info_table[j]);
				}
			}
		}
	}

	return bfs_new_element(curr_list_level, bfs_list);
}


template <typename T>
bool Edge<T>::operator<(const Edge<T>& e) const {
	if (dist < e.dist) {
		return true;
	}
	else {
		return false;
	}
}

template <typename T>
bool Edge<T>::operator>(const Edge<T>& e) const {
	if (dist > e.dist) {
		return true;
	}
	else {
		return false;
	}
}


template <typename T>
list<Edge<T>> Graph<T>::mst() {
	list<Edge<T>> mst_list, available_edge;
	list<T> unused_nodes, default_empty_list;
	Edge<T> edge(empty(), empty(), 0);
	Edge<T> min_edge(empty(), empty(), 0);
	typename list<T>::iterator iter_from, iter_to;
	list<T> connection_table[graph_nodes];
	int i, j;

	if (directive) {
		return mst_list;
	}
	
	//pairnw sthn arxh kathe komvo san enan ksexwristo grafo
	//kathe akmh pou prosthetw enwnei 2 grafous se enan
	//sto telos mono mia thesh tou pinaka tha exei mh kenh lista kai tha einai o telikos grafos
	for(i=0; i<graph_nodes; i++){
		connection_table[i].push_back(info_table[i]);
	}
	
	//ftiaxnw mia lista me tous komvous pou den exoun xrhsimopoihthei
	for (i = 0; i < graph_nodes; i++) {
		unused_nodes.push_back(info_table[i]);
	}
	
	//dhmiourgw tis akmes kai tis prosthetw se mia lista
	//elegxw mono tis theseis panw apo th diagwnia tou pinaka, wste to from na einai o komvos pou prostethhke prwtos
	for (i = 0; i < graph_nodes; i++) {
		for (j = i + 1; j < graph_nodes; j++) {
			if (table[i][j]!=INT_MAX) {
				edge.from = info_table[i];
				edge.to = info_table[j];
				edge.dist = table[i][j];
				available_edge.push_back(edge);
			}
		}
	}
	
	//elegxei an exoun xrhsimopoihthei oloi oi komvoi
	while (!unused_nodes.empty()) {
		min_edge.dist = INT_MAX;
		
		//vriskei th mikroterh akmh
		for (typename list<Edge<T>>::iterator it = available_edge.begin(); it != available_edge.end(); ++it) {
			if (*it < min_edge) {
				min_edge = *it;
			}
		}
		
		typename list<Edge<T>>::iterator it;
		for (it = available_edge.begin(); it != available_edge.end(); ++it) {
			if (!(*it > min_edge)) {
				break;
			}
		}
		
		//afairei thn akmh apo thn lista twn diathesimwn akmwn
		available_edge.erase(it);
		//vriskei kai afairei tous komvous auths ths akmhs apo th lista twn diathesimwn komvwn
		iter_from = find(unused_nodes.begin(), unused_nodes.end(), min_edge.from);
		iter_to = find(unused_nodes.begin(), unused_nodes.end(), min_edge.to);
		
		if (iter_from != unused_nodes.end()){
			unused_nodes.remove(min_edge.from);
		}
		if(iter_to != unused_nodes.end()) {
			unused_nodes.remove(min_edge.to);
		}
		
		//vriskei an oi duo komvoi sta akra ths akmhs anhkoun ston idio grafo
		//an den anhkoun ston idio grafo, tote h akmh prostithetai sth mst_list
		//ananewnetai ki o pinakas connection_table, wste oi duo komvoi na anhkoun sthn idia lista sthn idia thesh tou pinaka
		//prosthetw ola ta stoixeia ths mias listas sthn allh kai sth sunexeia thn antikathistw me kenh lista
		for(i=0; i<graph_nodes; i++){
			iter_from = find(connection_table[i].begin(), connection_table[i].end(), min_edge.from);
			if (iter_from != connection_table[i].end()){
				break;
			}
		}
		for(j=0; j<graph_nodes; j++){
			iter_to = find(connection_table[j].begin(), connection_table[j].end(), min_edge.to);
			if (iter_to != connection_table[j].end()){
				break;
			}
		}
		
		if(i!=j){
			mst_list.push_back(min_edge);
			for(typename list<T>::iterator it = connection_table[j].begin(); it!=connection_table[j].end(); ++it){
				connection_table[i].push_back(*it);
			}
			connection_table[j] = default_empty_list;
		}
	}
	return mst_list;
}

template <typename T>
list<T> Graph<T>::dijkstra(const T& from, const T& to){
	list<T> visited_nodes, dijkstra_list;
	T previous_node[graph_nodes];	//xrhsimopoiw auton ton pinaka gia na vrw apo poio komvo prohlthe kapoios komvos
	int i, j, k, curr_possition, min_distance, distance_until_now;
	T curr_from, curr_to, curr_node;
	typename list<T>::iterator iter;
	
	visited_nodes.push_back(from);
	
	while(visited_nodes.size() != graph_nodes){
		min_distance = INT_MAX;
		//elegxei tis akmes twn komvwn pou exoume episkeutei pros neous komvous
		for (typename list<T>::iterator it = visited_nodes.begin(); it != visited_nodes.end(); ++it) {
			for(i=0; i<graph_nodes; i++){
				if(info_table[i] == *it){
					break;
				}
			}
			
			//upologizei thn apostash tou komvou, pou ereunoume tis akmes tou, apo ton "from"
			if(*it != from){
				distance_until_now = 0;
				curr_possition = i;
				curr_node = info_table[curr_possition];
				while(curr_node != from){
					curr_node = previous_node[curr_possition];
					for(k=0; i<graph_nodes; k++){
						if(info_table[k] == curr_node){
							distance_until_now += table[k][curr_possition];
							curr_possition = k;
							break;
						}
					}
				}
			}
			else{
				distance_until_now = 0;
			}
			
			//psaxnei th mikroterh dunath nea akmh pou mporoume na prosthesoume
			for(j=0; j<graph_nodes; j++){
				if(table[i][j] + distance_until_now < min_distance){
					//elegxei an h akmh odhgei se komvo pou hdh exoume episkeutei
					iter = find(visited_nodes.begin(), visited_nodes.end(), info_table[j]);
					if(visited_nodes.end() == iter){
						min_distance = table[i][j] + distance_until_now;
						curr_from = info_table[i];
						curr_to = info_table[j];
					}
				}
			}
		}
		//eksetazei an brethhke nea akmh
		if(min_distance == INT_MAX){
			break;
		}
		//prosthetw to neo komvo pou episkefthkame sth lista
		visited_nodes.push_back(curr_to);
		for(i=0; i<graph_nodes; i++){
			if(info_table[i] == curr_to){
				break;
			}
		}
		previous_node[i] = curr_from;
	}
	
	//elegxei an episkefthkame ton komvo "to"
	iter = find(visited_nodes.begin(), visited_nodes.end(), to);
	if(visited_nodes.end() == iter){
		return dijkstra_list;
	}
	
	curr_node = to;
	
	//ftiaxnei to dijkstra_list
	while(1){
		for(i=0; i<graph_nodes; i++){
			if(info_table[i] == curr_node){
				break;
			}
		}
		dijkstra_list.push_front(curr_node);
		curr_node = previous_node[i];
		if(curr_node == from){
			dijkstra_list.push_front(curr_node);
			break;
		}
	}
	
	return dijkstra_list;
}

template<typename T>
list<T> Graph<T>::bellman_ford(const T& from, const T& to){
	list<T> bellman_ford_list, default_empty_list, previous_list;
	int node_cost[graph_nodes];	//apostash enos komvou apo to "from"
	int i, j, tries=0;
	T previous_node[graph_nodes];
	T curr_node;
	typename list<T>::iterator iter;
	
	for(i=0; i<graph_nodes; i++){
		if(info_table[i] == from){
			node_cost[i] = 0;
			previous_node[i] = from;
		}
		else{
			node_cost[i] = INT_MAX;
			previous_node[i] = to;
		}
	}
	
	while(1){
		for(i=0; i<graph_nodes; i++){
			if(node_cost[i] != INT_MAX){
				for(j=0 ; j<graph_nodes; j++){
					if(table[i][j] != INT_MAX){
						if(node_cost[i] + table[i][j] < node_cost[j]){
							node_cost[j] = node_cost[i] + table[i][j];
							previous_node[j] = info_table[i];
						}
					}
				}
			}
		}
		
		
		curr_node = to;
		//ftiaxnei to bellman_ford_list gia thn epanalhpsh trie
		while(1){
			for(i=0; i<graph_nodes; i++){
				if(info_table[i] == curr_node){
					break;
				}
			}
			
			//an to "from" den sundeetai me to "to" epistrefei kenh lista
			if(previous_node[i] == to){
				return default_empty_list;
			}
			//an sxmatizetai kuklos kai den epistrefei pote sto "from"
			iter = find(bellman_ford_list.begin(), bellman_ford_list.end(), curr_node);
			if(iter != bellman_ford_list.end()){
				break;
			}
			bellman_ford_list.push_front(curr_node);
			curr_node = previous_node[i];
			if(curr_node == from){
				bellman_ford_list.push_front(curr_node);
				break;
			}
		}
		
		//an h lista ulopoihthhke epituxws kai den allakse apo thn teleutaia fora
		//tote o algorithmos ekane th douleia tou epituxws kai gyrnaei th lista
		if(previous_list == bellman_ford_list && from == *bellman_ford_list.begin()){
			break;
		}
		
		//an exei perasei tis graph_nodes-1 epanalhpseis ki akoma ginontai allages
		//exoume arnhtiko kuklo kai petaei to exception
		if(tries == graph_nodes-1){
			throw NegativeGraphCycle();
		}
		previous_list = bellman_ford_list;
		bellman_ford_list = default_empty_list;
		tries++;
	}
	
	return bellman_ford_list;
}

template<typename T>
bool Graph<T>::print2DotFile(const char *filename) const{
	ofstream DotFile;
	int i, j;
	
    DotFile.open(filename);
	if(directive){
		DotFile << "digraph {\n";
		for(i=0; i<graph_nodes; i++){
			for(j=0; j<graph_nodes; j++){
				if(table[i][j]!=INT_MAX){
					DotFile <<"\t"<<info_table[i]<<" -> "<<info_table[j]<<"[label = \""<<table[i][j]<<"\"]"<<endl;
				}
			}
		}
	}
	else{
		DotFile << "graph {\n";
		for(i=0; i<graph_nodes; i++){
			for(j=0; j<i; j++){
				if(table[i][j]!=INT_MAX){
					DotFile <<"\t"<<info_table[i]<<" -- "<<info_table[j]<<"[label = \""<<table[i][j]<<"\"]"<<endl;
				}
			}
		}
	}
	DotFile << "}\n";
	DotFile.close();
	return true;
}

#endif
