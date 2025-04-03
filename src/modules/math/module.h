/**
 * @file math/module.h
 * @brief Math module header file.
 */

#pragma once
#include <cmath>

/**
 * @brief 3D vector structure.
 * 
 * This structure represents a 3D vector with x, y, and z components.
 * It provides various operations such as addition, subtraction, multiplication, and division.
 * It also has functions for dot product, cross product, length, normalization, and linear interpolation.
 * 
 * @note The vector components can be accessed using x, y, z or r, g, b names, as well as an array of floats.
 */
union vec3 {
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
    float v[3];
};
inline vec3 operator+(vec3 a, vec3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline vec3 operator-(vec3 a, vec3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
inline vec3 operator*(vec3 a, float b) {
    return {a.x * b, a.y * b, a.z * b};
}
inline vec3 operator/(vec3 a, float b) {
    return {a.x / b, a.y / b, a.z / b};
}
/// @brief Vector dot product operation.
/// @param a First vec3
/// @param b Second vec3
/// @return Result of a dot b
inline float dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
/// @brief Vector cross product operation.
/// @param a First vec3
/// @param b Second vec3
/// @return Result of a x b
inline vec3 cross(vec3 a, vec3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
/// @brief Vector length operation.
/// @param a The vec3
/// @return Result of |a|
inline float length(vec3 a) {
    return sqrtf(dot(a, a));
}
/// @brief Vector normalization operation.
/// @param a The vec3
/// @return Result of a / |a|
inline vec3 normalize(vec3 a) {
    return a / length(a);
}
inline vec3 operator-(vec3 a) {
    return {-a.x, -a.y, -a.z};
}
/// @brief Vector linear interpolation operation.
/// @param a The first vec3
/// @param b The second vec3
/// @param t The interpolation factor (0.0 to 1.0)
/// @return Result of linear interpolation between a and b
inline vec3 lerp(vec3 a, vec3 b, float t) {
    return a + (b - a) * t;
}
inline bool operator==(vec3 a, vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
inline bool operator!=(vec3 a, vec3 b) {
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

/**
 * @brief 4D vector structure.
 * 
 * This structure represents a 4D vector with x, y, z, and w components.
 * It provides various operations such as addition, subtraction, multiplication, and division.
 * It also has functions for dot product, length, and normalization.
 * 
 * @note The vector components can be accessed using x, y, z, w or r, g, b, a names, as well as an array of floats.
 */
union vec4 {
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
    float v[4];
};
inline vec4 operator+(vec4 a, vec4 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
inline vec4 operator-(vec4 a, vec4 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
inline vec4 operator*(vec4 a, float b) {
    return {a.x * b, a.y * b, a.z * b, a.w * b};
}
inline vec4 operator/(vec4 a, float b) {
    return {a.x / b, a.y / b, a.z / b, a.w / b};
}
/// @brief Vector dot product operation.
/// @param a First vec4
/// @param b Second vec4
/// @return Result of a dot b
inline float dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
/// @brief Vector length operation.
/// @param a The vec4
/// @return Result of |a|
inline float length(vec4 a) {
    return sqrtf(dot(a, a));
}
/// @brief Vector normalization operation.
/// @param a The vec4
/// @return Result of a / |a|
inline vec4 normalize(vec4 a) {
    return a / length(a);
}

/**
 * @brief 2D vector structure.
 * 
 * This structure represents a 2D vector with x and y components.
 * It provides various operations such as addition, subtraction, multiplication, and division.
 * It also has functions for dot product, length, and normalization.
 * 
 * @note The vector components can be accessed using x, y or u, v names, as well as an array of floats.
 */
union vec2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float f[2];

    vec2(float x, float y) : x(x), y(y) {}
    vec2() : x(0), y(0) {}
    vec2(int x, int y) : x((float)x), y((float)y) {}
    vec2(unsigned int x, unsigned int y) : x((float)x), y((float)y) {}
};
inline vec2 operator+(vec2 a, vec2 b) {
    return {a.x + b.x, a.y + b.y};
}
inline vec2 operator-(vec2 a, vec2 b) {
    return {a.x - b.x, a.y - b.y};
}
inline vec2 operator*(vec2 a, float b) {
    return {a.x * b, a.y * b};
}
inline vec2 operator/(vec2 a, float b) {
    return {a.x / b, a.y / b};
}
inline vec2 operator*(float a, vec2 b) {
    return {a * b.x, a * b.y};
}
inline vec2 operator/(float a, vec2 b) {
    return {a / b.x, a / b.y};
}
/// @brief Vector dot product operation.
/// @param a First vec2
/// @param b Second vec2
/// @return Result of a dot b
inline float dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}
/// @brief Vector length operation.
/// @param a The vec2
/// @return Result of |a|
inline float length(vec2 a) {
    return sqrtf(dot(a, a));
}
/// @brief Vector normalization operation.
/// @param a The vec2
/// @return Result of a / |a|
inline vec2 normalize(vec2 a) {
    return a / length(a);
}

/**
 * @brief 4x4 matrix structure.
 * 
 * This structure represents a 4x4 matrix with various operations such as multiplication and transformation.
 * It provides functions for creating identity, translation, scaling, rotation, perspective, and orthographic matrices.
 * 
 * @note The matrix components can be accessed using m[i][j] or x, y, z, w names for the columns.
 */
union mat4 {
    float m[4][4];
    struct {
        vec4 x, y, z, w;
    };

    inline float* operator[](int i) {
        if (i < 0 || i >= 4) {
            throw std::out_of_range("Index out of range");
        }
        return m[i];
    }
};
inline mat4 operator*(mat4 a, mat4 b) {
    mat4 result = {};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return result;
}
inline vec4 operator*(mat4 a, vec4 b) {
    vec4 result = {};
    for (int i = 0; i < 4; i++) {
        result.v[i] = a.m[i][0] * b.v[0] + a.m[i][1] * b.v[1] + a.m[i][2] * b.v[2] + a.m[i][3] * b.v[3];
    }
    return result;
}
/// @brief Matrix identity operation.
/// @return Identity matrix
inline mat4 identity() {
    mat4 result = {};
    for (int i = 0; i < 4; i++) {
        result.m[i][i] = 1.0f;
    }
    return result;
}
/// @brief Matrix translation operation.
/// @param v The translation vector
/// @return Translation matrix
inline mat4 translate(vec3 v) {
    mat4 result = identity();
    result.w.x = v.x;
    result.w.y = v.y;
    result.w.z = v.z;
    return result;
}
/// @brief Matrix scaling operation.
/// @param v The scaling vector
/// @return Scaling matrix
inline mat4 scale(vec3 v) {
    mat4 result = {};
    result.x.x = v.x;
    result.y.y = v.y;
    result.z.z = v.z;
    result.w.w = 1.0f;
    return result;
}
/// @brief Matrix rotation operation.
/// @param angle Rotation angle in radians
/// @param axis Rotation axis
/// @return Rotation matrix
/// @note The rotation is performed around the specified axis by the specified angle.
inline mat4 rotate(float angle, vec3 axis) {
    axis = normalize(axis);
    float s = sinf(angle);
    float c = cosf(angle);
    float oc = 1.0f - c;
    mat4 result = {};
    result.x.x = oc * axis.x * axis.x + c;
    result.x.y = oc * axis.x * axis.y - axis.z * s;
    result.x.z = oc * axis.x * axis.z + axis.y * s;
    result.y.x = oc * axis.y * axis.x + axis.z * s;
    result.y.y = oc * axis.y * axis.y + c;
    result.y.z = oc * axis.y * axis.z - axis.x * s;
    result.z.x = oc * axis.z * axis.x - axis.y * s;
    result.z.y = oc * axis.z * axis.y + axis.x * s;
    result.z.z = oc * axis.z * axis.z + c;
    result.w.w = 1.0f;
    return result;
}
/// @brief Matrix rotation operation.
/// @param angles Rotation angles in radians (x, y, z)
/// @return Rotation matrix
inline mat4 rotate(vec3 angles) {
    mat4 result = rotate(angles.z, {0, 0, 1}) * rotate(angles.y, {0, 1, 0}) * rotate(angles.x, {1, 0, 0});
    return result;
}
/// @brief Matrix perspective operation.
/// @param fov Field of view in degrees
/// @param aspect Aspect ratio (width / height)
/// @param nearp Near plane distance
/// @param farp Far plane distance
/// @return Perspective projection matrix
inline mat4 perspective(float fov, float aspect, float nearp, float farp) {
    float f = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);
    mat4 result = {};
    result.x.x = f / aspect;
    result.y.y = f;
    result.z.z = (farp + nearp) / (nearp - farp);
    result.z.w = -1.0f;
    result.w.z = 2.0f * farp * nearp / (nearp - farp);
    return result;
}

/// @brief Matrix orthographic operation.
/// @param left Left plane distance
/// @param right Right plane distance
/// @param bottom Bottom plane distance
/// @param top Top plane distance
/// @param nearp Near plane distance
/// @param farp Far plane distance
/// @return Orthographic projection matrix
inline mat4 orthographic(float left, float right, float bottom, float top, float nearp, float farp) {
    mat4 result = {};
    result.x.x = 2.0f / (right - left);
    result.y.y = 2.0f / (top - bottom);
    result.z.z = -2.0f / (farp - nearp);
    result.w.x = -(right + left) / (right - left);
    result.w.y = -(top + bottom) / (top - bottom);
    result.w.z = -(farp + nearp) / (farp - nearp);
    result.w.w = 1.0f;
    return result;
}
/**
 * @brief Camera structure.
 * 
 * This structure represents a camera in 3D space with position, orientation, and movement capabilities.
 * It provides functions for yaw, pitch, and movement operations.
 */
struct sCamera {
    /// @brief Camera position in 3D space.
    vec3 pos = {0, 0, 0};
    /// @brief Camera up vector.
    vec3 up = {0, 1, 0};
    /// @brief Camera forward vector.
    vec3 forward = {0, 0, -1};
    /// @brief Camera yaw angle in radians.
    float yaw = 0.0f;
    /// @brief Camera pitch angle in radians.
    float pitch = 0.0f;

    /// @brief Camera right vector.
    vec3 right() {
        return normalize(cross(forward, up));
    }
    /// @brief Camera left vector.
    vec3 left() {
        return normalize(cross(up, forward));
    }
    /// @brief Camera back vector.
    vec3 back() {
        return normalize(-forward);
    }
    /// @brief Camera down vector.
    vec3 down() {
        return normalize(-up);
    }

    /// @brief Camera right vector based on different forward vector.
    /// @param forward The forward vector to calculate the right vector from.
    /// @return The right vector based on the specified forward vector.
    vec3 right(vec3 forward) {
        return normalize(cross(forward, up));
    }
    /// @brief Camera left vector based on different forward vector.
    /// @param forward The forward vector to calculate the left vector from.
    /// @return The left vector based on the specified forward vector.
    vec3 left(vec3 forward) {
        return normalize(cross(up, forward));
    }
    /// @brief Camera back vector based on different forward vector.
    /// @param forward The forward vector to calculate the back vector from.
    /// @return The back vector based on the specified forward vector.
    vec3 back(vec3 forward) {
        return normalize(-forward);
    }
    /// @brief Camera down vector based on different up vector.
    /// @param up The up vector to calculate the down vector from.
    /// @return The down vector based on the specified up vector.
    vec3 down(vec3 up) {
        return normalize(-up);
    }
};

/// @brief Matrix view operation.
/// @param camera The camera to create the view matrix from.
/// @return View matrix based on the camera's position and orientation.
/// @note The view matrix is used to transform world coordinates to camera coordinates.
inline mat4 view(sCamera camera) {
    vec3 z = normalize(-camera.forward);
    vec3 x = normalize(cross(camera.up, z));
    vec3 y = cross(z, x);
    mat4 result = {};
    result.x.x = x.x;
    result.x.y = y.x;
    result.x.z = z.x;
    result.y.x = x.y;
    result.y.y = y.y;
    result.y.z = z.y;
    result.z.x = x.z;
    result.z.y = y.z;
    result.z.z = z.z;
    result.w.x = -dot(x, camera.pos);
    result.w.y = -dot(y, camera.pos);
    result.w.z = -dot(z, camera.pos);
    result.w.w = 1.0f;
    return result;
}
/// @brief Camera yaw operation.
/// @param camera The camera to apply the yaw to.
/// @param angle The yaw angle in radians.
inline void camYaw(sCamera *camera, float angle) {
    camera->yaw += angle;
    if (camera->yaw > 3.14159f) camera->yaw -= 2.0f * 3.14159f;
    if (camera->yaw < -3.14159f) camera->yaw += 2.0f * 3.14159f;
    camera->forward.x = cosf(camera->yaw) * cosf(camera->pitch);
    camera->forward.y = sinf(camera->pitch);
    camera->forward.z = sinf(camera->yaw) * cosf(camera->pitch);
    camera->forward = normalize(camera->forward);
}

/// @brief Camera pitch operation.
/// @param camera The camera to apply the pitch to.
/// @param angle The pitch angle in radians.
inline void camPitch(sCamera *camera, float angle) {
    camera->pitch += angle;
    if (camera->pitch > 3.14159f / 2.0f) camera->pitch = 3.14159f / 2.0f;
    if (camera->pitch < -3.14159f / 2.0f) camera->pitch = -3.14159f / 2.0f;
    camera->forward.x = cosf(camera->yaw) * cosf(camera->pitch);
    camera->forward.y = sinf(camera->pitch);
    camera->forward.z = sinf(camera->yaw) * cosf(camera->pitch);
    camera->forward = normalize(camera->forward);
}
/// @brief Camera movement operation.
/// @param camera The camera to move.
/// @param dir The direction vector to move the camera in.
/// @param speed The speed of the camera movement.
/// @note The direction vector should be normalized before passing it to this function.
inline void camMove(sCamera *camera, vec3 dir, float speed) {
    camera->pos = camera->pos + dir * speed;
}

/**
 * @brief Model transformation structure.
 * 
 * This structure represents a model transformation in 3D space with position, scale, and rotation.
 * It provides a matrix function to calculate the transformation matrix based on the current position, scale, and rotation.
 * 
 * @note The transformation matrix is used to transform model coordinates to world coordinates.
 */
struct sModelTransform {
    /// @brief Model position in 3D space.
    vec3 pos = {0, 0, 0};
    /// @brief Model scale in 3D space.
    vec3 sca = {1, 1, 1};
    /// @brief Model rotation in 3D space.
    vec3 rot = {0, 0, 0};
    /// @cond DOXYGEN_OMIT
    vec3 lastPos = {0, 0, 0};
    vec3 lastSca = {1, 1, 1};
    vec3 lastRot = {0, 0, 0};

    mat4 internal_matrix = identity();
    /// @endcond
    /// @brief Model transformation matrix.
    /// @return The transformation matrix based on the current position, scale, and rotation.
    mat4 matrix() {
        if (pos != lastPos || sca != lastSca || rot != lastRot) {
            internal_matrix = translate(pos) * rotate(rot) * scale(sca);
            lastPos = pos;
            lastSca = sca;
            lastRot = rot;
        }
        return internal_matrix;
    }    
};

