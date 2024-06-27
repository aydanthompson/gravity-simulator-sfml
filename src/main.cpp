#include "body.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

int main()
{
    // Window parameters
    const int WINDOW_X = 1080;
    const int WINDOW_Y = 1080;

    // Define scaling factors
    const float DISTANCE_SCALE_FACTOR = 0.0000005;
    const float SIZE_SCALE_FACTOR = 0.000005;

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Gravity Simulator");
    window.setFramerateLimit(60);

    // Body creation
    Body body_a("Earth", 0, 0, 5.974e24, 6378.1e3, 0, 0);
    Body body_b("Moon", 0.4055e9, 0, 0.07346e24, 1738.1e3, 0, -0.970e3);
    Body body_c("Red", 0.8055e9, 0, 0.07346e4, 5738.1e2, 0, -0.470e3);
    Body body_d("Magenta", 0, 0.8e9, 0.07346e8, 1738.1e3, 0.470e3, 0);
    Body body_e("Yellow", 0, -0.8e9, 0.07346e8, 1738.1e3, -0.470e3, 0);
    Body body_f("Green", 0.4055e9, -0.4055e9, 0.07346e8, 1738.1e3, 0, -0.970e3);

    std::vector<Body> bodies = {body_a, body_b, body_c, body_d, body_e, body_f};
    std::vector<sf::CircleShape> shapes = {};
    std::vector<sf::Color> colours = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::Magenta, sf::Color::Yellow, sf::Color::Green};

    // Views
    sf::View defaultView(sf::FloatRect(-float(WINDOW_X) / 2.f, -float(WINDOW_Y) / 2.f, float(WINDOW_X), float(WINDOW_Y)));
    sf::View yellowView(sf::FloatRect((-float(WINDOW_X) / 2.f) - (bodies[4].getX() * DISTANCE_SCALE_FACTOR), (-float(WINDOW_Y) / 2.f) - (bodies[4].getY() * DISTANCE_SCALE_FACTOR), float(WINDOW_X), float(WINDOW_Y)));
    window.setView(defaultView);

    // Shape creation
    for (int i = 0; i < bodies.size(); ++i)
    {
        sf::CircleShape shape(bodies[i].getRadius() * SIZE_SCALE_FACTOR);
        shape.setFillColor(colours[i]);
        shape.setOrigin(bodies[i].getRadius() * SIZE_SCALE_FACTOR, bodies[i].getRadius() * SIZE_SCALE_FACTOR);
        shapes.insert(shapes.end(), shape);
    }

    float dt = 1440.0f;

    float hours = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (Body &body_1 : bodies)
        {
            body_1.reset_force();
            for (Body &body_2 : bodies)
            {
                if (&body_2 != &body_1)
                {
                    body_1.update_force(body_2);
                }
            }
        }

        for (Body &body_1 : bodies)
        {
            body_1.update_position_e(dt);
        }

        hours += dt / (60 * 60);

        // std::cout << std::scientific << std::setprecision(1) << std::showpos;
        // std::cout << "d: Earth (" << earth.getX() << ", " << earth.getY() << "), Moon (" << moon.getX() << ", " << moon.getY() << ")\n";
        // std::cout << "v: Earth (" << earth.getVX() << ", " << earth.getVY() << "), Moon (" << moon.getVX() << ", " << moon.getVY() << ")\n";
        // std::cout << "f: Earth (" << earth.getFX() << ", " << earth.getFY() << "), Moon (" << moon.getFX() << ", " << moon.getFY() << ")\n";
        // std::cout << std::fixed << std::noshowpos;
        // std::cout << "Hours: " << hours << "\n";

        window.clear();

        for (int i = 0; i < shapes.size(); ++i)
        {
            shapes[i].setPosition(bodies[i].getX() * DISTANCE_SCALE_FACTOR, -bodies[i].getY() * DISTANCE_SCALE_FACTOR);
            window.draw(shapes[i]);
        }

        window.display();
    }

    return 0;
}
