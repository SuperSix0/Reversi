//#include <vector>
//#include <set>
//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//
//using namespace std;
//
//typedef struct position
//{
//	int x;
//	int y;
//	struct position() :x(0), y(0) {};
//	struct position(int xx, int yy) :x(xx), y(yy) {}
//	bool operator < (const struct position &a) const
//	{
//		return x < a.x || (x == a.x && y < a.y);
//	}
//}Position;
//
//set<Position>candidate;
//int board[8][8];//0 for empty,1 for black,2 for white;I'm black
//int maxDepth = 4, maxRandom = 100;
//int Move[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
//
//void check(vector<Position>&dst,int type)
//{
//	dst.clear();
//	int x, y, tx, ty;
//	bool enable;
//	set<Position>::iterator iter;
//	for (iter = candidate.begin(); iter != candidate.end(); iter++)
//	{
//		x = iter -> x;
//		y = iter -> y;
//		enable = false;
//		for (int i = 0; i < 8; i++)
//		{
//			tx = x + Move[i][0];
//			ty = y + Move[i][1];
//			if (tx < 0 || tx >= 8 || ty < 0 || ty >= 8)
//				continue;
//			if (board[tx][ty] == 0 || board[tx][ty] == type)
//				continue;
//			while (1)
//			{
//				tx = tx + Move[i][0];
//				ty = ty + Move[i][1];
//				if (tx < 0 || tx >= 8 || ty < 0 || ty >= 8)
//					break;
//				if (board[tx][ty] == 0)
//					break;
//				if (board[tx][ty] == type)
//				{
//					enable = true;
//					break;
//				}
//			}
//		}
//		if (enable == true)
//			dst.push_back(*iter);
//	}
//}
//
//int checkResult()//0 for not finish,1 for black win,2 for white win,3 for tie
//{
//	int white = 0, black = 0;
//	for (int i = 0; i < 8; i++)
//		for (int j = 0; j < 8; j++)
//			if (board[i][j] == 1)
//				black++;
//			else if (board[i][j] == 2)
//				white++;
//	if (black == 0)
//		return 2;
//	else if (white == 0)
//		return 1;
//	else if (black + white == 64)
//	{
//		if (black > white)
//			return 1;
//		else if (black < white)
//			return 2;
//		else
//			return 3;
//	}
//	else
//	{
//		int w, b;
//		vector<Position> dst;
//		check(dst, 1);
//		w = dst.size();
//		check(dst, 2);
//		b = dst.size();
//		if (w == 0 && b == 0)
//			return black > white ? 1 : 2;
//	}
//	return 0;
//}
//
//void change(Position src, vector<Position>&dst)
//{
//	dst.clear();
//	int x, y, maxx = src.x, maxy = src.y;
//	for (int i = 0; i < 8; i++)
//	{
//		x = src.x;
//		y = src.y;
//		maxx = src.x;
//		maxy = src.y;
//		while (1)
//		{
//			x += Move[i][0];
//			y += Move[i][1];
//			if (x < 0 || x >= 8 || y < 0 || y >= 8)
//				break;
//			if (board[x][y] == 0)
//				break;
//			if (board[x][y] == board[src.x][src.y])
//			{
//				maxx = x;
//				maxy = y;
//				break;
//			}
//		}
//		x = src.x;
//		y = src.y;
//		while (x != maxx || y != maxy)
//		{
//			x += Move[i][0];
//			y += Move[i][1];
//			if (board[src.x][src.y] != board[x][y])
//			{
//				board[x][y] = board[src.x][src.y];
//				dst.push_back(Position(x, y));
//			}
//		}
//	}
//}
//
//void undo(vector<Position>dst)
//{
//	for (int i = dst.size() - 1; i >= 0; i--)
//	{ 
//		int x = dst[i].x, y = dst[i].y;
//		board[x][y] = board[x][y] % 2 + 1;
//	}
//}
//
//bool randomDfs(int depth)
//{
//	/*if (depth > 100)
//	{
//		for (int i = 0; i < 8; i++)
//		{
//			for (int j = 0; j < 8; j++)
//				cout << board[i][j] << " ";
//			cout << endl;
//		}
//		system("pause");
//	}*/
//	//cout << "randomDfs:" << depth << endl;
//	bool ans;
//	int k = checkResult();
//	if (k == 1 || k == 3)
//		return true;
//	else if (k == 2)
//		return false;
//	vector<Position>dst;
//	check(dst, depth % 2 + 1);
//	if (dst.size() == 0)
//	{
//		ans = randomDfs(depth + 1);
//	}
//	else
//	{
//		bool success[8] = { false, false, false, false, false, false, false, false };
//		int i = rand() % dst.size();
//		Position d = dst[i];
//		vector<Position>().swap(dst);
//		candidate.erase(d);
//		board[d.x][d.y] = depth % 2 + 1;
//		int x, y;
//		for (int j = 0; j < 8; j++)
//		{
//			x = d.x + Move[j][0];
//			y = d.y + Move[j][1];
//			if (x < 0 || x >= 8 || y < 0 || y >= 8)
//				continue;
//			if (board[x][y] == 0)
//				success[j] = candidate.insert(Position(x, y)).second;
//		}
//		vector<Position> c;
//		change(d, c);
//		ans = randomDfs(depth + 1);
//		undo(c);
//		for (int j = 0; j < 8; j++)
//		{
//			x = d.x + Move[j][0];
//			y = d.y + Move[j][1];
//			if (x < 0 || x >= 8 || y < 0 || y >= 8)
//				continue;
//			if (board[x][y] == 0 && success[j] == true)
//				candidate.erase(Position(x,y));
//		}
//		candidate.insert(d);
//		board[d.x][d.y] = 0;
//	}
//	return ans;
//}
//
//double dfs(int depth)
//{
//	//cout << "Dfs:" << depth << endl;
//	double ans;
//	if (depth == maxDepth)
//	{
//		ans = 0;
//		for (int i = 0; i < maxRandom; i++)
//			if (randomDfs(depth))
//				ans++;
//		ans /= maxRandom;
//	}
//	else
//	{
//		int k = checkResult();
//		if (k == 1)
//			return 1;
//		else if (k == 2)
//			return 0;
//		else if (k == 3)
//			return 0.5;
//		vector<Position>dst;
//		check(dst,depth % 2 + 1);
//		ans = depth % 2;
//		if (dst.size() == 0)
//		{
//			ans = dfs(depth + 1);
//		}
//		else
//		{
//			for (unsigned int i = 0; i < dst.size(); i++)
//			{
//				bool success[8] = { false, false, false, false, false, false, false, false };
//				candidate.erase(dst[i]);
//				board[dst[i].x][dst[i].y] = depth % 2 + 1;
//				int x, y;
//				for (int j = 0; j < 8; j++)
//				{
//					x = dst[i].x + Move[j][0];
//					y = dst[i].y + Move[j][1];
//					if (x < 0 || x >= 8 || y < 0 || y >= 8)
//						continue;
//					if (board[x][y] == 0)
//						success[j] = candidate.insert(Position(x, y)).second;
//				}
//				vector<Position> c;
//				change(dst[i], c);
//				double rate = dfs(depth + 1);
//				undo(c);
//				if (depth % 2 == 0)
//				{
//					if (rate > ans)
//						ans = rate;
//				}
//				else
//				{
//					if (rate < ans)
//						ans = rate;
//				}
//				for (int j = 0; j < 8; j++)
//				{
//					x = dst[i].x + Move[j][0];
//					y = dst[i].y + Move[j][1];
//					if (x < 0 || x >= 8 || y < 0 || y >= 8)
//						continue;
//					if (board[x][y] == 0 && success[j] == true)
//						candidate.erase(Position(x, y));
//				}
//				candidate.insert(dst[i]);
//				board[dst[i].x][dst[i].y] = 0;
//			}
//		}
//	}
//	return ans;
//}
//
//Position top()
//{
//	//cout << "top\n";
//	vector<Position>dst;
//	check(dst, 1);
//	double r = 0;
//	Position ans(-1,-1);
//	for (unsigned int i = 0; i < dst.size(); i++)
//	{
//		candidate.erase(dst[i]);
//		board[dst[i].x][dst[i].y] = 1;
//		int x, y;
//		bool success[8] = { false, false, false, false, false, false, false, false };
//		for (int j = 0; j < 8; j++)
//		{
//			x = dst[i].x + Move[j][0];
//			y = dst[i].y + Move[j][1];
//			if (x < 0 || x >= 8 || y < 0 || y >= 8)
//				continue;
//			if (board[x][y] == 0)
//				success[j] = candidate.insert(Position(x, y)).second;
//		}
//		vector<Position> c;
//		change(dst[i], c);
//		double rate = dfs(1);
//		undo(c);
//		if (rate > r)
//		{
//			ans.x = dst[i].x;
//			ans.y = dst[i].y;
//		}
//		for (int j = 0; j < 8; j++)
//		{
//			x = dst[i].x + Move[j][0];
//			y = dst[i].y + Move[j][1];
//			if (x < 0 || x >= 8 || y < 0 || y >= 8)
//				continue;
//			if (board[x][y] == 0 && success[j] == true)
//				candidate.erase(Position(x, y));
//		}
//		candidate.insert(dst[i]);
//		board[dst[i].x][dst[i].y] = 0;
//	}
//	return ans;
//}
//
//void black()
//{
//	Position P;
//	vector<Position>dst;
//	check(dst, 1);
//	if (dst.size() == 0)
//		return;
//	P = top();
//	if (P.x == -1 || P.y == -1)
//		return;
//	cout << "黑子下:(" << P.x << "," << P.y << ")\n";
//	board[P.x][P.y] = 1;
//	change(P, dst);
//	candidate.erase(P);
//	int x, y;
//	for (int i = 0; i < 8; i++)
//	{
//		x = P.x + Move[i][0];
//		y = P.y + Move[i][1];
//		if (x < 0 || x >= 8 || y < 0 || y >= 8)
//			continue;
//		if (board[x][y] == 0)
//		{
//			candidate.insert(Position(x, y));
//		}
//	}
//	for (int i = 0; i < 8; i++)
//	{
//		for (int j = 0; j < 8; j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//void white()
//{
//	vector<Position>dst;
//	check(dst, 2);
//	if (dst.size() == 0)
//		return;
//	cout << "您可下的位置有:\n";
//	for (int i = 0; i < dst.size(); i++)
//	{
//		cout << "(" << dst[i].x << "," << dst[i].y << ")\n";
//	}
//	int x, y;
//	cin >> x >> y;
//	Position TMP(x, y);
//	if (candidate.count(TMP) == 0)
//	{
//		cout << "error:illegal position" << endl;
//		while (1);
//	}
//	else
//	{
//		candidate.erase(TMP);
//		board[TMP.x][TMP.y] = 2;
//		change(TMP, dst);
//	}
//	for (int i = 0; i < 8; i++)
//	{
//		x = TMP.x + Move[i][0];
//		y = TMP.y + Move[i][1];
//		if (x < 0 || x >= 8 || y < 0 || y >= 8)
//			continue;
//		if (board[x][y] == 0)
//		{
//			candidate.insert(Position(x, y));
//		}
//	}
//	for (int i = 0; i < 8; i++)
//	{
//		for (int j = 0; j < 8; j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//
//int main()
//{
//	srand(time(NULL));
//	board[3][3] = 1;
//	board[3][4] = 2;
//	board[4][3] = 2;
//	board[4][4] = 1;
//	for (int i = 0; i < 8; i++)
//	{
//		for (int j = 0; j < 8; j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << endl;
//	}
//	Position p[12];
//	p[0].x = 2; p[0].y = 2;
//	candidate.insert(p[0]);
//	p[1].x = 2; p[1].y = 3;
//	candidate.insert(p[1]);
//	p[2].x = 2; p[2].y = 4;
//	candidate.insert(p[2]);
//	p[3].x = 2; p[3].y = 5;
//	candidate.insert(p[3]);
//	p[4].x = 3; p[4].y = 5;
//	candidate.insert(p[4]);
//	p[5].x = 4; p[5].y = 5;
//	candidate.insert(p[5]);
//	p[6].x = 5; p[6].y = 5;
//	candidate.insert(p[6]);
//	p[7].x = 5; p[7].y = 4;
//	candidate.insert(p[7]);
//	p[8].x = 5; p[8].y = 3;
//	candidate.insert(p[8]);
//	p[9].x = 5; p[9].y = 2;
//	candidate.insert(p[9]);
//	p[10].x = 4; p[10].y = 2;
//	candidate.insert(p[10]);
//	p[11].x = 3; p[11].y = 2;
//	candidate.insert(p[11]);
//	
//	int k;
//	cout << "你想先手(0)/后手(1):\n";
//	cin >> k;
//	Label:
//	if (k == 0)
//	{
//		board[3][3] = 2;
//		board[3][4] = 1;
//		board[4][3] = 1;
//		board[4][4] = 2;
//		white();
//	}
//	while (1)
//	{
//		black();
//		white();
//	}
//	system("pause");
//}

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
clock_t Begin;
using namespace std;
int Move[8][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };
typedef struct node
{
	char board[8][8];
	int scores;
	int total;
	struct node *parent;
	struct node **child;
	int number;
	int x, y;
	int turn;
}Node;

bool check(char board[8][8], int x, int y,int turn)
{
	bool enable = false;
	for (int k = 0; k < 8; k++)
	{
		int ii = x + Move[k][0], jj = y + Move[k][1];
		if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
			|| board[ii][jj] == 0
			|| board[ii][jj] == turn)
			continue;
		while (1)
		{
			ii = ii + Move[k][0];
			jj = jj + Move[k][1];
			if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
				|| board[ii][jj] == 0)
				break;
			else if (board[ii][jj] == turn)
			{
				enable = true;
				break;
			}
		}
	}
	return enable;
}

int checkResult(char board[8][8])//0 for not finish,1 for black win,2 for white win,3 for tie
{
	int white = 0, black = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] == 1)
				black++;
			else if (board[i][j] == 2)
				white++;
	if (black == 0)
		return 2;
	else if (white == 0)
		return 1;
	else if (black + white == 64)
	{
		if (black > white)
			return 1;
		else if (black < white)
			return 2;
		else
			return 3;
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j] == 0)
				{
					if (check(board, i, j, 1) == true)
						return 0;
					if (check(board, i, j, 2) == true)
						return 0;
				}
			}
		}
		if (black > white)
			return 1;
		else if (black < white)
			return 2;
		else
			return 3;
	}
}

void add(Node *root,int turn)
{
	//cout << ":" << root->child.size();
	root->child = (Node **)malloc(sizeof(Node*) * 32);
	root->number = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//cout << i << " " << j << endl;
			if (root->board[i][j] == 0)
			{
				if (check(root->board,i,j,turn) == true)
				{
					Node* ptr;
					ptr = (Node*)malloc(sizeof(Node));
					ptr->scores = 0;
					ptr->total = 0;
					ptr->parent = root;
					ptr->number = 0;
					ptr->child = NULL;
					ptr->x = i;
					ptr->y = j;
					ptr->turn = turn % 2 + 1;
					for (int a = 0; a < 8; a++)
					{
						for (int b = 0; b < 8; b++)
						{
							ptr->board[a][b] = root->board[a][b];
						}
					}
					for (int k = 0; k <= 8; k++)
					{
						int ii = i + Move[k][0], jj = j + Move[k][1];
						if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
							|| root->board[ii][jj] == 0
							|| root->board[ii][jj] == turn)
							continue;
						while (1)
						{
							ii = ii + Move[k][0];
							jj = jj + Move[k][1];
							if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
								|| root->board[ii][jj] == 0)
								break;
							else if (root->board[ii][jj] == turn)
							{
								while (ii != i || jj != j)
								{
									ii -= Move[k][0];
									jj -= Move[k][1];
									ptr->board[ii][jj] = turn;
								}
								break;
							}
						}
					}
					root->child[root -> number++] = ptr;
				}
			}
		}
	}
	if (root->number == 0 && checkResult(root->board) == 0)
	{
		free(root->child);
		add(root, turn % 2 + 1);
	}
	else
	{
		Node **tmp;
		tmp = (Node **)malloc(sizeof(Node*) * root->number);
		for (int i = 0; i < root->number; i++)
		{
			tmp[i] = root->child[i];
		}
		free(root->child);
		root->child = tmp;
	}
}

Node* search(Node *root)
{
	Node *tmp = root;
	while (1)
	{
		if (tmp->total == 0)
		{
			return tmp;
		}
		if (checkResult(tmp->board) == 0)
		{
			for (int i = 0; i < tmp->number; i++)
			{
				if (tmp->child[i]->total == 0)
				{
					tmp = tmp->child[i];
					return tmp;
				}
			}
			double max = 0;
			double t;
			Node *tt = NULL;
			for (int i = 0; i < tmp->number; i++)
			{
				Node* ttmp = tmp->child[i];
				t = 1.0 * ttmp->scores / ttmp->total + sqrt(2 * log(tmp->total) / ttmp->total);
				if (t > max)
				{
					tt = ttmp;
					max = t;
				}
			}
			tmp = tt;
		}
		else
			return tmp;
	}
}

typedef struct position
{
	int x;
	int y; struct position(int xx, int yy) :x(xx), y(yy) {}
}Position;

bool Random(char board[8][8],int turn)
{
	char b[8][8];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			b[i][j] = board[i][j];
	while (1)
	{
		int Result = checkResult(b);
		if (Result == 2)
			return false;
		else if (Result == 1 || Result == 3)
			return true;
		vector<Position>next;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if(b[i][j] == 0 && check(b, i, j, turn) == true)
					next.push_back(Position(i, j));
			}
		}
		if (next.size() > 0)
		{
			int n = rand() % next.size();
			int x, y, xx = next[n].x, yy = next[n].y;
			//cout << xx << " " << yy << endl;
			for (int i = 0; i < 8; i++)
			{
				x = xx;
				y = yy;
				while (1)
				{
					x += Move[i][0];
					y += Move[i][1];
					if (x < 0 || x >= 8 || y < 0 || y >= 8 || b[x][y] == 0)
					{
						x = xx;
						y = yy;
						break;
					}
					if (b[x][y] == turn)
						break;
				}
				//cout << "x:y:" << x << " " << y;
				while (x != xx || y != yy)
				{
					x -= Move[i][0];
					y -= Move[i][1];
					if (turn != b[x][y])
					{
						b[x][y] = turn;
					}
				}
			}
		}
		turn = turn % 2 + 1;
		/*if (clock() - Begin > 10 * CLOCKS_PER_SEC)
		{
			cout << Result << endl;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
					if (b[i][j] != 0)
						cout << (int)b[i][j] << " ";
					else
						cout << "  ";
				cout << endl;
			}
			int white = 0, black = 0;
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 8; j++)
					if (b[i][j] == 1)
						black++;
					else if (b[i][j] == 2)
						white++;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (check(board, i, j, 1) == true)
						cout << "1:" << i << " " << j << endl;
					if (check(board, i, j, 2) == true)
						cout << "2:" << i << " " << j << endl;
				}
			}
			system("pause");
		}*/
	}
}

void Free(Node *root)
{
	for (int i = 0; i < root->number; i++)
	{
		if (root->child[i] != NULL)
			Free(root->child[i]);
	}
	free(root);
}

int main()
{
	srand(time(NULL));
	Node *root;
	root = (Node*)malloc(sizeof(Node));
	root->scores = 0;
	root->total = 0;
	root->parent = NULL;
	//root->child = (Node **)malloc(sizeof(Node*) * 8);
	root->child = NULL;
	root->number = 0;
	root->x = -1;
	root->y = -1;
	root->turn = 1;
	for (int a = 0; a < 8; a++)
	{
		for (int b = 0; b < 8; b++)
		{
			root->board[a][b] = 0;
		}
	}
	root->board[3][3] = 2;
	root->board[3][4] = 1;
	root->board[4][3] = 1;
	root->board[4][4] = 2;
	int r;
	while (1)
	{
		r = checkResult(root->board);
		switch (r)
		{
		case 1:
			cout << "黑子胜!";
			system("pause");
		case 2:
			cout << "白子胜!";
			system("pause");
		case 3:
			cout << "平局";
			system("pause");
		default:
			break;
		}
		Begin = clock();
		int i = 0;
		while(clock() - Begin < 50 * CLOCKS_PER_SEC)
		//for (int i = 0; i < 100; i++)
		{
			//Begin = clock();
			Node *ptr = search(root);
			bool result;
			i++;
			//cout << i << " ";
			/*for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
					if (ptr->board[i][j] != 0)
						cout << (int)ptr->board[i][j] << " ";
					else
						cout << "  ";
				cout << endl;
			}
			cout << endl;
			system("pause");*/
			r = checkResult(ptr->board);
			result = false;
			if (r == 0)
			{
				add(ptr, ptr->turn);
				//cout << " add";
				result = Random(ptr->board, ptr->turn);
				/*cout << result;
				system("pause");
				cout << " result";*/
			}
			else if (r == 1 || r == 3)
				result = true;
			else if (r == 2)
				result = false;
			if (result == true)
			{
				while (ptr != NULL)
				{
					ptr->total++;
					ptr->scores++;
					ptr = ptr->parent;
				}
			}
			else
			{
				while (ptr != NULL)
				{
					ptr->total++;
					ptr = ptr->parent;
				}
			}
			/*cout << root->scores << "/" << root->total << endl;
			system("pause");*/
		}
		double m = 0;
		int k;
		for (int i = 0; i < root->number; i++)
		{
			if (root->child[i]->scores > m * root->child[i]->total)
				k = i;
		}
		Node*ptr;
		ptr = root->child[k];
		cout << "黑子下:" << ptr->x << " " << ptr->y << endl;
		cout << "  0  1  2  3  4  5  6  7\n";
		cout << " *--*--*--*--*--*--*--*--*\n";
		for (int i = 0; i < 8; i++)
		{
			cout << i;
			for (int j = 0; j < 8; j++)
				if (ptr->board[i][j] != 0)
					cout << "| " << (int)ptr->board[i][j];
				else
					cout << "|  ";
			cout << "|\n *--*--*--*--*--*--*--*--*\n";
		}
		//cout << ptr->turn << endl;
		//system("pause");
		cout << "请输入:";
		while (ptr->turn == 2)
		{
			int x, y;
			cin >> x >> y;
			while (check(ptr->board, x, y, 2) == false)
			{
				cout << "不合法输入，请重新输入:" << endl;
				cin >> x >> y;
			}
			for (int i = 0; i < ptr->number; i++)
			{
				if (x == ptr->child[i]->x && y == ptr->child[i]->y)
				{
					ptr = ptr->child[i];
					break;
				}
			}
			cout << "  0  1  2  3  4  5  6  7\n";
			cout << " *--*--*--*--*--*--*--*--*\n";
			for (int i = 0; i < 8; i++)
			{
				cout << i;
				for (int j = 0; j < 8; j++)
					if (ptr->board[i][j] != 0)
						cout << "| " << (int)ptr->board[i][j];
					else
						cout << "|  ";
				cout << "|\n *--*--*--*--*--*--*--*--*\n";
			}
		}
		Node *p;
		p = ptr->parent;
		for(int i = p->number - 1 ;i >= 0;i--)
			if (p->child[i] == ptr)
			{
				p->child[i] = NULL;
				break;
			}
		ptr->parent = NULL;
		Free(root);
		root = ptr;
		cout << i << endl;
	}
	/*add(root, 1);
	for (int i = 0; i < root->number; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				cout << (int)root->child[i]->board[j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}*/
	system("pause");
}