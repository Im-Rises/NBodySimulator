#ifndef BARNES_HUT_OCTREE_H
#define BARNES_HUT_OCTREE_H

#include <glm/glm.hpp>

constexpr float GRAVITATIONAL_CONSTANT = 1;
constexpr float SOFTENING = 0.1F;

struct Particle {
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 sumOfForces;

    Particle() : mass(0.0F), position(glm::vec3(0.0F)), velocity(glm::vec3(0.0F)), sumOfForces(glm::vec3(0.0F)) {}
};

struct Bounds {
    glm::vec3 center;
    glm::vec3 halfDimension;

    Bounds() : center(glm::vec3(0.0F)), halfDimension(glm::vec3(0.0F)) {}

    Bounds(glm::vec3 center, glm::vec3 halfDimension) : center(center), halfDimension(halfDimension) {}

    bool contains(const Particle& particle) const {
        return particle.position.x >= center.x - halfDimension.x && particle.position.x <= center.x + halfDimension.x &&
               particle.position.y >= center.y - halfDimension.y && particle.position.y <= center.y + halfDimension.y &&
               particle.position.z >= center.z - halfDimension.z && particle.position.z <= center.z + halfDimension.z;
    }
};

struct BarnesHutOctreeNode {
    Particle particle;
    Bounds bounds;
    BarnesHutOctreeNode* children[8];

    BarnesHutOctreeNode() : particle(Particle()), bounds(Bounds()), children{ nullptr } {}

    BarnesHutOctreeNode(Particle particle, Bounds bounds) : particle(particle), bounds(bounds), children{ nullptr } {}

    ~BarnesHutOctreeNode() {
        for (auto& child : children)
        {
            delete child;
        }
    }

    bool isLeaf() const {
        return children[0] == nullptr;
    }

    void insert(Particle particle) {
        if (!isLeaf())
        {
            int octant = getOctant(particle.position);
            children[octant]->insert(particle);
        }
        else
        {
            if (this->particle.mass == 0.0F)
            {
                this->particle = particle;
            }
            else
            {
                if (bounds.halfDimension.x == 0.0F)
                {
                    bounds.halfDimension = glm::vec3(1.0F);
                }

                children[0] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(-bounds.halfDimension.x / 4.0F, -bounds.halfDimension.y / 4.0F, -bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[1] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(-bounds.halfDimension.x / 4.0F, -bounds.halfDimension.y / 4.0F, bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[2] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(-bounds.halfDimension.x / 4.0F, bounds.halfDimension.y / 4.0F, -bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[3] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(-bounds.halfDimension.x / 4.0F, bounds.halfDimension.y / 4.0F, bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[4] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(bounds.halfDimension.x / 4.0F, -bounds.halfDimension.y / 4.0F, -bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[5] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(bounds.halfDimension.x / 4.0F, -bounds.halfDimension.y / 4.0F, bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[6] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(bounds.halfDimension.x / 4.0F, bounds.halfDimension.y / 4.0F, -bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));
                children[7] = new BarnesHutOctreeNode(this->particle, Bounds(bounds.center + glm::vec3(bounds.halfDimension.x / 4.0F, bounds.halfDimension.y / 4.0F, bounds.halfDimension.z / 4.0F), bounds.halfDimension / 2.0F));

                int octant = getOctant(this->particle.position);
                children[octant]->insert(this->particle);
            }
        }
    }

    int getOctant(glm::vec3 position) const {
        int octant = 0;

        if (position.x >= bounds.center.x)
        {
            octant |= 4;
        }

        if (position.y >= bounds.center.y)
        {
            octant |= 2;
        }

        if (position.z >= bounds.center.z)
        {
            octant |= 1;
        }

        return octant;
    }

    void computeMassDistribution() {
        if (!isLeaf())
        {
            for (auto& child : children)
            {
                child->computeMassDistribution();
            }

            particle.mass = 0.0F;
            particle.position = glm::vec3(0.0F);
            particle.velocity = glm::vec3(0.0F);

            for (auto& child : children)
            {
                particle.mass += child->particle.mass;
                particle.position += child->particle.position * child->particle.mass;
                particle.velocity += child->particle.velocity * child->particle.mass;
            }

            particle.position /= particle.mass;
            particle.velocity /= particle.mass;
        }
    }

    void computeSumOfForces(Particle& particle, float theta) const {
        if (isLeaf())
        {
            if (this->particle.mass != 0.0F && &this->particle != &particle)
            {
                glm::vec3 direction = this->particle.position - particle.position;
                float distance = glm::length(direction);
                float force = GRAVITATIONAL_CONSTANT * particle.mass * this->particle.mass / (distance * distance + SOFTENING);
                particle.sumOfForces += direction * force / distance;
            }
        }
        else
        {
            float distance = glm::length(bounds.center - particle.position);
            float size = bounds.halfDimension.x;
            if (size / distance < theta)
            {
                glm::vec3 direction = bounds.center - particle.position;
                float force = GRAVITATIONAL_CONSTANT * particle.mass * this->particle.mass / (distance * distance + SOFTENING);
                particle.sumOfForces += direction * force / distance;
            }
            else
            {
                for (auto& child : children)
                {
                    child->computeSumOfForces(particle, theta);
                }
            }
        }
    }
};

class BarnesHutOctree {
public:
    BarnesHutOctree(Bounds bounds) : root(nullptr), bounds(bounds) {}

    ~BarnesHutOctree() {
        delete root;
    }

    void insert(Particle particle) {
        if (root == nullptr)
        {
            root = new BarnesHutOctreeNode(particle, bounds);
        }
        else
        {
            root->insert(particle);
        }
    }

    void computeMassDistribution() {
        root->computeMassDistribution();
    }

    void computeSumOfForces(Particle& particle, float theta) const {
        root->computeSumOfForces(particle, theta);
    }

private:
    BarnesHutOctreeNode* root;
    Bounds bounds;
};


#endif // BARNES_HUT_OCTREE_H
