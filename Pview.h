#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include "block.h"

#ifndef PVIEW
#define PVIEW

using namespace std;

class Pview{
public:
	Pview();
	void operator =(Pview&);
	void outputl();
	void outputr();
	void load_block(block);
	void clear_buf();
	void ini();
	void cls(int, int);
	~Pview() { }
	int x,y,att,scores,JG;
	int r;
	bool over;
	vector< vector<int> > solu;
	vector< pair<int,int> > loaded_locs;
	vector< pair<int,int> > has;
};

Pview::Pview()
{
	r = 0;
	JG = 48;
	x = 4; y = 4;
	vector< vector<int> > solu1(4,vector<int>(4,0));
	solu = solu1;
}

void Pview::operator=(Pview &tmp)
{
	this->over = tmp.over;
	this->has = tmp.has;
	this->loaded_locs = tmp.loaded_locs;
	this->solu = tmp.solu;
}

void Pview::outputl()
{
	for(int i = 0; i != y; ++i)
	{
		for(int j = 2; j != x; ++j)
		{
			if(solu[i][j] == 1)
			cout << "■";
			else cout << "□";
		}
		cout << endl;
	}
}

void Pview::outputr()
{
	for(int i = 0; i != y; ++i)
	{
		cls(JG,r++);
		for(int j = 2; j != x; ++j)
		{
			if(solu[i][j] == 1)
				cout << "■";
			else cout << "□";
		}
	}
}

void Pview::load_block(block tmp)
{
	for(int i = 0; i != tmp.locs.size(); ++i)
	{
		int x = tmp.locs[i]/4 + this->x/2;
		int y = tmp.locs[i]%4;
		solu[y][x] = 1;
		loaded_locs.push_back(make_pair(x,y));
	}
}

void Pview::clear_buf()
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

void Pview::ini()
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

void Pview::cls(int x, int y)
{
	HANDLE hOut;
	COORD OutChar;
	OutChar.X = x;
	OutChar.Y = y;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,OutChar);
}
#endif
