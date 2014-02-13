/*
 * Closest pair algorithm to run in nlogn time by
 * using the straddle-zone left/right recursive
 * algorithm
 *
 * @author: Serj Kazar
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

////////////////////////////////////////////////////////////
//
// Structs used to hold the points

/* forward declaration of ypoint */
struct yPoint;

struct xPoint{
  double x;
  yPoint *y;
};

struct yPoint{
  double y;
  xPoint *x;
};

struct ppair{
  bool isLegal;
  xPoint *x1;
  xPoint *x2;
};


////////////////////////////////////////////////////////////
//
// Operator overloads for above structs

inline bool operator>(const xPoint& lhs, const xPoint& rhs){return (lhs.x > rhs.x);}
inline bool operator<(const xPoint& lhs, const xPoint& rhs){return (lhs.x < rhs.x);}
inline bool operator==(const xPoint& lhs, const xPoint& rhs){return (lhs.x == rhs.x);}
inline bool operator>(const yPoint& lhs, const yPoint& rhs){return (lhs.y > rhs.y);}
inline bool operator<(const yPoint& lhs, const yPoint& rhs){return (lhs.y < rhs.y);}
inline bool operator==(const yPoint& lhs, const yPoint& rhs){return (lhs.y == rhs.y);}
struct compareX {
  bool operator() (xPoint *x1,xPoint *x2) { return (*x1<*x2);}
} compareXpoints;
struct compareY {
  bool operator() (yPoint *y1,yPoint *y2) { return (*y1<*y2);}
} compareYpoints;


////////////////////////////////////////////////////////////
//
// Functions

/*
 * getDistance: Takes in two points and returns the distance between them.
 */
double getDistance(xPoint *left, xPoint *right){
  return sqrt(pow((left->x - right->x),2)+pow((left->y->y - right->y->y),2));
}

/*
 * isStrictlyLarger: Compares two pairs and decides which is larger
 *
 * return value: true if p1 > p2
 */
bool isStrictlyLarger(ppair p1, ppair p2){
  xPoint *left = p1.x1;
  xPoint *right = p1.x2;
  double leftD = getDistance(left,right);

  left = p2.x1;
  right = p2.x2;
  double rightD = getDistance(left,right);

  return (leftD > rightD);
}

/*
 * closestpoint: recursive algorithm that does most of the work.
 *               will recursively find the smallest pair.
 *
 * return value: the smallest pair
 */
ppair closestpoint(vector<xPoint*> xPoints, vector<yPoint*> yPoints, vector<xPoint*>::iterator start, vector<xPoint*>::iterator end){

  /* There is one point only: return incomplete pair */
  if (start == end){
    ppair p;
    p.isLegal = false;
    p.x1=*start;
    return p;
  }
  /* There are two points: return as pair */
  if ((start+1) == end){
    ppair p;
    p.isLegal = true;
    p.x1 = (*start);
    p.x2 = *(start+1);
    return p;
  }

  /* Find the smallest point on the left and right*/
  vector<xPoint*>::iterator middle = start+((end-start)/2);
  ppair leftSmallest = closestpoint(xPoints, yPoints, start, middle);
  ppair rightSmallest = closestpoint(xPoints, yPoints, (middle+1), end);
  
  /* BASE CASES */
  /* If the left/right only have one point each, combine them into one pair*/
  if( !leftSmallest.isLegal && !rightSmallest.isLegal){
    ppair p;
    p.isLegal = true;
    p.x1 = leftSmallest.x1;
    p.x2 = rightSmallest.x1;
    return p;
  }
  /* If there is 1 point on the left, and 2 on the right, find the smallest of the three */
  else if(!leftSmallest.isLegal){
    double currentD = getDistance(rightSmallest.x1,rightSmallest.x2);
    double pair1 = getDistance(leftSmallest.x1,rightSmallest.x1);
    double pair2 = getDistance(leftSmallest.x1,rightSmallest.x2);
    if(pair1 < currentD && pair1 < pair2){
      ppair p;
      p.isLegal = true;
      p.x1 = leftSmallest.x1;
      p.x2 = rightSmallest.x1;
      return p;
    }
    if(pair2 < currentD && pair2 < pair1){
      ppair p;
      p.isLegal = true;
      p.x1 = leftSmallest.x1;
      p.x2 = rightSmallest.x2;
      return p;
    }
    return rightSmallest;
  }
  /* If there is 1 point on the right, and 2 on the left, find the smallest of the three */
  else if(!rightSmallest.isLegal){
    double currentD = getDistance(leftSmallest.x1,leftSmallest.x2);
    double pair1 = getDistance(rightSmallest.x1,leftSmallest.x1);
    double pair2 = getDistance(rightSmallest.x1,leftSmallest.x2);
    if(pair1 < currentD && pair1 < pair2){
      ppair p;
      p.isLegal = true;
      p.x1 = rightSmallest.x1;
      p.x2 = leftSmallest.x1;
      return p;
    }
    if(pair2 < currentD && pair2 < pair1){
      ppair p;
      p.isLegal = true;
      p.x1 = rightSmallest.x1;
      p.x2 = leftSmallest.x2;
      return p;
    }
    return leftSmallest;
  }

  /* If its not a base case, obtain the smallest pair of the two */
  ppair smallestPair;
  if(isStrictlyLarger(leftSmallest,rightSmallest)){
    smallestPair = leftSmallest;
  }
  else{
    smallestPair = rightSmallest;
  }

  /* Obtain points that are in the straddle zone */
  vector <xPoint*> straddlePoints;
  double delta = getDistance(smallestPair.x1,smallestPair.x2);
  xPoint *centralPoint = *middle;
  straddlePoints.push_back(centralPoint);
  for(vector <xPoint*>::iterator it=xPoints.begin();it<xPoints.end();++it){
    if ( abs((centralPoint->x - (*it)->x)) < delta){
      straddlePoints.push_back(*it);
    }
  }

  /* Go through all points in y, in order, and find the points in
  * the straddle zone. Get the distance between it and up to 6 points below
  * it then compare it with the current smallest point.
  */
  for(vector<yPoint*>::iterator it = yPoints.begin(); it != yPoints.end(); ++it) {
    /* If the point is in the straddle zone */
    if( find(straddlePoints.begin(), straddlePoints.end(), (*it)->x) != straddlePoints.end() ){
      /* Compare with the 6 points below it that are in the straddle zone as well*/
      vector<yPoint*>::iterator p = it;
      p++;
      for(int i = 0; i < 6; i++){
	/* Went too far down in the list */
	if(p == yPoints.end()){
	  break;
	}
	/* If the point is in the straddle zone and has less distance than the current smallest pair, update*/
	if (find(straddlePoints.begin(), straddlePoints.end(), (*p)->x) != straddlePoints.end()){
	  double dist = getDistance((*it)->x, (*p)->x);
	  double curSmallest = getDistance(smallestPair.x1, smallestPair.x2);
	  if(dist < curSmallest){
	    ppair pa;
	    pa.isLegal = true;
	    pa.x1 = (*p)->x;
	    pa.x2 = (*it)->x;
	    smallestPair=pa;
	  }
	}/* If in the straddle zone */	
	/* If its not in the straddle zone, do not increment i */
	else{ i--; }
	p++;
      }/* search next 6*/
    }/* if point is in straddle zone*/
  }/* all points in y*/
  
  return smallestPair;
}


int main(){
  
  vector<xPoint*> originalOrder; // Stores points in original order
  vector<xPoint*> xPoints;       // Stores x values in sorted order
  vector<yPoint*> yPoints;       // Stores y values in sorted order

   /* Grab input */
   while(cin){
     string line;
     getline(cin, line);
     if(line.empty()){
       break;
     }
     
     // treat string as a stringstream
     istringstream iss(line);

     //Obtain the point from stdin
     double x;
     double y;
     iss >> x;
     iss >> y;

     xPoint *xp = new xPoint;
     yPoint *yp = new yPoint;
     // Set values
     xp->x=x;
     yp->y=y;
     // Set pointers
     xp->y=yp;
     yp->x=xp;
	
     //insert into list
     originalOrder.push_back(xp);
     xPoints.push_back(xp);
     yPoints.push_back(yp);
     
   }

   /* Sort the two vectors by x and y */
   sort(xPoints.begin(),xPoints.end(),compareXpoints);
   sort(yPoints.begin(),yPoints.end(),compareYpoints);

   /* Obtain the smallest pair */
   ppair smallestPair;
   smallestPair = closestpoint(xPoints,yPoints,xPoints.begin(),(xPoints.end()-1));

   /* Find and output the pairs indices in the original list */
   int index1 = -1;
   int index2 = -1;
   xPoint *point1 = smallestPair.x1;
   xPoint *point2 = smallestPair.x2;
   for(int i =0; i < originalOrder.size();i++){
     if(originalOrder[i] == point1){
       index1 = i;
     }
     else if(originalOrder[i] == point2){
       index2 = i;
     }
   }
   if(index2 < index1){
     int tmp = index1;
     index1=index2;
     index2=tmp;
   }
   cout << index1 << endl;
   cout << index2 << endl;

   /* Free memory */
   for(int i=0;i<xPoints.size();i++){
     delete xPoints[i]->y;
     delete xPoints[i];
   }
}
