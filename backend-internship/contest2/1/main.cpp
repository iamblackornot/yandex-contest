#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class GameState
{
    typedef std::unordered_map<int, std::unordered_set<int>> PlayerHistory;
    typedef std::vector<PlayerHistory> GameHistory;
    static const int STREAK_TO_WIN = 5;

    public:
        GameState();
        void AddMove  (int x, int y);
        bool HasWinner() const;
        int  GetWinner() const;
    private:
        bool CheckWinCondition(int x, int y) const;
        int  CountUpStreak          (int x, int y) const;
        int  CountDownStreak        (int x, int y) const;
        int  CountLeftStreak        (int x, int y) const;
        int  CountRightStreak       (int x, int y) const;
        int  CountUpperLeftStreak   (int x, int y) const;
        int  CountLowerRightStreak  (int x, int y) const;
        int  CountUpperRightStreak  (int x, int y) const;
        int  CountLowerLeftStreak   (int x, int y) const;
        int  CountDirectionStreak   (int x, int y, int x_direction, int y_direction) const;

    private:
        GameHistory history;
        int         currPlayer;
        int         winner;
};

int main()
{
    int movesCount;
    GameState game;
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    input >> movesCount;

    for(size_t i = 0; i < movesCount; ++i)
    {
        int x;
        int y;

        input >> x;
        input >> y;

        game.AddMove(x, y);

        if(game.HasWinner())
        {
            if(i < movesCount - 1)
            {
                output << "Inattention";
                return 0;
            }

            break;
        }
    }

    if(!game.HasWinner())
    {
        output << "Draw";
        return 0;
    }

    if(game.GetWinner() == 0)
    {
        output << "First";
        return 0;
    }

    output << "Second";
    return 0;

}

inline GameState::GameState()
    : history(2, PlayerHistory()), currPlayer(0), winner(-1) {}

void GameState::AddMove(int x, int y)
{
    if(winner >= 0) { return; };

    history.at(currPlayer)[x].insert(y);
    winner = CheckWinCondition(x, y) ? currPlayer : winner;
    currPlayer = 1 - currPlayer;
}

bool GameState::HasWinner() const
{
    return winner >= 0;
}

int GameState::GetWinner() const
{
    return winner;
}

bool GameState::CheckWinCondition(int x, int y) const
{
    if(CountUpStreak(x, y)         + CountDownStreak(x, y)       + 1 >= STREAK_TO_WIN) { return true; }
    if(CountLeftStreak(x, y)       + CountRightStreak(x, y)      + 1 >= STREAK_TO_WIN) { return true; }
    if(CountUpperLeftStreak(x, y)  + CountLowerRightStreak(x, y) + 1 >= STREAK_TO_WIN) { return true; }
    if(CountUpperRightStreak(x, y) + CountLowerLeftStreak(x, y)  + 1 >= STREAK_TO_WIN) { return true; }

    return false;
}

int GameState::CountUpStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, 0, -1);
}

int GameState::CountDownStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, 0, 1);
}

int GameState::CountLeftStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, -1, 0);
}

int GameState::CountRightStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, 1, 0);
}

int GameState::CountUpperLeftStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, -1, -1);
}

int GameState::CountLowerRightStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, 1, 1);
}

int GameState::CountUpperRightStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, 1, -1);
}

int GameState::CountLowerLeftStreak(int x, int y) const
{
    return CountDirectionStreak(x, y, -1, 1);
}

int GameState::CountDirectionStreak(int x, int y, int x_direction, int y_direction) const
{
    int streak = 0;
    bool lastWasStreakCell = true;

    while(lastWasStreakCell)
    {
        x += x_direction;
        y += y_direction;

        PlayerHistory const& currPlayerHistory = history.at(currPlayer);

        if(currPlayerHistory.contains(x)
        && currPlayerHistory.at(x).contains(y))
        {
            ++streak;
        }
        else
        {
            lastWasStreakCell = false;
        }
    }

    return streak;
};
