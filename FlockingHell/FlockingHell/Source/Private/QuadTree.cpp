#include "Globals.h"
#include "QuadTree.h"

#include <raylib.h>

void QuadTree::Update()
{
	if (IsMouseButtonDown(0) && 
		GetMousePosition().x < GetScreenWidth()-PANEL_WIDTH && 
		GetMousePosition().x > 0 && 
		GetMousePosition().y < GetScreenHeight() &&
		GetMousePosition().y > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			const Point P = Point(GetMousePosition().x + GetRandomValue(-20, 20), GetMousePosition().y + GetRandomValue(-20, 20));
			Insert(P);
			NumberOfPoints++;
		}

		printf("Points: %i\n", NumberOfPoints);
	}
}

void QuadTree::Draw()
{
	DrawRectangleLines(Boundary.X - Boundary.Width, Boundary.Y - Boundary.Height, Boundary.Width*2, Boundary.Height*2, WHITE);

	if (bDivided)
	{
		Northeast->Draw();
		Northwest->Draw();
		Southeast->Draw();
		Southwest->Draw();
	}

	for (unsigned short i = 0; i < Points.size(); ++i)
		DrawCircle(Points[i].X, Points[i].Y, 3.0f, YELLOW);

}

bool QuadTree::Insert(Point Point)
{
	if (!Boundary.Contains(Point))
		return false;

	if (Points.size() < Capacity)
	{
		Points.emplace_back(Point);
		return true;
	}

	if (!bDivided)
		SubDivide();
	
	return Northeast->Insert(Point) || Northwest->Insert(Point) || Southeast->Insert(Point) || Southwest->Insert(Point);
}

void QuadTree::SubDivide()
{
	const int X = this->Boundary.X;
	const int Y = this->Boundary.Y;
	const int Width = this->Boundary.Width;
	const int Height = this->Boundary.Height;

	const Section BoundaryNE = Section(X + Width/2, Y - Height/2, Width/2, Height/2);
	const Section BoundaryNW = Section(X - Width/2, Y - Height/2, Width/2, Height/2);
	const Section BoundarySE = Section(X + Width/2, Y + Height/2, Width/2, Height/2);
	const Section BoundarySW = Section(X - Width/2, Y + Height/2, Width/2, Height/2);

	Northeast = new QuadTree(BoundaryNE, Capacity);
	Northwest = new QuadTree(BoundaryNW, Capacity);
	Southeast = new QuadTree(BoundarySE, Capacity);
	Southwest = new QuadTree(BoundarySW, Capacity);

	bDivided = true;
}

bool Section::Contains(const Point Point) const
{
	// Is this point within the boundary
	return (Point.X >= this->X - this->Width &&
			Point.X <= this->X + this->Width &&
			Point.Y >= this->Y - this->Height &&
			Point.Y <= this->Y + this->Height);
}
