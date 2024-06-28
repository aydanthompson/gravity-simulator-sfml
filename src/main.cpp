#include "body.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

void draw_grid(sf::RenderWindow &window, const sf::View &view)
{
    // Set major and minor spacing distance and colour.
    float major_spacing = 1.0e6f;
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

void focus_view_on_body(sf::RenderWindow &window, Body *body, const float DISTANCE_SCALE_FACTOR, float zoom_factor)
{
    if (!body)
        return;

    // Get coordinates of top left corner of window when body is in the centre.
    sf::Vector2f top_left = sf::Vector2f(body->get_x() * DISTANCE_SCALE_FACTOR, -body->get_y() * DISTANCE_SCALE_FACTOR);
    top_left -= sf::Vector2f(window.getSize()) / 2.0f;

    // Create view centred on body, retain current window size.
    sf::View view = sf::View(sf::FloatRect(top_left, sf::Vector2f(window.getSize())));
    view.zoom(zoom_factor);
    window.setView(view);
}

void draw_trails(sf::RenderWindow &window, Body &body)
{
    std::vector<sf::Vector2f> position_history = body.get_position_history();
    sf::Color base_colour = body.get_colour();

    if (position_history.size() >= 2)
    {
        sf::VertexArray lines(sf::LinesStrip, position_history.size());
        for (size_t i = 0; i < position_history.size(); ++i)
        {
            lines[i].position = position_history[i];
            float alpha = static_cast<float>(i) / position_history.size() * 255;
            lines[i].color = sf::Color(base_colour.r, base_colour.g, base_colour.b, static_cast<sf::Uint8>(alpha));
        }
        window.draw(lines);
    }
}

std::vector<Body> load_bodies_from_json(const std::string &setup_name)
{
    std::string filename = "data/bodies.json";

    std::printf("Attempting to load \"%s\" from \"%s\"\n", setup_name.c_str(), filename.c_str());

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file \"" << filename << "\"" << std::endl;
        return {};
    }

    nlohmann::json json;
    file >> json;
    file.close();

    std::vector<Body> bodies = {};
    if (json.contains("setups") && json["setups"].contains(setup_name))
    {
        for (const auto &body : json["setups"][setup_name])
        {
            std::string name = body["name"];
            sf::Color colour(body["colour"]["r"],
                             body["colour"]["g"],
                             body["colour"]["b"],
                             body["colour"]["a"]);
            float x = body["x"];
            float y = body["y"];
            double mass = body["mass"];
            double radius = body["radius"];
            float vx = body["vx"];
            float vy = body["vy"];

            bodies.push_back(Body(name, colour, x, y, mass, radius, vx, vy));
        }
    }

    return bodies;
}

int main()
{
    // Set window parameters.
    const int WINDOW_X = 1920;
    const int WINDOW_Y = 1080;

    // Define scaling factors.
    const float DISTANCE_SCALE_FACTOR = 0.0000005;
    const float SIZE_SCALE_FACTOR = 0.0001;

    // Configure window.
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4.0;
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Gravity Simulator", sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);

    // Create bodies.
    std::vector<Body> bodies = load_bodies_from_json("solar_system");

    // Create shapes.
    std::vector<sf::CircleShape> shapes = {};
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        sf::CircleShape shape(bodies[i].get_radius() * SIZE_SCALE_FACTOR);
        shape.setFillColor(bodies[i].get_colour());
        shape.setOrigin(bodies[i].get_radius() * SIZE_SCALE_FACTOR, bodies[i].get_radius() * SIZE_SCALE_FACTOR);
        shapes.push_back(shape);
    }

    // Create views.
    float window_x_halved = float(WINDOW_X) / 2.f;
    float window_y_halved = float(WINDOW_Y) / 2.f;
    sf::View default_view(sf::FloatRect(-window_x_halved, -window_y_halved, float(WINDOW_X), float(WINDOW_Y)));
    window.setView(default_view);

    Body *focused_body = &bodies[0];

    // Set timescale.
    // 1440 is equivalent to 1 day per second at 60 updates per second.
    float dt = 1440.0f;

    int frame_count = 0;
    int record_position_interval = 10;
    float zoom_factor = 1.0f;

    float hours = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.mouseButton.button == sf::Mouse::Left && event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos, window.getView());
                for (size_t i = 0; i < shapes.size(); ++i)
                {
                    if (shapes[i].getGlobalBounds().contains(world_pos))
                    {
                        focused_body = &bodies[i];
                    }
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoom_factor *= 0.9f;
                else if (event.mouseWheelScroll.delta < 0)
                    zoom_factor *= 1.1f;
            }
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
        for (Body &body : bodies)
        {
            body.update_position_e(dt);
            if (frame_count % record_position_interval == 0)
            {
                body.store_position(DISTANCE_SCALE_FACTOR);
            }
        }

        focus_view_on_body(window, focused_body, DISTANCE_SCALE_FACTOR, zoom_factor);

        hours += dt / (60 * 60);

        window.clear();

        draw_grid(window, window.getView());

        // Update and draw all shapes.
        for (size_t i = 0; i < shapes.size(); ++i)
        {
            shapes[i].setPosition(bodies[i].get_x() * DISTANCE_SCALE_FACTOR, -bodies[i].get_y() * DISTANCE_SCALE_FACTOR);
            window.draw(shapes[i]);
            draw_trails(window, bodies[i]);
        }

        frame_count++;

        window.display();
    }

    return 0;
}
