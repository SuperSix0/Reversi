#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#define C 1.0				//logn项系数
#define N 1.0
#define E 0.2
#define CORNER 0.5			//角落系数
#define BAD 0.6			//坏子系数
#define MOVABLE 0.01		//行动力系数
#define SEARCH_TIME 15		//搜索时间
using namespace std;

char myself, enemy;
int session_id = 2;
clock_t Begin;
int Move[8][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };
typedef struct node
{
	char board[8][8];
	int scores;
	int total;
	struct node *parent;
	struct node **child;
	int number;
	int depth;
	int x, y;
	char turn;
}Node;

bool check(char board[8][8], int x, int y, char turn)
{
	bool enable = false;
	for (int k = 0; k < 8; k++)
	{
		int ii = x + Move[k][0], jj = y + Move[k][1];
		if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
			|| board[ii][jj] == '0'
			|| board[ii][jj] == turn)
			continue;
		while (1)
		{
			ii = ii + Move[k][0];
			jj = jj + Move[k][1];
			if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
				|| board[ii][jj] == '0')
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

//chess:1为黑,2为白
float evaluate(char board[8][8], char chess)
{
	float result = 0;
	if (board[0][0] == chess) result += CORNER; else if (board[0][0] == '0' && board[1][1] == chess) result -= BAD * CORNER;
	if (board[0][7] == chess) result += CORNER;	else if (board[0][7] == '0' && board[1][6] == chess) result -= BAD * CORNER;
	if (board[7][0] == chess) result += CORNER;	else if (board[7][0] == '0' && board[6][1] == chess) result -= BAD * CORNER;
	if (board[7][7] == chess) result += CORNER; else if (board[7][7] == '0' && board[6][6] == chess) result -= BAD * CORNER;
	return result;
}

int checkResult(char board[8][8])//0 for not finish,1 for black win,2 for white win,3 for tie
{
	int white = 0, black = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] == 'B')
				black++;
			else if (board[i][j] == 'W')
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
				if (board[i][j] == '0')
				{
					if (check(board, i, j, 'W') == true)
						return 0;
					if (check(board, i, j, 'B') == true)
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

void add(Node *root)
{
	root->child = (Node **)malloc(sizeof(Node*) * 32);
	root->number = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (root->board[i][j] == '0')
			{
				if (check(root->board, i, j, root->turn) == true)
				{
					Node* ptr;
					ptr = (Node*)malloc(sizeof(Node));
					ptr->scores = 0;
					ptr->total = 0;
					ptr->depth = root->depth + 1;
					ptr->parent = root;
					ptr->number = 0;
					ptr->child = NULL;
					ptr->x = i;
					ptr->y = j;
					ptr->turn = root->turn == 'W' ? 'B' : 'W';
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
							|| root->board[ii][jj] == '0'
							|| root->board[ii][jj] == root->turn)
							continue;
						while (1)
						{
							ii = ii + Move[k][0];
							jj = jj + Move[k][1];
							if (ii < 0 || ii >= 8 || jj < 0 || jj >= 8
								|| root->board[ii][jj] == '0')
								break;
							else if (root->board[ii][jj] == root->turn)
							{
								while (ii != i || jj != j)
								{
									ii -= Move[k][0];
									jj -= Move[k][1];
									ptr->board[ii][jj] = root->turn;
								}
								break;
							}
						}
					}
					root->child[root->number++] = ptr;
				}
			}
		}
	}
	if (root->number == 0 && checkResult(root->board) == 0)
	{
		free(root->child);
		root->turn = root->turn == 'W' ? 'B' : 'W';
		add(root);
	}
	else
	{
		Node **tmp;
		tmp = (Node **)malloc(sizeof(Node*) * root->number);
		for (int i = 0; i < root->number; i++)
		{
			tmp[i] = root->child[i];
			tmp[i]->depth = root->depth + 1;
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
			double max = -10000;
			double t;
			Node *tt = NULL;
			for (int i = 0; i < tmp->number; i++)
			{
				Node* ttmp = tmp->child[i];
				ttmp->depth = tmp->depth + 1;
				if (ttmp->depth < 0 || ttmp->depth>64) cout << "error" << endl;
				if (tmp->turn == myself)
				{
					if (tmp->depth <= 44) t = N * ttmp->scores / ttmp->total + C * sqrt(2 * log(tmp->total) / ttmp->total) + E * (evaluate(ttmp->board, ttmp->turn=='W'?'B':'W') - MOVABLE * ttmp->number);
					else t = N * ttmp->scores / ttmp->total + C * sqrt(2 * log(tmp->total) / ttmp->total);
				}
				else
				{
					if (tmp->depth <= 44) t = N * (1 - 1.0*ttmp->scores / ttmp->total) + C * sqrt(2 * log(tmp->total) / ttmp->total) + E * (evaluate(ttmp->board, ttmp->turn == 'W' ? 'B' : 'W') - MOVABLE * ttmp->number);
					else t = N * (1 - 1.0*ttmp->scores / ttmp->total) + C * sqrt(2 * log(tmp->total) / ttmp->total);
				}
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

bool Random(char board[8][8], char turn)
{
	char b[8][8];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			b[i][j] = board[i][j];
	while (1)
	{
		int Result = checkResult(b);
		if (Result == 2)
			return myself== 'W'?true:false;
		else if (Result == 1 || Result == 3)
			return myself=='B'?true:false;
		vector<Position>next;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (b[i][j] == '0' && check(b, i, j, turn) == true)
					next.push_back(Position(i, j));
			}
		}
		if (next.size() > 0)
		{
			int n = rand() % next.size();
			int x, y, xx = next[n].x, yy = next[n].y;
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
		turn = turn == 'W' ? 'B' : 'W';
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
	cout << "电脑执黑棋(B)/白棋(W):\n";
	while (myself != 'W' && myself != 'B')
		cin >> myself;
	enemy = myself == 'W' ? 'B' : 'W';
	Node *root;
	root = (Node*)malloc(sizeof(Node));
	root->scores = 0;
	root->total = 0;
	root->parent = NULL;
	root->depth = 1;
	root->child = NULL;
	root->number = 0;
	root->x = -1;
	root->y = -1;
	root->turn = 'B';
	for (int a = 0; a < 8; a++)
	{
		for (int b = 0; b < 8; b++)
		{
			root->board[a][b] = '0';
		}
	}
	root->board[3][3] = 'W';
	root->board[3][4] = 'B';
	root->board[4][3] = 'B';
	root->board[4][4] = 'W';
	cout << "初始棋盘:\n";
	cout << "  0  1  2  3  4  5  6  7\n";
	cout << " *--*--*--*--*--*--*--*--*\n";
	for (int i = 0; i < 8; i++)
	{
		cout << i;
		for (int j = 0; j < 8; j++)
			if (root->board[i][j] != '0')
				cout << "| " << root->board[i][j];
			else
				cout << "|  ";
		cout << "|\n *--*--*--*--*--*--*--*--*\n";
	}
	int r;
	Node*ptr=NULL;
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
		ptr = root;
		
		Begin = clock();
		int i = 0;
		while (clock() - Begin < SEARCH_TIME * CLOCKS_PER_SEC)
		{
			ptr = search(root);
			bool result;
			i++;
			r = checkResult(ptr->board);
			result = false;
			if (r == 0)
			{
				add(ptr);
				result = Random(ptr->board, ptr->turn);
			}
			else if (r == 1 || r == 3)
				result = myself == 'B' ? true : false;
			else if (r == 2)
				result = myself == 'W' ? true : false;
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
		}
		cout << i << endl;
		ptr = root;
		if (root->turn == myself)
		{
			double m = 0;
			int k = 0;
			for (int i = 0; i < root->number; i++)
			{
				int movable_number;
				int max_do = -10000;
				for (int ii = 0; ii < root->child[i]->number; ii++)
				{
					if (root->child[i]->child[ii]->total > max_do)
					{
						max_do = root->child[i]->child[ii]->total;
						movable_number = root->child[i]->child[ii]->number;
					}
				}
				if (1.0*root->child[i]->scores / root->child[i]->total + E * (evaluate(root->child[i]->board, root->turn) + MOVABLE * movable_number) > m)
				{
					m = 1.0*root->child[i]->scores / root->child[i]->total + E * (evaluate(root->child[i]->board, root->turn) + MOVABLE * movable_number);
					k = i;
				}
				cout << "胜率" << 1.0*root->child[i]->scores / root->child[i]->total << endl;
			}
			ptr = root->child[k];
			//cout << ptr->turn << endl;
			cout << "电脑下:" << ptr->x << " " << ptr->y << endl;
			cout << ptr->turn << endl;
			//cout << "evaluate: " << evaluate(ptr->board, 3 - ptr->turn) + MOVABLE * ptr->number << endl;
			cout << "这一步" << 1.0 * ptr->scores / ptr->total << endl;
			cout << "  0  1  2  3  4  5  6  7\n";
			cout << " *--*--*--*--*--*--*--*--*\n";
			for (int i = 0; i < 8; i++)
			{
				cout << i;
				for (int j = 0; j < 8; j++)
					if (ptr->board[i][j] != '0')
						cout << "| " << ptr->board[i][j];
					else
						cout << "|  ";
				cout << "|\n *--*--*--*--*--*--*--*--*\n";
			}
		}
		while (ptr->turn == enemy)
		{
			cout << "请输入:";
			int x, y;
			cin >> x >> y;
			while (check(ptr->board, x, y, enemy) == false)
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
					if (ptr->board[i][j] != '0')
						cout << "| " << ptr->board[i][j];
					else
						cout << "|  ";
				cout << "|\n *--*--*--*--*--*--*--*--*\n";
			}
		}
		Node *p;
		p = ptr->parent;
		for (int i = p->number - 1; i >= 0; i--)
			if (p->child[i] == ptr)
			{
				p->child[i] = NULL;
				break;
			}
		ptr->parent = NULL;
		Free(root);
		root = ptr;
	}
	system("pause");
}