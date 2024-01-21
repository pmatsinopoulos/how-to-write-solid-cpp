#include <iostream>

struct Degrees;
struct Radians;

namespace numbers
{
constexpr float pi = 3.14F;
}

struct Degrees
{
    Degrees(float v);
    Degrees(Radians r);

    bool operator==(const Degrees& other) const
    {
        return value == other.value;
    }

    bool operator!=(const Degrees& other) const
    {
        return !(*this == other);
    }

    float value;
};

struct Radians
{
    Radians(float v);
    Radians(Degrees d);

    bool operator==(const Radians& other) const
    {
        return value == other.value;
    }

    bool operator!=(const Radians& other) const
    {
        return !(*this == other);
    }

    float value;
};

Degrees::Degrees(float v)
    : value{v}
{
}

Degrees::Degrees(Radians r)
    : value{r.value * 180 / numbers::pi}
{
}

Radians::Radians(float v)
    : value{v}
{
}

Radians::Radians(Degrees d)
    : value{d.value * numbers::pi / 180}
{
}

class Car
{
public:
    virtual ~Car()                   = default;
    virtual void rotate(Degrees angle) = 0;
    virtual void rotate(Radians angle) = 0;
};

class FrontWheelDriveCar : public Car
{
public:
    void rotate(Radians angle) override
    {
        actuate_front_wheel_drive_system(Degrees{angle});
    }

    void rotate(Degrees angle) override
    {
        actuate_front_wheel_drive_system(angle);
    }

    Degrees getDegrees() const
    {
        return mAngle;
    }

private:
    // Note: Do not change the body or the signature of this function
    void actuate_front_wheel_drive_system(Degrees degrees)
    {
        mAngle = degrees;
    }

    Degrees mAngle{0.0F};
};

class RearWheelDriveCar : public Car
{
public:
    void rotate(Degrees angle) override
    {
        actuate_rear_wheel_drive_system(Radians{angle});
    }

    void rotate(Radians angle) override
    {
        actuate_rear_wheel_drive_system(angle);
    }

    Radians getRadians() const
    {
        return mAngle;
    }

private:
    // Note: Do not change the body or the signature of this function
    void actuate_rear_wheel_drive_system(Radians radians)
    {
        mAngle = radians;
    }

    Radians mAngle{0.0F};
};

void rotate_wheels(Car& car)
{
    // TODO: Rotating the wheels of a car should not depend on the type of car
    // Q1: How is the Liskov Substitution Principle violated by the current implementation?
    // Q2: How would you refactor the code to abide by the Liskov Substitution Principle?
    //     You should not change the `actuate_` functions.
    //     Focus on the `Car` interface and the `rotate` implementation.
    //     Try to make things as robust and statically safe as possible.
    // (After refactoring, the program should stop failing and output "OK")
    car.rotate(Degrees{90});
}

int main()
{
    FrontWheelDriveCar front_wheel_drive_car{};
    RearWheelDriveCar rear_wheel_drive_car{};

    rotate_wheels(front_wheel_drive_car);
    rotate_wheels(rear_wheel_drive_car);

    Degrees fwdCarDegrees = front_wheel_drive_car.getDegrees();
    Radians rwdCarRadians = rear_wheel_drive_car.getRadians();

    if (fwdCarDegrees != Degrees{rwdCarRadians})
    {
        std::cerr << "Front wheel drive car angle in degrees: " << fwdCarDegrees.value << std::endl;
        std::cerr << "Front wheel drive car angle in radians: " << Radians{fwdCarDegrees}.value
                  << std::endl;
        std::cerr << "--------------------------------------" << std::endl;
        std::cerr << "Rear wheel drive car angle in degrees: " << Degrees{rwdCarRadians}.value
                  << std::endl;
        std::cerr << "Rear wheel drive car angle in radians: " << rwdCarRadians.value << std::endl;

        std::cerr << "The same angle was passed to both cars, but they have different values!"
                  << std::endl;

        return 1;
    }

    std::cout << "OK" << std::endl;

    return 0;
}
