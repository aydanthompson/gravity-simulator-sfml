#include "body.hpp"
#include <iostream>
#include <cmath>
#include <string>

const long double G = 6.67408e-11;

Body::Body(const std::string &name, long double x, long double y, long double mass, long double radius, long double vx, long double vy)
    : name(name), x(x), y(y), mass(mass), radius(radius), vx(vx), vy(vy), fx(0), fy(0) {}

void Body::update_position_rk4(float dt)
{
    // Calculate acceleration components.
    long double ax = fx / mass;
    long double ay = fy / mass;

    // Update velocity components using RK4.
    long double k1vx = ax * dt;
    long double k1vy = ay * dt;
    long double k2vx = (ax + 0.5 * k1vx) * dt;
    long double k2vy = (ay + 0.5 * k1vy) * dt;
    long double k3vx = (ax + 0.5 * k2vx) * dt;
    long double k3vy = (ay + 0.5 * k2vy) * dt;
    long double k4vx = (ax + k3vx) * dt;
    long double k4vy = (ay + k3vy) * dt;
    vx += (k1vx + 2.0 * k2vx + 2.0 * k3vx + k4vx) / 6.0;
    vy += (k1vy + 2.0 * k2vy + 2.0 * k3vy + k4vy) / 6.0;

    // Update position components.
    x += vx * dt;
    y += vy * dt;
}

void Body::update_position_e(float dt)
{
    // Calculate acceleration components.
    long double ax = fx / mass;
    long double ay = fy / mass;

    // Update velocity components using Euler's method.
    vx += ax * dt;
    vy += ay * dt;

    // Update position components.
    x += vx * dt;
    y += vy * dt;
}

void Body::update_force(const Body &body_alt)
{
    // Calculate total distance using x and y components.
    long double dx = get_x() - body_alt.get_x();
    long double dy = get_y() - body_alt.get_y();
    long double d = sqrt(pow(dx, 2) + pow(dy, 2));

    // Calculate gravitational force between the two bodies.
    long double f = (G * get_mass() * body_alt.get_mass()) / pow(d, 2);

    // Calculate the x and y components of the force, add to the current applied force.
    fx -= f * dx / d;
    fy -= f * dy / d;
}

void Body::reset_force()
{
    fx = 0;
    fy = 0;
}

// Getters
std::string Body::get_name() const { return name; }
long double Body::get_x() const { return x; }
long double Body::get_y() const { return y; }
long double Body::get_vx() const { return vx; }
long double Body::get_vy() const { return vy; }
long double Body::get_fx() const { return fx; }
long double Body::get_fy() const { return fy; }
long double Body::get_mass() const { return mass; }
long double Body::get_radius() const { return radius; }
