/****************************************
 *
 * Algorithm for finding edge types using
 * a DFS
 *
 * @author: Serj Kazar
 *
 ****************************************/

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

/* Define colours */
#define WHITE 0
#define GRAY 1
#define BLACK 2

#define TREE 0
#define FORWARD 1
#define BACK 2
#define CROSS 3

unsigned int timer;

struct vertex{
  unsigned int vecNumber;
  int colour;
  unsigned int discoverTime;
  unsigned int finishTime;
  vector<unsigned int> *neighbours;
};

struct edge{
  unsigned int v1;
  unsigned int v2;
  int treeType;
};

/* Print out proper error */
void errorOut(string errno){
  cout << "ERROR: " << errno << endl;
  exit(0);  
}

/* Print out the correct string for each type */
string printTreeType(int i){
  if(i == TREE){
    return "tree-edge";
  }
  else if (i == BACK){
    return "back-edge";
  }
  else if(i == FORWARD){
    return "forward-edge";
  }
  else if(i == CROSS){
    return "cross-edge";
  }
  else{
    return "no-such-edge";
  }
}

/* Given a vertex number, find its coresponding vertex. Returns NULL if not found */
vertex* findVertex(unsigned int vecNumber, vector<vertex*> mainVec){
  for(unsigned int i = 0; i < mainVec.size(); i++){
    if(mainVec[i]->vecNumber == vecNumber){
      return mainVec[i];
    }
  }
  return NULL;
}

/* Search through the edge list and find the edge corresponding to the two vertices and set its type */
void typeTheTree(int type, unsigned int v1, unsigned int v2, vector<edge*> inputEdges){
  for(unsigned int i = 0; i < inputEdges.size(); i++){
    if(inputEdges[i]->v1 == v1 && inputEdges[i]->v2 == v2){
      inputEdges[i]->treeType = type;
    }
  }
}

/* Main DFS recursive algorithm*/
void DFSvisit(vertex *v, vector<vertex*> mainVec, vector<edge*> inputEdges){
  v->colour = GRAY;
  timer++;
  v->discoverTime=timer;
  vector<unsigned int> *vec = v->neighbours;
  for(unsigned int i = 0; i < vec->size(); i++){
    vertex *w = findVertex(vec->at(i),mainVec);
    if(w == NULL){
      ostringstream convert;
      convert << v->vecNumber;
      string vertexNumber = convert.str();
      ostringstream convert2;
      convert2 << vec->at(i);
      string adjacentNumber = convert2.str();
      string msg = "Vertex "+vertexNumber+" of neighbour "+adjacentNumber+" is invalid.";
      errorOut(msg);
    }
    /* Find the correct edge types */
    if(w->colour == WHITE){
      typeTheTree(TREE, v->vecNumber, w->vecNumber, inputEdges);
      DFSvisit(w,mainVec,inputEdges);
    }
    else if(w->colour == BLACK && (w->discoverTime < v->discoverTime) ){
      typeTheTree(CROSS, v->vecNumber, w->vecNumber, inputEdges);
    }
    else if(w->colour == BLACK && (v->discoverTime < w->discoverTime) ){
      typeTheTree(FORWARD, v->vecNumber, w->vecNumber, inputEdges);
    }
    else if(w->colour == GRAY){
      typeTheTree(BACK, v->vecNumber, w->vecNumber, inputEdges);    
    }
    
  }
  v->colour = BLACK;
  timer++;
  v->finishTime= timer;
}

/* DFS starter function */
void DFS(vector<vertex*> vec, vector<edge*> inputEdges){
  for(unsigned int i = 0; i < vec.size(); i++){
    if( vec[i]->colour == WHITE ){
      DFSvisit(vec[i], vec, inputEdges);
    }
  }
}

int main(){

  vector< vertex *> adjacentList;

  /* Obtain the vectors*/
  while(cin){
    string line;
    getline(cin, line);
    if(line.empty()){
      break;
    }

    // treat string as a stringstream                                                                                                                                                                       
    istringstream iss(line);
    
    //Obtain the point from stdin                                                                                                                                                                           
    unsigned int vecNum;
    string colon;
    iss >> vecNum;
    iss >> colon;
    
    vertex *vec = new vertex;
    vec->vecNumber = vecNum;
    vec->colour = WHITE;
    vec->discoverTime = 0;
    vec->finishTime = 0;
    vec->neighbours = new vector<unsigned int>;

    /* Error if the second argument is not a colon */
    if(colon != ":"){
      errorOut("Invalid syntax when listing adjacency list!");
    }

    /* Grab each neighbour and put it in the neighbour vector */
    while(iss.rdbuf()->in_avail() != 0){      
      unsigned int nextVec;
      iss >> nextVec;
      vec->neighbours->push_back(nextVec);
    }
    adjacentList.push_back(vec);
  }

  /* Obtain the input edges*/
  vector<edge*> inputEdges;
  while(cin){
    string line;
    getline(cin, line);
    if(line.empty()){
      break;
    }

    // treat string as a stringstream                                                                                                                                                                       
    istringstream iss(line);
    
    //Obtain the point from stdin                                                                                                                                                                           
    unsigned int v1;
    unsigned int v2;
    iss >> v1;
    iss >> v2;
    
    
    edge *e = new edge;
    e->v1 = v1;
    e->v2 = v2;
    e->treeType = -1;
    inputEdges.push_back(e);
  }
  
  /* Sort the neighbours */
  for(unsigned int i=0; i <adjacentList.size(); i++ ){
    sort (adjacentList[i]->neighbours->begin(), adjacentList[i]->neighbours->end());
  }

  /* Main DFS algorithm */
  DFS(adjacentList, inputEdges);

  /* Print output */
  for(unsigned int i=0;i<inputEdges.size();i++){
    cout << inputEdges[i]->v1 << " " << inputEdges[i]->v2 << " " << printTreeType(inputEdges[i]->treeType) << endl;
  }
}
