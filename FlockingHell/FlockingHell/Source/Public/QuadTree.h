#pragma once
#include <vector>

struct Point
{
	Point(const int X, const int Y)
	{
		this->X = X;
		this->Y = Y;
	}

	int X, Y;
};

struct Section
{
	Section(const int X, const int Y, const int Width, const int Height)
	{
		this->X = X;	
		this->Y = Y;	
		this->Width = Width;	
		this->Height = Height;	
	}

	bool Contains(Point Point) const;
	bool Intersects(Section Range) const;

	int X, Y, Width, Height;
};

struct QuadTree
{
	explicit QuadTree(const Section Boundary, const unsigned short Capacity)
	{
		this->Boundary = Boundary;
		this->Capacity = Capacity;
	}

	void Init();
	void Update();
	void Draw();

	std::vector<Point> Query(Section Range);

	bool Insert(Point Point);

	std::vector<Point> Points;
	std::vector<Point> PointsInRange;

	Section Boundary{0, 0, 0, 0};
	Section Range{0, 0, 0, 0};

	QuadTree* Northeast{};
	QuadTree* Northwest{};
	QuadTree* Southeast{};
	QuadTree* Southwest{};

	unsigned short Capacity{};

	bool bDivided{};

private:
	void SubDivide();
};
