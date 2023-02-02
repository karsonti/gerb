#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Gerb : public olc::PixelGameEngine
{
public:
    Gerb()
    {
        sAppName = "Gerb";
    }

    struct sBall
    {
        olc::vf2d pos;
        olc::vf2d vel;
        olc::Pixel col;
        float radius;
    };

    std::vector<sBall> vBalls;

public:
    void AddBall(float radius = 4.0f)
    {
        static int ball_count = 0;
        float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f - 0.3f;
        float fSpeed = float(rand()) / float(RAND_MAX) * 100.0f + 20.0f;

        vBalls.push_back(
            {
                olc::vf2d(float(ScreenWidth()) / 2.0f, float(ScreenHeight()) / 2.0f),
                olc::vf2d(fSpeed * cos(fAngle), fSpeed * sin(fAngle)),
                olc::Pixel(olc::BLACK),
                radius
            }
        );
    }

    void RemoveBall()
    {
        if (vBalls.empty())
            return;

        vBalls.pop_back();
    }

    void RenderBalls(const float fElapsedTime)
    {
        for (auto& ball : vBalls)
        {
            ball.pos += ball.vel * fElapsedTime;
            if (ball.pos.x < ball.radius || ball.pos.x >(ScreenWidth()))
            {
                ball.pos.x = std::clamp(ball.pos.x, ball.radius, (ScreenWidth() - ball.radius));
                ball.vel.x *= -1.0f;
            }

            if (ball.pos.y < ball.radius || ball.pos.y >(ScreenHeight()))
            {
                ball.pos.y = std::clamp(ball.pos.y, ball.radius, (ScreenHeight() - ball.radius));
                ball.vel.y *= -1.0f;
            }
        }

        for (const auto& ball : vBalls)
            FillCircle(ball.pos, int32_t(ball.radius), ball.col);
    }

    bool OnUserCreate() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        system("unclutter -idle 0 & wmctrl -r 'OneLoneCoder' -b toggle,fullscreen");

        AddBall();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime)
    {
        if (GetKey(olc::Key::ESCAPE).bPressed) return false;
        if (GetKey(olc::Key::K1).bPressed) AddBall();
        if (GetKey(olc::Key::K0).bPressed) RemoveBall();

        Clear(olc::WHITE);
        RenderBalls(fElapsedTime);
        return true;
    }
};

int main()
{
    Gerb gerb;
    if (gerb.Construct(170, 100, 1, 1))
        gerb.Start();
    return 0;
}