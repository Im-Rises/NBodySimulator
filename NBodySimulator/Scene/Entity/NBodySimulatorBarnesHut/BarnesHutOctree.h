#ifndef BARNES_HUT_OCTREE_H
#define BARNES_HUT_OCTREE_H

#include <glm/glm.hpp>

struct Particle {
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 sumOfForces;

    Particle() : mass(0.0F), position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)), sumOfForces(glm::vec3(0.0F)) {}
};

struct BarnesHutOctreeNode {
    float size;
    glm::vec3 position;

    BarnesHutOctreeNode* children[8];
    Particle* particle;

    glm::vec3 centerOfMass;
    float totalMass;

    BarnesHutOctreeNode() : size(0.0F), position(glm::vec3(0.0F)), children(), particle(nullptr), centerOfMass(glm::vec3(0.0F)), totalMass(0.0F) {}
    BarnesHutOctreeNode(float size, glm::vec3 position) : size(size), position(position), children(), particle(nullptr), centerOfMass(glm::vec3(0.0F)), totalMass(0.0F) {}

    ~BarnesHutOctreeNode() {
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
            if (this->particle == nullptr) {
                this->particle = particle;
            } else {
                this->subdivide();
                this->insert(this->particle);
                this->insert(particle);
                this->particle = nullptr;
            }
        }
    }

    void subdivide() {
        for (int i = 0; i < 8; i++) {
            glm::vec3 childPosition = this->position;
            childPosition.x += (i & 1) ? this->size * 0.25F : -this->size * 0.25F;
            childPosition.y += (i & 2) ? this->size * 0.25F : -this->size * 0.25F;
            childPosition.z += (i & 4) ? this->size * 0.25F : -this->size * 0.25F;
            this->children[i] = new BarnesHutOctreeNode(this->size * 0.5F, childPosition);
        }

        this->insert(this->particle);
        this->particle = nullptr;
    }



class BarnesHutOctree {
};

#endif // BARNES_HUT_OCTREE_H
