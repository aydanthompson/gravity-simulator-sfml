#include "body.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "Gravity Simulator");
    window.setFramerateLimit(0);

    // Define scaling factors
    const float distanceScaleFactor = 0.000001;
    const float sizeScaleFactor = 0.00001;

    // Body creation
    Body earth(0, 0, 5.974e24, 6378.1e3, 0, 0);
    Body moon(0.4055e9, 0, 0.07346e24, 1738.1e3, 0, -0.970e3);

    // Shape creation
    sf::CircleShape earthShape(earth.getRadius() * sizeScaleFactor);
    earthShape.setFillColor(sf::Color::Blue);
    earthShape.setOrigin(earth.getRadius() * sizeScaleFactor, earth.getRadius() * sizeScaleFactor);
    sf::CircleShape moonShape(moon.getRadius() * sizeScaleFactor);
    moonShape.setFillColor(sf::Color::White);
    moonShape.setOrigin(moon.getRadius() * sizeScaleFactor, moon.getRadius() * sizeScaleFactor);

    float dt = 1.0;

    long double mfx, mfy, efx, efy;
    float hours = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        calculateForce(earth, moon, efx, efy);
        // earth.update(dt, efx, efy);
        calculateForce(moon, earth, mfx, mfy);
        moon.update_e(dt, mfx, mfy);

        hours += dt / (60 * 60);

        std::cout << std::scientific << std::setprecision(1) << std::showpos;
        std::cout << "d: Earth (" << earth.getX() << ", " << earth.getY() << "), Moon (" << moon.getX() << ", " << moon.getY() << ")\n";
        std::cout << "v: Earth (" << earth.getVX() << ", " << earth.getVY() << "), Moon (" << moon.getVX() << ", " << moon.getVY() << ")\n";
        std::cout << "f: Earth (" << efx << ", " << efy << "), Moon (" << mfx << ", " << mfy << ")\n";
        std::cout << std::fixed << std::noshowpos;
        std::cout << "Hours: " << hours << "\n";

        earthShape.setPosition(earth.getX() * distanceScaleFactor, -earth.getY() * distanceScaleFactor);
        moonShape.setPosition(moon.getX() * distanceScaleFactor, -moon.getY() * distanceScaleFactor);

        window.clear();

        window.draw(earthShape);
        window.draw(moonShape);

        window.display();
    }

    return 0;
}
