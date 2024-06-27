#include "body.hpp"
#include <iostream>
#include <cmath>

const long double G = 6.67408e-11;

Body::Body(long double x, long double y, long double mass, long double radius, long double vx, long double vy)
    : x(x), y(y), mass(mass), radius(radius), vx(vx), vy(vy) {}

void Body::update_rk4(float dt, long double fx, long double fy)
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

void Body::update_e(float dt, long double fx, long double fy)
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

// Getters
long double Body::getX() const { return x; }
long double Body::getY() const { return y; }
long double Body::getVX() const { return vx; }
long double Body::getVY() const { return vy; }
long double Body::getMass() const { return mass; }
long double Body::getRadius() const { return radius; }

void calculateForce(const Body &body1, const Body &body2, long double &fx, long double &fy)
{
    // Calculate total distance using x and y components.
    long double dx = body2.getX() - body1.getX();
    long double dy = body2.getY() - body1.getY();
    long double d = sqrt(pow(dx, 2) + pow(dy, 2));

    // std::cout << "Distance: " << d / 1e3 << "km\n";

    // Calculate gravitational force between the two bodies.
    long double f = (G * body1.getMass() * body2.getMass()) / pow(d, 2);

    // Calculate the x and y components of the force.
    fx = f * dx / d;
    fy = f * dy / d;
}
