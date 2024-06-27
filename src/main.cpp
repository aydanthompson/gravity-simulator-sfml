#include "body.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

int main()
{
    // Set window parameters.
    const int WINDOW_X = 1080;
    const int WINDOW_Y = 1080;

    // Define scaling factors.
    const float DISTANCE_SCALE_FACTOR = 0.0000005;
    const float SIZE_SCALE_FACTOR = 0.000005;

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Gravity Simulator");
    window.setFramerateLimit(60);

    // Create bodies.
    Body body_a("Earth", 0, 0, 5.974e24, 6378.1e3, 0, 0);
    Body body_b("Moon", 0.4055e9, 0, 0.07346e24, 1738.1e3, 0, -0.970e3);
    Body body_c("Red", 0.8055e9, 0, 0.07346e4, 5738.1e2, 0, -0.470e3);
    Body body_d("Magenta", 0, 0.8e9, 0.07346e8, 1738.1e3, 0.470e3, 0);
    Body body_e("Yellow", 0, -0.8e9, 0.07346e8, 1738.1e3, -0.470e3, 0);
    Body body_f("Green", 0.4055e9, -0.4055e9, 0.07346e8, 1738.1e3, 0, -0.970e3);
    std::vector<Body> bodies = {body_a, body_b, body_c, body_d, body_e, body_f};

    // Create shapes.
    std::vector<sf::CircleShape> shapes = {};
    std::vector<sf::Color> colours = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::Magenta, sf::Color::Yellow, sf::Color::Green};
    for (int i = 0; i < bodies.size(); ++i)
    {
        sf::CircleShape shape(bodies[i].get_radius() * SIZE_SCALE_FACTOR);
        shape.setFillColor(colours[i]);
        shape.setOrigin(bodies[i].get_radius() * SIZE_SCALE_FACTOR, bodies[i].get_radius() * SIZE_SCALE_FACTOR);
        shapes.insert(shapes.end(), shape);
    }

    // Create views.
    float window_x_halved = float(WINDOW_X) / 2.f;
    float window_y_halved = float(WINDOW_Y) / 2.f;
    sf::View default_view(sf::FloatRect(-window_x_halved, -window_y_halved, float(WINDOW_X), float(WINDOW_Y)));
    sf::View focus_view(sf::FloatRect(-window_x_halved - (bodies[0].get_x() * DISTANCE_SCALE_FACTOR), -window_y_halved - (bodies[0].get_y() * DISTANCE_SCALE_FACTOR), float(WINDOW_X), float(WINDOW_Y)));
    window.setView(default_view);

    // Set timescale.
    // 1440 is equivalent to 1 day per second at 60 updates per second.
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

        // Update the force applied to each body.
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

        // Update the position of all bodies.
        for (Body &body_1 : bodies)
        {
            body_1.update_position_e(dt);
        }

        // Focus the view on a specific body.
        focus_view.setCenter(bodies[0].get_x() * DISTANCE_SCALE_FACTOR, -bodies[0].get_y() * DISTANCE_SCALE_FACTOR);
        window.setView(focus_view);

        hours += dt / (60 * 60);

        window.clear();

        // Update and draw all shapes.
        for (int i = 0; i < shapes.size(); ++i)
        {
            shapes[i].setPosition(bodies[i].get_x() * DISTANCE_SCALE_FACTOR, -bodies[i].get_y() * DISTANCE_SCALE_FACTOR);
            window.draw(shapes[i]);
        }

        window.display();
    }

    return 0;
}
