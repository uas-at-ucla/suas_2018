#include "lib/physics_structs/physics_structs.h"

namespace spinny {

Vector3D& operator+=(Vector3D& vector, double scalar) {
  vector.x += scalar;
  vector.y += scalar;
  vector.z += scalar;

  return vector;
}

Vector3D& operator-=(Vector3D& vector, double scalar) {
  vector.x -= scalar;
  vector.y -= scalar;
  vector.z -= scalar;

  return vector;
}

Vector3D& operator*=(Vector3D& vector, double scalar) {
  vector.x *= scalar;
  vector.y *= scalar;
  vector.z *= scalar;

  return vector;
}

Vector3D& operator/=(Vector3D& vector, double scalar) {
  vector.x /= scalar;
  vector.y /= scalar;
  vector.z /= scalar;

  return vector;
}

double DegreesToRadians(double degrees) { return degrees / 180.0 * M_PI; }

double GetMagnitude(Vector3D vector) {
  return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

double GetDistance2D(Position3D start, Position3D end) {
  // Uses haversine formula.

  start.latitude = DegreesToRadians(start.latitude);
  start.longitude = DegreesToRadians(start.longitude);
  end.latitude = DegreesToRadians(end.latitude);
  end.longitude = DegreesToRadians(end.longitude);

  double d_latitude = end.latitude - start.latitude;
  double d_longitude = end.longitude - start.longitude;

  double a =
      pow(sin(d_latitude / 2), 2) +
      cos(start.latitude) * cos(end.latitude) * pow(sin(d_longitude / 2), 2);

  double c = 2 * asin(sqrt(a));
  double r = physical_constants::kRadiusOfEarthInMeters;
  return c * r;
}

double GetDistance3D(Position3D start, Position3D end) {
  double ground_distance = GetDistance2D(start, end);
  double climb_distance = end.altitude - start.altitude;

  return sqrt(pow(ground_distance, 2) + pow(climb_distance, 2));
}

Vector3D PointTowards(Position3D start, Position3D end) {
  double dx = GetDistance2D({start.latitude, 0, 0}, {end.latitude, 0, 0});
  double dy = GetDistance2D({0, start.longitude, 0}, {0, end.longitude, 0});
  double dz = start.altitude - end.altitude;  // In NED coordinates.

  Vector3D pointing_vector = {dx, dy, dz};

  pointing_vector /= GetMagnitude(pointing_vector);

  return pointing_vector;
}

}  // namespace spinny
