#include "Matrix4x4.hpp"

namespace Math {

    Matrix4x4::Matrix4x4() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    Matrix4x4 Matrix4x4::rotation(const Vector3D &axisRaw, float angleDegrees) {
        Vector3D axis = axisRaw;
        axis.normalize();

        float rad = angleDegrees * M_PI / 180.0f;
        float c = std::cos(rad);
        float s = std::sin(rad);
        float t = 1.0f - c;

        float x = axis.x, y = axis.y, z = axis.z;

        Matrix4x4 R;
        R.m[0][0] = t*x*x + c;
        R.m[0][1] = t*x*y - s*z;
        R.m[0][2] = t*x*z + s*y;
        R.m[0][3] = 0.0f;

        R.m[1][0] = t*x*y + s*z;
        R.m[1][1] = t*y*y + c;
        R.m[1][2] = t*y*z - s*x;
        R.m[1][3] = 0.0f;

        R.m[2][0] = t*x*z - s*y;
        R.m[2][1] = t*y*z + s*x;
        R.m[2][2] = t*z*z + c;
        R.m[2][3] = 0.0f;

        R.m[3][0] = R.m[3][1] = R.m[3][2] = 0.0f;
        R.m[3][3] = 1.0f;

        return R;
    }

    Matrix4x4 Matrix4x4::inverse() const {
        Matrix4x4 inv;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                inv.m[i][j] = m[j][i];

        return inv;
    }

    Vector3D Matrix4x4::operator*(const Vector3D &v) const {
        return Vector3D(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
        );
    }

    Point3D Matrix4x4::operator*(const Point3D &p) const {
        // IMPORTANT: Gérer correctement la composante homogène (w=1)
        float x = m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3];
        float y = m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3];
        float z = m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3];
        float w = m[3][0]*p.x + m[3][1]*p.y + m[3][2]*p.z + m[3][3];
        
        return Point3D(x/w, y/w, z/w);
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const {
        Matrix4x4 result;
    
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                result.m[row][col] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[row][col] += m[row][k] * other.m[k][col];
                }
            }
        }
    
        return result;
    }

    Matrix4x4 Matrix4x4::translate(const Vector3D &offset) {
        Matrix4x4 T;
        T.m[0][3] = offset.x;
        T.m[1][3] = offset.y;
        T.m[2][3] = offset.z;
        return T;
    }
    
    Matrix4x4 Matrix4x4::rotateAroundPoint(const Vector3D &axis, float angleDeg, const Point3D &pivot) {
        Matrix4x4 toOrigin = translate(-pivot.toVector());
        Matrix4x4 rot = rotation(axis, angleDeg);
        Matrix4x4 back = translate(pivot.toVector());
    
        return back * rot * toOrigin;
    }
    
}
