#ifndef _GRAPH_UI_
#define _GRAPH_UI_

#include "NegativeGraphCycle.hpp"

template <typename T>
int graphUI() {
  
  string option, line;
  //int distance;
  bool digraph = false;
  
  cin >> option;
  if(!option.compare("digraph"))
    digraph = true;
  Graph<T> g(digraph);
  
  while(true) {
    
    std::stringstream stream;
    cin >> option;
    
    if(!option.compare("av")) {		
		getline(std::cin, line);
		stream << line;
		T vtx(stream);
		
		if(g.addVtx(vtx)){
			cout << "av " << vtx << " OK\n";
		}
		else{
			cout << "av " << vtx << " NOK\n";
		}
	}
    else if(!option.compare("rv")) {
		getline(std::cin, line);
		stream << line;
		T vtx(stream);
		if(g.rmvVtx(vtx)){
			cout << "rv " << vtx << " OK\n";
		}
		else{
			cout << "rv " << vtx << " NOK\n";
		}
    }
    else if(!option.compare("ae")) {
		getline(std::cin, line);
		stream << line;
		T from(stream);
		T to(stream);
		int cost;
		stream >> cost;
		if(g.addEdg(from, to, cost)){
			cout << "ae " << from <<" "<< to << " OK\n";
		}
		else{
			cout << "ae " << from <<" "<< to << " NOK\n";
		}
    }
    else if(!option.compare("re")) {
		getline(std::cin, line);
		stream << line;
		T from(stream);
		T to(stream);
		if(g.rmvEdg(from, to)){
			cout << "re " << from <<" "<< to << " OK\n";
		}
		else{
			cout << "re " << from <<" "<< to << " NOK\n";
		}
    }
    else if(!option.compare("dot")) {
		getline(std::cin, line);
		
		if(g.print2DotFile(line.c_str())){
			cout << "dot " << line << " OK\n";
		}
		else{
			cout << "dot " << line << " NOK\n";
		}
    }
    else if(!option.compare("bfs")) {
		getline(std::cin, line);
		stream << line;
		T node(stream);
		
		list<T> bfs_list = g.bfs(node);
		cout << "\n----- BFS Traversal -----\n";
		for(typename list<T>::iterator it = bfs_list.begin(); it!=bfs_list.end(); ++it){
			if(*it == node){
				cout << node;
			}
			else{
				cout << " -> " << *it;
			}
		}
		cout << "\n-------------------------\n";
    }
    else if(!option.compare("dfs")) {
		getline(std::cin, line);
		stream << line;
		T node(stream);
		
		list<T> dfs_list = g.dfs(node);
		cout << "\n----- DFS Traversal -----\n";
		for(typename list<T>::iterator it = dfs_list.begin(); it!=dfs_list.end(); ++it){
			if(*it == node){
				cout << node;
			}
			else{
				cout << " -> " << *it;
			}
		}
		cout << "\n-------------------------\n";
    }
    else if(!option.compare("dijkstra")) {
		getline(std::cin, line);
		stream << line;
		T from(stream);
		T to(stream);
		list<T> dijkstra_list = g.dijkstra(from, to);
		
		cout << "Dijkstra (" << from << " - " << to <<"): ";
		for(typename list<T>::iterator it = dijkstra_list.begin(); it!=dijkstra_list.end(); ++it){
			if(*it == from){
				cout << from;
			}
			else{
				cout << ", " << *it;
			}
		}
		cout << endl;
    }
    else if(!option.compare("bellman-ford")) {
		list<T> bellman_ford_list;
		getline(std::cin, line);
		stream << line;
		T from(stream);
		T to(stream);
		
		try{
			bellman_ford_list = g.bellman_ford(from, to);
			
			cout << "Bellman-Ford (" << from << " - " << to <<"): ";
			
			if(bellman_ford_list.empty()){
				cout<<"Negative Graph Cycle!\n";
			}
			else{
				for(typename list<T>::iterator it = bellman_ford_list.begin(); it!=bellman_ford_list.end(); ++it){
					if(*it == from){
						cout << from;
					}
					else{
						cout << ", " << *it;
					}
				}
				cout << endl;
			}
		}
		catch(exception& ex){
			cout<<ex.what()<<endl;
		}
    }
	else if(!option.compare("mst")) {
		int sum=0;
		
		list<Edge<T>> mst_list = g.mst();
		
		cout << "\n--- Min Spanning Tree ---\n";
		for(typename list<Edge<T>>::iterator iter_mst = mst_list.begin(); iter_mst!=mst_list.end(); ++iter_mst){
			cout << *iter_mst << endl;
			sum += (*iter_mst).dist;
		}
		cout << "MST Cost: " << sum << endl;
    }
    else if(!option.compare("q")) {
      cerr << "bye bye...\n";
      return 0;
    }
    else if(!option.compare("#")) {
      string line;
      getline(cin,line);
      cerr << "Skipping line: " << line << endl;
    }
    else {
      cout << "INPUT ERROR\n";
      return -1;
    }
  }
  return -1;  
}

#endif
