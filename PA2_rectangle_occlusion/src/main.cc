#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

#define _USE_MATH_DEFINES
 
#include <cmath>

#define NUM_OBJECTS 20
#define MAX_RADIUS  20
#define MAX_COORDS  90
#define MIN_RATIO   0.25

typedef struct
{
    double x;
    double y;
    double dist = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
} Point;

typedef struct
{
    double dist;
    Point center;
    Point rect[4];
    double angles[4] { 360, 360, 0, 0};
    int radius;
    int min = 3;
    int max = 0;
    std::string q = "q1";
} Object;

typedef struct
{
    bool covered;
    double min;
    double max;
    Point p_min;
    Point p_max;
} Angle;

std::vector<Object> objects;

Object GetRandomCircle()
{
    Object object;
    object.center.x = -MAX_COORDS + (rand() % MAX_COORDS * 2);
    object.center.y = -MAX_COORDS + (rand() % MAX_COORDS * 2);
    object.radius = (rand() % MAX_RADIUS * (1 - MIN_RATIO)) + (MAX_RADIUS * (1 - MIN_RATIO));
    // prevent spawning on origin
    if (std::abs(object.center.x) - object.radius <= 0 || std::abs(object.center.y) - object.radius <= 0) return GetRandomCircle();
    return object;
}

bool ObjectInObjectsRadius(Object object)
{
    // can't collide if there's only one
    if (objects.size() <= 1) return false;
    // check if there's an intersection in a square around the circle
    for (size_t i = 0; i < objects.size(); i++)
        if (std::sqrt(std::pow(objects[i].center.x -  object.center.x, 2) + std::pow(objects[i].center.y - object.center.y, 2)) <= objects[i].radius + object.radius)
            return true;
    return false;
}

void RectInCircle(Object* object)
{
    const int angle = rand() % 360; // random and for the rect
    const int tilt  = (rand() % 30) + 30; // random tilt for the other side
    // get adjustments from angles
    const double dx = std::cos((angle*(2*acos(0.0)))/180) * object->radius;
    const double dy = std::sin((angle*(2*acos(0.0)))/180) * object->radius;
    const double tilt_dx = std::cos(((angle+tilt)*(2*acos(0.0)))/180) * object->radius;
    const double tilt_dy = std::sin(((angle+tilt)*(2*acos(0.0)))/180) * object->radius;
    // apply adjustments
    object->rect[0].x = object->center.x + dx;
    object->rect[0].y = object->center.y + dy;
    object->rect[1].x = object->center.x + tilt_dx;
    object->rect[1].y = object->center.y + tilt_dy;

    object->rect[2].x = object->center.x - dx;
    object->rect[2].y = object->center.y - dy;
    object->rect[3].x = object->center.x - tilt_dx;
    object->rect[3].y = object->center.y - tilt_dy;
}

bool SortObjectByDistance(Object const& lhs, Object const& rhs)
{
    return lhs.dist < rhs.dist;
}

bool SortPointByDistance(Point const& lhs, Point const& rhs)
{
    return lhs.dist < rhs.dist;
}

bool SortObjectByMinAngle(Object const& lhs, Object const& rhs)
{
    return lhs.angles[lhs.min] < rhs.angles[rhs.min];
}

bool SortAngleByMinAngle(Angle const& lhs, Angle const& rhs)
{
    return lhs.min < rhs.min;
}

int main()
{
    // seed rng
    srand(std::time(nullptr));

    // create object
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        // create circle + check collision
        std::cout << "Generating object " << i + 1 << " / " << NUM_OBJECTS << "\n";
        Object object;
        do
        {
            object = GetRandomCircle();
        } while (ObjectInObjectsRadius(object));

        // set distance
        object.dist = std::sqrt(std::pow(object.center.x, 2) + std::pow(object.center.y, 2));
        
        // create rect
        RectInCircle(&object);

        // get min/max angles + points
        for (int j = 0; j < 4; j++)
        {
            object.angles[j] = std::atan2(object.rect[j].y, object.rect[j].x);
            object.angles[j] = object.angles[j] * 360 / (2*M_PI);
            if (object.angles[j] < 0) object.angles[j] = object.angles[j] + 360;
        }

        // min/max
        object.max = std::distance(object.angles, std::max_element(object.angles, object.angles + 4));
        object.min = std::distance(object.angles, std::min_element(object.angles, object.angles + 4));

        // store, repeat
        objects.push_back(object);
    }

    // find object occlusion,
    // check each object against rest
    std::vector<Angle> all_angles;
    std::sort(objects.begin(), objects.end(), &SortObjectByMinAngle);
    for (size_t i = 0; i < objects.size(); i++)
    {
        std::cout << "Dist: " << objects[i].dist << '\n';
        for (size_t j = 0; j < objects.size(); j++)
        {
            if (i == j) continue;
            if (objects[i].angles[objects[i].min] > objects[j].angles[objects[j].min]) continue;
            // get min/max angle between rects
            double min_angle = std::max(objects[i].angles[objects[i].min], objects[j].angles[objects[j].min]);
            double max_angle = std::min(objects[i].angles[objects[i].max], objects[j].angles[objects[j].max]);
            // get min/max point from the angles
            Point min_point = (objects[i].angles[objects[i].min] < objects[j].angles[objects[j].min]) ? objects[j].rect[objects[j].min] : objects[i].rect[objects[i].min];
            Point max_point = (objects[i].angles[objects[i].max] < objects[j].angles[objects[j].max]) ? objects[i].rect[objects[i].max] : objects[j].rect[objects[j].max];

            // Check if there is an occluded range
            if (min_angle < max_angle)
                all_angles.push_back({ false, min_angle, max_angle, min_point, max_point });
        }
    }

    std::cout << "tallying,,,\n";

    // tally coverage
    std::vector<Angle> angles;
    double coverage = 0;
    std::sort(all_angles.begin(), all_angles.end(), &SortAngleByMinAngle);

    // check each arc for overlap with the rest
    for (size_t i = 0; i < all_angles.size(); i++)
    {
        std::cout << i << "/" << all_angles.size() << '\n';
        Angle currentAngle = all_angles[i];
        for (size_t j = 0; j < all_angles.size(); j++)
        {
            if (currentAngle.min > all_angles[j].max) continue;
            if (i == j) continue;
            // if there's overlap
            if (currentAngle.max > all_angles[j].min)
            {
                currentAngle.max = std::max(currentAngle.max, all_angles[j].max);
                all_angles[j].covered = true;
                std::cout << "new max: " << currentAngle.max << '\n';
            }
        }
        if (currentAngle.min != currentAngle.max)
        {
            // no overlap, add angle
            angles.push_back(currentAngle);
            if (!currentAngle.covered) coverage += currentAngle.max - currentAngle.min;
        }
    }

    std::cout << "\n---------------\n";
    std::cout << "\nCoverage: " << coverage << "/360.00\n";
    std::cout << "\n---------------\n";

    // serialize
    std::ofstream f;
    f.open("data.csv", std::ios::trunc);
    f.setf(std::ios::fixed);
    f.precision(8);
    // angles
    for (size_t i = 0; i < angles.size(); i++)
    {
        f << "ang" << ", "; // type
        f << angles[i].p_min.x << ", " << angles[i].p_min.y << ", ";
        f << angles[i].p_max.x << ", " << angles[i].p_max.y << ", ";
        f << angles[i].min << ", " << angles[i].max << ", ";
        f << '\n';
    }
    // angles
    for (size_t i = 0; i < all_angles.size(); i++)
    {
        f << "vec" << ", "; // type
        f << all_angles[i].p_min.x << ", " << all_angles[i].p_min.y << ", ";
        f << all_angles[i].p_max.x << ", " << all_angles[i].p_max.y << ", ";
        f << all_angles[i].min << ", " << all_angles[i].max << ", ";
        f << '\n';
    }
    // objects
    for (size_t i = 0; i < objects.size(); i++)
    {
        f << "obj" << ", "; // type
        f << objects[i].center.x << ", ";
        f << objects[i].center.y << ", "; 
        f << objects[i].radius << ", ";
        // rects
        for (int j = 0; j < 4; j++)
        {
            f << std::setw(13) << std::setfill(' ') << objects[i].rect[j].x << ", ";
            f << std::setw(13) << std::setfill(' ') << objects[i].rect[j].y << ", ";
        }
        f << objects[i].angles[objects[i].min] << ", " << objects[i].angles[objects[i].max];
        f << ", " << objects[i].q;
        f << '\n';
    }
}