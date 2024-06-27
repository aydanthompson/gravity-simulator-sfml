#include "body.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

void draw_grid(sf::RenderWindow &window, const sf::View &view)
{
    // Set major and minor spacing distance and colour.
    float major_spacing = 100.0f;
    float minor_spacing = major_spacing / 10.0f;
    sf::Color major_colour = sf::Color(64, 64, 64, 32);
    sf::Color minor_colour = sf::Color(64, 64, 64, 16);

    sf::Vector2f view_size = view.getSize();
    sf::Vector2f view_centre = view.getCenter();

    sf::Vector2f top_left = view_centre - view_size / 2.0f;
    sf::Vector2f bottom_right = view_centre + view_size / 2.0f;

    // Stop lines from being drawn between pixels, fixes flickering.
    float start_x = std::floor(top_left.x / minor_spacing) * minor_spacing;
    float start_y = std::floor(top_left.y / minor_spacing) * minor_spacing;

    sf::VertexArray grid_lines(sf::Lines);

    // Draw vertical lines.
    for (float x = start_x; x <= bottom_right.x; x += minor_spacing)
    {
        sf::Color colour = ((int(x) % int(major_spacing)) == 0) ? major_colour : minor_colour;
        grid_lines.append(sf::Vertex(sf::Vector2f(x, top_left.y), colour));
        grid_lines.append(sf::Vertex(sf::Vector2f(x, bottom_right.y), colour));
    }

    // Draw horizontal lines.
    for (float y = start_y; y <= bottom_right.y; y += minor_spacing)
    {
        sf::Color colour = ((int(y) % int(major_spacing)) == 0) ? major_colour : minor_colour;
        grid_lines.append(sf::Vertex(sf::Vector2f(top_left.x, y), colour));
        grid_lines.append(sf::Vertex(sf::Vector2f(bottom_right.x, y), colour));
    }

    window.draw(grid_lines);
}

void focus_view_on_body(sf::RenderWindow &window, Body &body, const float DISTANCE_SCALE_FACTOR)
{
    // Get coordinates of top left corner of window when body is in the centre.
    sf::Vector2f top_left = sf::Vector2f(body.get_x() * DISTANCE_SCALE_FACTOR, -body.get_y() * DISTANCE_SCALE_FACTOR);
    top_left -= sf::Vector2f(window.getSize()) / 2.0f;

    // Create view centred on body, retain current window size.
    sf::View view = sf::View(sf::FloatRect(top_left, sf::Vector2f(window.getSize())));
    window.setView(view);
}

int main()
{
    // Set window parameters.
    const int WINDOW_X = 1080;
    const int WINDOW_Y = 1080;

    // Define scaling factors.
    const float DISTANCE_SCALE_FACTOR = 0.0000005;
    const float SIZE_SCALE_FACTOR = 0.000005;

    // Configure window.
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4.0;
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Gravity Simulator", sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);
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

        focus_view_on_body(window, bodies[1], DISTANCE_SCALE_FACTOR);

        hours += dt / (60 * 60);

        window.clear();

        draw_grid(window, window.getView());

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
