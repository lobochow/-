#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include "block.h"
#include "Pview.h"

#ifndef VIEW
#define VIEW

using namespace std;

class view{
public:
	view(int, int);
	void operator =(view&);
	void outputvl();
	void outputvr(int&);
	void load_block(block);
	void cls(int, int);
	void clear_buf();
	void attack();
	bool down();
	void left();
	void right();
	void change();
	void check();
	void load_has();
	void ini();
	void next1();
	~view() { }
	int x,y,att,scores,JG,S;
	bool over;
	Pview next;
	vector< vector<int> > solu;
	vector< pair<int,int> > loaded_locs;
	vector< pair<int,int> > has;
};

view::view(int a, int b)
{
	JG = 48;
	att = 0;
	S = 0;
	scores = 3;
	over = 0;
	x = a; y = b;
	vector< vector<int> > solu1(b,vector<int>(a,0));
	solu = solu1;
}

void view::operator=(view &tmp)
{
	this->att = tmp.att;
	this->scores = tmp.scores;
	this->over = tmp.over;
	this->has = tmp.has;
	this->loaded_locs = tmp.loaded_locs;
	this->solu = tmp.solu;
	this->next = tmp.next;
}

void view::outputvl()
{
	cls(0,0);
	cout << "下一个方块" << endl;
	next.outputl();
	cout << endl;
	cout <<"得分:" << scores << "     按f有特殊效果(1分一次)";
	cout << endl << endl;
	for(int i = 2; i != y; ++i)
	{
		for(int j = 0; j != x; ++j)
		{
			if(solu[i][j] == 1)
			cout << "■";
			else cout << "□";
		}
		cout << endl;
	}
}

void view::outputvr(int &r)
{
	cls(JG,r++);
	cout << "下一个方块";
	next.outputr();
	cls(JG,r++);
	cls(JG,r++);
	cls(JG,r++);
	for(int i = 2; i != y; ++i)
	{
		cls(JG,r++);
		for(int j = 0; j != x; ++j)
		{
			if(solu[i][j] == 1)
			cout << "■";
			else cout << "□";
		}
	}
	r = 0;
}

void view::load_block(block tmp)
{
	for(int i = 0; i != tmp.locs.size(); ++i)
	{
		int x = tmp.locs[i]/4 + this->x/2;
		int y = tmp.locs[i]%4;
		solu[y][x] = 1;
		loaded_locs.push_back(make_pair(x,y));
	}
}

void view::load_has()
{
	auto begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		has.push_back(*begin++);
	}
}

void view::clear_buf()
{
	for(int i = 0; i != 2; ++i)
	{
		for(int j = 0 ; j != x; ++j)
		{
			solu[i][j] = 0;
		}
	}
	loaded_locs.clear();
}

void view::attack()
{
	auto begin = has.begin();
	while(begin != has.end())
	{
		if(begin->second - 1 < 2)
		{
			over = 0;
		}
		++begin;
	}

	begin = has.begin();
	while(begin != has.end())
	{
		solu[begin->second--][begin->first] = 0;
		++begin;
	}

	begin = has.begin();
	while(begin != has.end())
	{
		solu[begin->second][begin->first] = 1;
		++begin;
	}
	for(int i = att%2; i < x; i +=2)
	{
		has.push_back(make_pair(i,y-1));
		solu[y-1][i] = 1;
	}
	++att;
}

bool view::down()
{
	for(int i = 0; i != loaded_locs.size(); ++i)
	{
		int x = loaded_locs[i].first;
		int y = loaded_locs[i].second;
		if(y+1 >= this->y) return false;
		pair<int,int> next1 = make_pair(x,y+1);
		auto it = find(loaded_locs.begin(),loaded_locs.end(),next1);
		if(it == loaded_locs.end() && solu[y+1][x] == 1) return false;
	}
	auto begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		int x = begin->first;
		int y = begin->second++;
		solu[y][x] = 0;
		++begin;
	}
	begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		solu[begin->second][begin->first] = 1;
		++begin;
	}
	return true;
}

void view::left()
{
	for(int i = 0; i != loaded_locs.size(); ++i)
	{
		int x = loaded_locs[i].first;
		int y = loaded_locs[i].second;
		if(x-1 == -1) return;
		pair<int,int> next1 = make_pair(x-1,y);
		auto it = find(loaded_locs.begin(),loaded_locs.end(),next1);
		if(solu[y][x-1] == 1 && it == loaded_locs.end()) return;
	}
	auto begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		int x = begin->first--;
		int y = begin->second;
		solu[y][x] = 0;
		++begin;
	}
	begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		int x = begin->first;
		int y = begin->second;
		solu[y][x] = 1;
		++begin;
	}
}

void view::right()
{
	for(int i = 0; i != loaded_locs.size(); ++i)
	{
		int x = loaded_locs[i].first;
		int y = loaded_locs[i].second;
		if(x+1 == this->x) return;
		pair<int,int> next1 = make_pair(x+1,y);
		auto it = find(loaded_locs.begin(),loaded_locs.end(),next1);
		if(solu[y][x+1] == 1 && it == loaded_locs.end()) return;
	}
	auto begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		int x = begin->first++;
		int y = begin->second;
		solu[y][x] = 0;
		++begin;
	}
	begin = loaded_locs.begin();
	while(begin != loaded_locs.end())
	{
		int x = begin->first;
		int y = begin->second;
		solu[y][x] = 1;
		++begin;
	}
}

void view::change()
{
	auto begin = loaded_locs.begin();
	int x0 = begin->first;
	int y0 = begin->second;
	++begin;
	while(begin != loaded_locs.end())
	{
		int x = begin->first - x0;
		int y = begin->second - y0;
		int tmp = x;
		x = x0 - y;
		y = y0 + tmp;
		if(x <= -1 || x >= this->x || y <= -1 || y >= this->y) return;
		pair<int,int> next1 = make_pair(x,y);
		auto it = find(loaded_locs.begin(),loaded_locs.end(),next1);
		if(solu[y][x] == 1 && it == loaded_locs.end()) return;
		++begin;
	}
	begin = loaded_locs.begin()+1;
	while(begin != loaded_locs.end())
	{
		int x = begin->first - x0;
		int y = begin->second - y0;
		int tmp = x;
		x = x0 - y;
		y = y0 + tmp;
		solu[begin->second][begin->first] = 0;
		begin->first = x;
		begin->second = y;
		++begin;
	}
	begin = loaded_locs.begin()+1;
	while(begin != loaded_locs.end())
	{
		solu[begin->second][begin->first] = 1;
		++begin;
	}
}

void view::check()
{
	int rows = 0;
	bool signal = 0;
	for(int level = y-1; level != 2;)
	{
		for(int i = 0; i != x; ++i)
		{
			if(solu[level][i] == 0)
			{
				signal = 1;
				break;
			}
		}
		if(signal)
		{
			--level;
			signal = 0;
			continue;
		}
		else ++rows;
		auto begin = has.begin();
		while(begin != has.end())
		{
			if(begin->second == level)
			{
				solu[begin->second][begin->first] = 0;
				has.erase(begin);
				begin = has.begin();
			}
			else
			++begin;
		}

		begin = has.begin();
		while(begin != has.end())
		{
			if(begin->second+1 < y && begin->second <= level)
			solu[begin->second++][begin->first] = 0;
			++begin;
		}

		begin = has.begin();
		while(begin != has.end())
		{
			if(begin->second <= level)
			solu[begin->second][begin->first] = 1;
			++begin;
		}
	}
	if(rows > 1) scores += rows*rows;
	else scores += rows;
}

void view::ini()
{
	for(int i = 0; i != x; ++i)
	{
		for(int j = 0; j != y; ++j)
		{
			solu[j][i] = 0;
		}
	}
	has.clear();
}

void view::cls(int x, int y)
{
	HANDLE hOut;
	COORD OutChar;
	OutChar.X = x;
	OutChar.Y = y;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,OutChar);
}

#endif VIEW
