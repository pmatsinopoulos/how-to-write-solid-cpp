#include <iostream>
#include <sstream>
#include <string>

namespace serial_port
{
/**
 * @brief Open the serial port, must be called once and before `write`
 */
void open()
{
    std::cout << "serial_port::open()" << std::endl;
}

/**
 * @brief Write to the serial port, must be called after open
 * @param message The message to write
 */
void write(const std::string& message)
{
    std::cout << message;
}

/**
 * @brief Close the serial port
 * Must be called once after `open` and the final `write`
 */
void close()
{
    std::cout << "serial_port::close()" << std::endl;
}
} // namespace serial_port

class PowerManager
{
public:
    PowerManager()
    {
        serial_port::open();
    }

    /// Turn on the power
    void power_on()
    {
        send_command("power_on");
    }

    /// Turn off the power
    void power_off()
    {
        send_command("power_off");
    }

    ~PowerManager()
    {
        serial_port::close();
    }

private:
    void send_command(const std::string& command)
    {
        // A command to the FSC300 sensor need to be formatted as follows:
        // 1. Start with `FSC300:`
        // 2. Followed by the length of the command
        // 3. Followed by a colon
        // 4. Followed by the command itself
        // 5. End with a newline
        std::string msg{"FSC300:" + std::to_string(command.size()) + ":" + command + "\n"};
        serial_port::write(msg);
    }
};

int main()
{
    // Redirect cout to a buffer, so we can check the output
    std::stringstream buffer{};
    auto old_output = std::cout.rdbuf(buffer.rdbuf());

    {
        // TODO: PowerManager is working as expected but it may have multiple responsibilities.
        // Q1: What are the responsibilities of PowerManager?
        // Q2: In which scenarios would PowerManager need to change?
        // Q3: What are the advantages of the current implementation?
        // Q4: How would you refactor PowerManager to abide by the Single Responsibility Principle?
        // (Ensure that running this program after refactoring outputs "OK")
        PowerManager pm{};
        pm.power_on();
        pm.power_off();
    }

    std::string expected{"serial_port::open()\n"
                         "FSC300:8:power_on\n"
                         "FSC300:9:power_off\n"
                         "serial_port::close()\n"};
    std::string actual{buffer.str()};
    std::cout.rdbuf(old_output); // Restore cout

    if (expected != actual)
    {
        std::cerr << "Expected: " << expected << std::endl;
        std::cerr << "Actual: " << actual << std::endl;
        return 1;
    }
    std::cout << "OK" << std::endl;

    return 0;
}
