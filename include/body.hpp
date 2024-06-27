#ifndef BODY_HPP
#define BODY_HPP

#include <string>

class Body
{
public:
    Body(const std::string &name, long double x, long double y, long double mass, long double radius, long double vx, long double vy);
    void update_position_rk4(float dt, long double fx, long double fy);
    void update_position_e(float dt);
    void update_force(const Body &body_alt);
    long double getX() const;
    long double getY() const;
    long double getVX() const;
    long double getVY() const;
    long double getMass() const;
    long double getRadius() const;
    long double getFX() const;
    long double getFY() const;
    std::string get_name() const;
    void reset_force();

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
};

#endif // BODY_HPP