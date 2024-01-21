#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

struct Image
{
    std::vector<int> data{};
    std::size_t width{};
    std::size_t height{};
    std::vector<int> diagnosticData{};
};

struct SonyImage : Image
{
};

// TODO: Leaky abstractions
// Q1: How does the design violate the Dependency Inversion Principle and why is it a problem?
// Q2: How would you refactor the code to abide by the Dependency Inversion Principle?

class Camera
{
public:
    virtual ~Camera() = default;
    virtual void warmUp() = 0;
    /**
     * @brief Capture an image
     * @return The captured image
     */
    virtual Image captureImage() = 0;
};

class SonyCamera : public Camera
{
public:
    void warmUp() override
    {
        // Satisfy the Sony sensor requirement of waiting at least 1ms before capturing an image
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
        std::cout << "SonyCamera::warmUpSensor()" << std::endl;
    }

    Image captureImage() override
    {
        std::cout << "SonyCamera::captureImage()" << std::endl;
        SonyImage sonyImage{};
        sonyImage.data               = {1, 2, 3, 4};
        sonyImage.width              = 2;
        sonyImage.height             = 2;
        sonyImage.diagnosticData = {0, 0, 0, 1, 1, 1, 2, 2, 2};
        return sonyImage;
    }
};

class AutonomousEmergencyBraking
{
public:
    AutonomousEmergencyBraking(Camera& camera)
        : mCamera{camera}
    {
    }

    void detectAndAvoidCollision()
    {
        mCamera.warmUp();
        Image image{mCamera.captureImage()};
        for (std::size_t x = 0; x < image.width; ++x)
        {
            for (std::size_t y = 0; y < image.height; ++y)
            {
                // Do something with the image
                static_cast<void>(image.data.at(x + y));
            }
        }
    }

private:
    Camera& mCamera;
};

int main()
{
    // Redirect cout to a buffer, so we can check the output
    std::stringstream buffer{};
    auto old_output = std::cout.rdbuf(buffer.rdbuf());

    SonyCamera camera{};
    AutonomousEmergencyBraking aeb{camera};
    aeb.detectAndAvoidCollision();

    const std::string expected_output{"SonyCamera::warmUpSensor()\n"
                                      "SonyCamera::captureImage()\n"};
    const std::string actual{buffer.str()};
    std::cout.rdbuf(old_output); // Restore cout
    if (expected_output != actual)
    {
        std::cerr << "Expected output:\n" << expected_output << "\n\n";
        std::cerr << "Actual output:\n" << actual << "\n\n";
        return 1;
    }

    std::cout << "OK" << std::endl;

    return 0;
}
