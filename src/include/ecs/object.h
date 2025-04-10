/**
 * @file ecs/object.h
 * @brief ECS GameObject
 */

namespace ecs {
    // The client and server protocol will have multiple types of communication, there is the most common, and most intuitive.
    // But there is also a "slow-connection" mode where the server will try to send the least amount of data needed, but the client has to do more work to put everything together.
    // There is also a "fast-connection" mode where the server will send more data, and the client doesn't have to do as much work to put everything together.

    struct GameObject;
    struct Component;
    struct NetworkState; // The component's network state, this has enums telling the internals how to handle this object's data (client-only, server-only, etc.)

    struct Layer;
    struct Scene; // the constructor actualy creates the scene, these constructors will eventually be auto-generated from the scene file
    struct Transform;
    struct Camera;
    struct Light;
    struct MeshRenderer;
    struct Mesh;
    struct Material;

    struct CubeRenderer;
    struct SphereRenderer;
    struct PlaneRenderer;
    struct CapsuleRenderer;
    struct CylinderRenderer;
    struct QuadRenderer;
}