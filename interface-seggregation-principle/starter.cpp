#include <functional>
#include <iostream>
#include <string>

enum class PowerState
{
    ON,
    SUSPEND,
    HIBERNATE,
    OFF
};

class PowerStateManager
{
public:
    virtual ~PowerStateManager()                                                      = default;
    virtual void powerOn()                                                            = 0;
    virtual void suspend()                                                            = 0;
    virtual void hibernate()                                                          = 0;
    virtual void powerOff()                                                           = 0;
};

class PowerStateListenerRegistrar
{
public:
    virtual void registerPowerStateListener(std::function<void(PowerState)> listener) = 0;
};

class AndroidPowerStateManager : public PowerStateManager,
                                 public PowerStateListenerRegistrar
{
public:
    void powerOn() override
    {
        notifyListeners(PowerState::ON);
    }
    void suspend() override
    {
        notifyListeners(PowerState::SUSPEND);
    }

    void hibernate() override
    {
        notifyListeners(PowerState::HIBERNATE);
    }

    void powerOff() override
    {
        notifyListeners(PowerState::OFF);
    }

    void registerPowerStateListener(std::function<void(PowerState)> listener) override
    {
        mListeners.push_back(listener);
    }

private:
    void notifyListeners(PowerState state)
    {
        for (auto& listener : mListeners)
        {
            listener(state);
        }
    }

    std::vector<std::function<void(PowerState)>> mListeners{};
};

// TODO: MusicPlayer accessing PowerStateManager
// Q1: What is the problem with this design when it comes to the Interface Segregation Principle?
// Q2: Are there any benefits to the current approach?
// Q3: How would you refactor the code to abide by the Interface Segregation Principle?

class MusicPlayer
{
public:
    MusicPlayer(PowerStateListenerRegistrar& powerStateListenerRegistrar)
    {
        powerStateListenerRegistrar.registerPowerStateListener(
            [this](PowerState state)
            {
                switch (state)
                {
                case PowerState::SUSPEND:
                    pause();
                    break;
                case PowerState::HIBERNATE:
                case PowerState::OFF:
                    stop();
                    break;
                default:
                    play();
                    break;
                }
            });
    }

    void play()
    {
        mLastAction = "play";
    }

    void pause()
    {
        mLastAction = "pause";
    }

    void stop()
    {
        mLastAction = "stop";
    }

    std::string getLastAction() const
    {
        return mLastAction;
    }

private:
    std::string mLastAction{};
};

int main()
{
    AndroidPowerStateManager powerStateManager{};
    MusicPlayer musicPlayer{powerStateManager};

    powerStateManager.powerOn();
    if (musicPlayer.getLastAction() != "play")
    {
        std::cerr << "Music player should be playing" << std::endl;
        std::cerr << "Last action: " << musicPlayer.getLastAction() << std::endl;
        return 1;
    }

    powerStateManager.suspend();
    if (musicPlayer.getLastAction() != "pause")
    {
        std::cerr << "Music player should be paused" << std::endl;
        std::cerr << "Last action: " << musicPlayer.getLastAction() << std::endl;
        return 1;
    }

    powerStateManager.powerOff();
    if (musicPlayer.getLastAction() != "stop")
    {
        std::cerr << "Music player should be stopped" << std::endl;
        std::cerr << "Last action: " << musicPlayer.getLastAction() << std::endl;
        return 1;
    }

    std::cout << "OK" << std::endl;

    return 0;
}
