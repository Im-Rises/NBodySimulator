#ifndef OCTREE_H
#define OCTREE_H

#include <glm/glm.hpp>
#include <vector>
#include <array>

struct Particle {
    float mass;
    glm::vec3 position;
};

struct OctreeNode {
    float size;
    glm::vec3 position;

    std::array<OctreeNode*, 8> children;
    std::vector<Particle*> particles;

    glm::vec3 centerOfMass;
    float totalMass;

    OctreeNode() : size(0.0F), position(glm::vec3(0.0F)), children(), particles(), centerOfMass(glm::vec3(0.0F)), totalMass(0.0F) {}
    OctreeNode(float size, glm::vec3 position) : size(size), position(position), children(), particles(), centerOfMass(glm::vec3(0.0F)), totalMass(0.0F) {}

    ~OctreeNode() {
        for (auto& child : this->children) {
            delete child;
        }
    }

    void insert(Particle* particle) {
        if (this->children[0] != nullptr) {
            int index = 0;
            if (particle->position.x >= this->position.x) {
                index |= 1;
            }
            if (particle->position.y >= this->position.y) {
                index |= 2;
            }
            if (particle->position.z >= this->position.z) {
                index |= 4;
            }
            this->children[index]->insert(particle);
        } else {
            this->particles.push_back(particle);
            if (this->particles.size() > 1) {
                this->subdivide();
            }
        }
    }

    void subdivide() {
        for (int i = 0; i < 8; i++) {
            glm::vec3 childPosition = this->position;
            childPosition.x += (i & 1) ? this->size * 0.25F : -this->size * 0.25F;
            childPosition.y += (i & 2) ? this->size * 0.25F : -this->size * 0.25F;
            childPosition.z += (i & 4) ? this->size * 0.25F : -this->size * 0.25F;
            this->children[i] = new OctreeNode(this->size * 0.5F, childPosition);
        }
        for (auto& particle : this->particles) {
            int index = 0;
            if (particle->position.x >= this->position.x) {
                index |= 1;
            }
            if (particle->position.y >= this->position.y) {
                index |= 2;
            }
            if (particle->position.z >= this->position.z) {
                index |= 4;
            }
            this->children[index]->insert(particle);
        }
        this->particles.clear();
    }


};

class Octree {
public:
    Octree(int maxDepth, int maxParticlesPerLeaf);
    ~Octree();

public:
    void insert(Particle* particle);
    void subdivide();
    void calculateCenterOfMass();
    void calculateForce(Particle* particle);

private:
    OctreeNode* root;
    int maxDepth;
    int maxParticlesPerLeaf;
};

#endif // OCTREE_H
