#ifndef BODY_HPP
#define BODY_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class Body
{
public:
    Body(const std::string &name, long double x, long double y, long double mass, long double radius, long double vx, long double vy);
    void update_position_rk4(float dt);
    void update_position_e(float dt);
    void update_force(const Body &body_alt);
    long double get_x() const;
    long double get_y() const;
    long double get_vx() const;
    long double get_vy() const;
    long double get_mass() const;
    long double get_radius() const;
    long double get_fx() const;
    long double get_fy() const;
    std::string get_name() const;
    void reset_force();
    void store_position(const float);
    std::vector<sf::Vector2f> get_position_history() const;

private:
    std::string name;
    long double x;
    long double y;
    long double mass;
    long double radius;
    long double vx;
    long double vy;
    long double fx;
    long double fy;
    std::vector<sf::Vector2f> position_history;
};

#endif // BODY_HPP