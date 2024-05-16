#include <iostream>
#include <vector>
#include <limits>

using namespace std;

enum node{
	  a ,b ,c ,d ,e ,f, emptyy = -1
};

struct Map{
  std::vector<std::vector<std::pair<node, int>>>tentativePath;
  std::vector<std::vector<std::pair<node, int>>>shortestPath;
}map;

std::ostream& operator<<(std::ostream& os, node node){
  switch(node){
  case  a :os<<'A';break;
  case  b :os<<'B';break;
  case  c :os<<'C';break;
  case  d :os<<'D';break;
  case  e :os<<'E';break;
  case  f :os<<'F';break;
  case  emptyy :os<<"empty";break;
  }
  return os;
}


void findShortestPath(const std::vector<std::vector<std::pair<node, int>>>graph, const node sourceNode);
void displayResult(const node startnode, const node arrivedNode);
void displayPaths(const std::vector < std::vector<std::pair<node, int>>>paths);
bool Visited(const std::vector<node>&visited, const node&nd);
bool operator<(const std::vector<std::pair<node, int>>&tv, const std::vector<std::pair<node, int>>&sv);

bool operator<(const std::vector<std::pair<node, int>>&vs, const std::vector<std::pair<node, int>>&vt){
  // NOTE: Reread how the left and right side of operator work
  int ssdistance{0}, ttdistance{0};
  for (const std::pair<node, int>& sp :vs){
    ssdistance+= sp.second;
  }

  for (const std::pair<node, int>& tp :vt){
    ttdistance+= tp.second;
  }

  if(ttdistance < ssdistance){
    return 1;
  } else {
    return 0;
  }
  
}

void findShortestPath(const std::vector<std::vector<std::pair<node, int>>>graph, const node sourceNode)
{
  int turn{0};

  std::vector<node>visited;
  std::vector<node>tentativeNodes;

  
  int distance = std::numeric_limits<int>::max();

  // NOTE: step 1: add all possible paths to tentative Path and the shortest path to the shortestPath
  // step 2: 
  node Current =  emptyy;
  node nextNode = emptyy;

  Current = sourceNode;
  tentativeNodes.push_back(Current);
  
  // Mark the position of vector contain smallest path
  map.shortestPath.clear();

  int mark{-1};
  int smark, tmark;
  std::vector<int>missingPaths;
  // uint64_t mark1{0};
  // TODO: Recheck the condition to get out of the loop

  size_t visited_count{0};
    
  std::vector<int> Mdistance;

  while(true)
    {
      // NOTE: Try to put the adding thing in one graph[Current] loop
      // to reduce the code load and easier to debug
      std::cout<<"Node turn "<<turn<<" is:"<<Current<<std::endl;
      if (!graph[static_cast<int>(Current)].empty())
	{

	  if (map.tentativePath.empty())
	    {
	      // NOTE: Create the room first then make pair later
	      map.tentativePath.clear();
	      for(int i{0}; i < static_cast<int>(graph[static_cast<int>(Current)].size()); i++)
		{		  
		  map.tentativePath.emplace_back(); // Ensure the outer vector has enough space

		  std::vector<std::pair<node, int>>& currentInnerVector = map.tentativePath.back(); // Access the inner vector

		  if(currentInnerVector.empty()){
		    currentInnerVector.emplace_back(std::make_pair(Current, 0 ));
		    currentInnerVector.emplace_back( std::make_pair(graph[static_cast<int>(Current)][i].first, graph[static_cast<int>(Current)][i].second));
		    tentativeNodes.push_back(graph[static_cast<int>(Current)][i].first);
		  }

		  if (!map.tentativePath.empty() && !currentInnerVector.empty()) {
		    std::cout<<map.tentativePath.back().back().first<<": "<<map.tentativePath.back().back().second<<std::endl;
		  }

		}
	      std::cout<<"Tentative path "<<"turn "<<turn<<std::endl;
	      displayPaths(map.tentativePath);
	    }

	  else if (!map.tentativePath.empty())
	    {
	      if (!map.tentativePath.empty()){
		for (const std::vector<std::pair<node, int>> &v : map.tentativePath)
		  {

		    if (v.back().first == Current)
		      {
		  
			// NOTE: This mark's job is to decide where to keep
			// growing paths and it is created in tentative path loop.
			// when the tentative vector already has paths in it
			// assuming that every path's back is unique
		  
			mark = static_cast<int>(&v - &map.tentativePath.front());
			// mark1 = &v - &map.tentativePath.front();
			break;
		      }

		    if (mark != -1)
		      {
			break;
		      }
		  }
		auto it = graph[static_cast<int>(Current)].begin();
		//NOTE: Clone path that have current Node at the end
		
		// NOTE: First solution I came up with is when add node to the clone
		// I think I should add a check condition to see whether that node is already is in the tentative Path
		// I will pop that out if it has longer distance
		// visited_count = 0;
		
		while (it != graph[static_cast<int>(Current)].end())
		  {		    
		    // If the node haven't been visited
		    // Update the tentative path
		    
		    // NOTE: May be the bug lie here, thing is
		    // may be we add clone without considering the tail node
		    // is already in the visited list
		    if (!Visited(tentativeNodes, (*it).first)){
		      map.tentativePath.push_back(map.tentativePath[mark]);
		      map.tentativePath.back().push_back(*it);
		      tentativeNodes.push_back((*it).first);
		    }
		    it++;
		  }		  		

		std::cout<<"Tentative path "<<"turn "<<turn<<std::endl;
		displayPaths(map.tentativePath);
		
	      }
	    }	  	  
	}
      
      //NOTE: Time to add path to shortest Path and decide next node
            
      distance = std::numeric_limits<int>::max();
      visited_count = 0;
      
      for (const std::pair<node, int> &p1: graph[static_cast<int>(Current)])
	{
	  if((p1.second < distance) && !Visited(visited, p1.first))
	    {
	      distance = p1.second;
	      nextNode = p1.first;
	      mark = static_cast<int>(&p1 - &graph[static_cast<int>(Current)].front());		    
	    } else if (Visited(visited, p1.first)) {
	    visited_count++;
	  }
	}

      if (visited_count == graph[static_cast<int>(Current)].size()){
	mark = -1;
	nextNode = emptyy;
      }

      if (mark != -1){	  
	if(map.shortestPath.empty())
	  {
	    map.shortestPath.push_back({std::make_pair(Current, 0), graph[static_cast<int>(Current)][mark]});
	  }
	else
	  {
	    map.shortestPath.push_back(map.shortestPath.back());
	    map.shortestPath.back().push_back(graph[static_cast<int>(Current)][mark]);
	  }	
      }
      std::cout<<"Current shortest paths turn "<<turn<<std::endl;
      displayPaths(map.shortestPath);
      // ==========================================================================================
      // NOTE: Still working here
      // This part is the alternative code for compensating the shortest paths which took
      // from the tentative path collection
      if (nextNode == emptyy) {

	smark = 0;
	tmark = 0;
	bool Break = false;
	while (1){
	  // NOTE: I unintentionally created an infinite loop here
	  for (const std::vector<std::pair<node, int>>&vs : map.shortestPath) {
	    smark = 0;
	    tmark = 0;
	    for (const std::vector<std::pair<node, int>>&vt : map.tentativePath){	  
	      if ((vt.back().first == vs.back().first)&&(vt < vs)){
		smark = static_cast<int>(&vs - &map.shortestPath.front());
		tmark = static_cast<int>(&vt - &map.tentativePath.front());
		break;
	      }    
	      // TODO: Try to put an ending flag here to got out of the infinite loop
	      // I'm stuck here
	      if ((vs == map.shortestPath.back()) && (!(vt < vs))){
		Break = true;
	      } // Bugs lie here
	    }
	    if ((smark!=0)&&(tmark!=0)){
	      break;
	    }
	  }
	  

	  std::cout<<"Start to replace the shortest path if the alternative path is shorter\n";
	  map.shortestPath.erase(map.shortestPath.begin()+smark);
	  map.shortestPath.push_back(map.tentativePath[tmark]);
	
	  if (Break){
	    break;
	  }
	}
	std::cout<<"after loop  Milestone"<<std::endl;
	// displayPaths(map.shortestPath);      
	// =========================================================================================
	// Time to mark current node as visited and move on to the next node
      }
      if (!Visited(visited, Current)&&!Visited(visited, nextNode)){	
	visited.push_back(Current);
	Current = nextNode;
      }
      
      std::cout<<"visited nodes turn "<<turn<<" are: "<<visited.size()<<std::endl;
      for (const node& n:visited){
	std::cout<<n<<" ";
      }
      
      std::cout<<"\n";
      turn++;


      // NOTE: Instead of trying to find and insert paths fron tentative Path
      // Find it in tentative Path and print it     
      if ((visited.size() == graph.size())||(nextNode == emptyy)){
	std::cout<<"number of graph's nodes  turn "<<turn<<" is: "<<graph.size()<<std::endl;
	break;	
      }
    }		
}

bool Visited(const std::vector<node>&visited, const node&nd){

  for (const auto& n: visited){
    if (n == nd){
      return true;
      break;
    }
  }
  return false;
}

void displayResult(const node startnode, const node arrivedNode)
{
  // NOTE: Show the results
  std::cout<<"So the shortest path from node "<<startnode<<" to node "<< arrivedNode
	   <<" is: \n";
  int Mark = -1;
  int Distance{0};

  // Working progress up to here  
  displayPaths(map.shortestPath);
  for (const std::vector<std::pair<node, int>> &v1: map.shortestPath)
    {
      if (v1.back().first == arrivedNode)
	{
	  Mark = static_cast<int>(&v1 - &map.shortestPath.front());
	  break;
	}
    }

  std::cout << "Source Node";

  for(const std::pair<node, int> &p3: map.shortestPath[Mark])
    {
      if ((&p3 - &map.shortestPath[Mark].front())!=0)
	std::cout<<"- "<<"("<<p3.second<<")"<<" - "<<p3.first;
      Distance += p3.second;
    }
  std::cout<<"\nAnd the sum distance is: "<<Distance;

}

void displayPaths(const std::vector < std::vector<std::pair<node, int>>>paths) {
  for (const auto& v : paths) {
    std::cout << "Path " << &v - &paths.front()<<": ";
    for (const auto& p : v) {
      std::cout << "(" << p.first << ", " << p.second << ") ";
    }
    std::cout << endl;
  }
}

int main (int argc, char* argv[])
{
  std::vector<std::vector<std::pair<node, int>>>graph;
  node arrivedNode = f;

  graph.push_back({{b, 1},{d, 2}}); // Node A with the neighbor is node B and D
  graph.push_back({{a, 1},{c, 4},{e, 3}}); // Node B with the neighbor is node A, C and E
  graph.push_back({{b, 4},{f, 5}}); // Node C with the neighbor is node B and F
  graph.push_back({{a, 2},{e, 7}}); // Node D with the neighbor is node A and E
  graph.push_back({{b, 3},{d, 7},{f, 6}}); // Node E with the neighbor is node B, D and F
  graph.push_back({{c, 5},{e, 6}}); // Node F with the neighbor is node C and E

  if (!graph.empty() && !graph[0].empty()){
    for (std::vector<std::pair<node, int>> v : graph){
      for (std::pair<node, int>& n : v){
	std::cout<<"Node: "<<n.first<<" , distance: "<<n.second<<" ";
      }
      std::cout<<"\n";
    }
    findShortestPath(graph,  a);
    std::cout << "After finding the shortest paths milestone" << std::endl;
  }
  displayPaths(map.tentativePath);
  // displayPaths(map.shortestPath);
  displayResult(a, arrivedNode);
}
