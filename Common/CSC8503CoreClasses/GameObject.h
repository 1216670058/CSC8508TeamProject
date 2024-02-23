#pragma once
#include "Transform.h"
#include "CollisionVolume.h"

#include "AABBVolume.h"
#include "OBBVolume.h"
#include "SphereVolume.h"
#include "CapsuleVolume.h"

using std::vector;

namespace NCL::CSC8503 {
    class NetworkObject;
    class RenderObject;
    class PhysicsObject;

    struct GridCenter {
        int x;
        int y;
    };

    class GameObject {
    public:
        GameObject(const std::string& name = "");
        ~GameObject();

        void SetBoundingVolume(CollisionVolume* vol) {
            boundingVolume = vol;
            Vector3 a;
            switch (boundingVolume->type)
            {
            case VolumeType::AABB:
                a = ((AABBVolume&)*vol).GetHalfDimensions();
                SetBoundingRadius(std::max(a.x, std::max(a.y, a.z)));
                break;
            case VolumeType::OBB:
                a = ((OBBVolume&)*vol).GetHalfDimensions();
                SetBoundingRadius(std::max(a.x, std::max(a.y, a.z)));
                break;
            case VolumeType::Sphere:
                SetBoundingRadius(((SphereVolume&)*vol).GetRadius());
                break;
            case VolumeType::Capsule:
                SetBoundingRadius(((CapsuleVolume&)*vol).GetHalfHeight());
                break;
            default:
                a = ((AABBVolume&)*vol).GetHalfDimensions();
                SetBoundingRadius(std::max(a.x, std::max(a.y, a.z)));
                break;
            }
        }

        const CollisionVolume* GetBoundingVolume() const {
            return boundingVolume;
        }

        float GetBoundingRadius() const { return boundingRadius; }
        void SetBoundingRadius(float f) { boundingRadius = f; }

        bool IsActive() const {
            return isActive;
        }
        void SetActive(bool active) {
            isActive = active;
        }

        Transform& GetTransform() {
            return transform;
        }

        RenderObject* GetRenderObject() const {
            return renderObject;
        }

        PhysicsObject* GetPhysicsObject() const {
            return physicsObject;
        }

        NetworkObject* GetNetworkObject() const {
            return networkObject;
        }

        void SetRenderObject(RenderObject* newObject) {
            renderObject = newObject;
        }

        void SetPhysicsObject(PhysicsObject* newObject) {
            physicsObject = newObject;
        }

        const std::string& GetName() const {
            return name;
        }

        virtual void OnCollisionBegin(GameObject* otherObject) {
            //std::cout << "OnCollisionBegin event occured!\n";
        }

        virtual void OnCollisionEnd(GameObject* otherObject) {
            //std::cout << "OnCollisionEnd event occured!\n";
        }

        virtual void Update(float dt){}

        bool GetBroadphaseAABB(Vector3& outsize) const;

        void UpdateBroadphaseAABB();

        void SetWorldID(int newID) {
            worldID = newID;
        }
        int		GetWorldID() const {
            return worldID;
        }

        void SetTypeID(int newID) {
            typeID = newID;
        }
        int		GetTypeID() const {
            return typeID;
        }

        double EuclideanDistance(float x1, float y1, float x2, float y2);
        Vector3 FindNearestGridCenter(const Vector3& position);

    protected:
        Transform			transform;

        CollisionVolume* boundingVolume;
        float boundingRadius = 1.0f;
        PhysicsObject* physicsObject;
        RenderObject* renderObject;
        NetworkObject* networkObject;

        bool		isActive;
        bool        triggerDelete;
        int			worldID;
        int         typeID;
        std::string	name;

        int GRID_SIZE_X = 10;
        int GRID_SIZE_Y = 10;
        float GRID_CENTER_X = 5.0f;
        float  GRID_CENTER_Y = 5.0f;

        Vector3 broadphaseAABB;
    };
}

