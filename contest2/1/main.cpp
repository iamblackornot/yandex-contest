#include <fstream>
#include <memory>

struct Point
{
    int x;
    int y;
};

class MoveDirectionState;
typedef std::unique_ptr<MoveDirectionState> MoveStatePtr;

class MoveDirectionState
{
public:
    virtual MoveStatePtr move() const = 0;
    Point GetLocation() const { return m_location; }
protected:
    MoveDirectionState(Point location, int stepSize, int movesLeft)
        : m_location(location), m_stepSize(stepSize), m_movesLeft(movesLeft){}

    Point m_location;
    int m_stepSize;
    int m_movesLeft;
};


class LeftDirection;
class DownDirection;
class RightDirection;
class UpDirection;

class LeftDirection : public MoveDirectionState
{
public:
    LeftDirection(Point location, int stepSize, int movesLeft)
            : MoveDirectionState(location, stepSize, movesLeft) {}

    MoveStatePtr move() const override final;
};

class DownDirection : public MoveDirectionState
{
public:
    DownDirection(Point location, int stepSize, int movesLeft)
            : MoveDirectionState(location, stepSize, movesLeft) {}

    MoveStatePtr move() const override final;
};

class RightDirection : public MoveDirectionState
{
public:
    RightDirection(Point location, int stepSize, int movesLeft)
            : MoveDirectionState(location, stepSize, movesLeft) {}

    MoveStatePtr move() const override final;
};

class UpDirection : public MoveDirectionState
{
public:
    UpDirection(Point location, int stepSize, int movesLeft)
            : MoveDirectionState(location, stepSize, movesLeft) {}

    MoveStatePtr move() const override final;
};

MoveStatePtr LeftDirection::move() const
{
    if(m_movesLeft > 0)
    {
        Point newLocation = Point{ m_location.x - 1, m_location.y };

        return std::make_unique<LeftDirection>(newLocation, m_stepSize, m_movesLeft - 1);
    }

    return std::make_unique<DownDirection>(m_location, m_stepSize, m_stepSize)->move();
}
MoveStatePtr DownDirection::move() const
{
    if(m_movesLeft > 0)
    {
        Point newLocation = Point{ m_location.x, m_location.y - 1 };

        return std::make_unique<DownDirection>(newLocation, m_stepSize, m_movesLeft - 1);
    }

    return std::make_unique<RightDirection>(m_location, m_stepSize + 1, m_stepSize + 1)->move();
}
MoveStatePtr RightDirection::move() const
{
    if(m_movesLeft > 0)
    {
        Point newLocation = Point{ m_location.x + 1, m_location.y };

        return std::make_unique<RightDirection>(newLocation, m_stepSize, m_movesLeft - 1);
    }

    return std::make_unique<UpDirection>(m_location, m_stepSize, m_stepSize)->move();
}
MoveStatePtr UpDirection::move() const
{
    if(m_movesLeft > 0)
    {
        Point newLocation = Point{ m_location.x, m_location.y + 1 };

        return std::make_unique<UpDirection>(newLocation, m_stepSize, m_movesLeft - 1);
    }

    return std::make_unique<LeftDirection>(m_location, m_stepSize + 1, m_stepSize + 1)->move();
}




int main()
{
    std::ifstream inputFile("input.txt");
    int n;

    MoveStatePtr robotState = std::make_unique<LeftDirection>(Point{ 0, 0 }, 1, 1);

    inputFile >> n;

    while(n > 0)
    {
        robotState = robotState->move();
        --n;
    }

    std::ofstream outputFile("output.txt");

    Point loc = robotState->GetLocation();
    outputFile << loc.x << " " << loc.y;

    return 0;
}

