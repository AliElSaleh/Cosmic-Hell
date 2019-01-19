#include "Globals.h"
#include "QuadTree.h"

#include <raylib.h>

int TotalPoints;

void QuadTree::Init()
{
	Points.clear();
	PointsInRange.clear();
	TotalPoints = 0;
	bDivided = false;

	for (int i = 0; i < 300; i++)
	{
		const Point P = Point(GetRandomValue(10, GetScreenWidth()-PANEL_WIDTH - 10), GetRandomValue(10, GetScreenHeight() - 10));
		Insert(P);
	}

	Range.Width = 50;
	Range.Height = 50;
	Range.X = GetRandomValue(Range.Width, GetScreenWidth()-PANEL_WIDTH - Range.Width);
	Range.Y = GetRandomValue(Range.Height, GetScreenHeight() - Range.Height);

	PointsInRange = Query(Range);

	printf("Total points: %i", TotalPoints);
	printf("\n");
	printf("Points found: %u\n", unsigned(PointsInRange.size()));
}

void QuadTree::Update()
{
	if (GetMousePosition().x > 0 && GetMousePosition().x < GetScreenWidth()-PANEL_WIDTH &&
		GetMousePosition().y > 0 && GetMousePosition().y < GetScreenHeight())
	{
		if (Range.X - Range.Width <= GetScreenWidth()-PANEL_WIDTH && 
			Range.X + Range.Width >= 0 && 
			Range.Y - Range.Height <= GetScreenHeight() &&
			Range.Y + Range.Height >= 0)
		{	
			PointsInRange = Query(Range);
			printf("Points found: %u\n", unsigned(PointsInRange.size()));
		}		
	}
	else
		printf("Cursor outside of window.\n");

	if (GetMousePosition().x + Range.Width > GetScreenWidth()-PANEL_WIDTH)
		Range.X = GetScreenWidth()-PANEL_WIDTH - Range.Width;
	else if (GetMousePosition().x - Range.Width < 0)
		Range.X = Range.Width;
	else
		Range.X = GetMousePosition().x;


	if (GetMousePosition().y + Range.Height > GetScreenHeight())
		Range.Y = GetScreenHeight() - Range.Height;
	else if (GetMousePosition().y - Range.Height < 0)
		Range.Y = Range.Height;
	else
		Range.Y = GetMousePosition().y;

	// Refresh
	if (IsKeyPressed(KEY_R))
		Init();
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

	DrawRectangleLines(Range.X - Range.Width, Range.Y - Range.Height, Range.Width*2, Range.Height*2, GREEN);

	for (Point P : PointsInRange)
		DrawCircle(P.X, P.Y, 3.0f, GREEN);

	DrawText("Refresh [R]", GetScreenWidth()-PANEL_WIDTH+10, 50, 20, WHITE);

	DrawText(FormatText("Total Points: %02i", TotalPoints), 710, 100, 20, WHITE);
}

std::vector<Point> QuadTree::Query(const Section Range)
{
	std::vector<Point> Found;

	if (!Boundary.Intersects(Range))
		return std::vector<Point>();

	for (Point P : Points)
	{
		if(Range.Contains(P))
			Found.push_back(P);
	}

	if (bDivided)
	{
		auto FoundChildren = Northeast->Query(Range);
		Found.insert(Found.end(), FoundChildren.begin(), FoundChildren.end());

		FoundChildren = Northwest->Query(Range);
		Found.insert(Found.end(), FoundChildren.begin(), FoundChildren.end());
		
		FoundChildren = Southeast->Query(Range);
		Found.insert(Found.end(), FoundChildren.begin(), FoundChildren.end());

		FoundChildren = Southwest->Query(Range);
		Found.insert(Found.end(), FoundChildren.begin(), FoundChildren.end());
	}

	return Found;
}

bool QuadTree::Insert(Point Point)
{
	if (!Boundary.Contains(Point))
		return false;

	if (Points.size() < Capacity)
	{
		TotalPoints++;
		Points.emplace_back(Point);
		return true;
	}

	if (!bDivided)
		SubDivide();

	return Northeast->Insert(Point) || Northwest->Insert(Point) || Southeast->Insert(Point) || Southwest->Insert(Point);
}

bool Section::Intersects(const Section Range) const
{
	return !(Range.X - Range.Width > this->X + this->Width ||
		Range.X	+ Range.Width < this->X - this->Width ||
		Range.Y - Range.Height > this->Y + this->Height ||
		Range.Y	+ Range.Height < this->Y - this->Height);
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
