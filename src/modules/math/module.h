#pragma once
#include <cmath>


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
inline vec3 operator*(float a, vec3 b) {
    return {a * b.x, a * b.y, a * b.z};
}
inline vec3 operator/(float a, vec3 b) {
    return {a / b.x, a / b.y, a / b.z};
}
inline float dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline vec3 cross(vec3 a, vec3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
inline float length(vec3 a) {
    return sqrtf(dot(a, a));
}
inline vec3 normalize(vec3 a) {
    return a / length(a);
}
inline vec3 operator-(vec3 a) {
    return {-a.x, -a.y, -a.z};
}
inline vec3 lerp(vec3 a, vec3 b, float t) {
    return a + (b - a) * t;
}
inline bool operator==(vec3 a, vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
inline bool operator!=(vec3 a, vec3 b) {
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

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
inline vec4 operator*(float a, vec4 b) {
    return {a * b.x, a * b.y, a * b.z, a * b.w};
}
inline vec4 operator/(float a, vec4 b) {
    return {a / b.x, a / b.y, a / b.z, a / b.w};
}
inline float dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
inline float length(vec4 a) {
    return sqrtf(dot(a, a));
}
inline vec4 normalize(vec4 a) {
    return a / length(a);
}

union mat4 {
    float m[4][4];
    struct {
        vec4 x, y, z, w;
    };
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
inline mat4 identity() {
    mat4 result = {};
    for (int i = 0; i < 4; i++) {
        result.m[i][i] = 1.0f;
    }
    return result;
}
inline mat4 translate(vec3 v) {
    mat4 result = identity();
    result.w.x = v.x;
    result.w.y = v.y;
    result.w.z = v.z;
    return result;
}
inline mat4 scale(vec3 v) {
    mat4 result = {};
    result.x.x = v.x;
    result.y.y = v.y;
    result.z.z = v.z;
    result.w.w = 1.0f;
    return result;
}
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

inline mat4 rotate(vec3 angles) {
    mat4 result = rotate(angles.z, {0, 0, 1}) * rotate(angles.y, {0, 1, 0}) * rotate(angles.x, {1, 0, 0});
    return result;
}

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

struct sCamera {
    vec3 pos = {0, 0, 0};
    vec3 up = {0, 1, 0};
    vec3 forward = {0, 0, -1};
    float yaw = 0.0f;
    float pitch = 0.0f;

    vec3 right() {
        return normalize(cross(forward, up));
    }
    vec3 left() {
        return normalize(cross(up, forward));
    }
    vec3 back() {
        return normalize(-forward);
    }
    vec3 down() {
        return normalize(-up);
    }

    vec3 right(vec3 forward) {
        return normalize(cross(forward, up));
    }
    vec3 left(vec3 forward) {
        return normalize(cross(up, forward));
    }
    vec3 back(vec3 forward) {
        return normalize(-forward);
    }
    vec3 down(vec3 up) {
        return normalize(-up);
    }
};

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

inline void camYaw(sCamera *camera, float angle) {
    camera->yaw += angle;
    if (camera->yaw > 3.14159f) camera->yaw -= 2.0f * 3.14159f;
    if (camera->yaw < -3.14159f) camera->yaw += 2.0f * 3.14159f;
    camera->forward.x = cosf(camera->yaw) * cosf(camera->pitch);
    camera->forward.y = sinf(camera->pitch);
    camera->forward.z = sinf(camera->yaw) * cosf(camera->pitch);
    camera->forward = normalize(camera->forward);
}

inline void camPitch(sCamera *camera, float angle) {
    camera->pitch += angle;
    if (camera->pitch > 3.14159f / 2.0f) camera->pitch = 3.14159f / 2.0f;
    if (camera->pitch < -3.14159f / 2.0f) camera->pitch = -3.14159f / 2.0f;
    camera->forward.x = cosf(camera->yaw) * cosf(camera->pitch);
    camera->forward.y = sinf(camera->pitch);
    camera->forward.z = sinf(camera->yaw) * cosf(camera->pitch);
    camera->forward = normalize(camera->forward);
}

inline void camMove(sCamera *camera, vec3 dir, float speed) {
    camera->pos = camera->pos + dir * speed;
}

struct sModelTransform {
    vec3 pos = {0, 0, 0};
    vec3 sca = {1, 1, 1};
    vec3 rot = {0, 0, 0};

    vec3 lastPos = {0, 0, 0};
    vec3 lastSca = {1, 1, 1};
    vec3 lastRot = {0, 0, 0};

    mat4 internal_matrix = identity();
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

