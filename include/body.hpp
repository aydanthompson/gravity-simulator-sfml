#ifndef BODY_HPP
#define BODY_HPP

class Body
{
public:
    Body(long double x, long double y, long double mass, long double radius, long double vx, long double vy);
    void update_rk4(float dt, long double fx, long double fy);
    void update_e(float dt, long double fx, long double fy);
    long double getX() const;
    long double getY() const;
    long double getVX() const;
    long double getVY() const;
    long double getMass() const;
    long double getRadius() const;

private:
    long double x;
    long double y;
    long double mass;
    long double radius;
    long double vx;
    long double vy;
};

void calculateForce(const Body &body1, const Body &body2, long double &fx, long double &fy);

#endif // BODY_HPP