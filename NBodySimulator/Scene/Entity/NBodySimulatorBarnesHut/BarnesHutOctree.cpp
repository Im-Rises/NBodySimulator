#include <iostream>
#include "BarnesHutOctree.h"

BarnesHutOctree::BarnesHutOctreeNode::BarnesHutOctreeNode(Bounds bounds, int depth)
    : children({}),
      particles({}),
      bounds(bounds),
      isLeaf(true),
      depth(depth),
      centerOfMass(),
      mass(0) {
}
BarnesHutOctree::BarnesHutOctreeNode::~BarnesHutOctreeNode() {
    for (auto* child : children)
    {
        delete child;
    }
}

auto BarnesHutOctree::BarnesHutOctreeNode::insert(Particle* particle) -> bool {
    if (!bounds.contains(*particle))
    {
        return false;
    }

    if (isLeaf)
    {
        if (this->particles.size() < CAPACITY || depth >= MAX_DEPTH)
        {
            this->particles.push_back(particle);
            return true;
        }

        subdivide();
    }
    return (
        children[0]->insert(particle) ||
        children[1]->insert(particle) ||
        children[2]->insert(particle) ||
        children[3]->insert(particle) ||
        children[4]->insert(particle) ||
        children[5]->insert(particle) ||
        children[6]->insert(particle) ||
        children[7]->insert(particle));
}

void BarnesHutOctree::BarnesHutOctreeNode::subdivide() {
    isLeaf = false;
    const auto x = bounds.center.x;
    const auto y = bounds.center.y;
    const auto z = bounds.center.z;
    const auto newHalfDimension = bounds.halfDimension * 0.5F;
    const int newDepth = depth + 1;
    children[0] = new BarnesHutOctreeNode(Bounds(glm::vec3(x - newHalfDimension.x, y - newHalfDimension.y, z - newHalfDimension.z), newHalfDimension), newDepth);
    children[1] = new BarnesHutOctreeNode(Bounds(glm::vec3(x + newHalfDimension.x, y - newHalfDimension.y, z - newHalfDimension.z), newHalfDimension), newDepth);
    children[2] = new BarnesHutOctreeNode(Bounds(glm::vec3(x - newHalfDimension.x, y + newHalfDimension.y, z - newHalfDimension.z), newHalfDimension), newDepth);
    children[3] = new BarnesHutOctreeNode(Bounds(glm::vec3(x + newHalfDimension.x, y + newHalfDimension.y, z - newHalfDimension.z), newHalfDimension), newDepth);
    children[4] = new BarnesHutOctreeNode(Bounds(glm::vec3(x - newHalfDimension.x, y - newHalfDimension.y, z + newHalfDimension.z), newHalfDimension), newDepth);
    children[5] = new BarnesHutOctreeNode(Bounds(glm::vec3(x + newHalfDimension.x, y - newHalfDimension.y, z + newHalfDimension.z), newHalfDimension), newDepth);
    children[6] = new BarnesHutOctreeNode(Bounds(glm::vec3(x - newHalfDimension.x, y + newHalfDimension.y, z + newHalfDimension.z), newHalfDimension), newDepth);
    children[7] = new BarnesHutOctreeNode(Bounds(glm::vec3(x + newHalfDimension.x, y + newHalfDimension.y, z + newHalfDimension.z), newHalfDimension), newDepth);
    for (auto* particle : particles)
    {
        children[0]->insert(particle);
        children[1]->insert(particle);
        children[2]->insert(particle);
        children[3]->insert(particle);
        children[4]->insert(particle);
        children[5]->insert(particle);
        children[6]->insert(particle);
        children[7]->insert(particle);
    }

    particles.clear();
    this->isLeaf = false;
}

void BarnesHutOctree::BarnesHutOctreeNode::computeMassDistribution() {
    if (!this->isLeaf)
    {
        for (auto* child : children)
        {
            child->computeMassDistribution();
            this->mass += child->mass;
            this->bounds.center += child->bounds.center * child->mass;
        }

        this->bounds.center /= this->mass;
    }
    else if (!this->particles.empty())
    {
        for (const auto* p : particles)
        {
            this->mass += p->mass;
            this->bounds.center += p->position;
        }

        this->bounds.center /= this->particles.size();
    }
    else
    {
        this->mass = 0;
        this->bounds.center = glm::vec3(0);
    }
}

void BarnesHutOctree::BarnesHutOctreeNode::computeSumOfForces(Particle& particle, float theta, float gravity, float softening) const {
    const float s = bounds.halfDimension.x * 2;
    const float d = glm::distance(particle.position, bounds.center);

    if (s / d < theta)
    {
        particle.appendForceFrom(this->bounds.center, this->mass, gravity, softening);
    }
    else
    {
        if (!this->isLeaf)
        {
            for (const auto* child : children)
            {
                child->computeSumOfForces(particle, theta, gravity, softening);
            }
        }
        else
        {
            for (const auto* other : this->particles)
            {
                if (particle.id != other->id)
                {
                    particle.appendForceFrom(other->position, other->mass, gravity, softening);
                }
            }
        }
    }
}

BarnesHutOctree::BarnesHutOctree(Bounds bounds)
    : root(bounds, 0) {}

void BarnesHutOctree::insert(Particle* particle) {
    root.insert(particle);
}

void BarnesHutOctree::computeMassDistribution() {
    root.computeMassDistribution();
}

void BarnesHutOctree::computeSumOfForces(Particle& particle, float theta, float gravity, float softening) const {
    root.computeSumOfForces(particle, theta, gravity, softening);
}
