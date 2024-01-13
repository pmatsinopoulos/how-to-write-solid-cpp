#include <iostream>
#include <sstream>
#include <string>

namespace old_library
{
void configure()
{
    std::cout << "configureOldPeripheralDevice()" << std::endl;
}
} // namespace old_library

namespace previous_library
{
void configure()
{
    std::cout << "configurePreviousPeripheralDevice()" << std::endl;
}
} // namespace previous_library

namespace current_library
{
void configure()
{
    std::cout << "configureCurrentPeripheralDevice()" << std::endl;
}
} // namespace current_library

class Library
{
public:
    virtual std::string build_message_for_sending(const std::string& message) = 0;
    virtual std::string read(const std::string& message) = 0;
    virtual ~Library() = default;
};

class LibraryLessThan2 : public Library
{
public:
    std::string build_message_for_sending(const std::string& message)
    {
        old_library::configure();
        return "<" + message + ">";
    }

    std::string read(const std::string& message)
    {
       return message.substr(2, message.size() - 4);
    }
};

class LibraryLessThan5 : public Library
{
public:
    std::string build_message_for_sending(const std::string& message)
    {
        previous_library::configure();
        return "[" + message + "]";
    }

    std::string read(const std::string& message)
    {
       return message.substr(1, message.size() - 3);
    }
};

class LibraryCurrent : public Library
{
public:
    std::string build_message_for_sending(const std::string& message)
    {
        current_library::configure();
        return "{" + message + "}";
    }

    std::string read(const std::string& message)
    {
        return message.substr(1, message.size() - 2);
    }
};

class SerialPort
{
public:
    SerialPort(Library* library)
        : mLibrary{library}
    {
    }

    void send(std::string message)
    {
        message = mLibrary->build_message_for_sending(message);

        message = "--" + message + "--";
        std::cout << "Sending message: " << message << std::endl;
    }

    std::string read()
    {
        // Some common functionality
        std::string message{};
        std::cin >> message;
        if (message.empty())
        {
            return "EMPTY";
        }

        message = mLibrary->read(message);

        // Some common functionality
        message = "++" + message + "++";

        return message;
    }

private:
    Library *mLibrary;
};

int main()
{
    // Redirect cout to a buffer, so we can check the output
    std::stringstream output{};
    auto old_output = std::cout.rdbuf(output.rdbuf());
    std::stringstream input{};
    auto old_input = std::cin.rdbuf(input.rdbuf());

    // TODO: We may need to change the SerialPort class to support new versions.
    // Q1: Which parts of SerialPort need to be modified once a new protocol version is introduced?
    // Q2: What is the problem with the current implementation?
    // Q3: How would you refactor SerialPort to abide by the Open-Closed Principle?
    //     Mind that this is not a "classic" Open-Closed Principle refactoring example,
    //     in the sense that it does not involve a switch-case statement.
    //     Nonetheless, the violation is there and you are tasked to refactor it.
    // (Ensure that running this program after refactoring outputs "OK")
    LibraryLessThan2 l2 = LibraryLessThan2();
    SerialPort oldDeviceSerialPort{&l2};
    oldDeviceSerialPort.send("Hello");

    const std::string expectedOutput{"configureOldPeripheralDevice()\n"
                                     "Sending message: --<Hello>--\n"};
    const std::string actualOutput{output.str()};

    if (actualOutput != expectedOutput)
    {
        std::cerr << "Expected: " << expectedOutput << std::endl;
        std::cerr << "Actual: " << actualOutput << std::endl;
        return 1;
    }
    std::cout.rdbuf(old_output); // Restore cout

    input.str("!!Howdy%%");
    const std::string receivedMessage{oldDeviceSerialPort.read()};
    const std::string expectedMessage{"++Howdy++"};
    std::cin.rdbuf(old_input); // Restore cin

    if (receivedMessage != expectedMessage)
    {
        std::cerr << "Expected: " << expectedMessage << std::endl;
        std::cerr << "Actual: " << receivedMessage << std::endl;
        return 1;
    }
    std::cout << "OK" << std::endl;

    return 0;
}
