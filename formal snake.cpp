#include <bits/stdc++.h>
#include <windows.h>
#define L 0
#define R 1
#define U 2
#define D 3
#define size 20
using namespace std;

int C[5]={9,7,2,4,15};

void color(const unsigned short textColor){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor);//设定蛇身颜色
}

bool operator==(COORD &A, COORD&B){
	if (A.X == B.X&&A.Y == B.Y)return true;
	return false;
}

bool operator!=(COORD &A, COORD&B) {
	return !(A == B);
}

class snake {
private:
	int len=2;
	COORD food;
	deque<COORD>body;
	bool board[size][size];
	bool path_board[size][size];
	int path[size][size];
public:
	snake() {
		init_board(board);
		body.push_back({ 0,0 });
		body.push_back({ 2,0 });
		print(body[0], '*');
		print(body[1], '*');
		board[0][0] = true;
		board[1][0] = true;
		init_board(path_board);
		init_path();
		set_path_board({ 0,0 }, true);
		create_food();
	}
	
	void action() {
		COORD t = body.front();
		switch (path[t.X/2][t.Y]) {
		case L:t.X-=2; move(t); break;
		case R:t.X+=2; move(t); break;
		case D:t.Y++; move(t); break;
		case U:t.Y--; move(t); break;
		default:break;
		}
		Sleep(1);
	}
	
	void print(COORD p, char c){
	HANDLE Houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	color(C[len%5]);
	SetConsoleCursorPosition(Houtput, p);
	printf("%c ", c);
	}
	
	void init_board(bool A[size][size]) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			A[i][j] = false;
	}

	void create_food() {
		len++;
		do{
			food.X = rand() % size*2;
			food.Y = rand() % size;
		} while (board[food.X/2][food.Y]);
        print(food,'*');
		del_path();
		set_path();
		mark();
		
	}

	void mark(){
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		color(7);
		SetConsoleCursorPosition(hout,{2*size+10,0});
		printf("蛇身长度: %d",len);
	}
	
	void del_path() {
		for (int i = 0; i <=size-2; i += 2) {
			for (int j = 1; j < size-2; j += 2) {
				if (matrix_no_point({ i*2,j - 1 }) || matrix_no_point({ i*2,j + 1 })) {
					del_path({ i*2, j });
				}
				if (matrix_no_point({ (j-1)*2,i}) || matrix_no_point({ (j+1)*2,i })) {
					del_path({ j*2, i });
				}
			}
		}
	}

	void set_path() {
		COORD start = body.front();
		start.X = ((start.X / 2)& 0xfffe)*2;
		start.Y = start.Y & 0xfffe;
		COORD end = food;
		end.X = ((end.X / 2)& 0xfffe)*2;
		end.Y = end.Y & 0xfffe;
		int dx,dy;
		if (start.X > end.X)dx = -4;
		else dx = 4;
		if (start.Y > end.Y)dy = -2;
		else dy = 2;
		while (start.X != end.X) {
			int x = start.X;
			start.X += dx;
			add_path({ (x + start.X) / 2 , start.Y });
		}
		while (start.Y != end.Y) {
			int y = start.Y;
			start.Y += dy;
			add_path({end.X,(start.Y+y) / 2});
		}
	}

	void move(COORD x) {
		if (x != food) {
			print(body.back(), ' ');
			set_board(body.back(),false);
			body.pop_back();
			body.push_front(x);
			set_board(x,true);
			print(x, '*');
		}
		else {
			body.push_front(x);
			set_board(x,true);
			create_food();
		}del_path();
		set_path();
	}


	void init_path() {
		for (int i = 0; i < size; i += 2) {
			for (int j = 0; j < size; j += 2) {
				path[i][j] = R;
				path[i + 1][j + 1] = L;
				path[i][j + 1] = U;
				path[i + 1][j] = D;
			}
		}
	}

	void set_board(COORD &X,bool t) {
	board[X.X/2][X.Y] = t;
	}
	
	void set_path_board(COORD x, bool t) {
		path_board[x.X/2][x.Y] = t;
		path_board[x.X/2+1][x.Y] = t;
		path_board[x.X/2][x.Y+1] = t;
		path_board[x.X/2+1][x.Y+1] = t;
	}

	bool matrix_no_point(COORD x) {
		return board[x.X/2][x.Y] == false &&
			board[x.X/2 + 1][x.Y] == false &&
			board[x.X/2][x.Y + 1] == false &&
			board[x.X/2 + 1][x.Y + 1] == false;
	}

	bool path_board_not_all_true(COORD x) {
		return !(path_board[x.X/2][x.Y] &&path_board[x.X/2+1][x.Y+1]);
		
	}

	void add_path(COORD x) {
		if ((x.X/2) % 2) {
			if (path_board_not_all_true(x)) {
				path[x.X/2][x.Y] = R;
				path[x.X/2 + 1][x.Y + 1] = L;
				set_path_board({ x.X - 2,x.Y }, true);
				set_path_board({ x.X + 2,x.Y }, true);
			}
		}
		else {
			if (path_board_not_all_true(x)) {
				path[x.X/2 + 1][x.Y] = D;
				path[x.X/2][x.Y + 1] = U;
				set_path_board({ x.X,x.Y + 1 }, true);
				set_path_board({ x.X,x.Y - 1 }, true);
			}
		}
	}

	void if_del(COORD x) {
		if (board[x.X/2][x.Y] == false && board[x.X/2 + 1][x.Y] == false &&
		board[x.X/2 + 1][x.Y + 1] == false && board[x.X/2][x.Y + 1] == false)
		{
			set_path_board(x, false);
		}
	}

	void del_path(COORD x) {
		if ((x.X/2) % 2) {
			path[x.X/2][x.Y] = D;
			path[x.X/2 + 1][x.Y + 1] = U;
			if_del({ x.X - 2,x.Y });
			if_del({ x.X + 2,x.Y });
		}
		else {
			path[x.X/2 + 1][x.Y] = L;
			path[x.X/2][x.Y + 1] = R;
			if_del({ x.X,x.Y-1 });
			if_del({ x.X,x.Y+1 });
		}
	}
};

int main(){
	cout<<"按任意键开始";
	getchar();
	system("cls");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
	srand(time(NULL));
	snake a;
	while (true)
		a.action();
}
