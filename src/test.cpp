#include "body.hpp"
#include <iostream>

int main()
{
    // Body creation
    Body earth(0, 0, 5.974e24, 6378.1e3, 0, 0);
    Body moon(0.4055e9, 0, 0.07346e24, 1738.1e3, 0, -1.082e3);

    // Simulation setup
    double dt = 1.0;
    double simulationTime = 86400.0 * 30.0; // 30 days in seconds
    int numSteps = static_cast<int>(simulationTime / dt);

    // Run simulation
    for (int i = 0; i < numSteps; i++)
    {
        double fx, fy;
        calculateForce(earth, moon, fx, fy);

        // Update bodies
        earth.update_rk4(dt, fx, fy);
        moon.update_rk4(dt, -fx, -fy);

        std::cout << "Step " << i << ": Earth (" << earth.getX() << ", " << earth.getY() << "), Moon (" << moon.getX() << ", " << moon.getY() << ")\n";
    }

    return 0;
}
