#include<stdio.h>
#include <stdlib.h>
#include<conio.h>

#define INFINITY 9999

int coord[50][2]; //save (x,y) coordinates
int point[50];	  //save sequence of points
int type[50];     //save number of branches at point
int explored[50];   //save number of times a point is passed
int final;
int totalPoints; // number of respective points
int compass;// 0 = N, 1 = E, 2 = S, 3 = W
int count; // number of times passing a point, place in sequence
int X,Y; //set to coordinates of current point
int branches;
int left,forward,right, back;
int l, f, r;
int eMargin = 0.1;
int previous;
int distance;
int following = 0;
int followCount=0;
int path[20];
int button;
int stage2 = 0;

void shuffleOrientation(void);
void turn(void);
void asessPoint(int distance);
void getDirection(int prev, int c);
void dijkstra(int G[totalPoints][totalPoints],int n,int startnode);
void solve(int begin);
int checkBranches(int p);

int main(void){
while(1)
	{
		if(stage2)
	{
		printf("waiting for button press \n");
		fflush(stdout);
		scanf("%d", &button);
		X=0;
		Y=0;
		asessPoint(0);
		stage2 = 0;
	}

	printf("X = %d ",coord[point[count-1]][0]);
	printf("Y = %d \n",coord[point[count-1]][1]);
	printf("%d \n",point[count - 1]);
	printf("enter distance left forward right \n");
	fflush(stdout);
	scanf("%d %d %d %d", &distance, &l, &f, &r);
	printf("The number you typed was %d %d %d %d\n", distance, l, f, r);
	if(distance==420)
	{
		break;
	}
	if(l+f+r>3)
	{
		final = totalPoints;
		l = 0;
		f = 0;
		r = 0;
		printf("final point = point %d",final);
		//fflush(stdout);
	}
	asessPoint(distance);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////
//*************Navigation*******************************************************
/////////////////////////////////////////////////////////////////////////////////
void findUnexplored(void)
{ // if we reach here, l f r b all = 0; this function will check all points starting from surrounding points and then set a direction positive which leads to an unexplored point. Than shuffle orientation (as others all = 0). than call turn again.
	printf("finding unexplored points\n");
	fflush(stdout);
	int cPoint = point[count-1]; //current point
	for (volatile int i = 0; i< count; i++)
	{
		for (volatile int h = 1; h < count; h++)
		{
			//printf("entered j %d\n",h);
			fflush(stdout);
			if(point[h] == cPoint)
			{
				//printf("found point\n");
				fflush(stdout);
				int prev = point[h-i];
				if(type[prev]>explored[prev])
				{
					if(checkBranches(prev))
					{
					printf("prev\n");
					fflush(stdout);
					getDirection(point[h-1], cPoint);
					shuffleOrientation();
					turn();
					return;
					//get direction from cPoint -> prev
					}
				}
				int post = point[h+i];
				if(type[post]>explored[post])
				{
					if(checkBranches(post))
					{
					printf("post\n");
					fflush(stdout);
					getDirection(point[h+1], cPoint);
					shuffleOrientation();
					turn();
					return;
					}
					//get direction from cPoint -> post
				}
			}
		}
	}
	printf("maze explored\n");
	following = 1;
	solve(point[count-1]);
	//stop, calculate shortest path
}

int checkBranches(int p)
{
	int backBranche = 1;//There will always be a path behind you unless it is the starting point
	if(p==0)
	{
		backBranche = 0;
	}
	int pathsTaken[4] = {100, 100, 100, 100};
	int numPaths = 0;
	int confirmedType = 0;
	for(volatile int j = 0;j<count-1; j++)
	{
		if(point[j] == p)
		{
			int prevSeen = 0;
			int postSeen = 0;
			for(volatile int k = 0; k<4;k++)
			{
				if(point[j-1]==pathsTaken[k])
				{
					prevSeen = 1;
				}
				if(point[j+1]==pathsTaken[k])
				{
					postSeen = 1;
				}

			}
			if(!prevSeen)
			{
				pathsTaken[numPaths] = point[j-1];
				numPaths ++;
			}
			if(!postSeen && (point[j-1]!=point[j+1]))
			{
				pathsTaken[numPaths] = point[j+1];
				numPaths ++;
			}
		}
	}
	for(volatile int k = 0; k<4;k++)
	{
		if(pathsTaken[k]<99)
		{
			confirmedType ++;
		}
	}
	if(confirmedType < type[p] + backBranche)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void turn(void)
{

	if(left)
	{
		printf("turn left\n");
		fflush(stdout);
		compass += 3;
		compass = compass%4;
		return;
	}
	else if (forward)
	{
		printf("go straight\n");
		fflush(stdout);
		return;
	}
	else if(right)
	{
		printf("turn right\n");
		fflush(stdout);
		compass += 1;
		compass = compass%4;
		return;
	}
	else if(back)
	{
		printf("U-Turn \n");
		fflush(stdout);
		compass += 2;
		compass = compass%4;
		return;
	}
	else
	{
		if(type[point[count-1]] == 0)
		{
			printf("U-Turn \n");
			fflush(stdout);
			compass += 2;
			compass = compass%4;
			return;
		}
		else
		{
			findUnexplored();
			return;
		}

	}

							// provide instruction for when all neighbouring routes are explored

}

void getDirection(int prev, int c)
{// given two points
	printf("getting direction\n");
	int prevX = coord[prev][0];
	int prevY = coord[prev][1];
	int cX = coord[c][0];
	int cY = coord[c][1];
	if( abs(prevX - cX) > eMargin)		// checking for direction of previous path
	{
		if(prevX < cX)
		{
			left = 1;
		}
		if(prevX > X)
		{
			right = 1;
		}
	}
	else
	{
		if(prevY < cY)
		{
			back = 1;
		}
		if(prevY > cY)
		{
			forward = 1;
		}
	}
}

void decideTurn(int pointID)
{
	printf("deciding turn, pointID = %d \n",pointID);
	int prev, post;
	left = 1;
	right = 1;
	forward = 1;
	back = 1;
	//write code to eliminate dead ends
	if(compass == 0)
	{
		left = left&l;
		right = right&r;
		forward = forward&f;
		back = 0;
	}
	if(compass == 1)
	{
		forward = forward&l;
		right = right&f;
		back = back&r;
		left = 0;
	}
	if(compass == 2)
	{
		left = left&r;
		back = back&f;
		right = right&l;
		forward = 0;
	}
	if(compass == 3)
	{
		forward = forward&r;
		left = left&f;
		back = back&l;
		right = 0;
	}
	for(volatile int i = 0; i<count-1;i++) //latest point was logged at count-1 therefore we do not want to get that high as we only need to check prev
	{
		if (point[i] == pointID)	//check if left forward right back have been explored assuming facing north
		{
			//printf("i, point[i] = %d, %d \n", i, point[i]);
			//fflush(stdout);
			prev = point[i-1];
			post = point[i+1];
			int prevX = coord[prev][0];
			int prevY = coord[prev][1];
			int postX = coord[post][0];
			int postY = coord[post][1];
			if( abs(prevX-X) > eMargin)		// checking for direction of previous path
			{
				if(prevX < X)
				{
					left = 0;
				}
				if(prevX > X)
				{
					right = 0;
				}
			}
			else
			{
				if(prevY < Y)
				{
					//printf("south\n");
					//fflush(stdout);
					back = 0;
				}
				if(prevY > Y)
				{
					forward = 0;
				}
			}
			if( abs(postX-X) > eMargin) // checking for path taken from point
			{
				if(postX < X)
				{
					//printf("west\n");
					//fflush(stdout);
					left = 0;
				}
				if(postX > X)
				{
					right = 0;
				}
			}
			else
			{
				if(postY < Y)
				{
					back = 0;
				}
				if(postY > Y)
				{
					forward = 0;
				}
			}
		}
	}
	shuffleOrientation();
	back = 0;
	left = left&l;
	forward = forward&f;
	right = right&r;
	turn();

}


void saveNewPoint(void)
{
	printf("new point \n");
	point[count] = totalPoints;
	coord[totalPoints][0] = X;
	coord[totalPoints][1] = Y;
	type[totalPoints] = branches;
	explored[totalPoints] = 1;
	count ++;
	totalPoints ++;
	turn();
}

void revisitPoint(int i)
{
	printf("point revisited\n");
	point[count] = i;
	explored[i] ++;
	count ++;
	decideTurn(i);
}

void asessPoint(int distance)
{
	printf("asessing point\n");
	previous = 0;
	branches = l + r + f;
	left = l;
	forward = f;
	right = r;
	back = 1;
	if (compass == 0)
	{
		Y += distance;
	}
	if (compass == 1)
	{
		X += distance;
	}
	if (compass == 2)
	{
		Y -= distance;
	}
	if (compass ==3)
	{
		X -= distance;
	}

	if(!following)
	{
	for( volatile int h = 0; h< count; h++)
	{
		//printf("coords X, Y %d, %d \n", coord[h][0], coord[h][1]);
		if(coord[h][0] == X){
			if(coord[h][1] == Y)
			{
				previous = 1;
				printf("previous point\n");
				fflush(stdout);
				revisitPoint(h);
				return;
			}
		}

	}
	if(!previous){
		saveNewPoint();
	}
	}
	else
	{
		left=0;
		right=0;
		forward=0;
		back = 0;
		getDirection(path[followCount-1], path[followCount]);
		shuffleOrientation();
		left = left&l;
		forward = forward&f;
		right = right&r;
		followCount --;
		turn();
		if(followCount == 0)
		{
			stage2 = 1;
			compass = 0;
			solve(0);
			//asessPoint(0);
		}
		return;
	}
}

void shuffleOrientation(void) //Rearrange L F R B from forward = north to forward = current orientation
{
	for (int i=0; i<compass; i++)
	{
		int hold = left;
		left = forward;
		forward = right;
		right = back;
		back = hold;
	}
}


///////////////////////////////////////////////////////////////////
//**********Dijkstra**********************************************
//////////////////////////////////////////////////////////////////

int getDistance(int cPoint,int nPoint)
{
	int cX = coord[cPoint][0];
	int nX = coord[nPoint][0];
	if( abs(cX-nX) > eMargin)
	{
		return abs( cX - nX );
	}
	else
	{
		return abs( coord[cPoint][1] - coord[nPoint][1] );
	}
}

void solve(int begin)
{
	int adj[totalPoints][totalPoints];
	int cPoint, nPoint, distance;
	for (volatile int i=0; i<totalPoints; i++)
		{
			for(volatile int j= 0; j< totalPoints; j++)
			{
				adj[i][j] = 0;
			}
		}
	for (volatile int i = 0; i<count-1;i++)
	{
		cPoint = point[i];
		nPoint = point[i+1];
		distance = getDistance(cPoint, nPoint);
//		printf("distance=%d",distance);
		fflush(stdout);
		adj[cPoint][nPoint] = distance;
		adj[nPoint][cPoint] = distance;
	}
/*	for (volatile int i=0; i<totalPoints; i++)
	{
		for(volatile int j= 0; j< totalPoints; j++)
		{
//			printf("%d, ", adj[i][j]);
		}
//		printf("\n");
		fflush(stdout);
	}
*/

	dijkstra(adj, totalPoints, begin);
	if(!stage2)
	{
	asessPoint(0);
	}

}

void dijkstra(int G[totalPoints][totalPoints],int n,int startnode)
{

	for(volatile int g=0; g<20; g++)
	{
	path[g] = 0;
	}
	int cost[totalPoints][totalPoints],distance[totalPoints],pred[totalPoints];
	int visited[totalPoints],num,mindistance,nextnode,i,j;

	//pred[] stores the predecessor of each node
	//count gives the number of nodes seen so far
	//create the cost matrix
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			if(G[i][j]==0)
				cost[i][j]=INFINITY;
			else
				cost[i][j]=G[i][j];

	//initialize pred[],distance[] and visited[]
	for(i=0;i<n;i++)
	{
		distance[i]=cost[startnode][i];
		pred[i]=startnode;
		visited[i]=0;
	}

	distance[startnode]=0;
	visited[startnode]=1;
	num=1;

	while(num<n-1)
	{
		mindistance=INFINITY;

		//nextnode gives the node at minimum distance
		for(i=0;i<n;i++)
			if(distance[i]<mindistance&&!visited[i])
			{
				mindistance=distance[i];
				nextnode=i;
			}

			//check if a better path exists through nextnode
			visited[nextnode]=1;
			for(i=0;i<n;i++)
				if(!visited[i])
					if(mindistance+cost[nextnode][i]<distance[i])
					{
						distance[i]=mindistance+cost[nextnode][i];
						pred[i]=nextnode;
					}
		num++;
	}

	//print the path and distance of each node
	for(i=0;i<n;i++)
		if(i!=startnode)
		{
		//	printf("\nDistance of node%d=%d",i,distance[i]);
		//	printf("\nPath=%d",i);
			j=i;
		if(j == final)
		{
			do
			{

				path[followCount] = j;
				followCount ++;
				j=pred[j];
			//	printf("<-%d",j);
			}while(j!=startnode);
		}
	}
	if(!stage2)
	{
	path[followCount] = point[count-1];
	}
}






