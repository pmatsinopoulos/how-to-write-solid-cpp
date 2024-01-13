struct Motor {
    float getSpeed() { return 42; }
};

class Car {
public:
    Car(Motor frontMonitor, Motor rearMonitor) {
        mFrontMotor = frontMonitor;
        mRearMotor = rearMonitor;
    }

    float getSpeed() {
        return (mFrontMotor.getSpeed() + mRearMotor.getSpeed()) / 2;
    }

private:
    Motor mFrontMotor;
    Motor mRearMotor;
};

int main() {
    // TODO: Refactor the `Car` class to use dependency injection for the two `Car`
    // instances it needs
    Car car = Car(Motor(), Motor());

    if (car.getSpeed() != 42) {
        return 1;
    }

    return 0;
}
