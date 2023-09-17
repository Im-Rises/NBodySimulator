#include "Octree.h"

Octree::Octree(int maxDepth, int maxParticlesPerLeaf) {
    this->maxDepth = maxDepth;
    this->maxParticlesPerLeaf = maxParticlesPerLeaf;
    this->root = new OctreeNode();
}

Octree::~Octree() {
    delete this->root;
}

void Octree::insert(Particle* particle) {

}

void Octree::subdivide() {

}

void Octree::calculateCenterOfMass() {

}

void Octree::calculateForce(Particle* particle) {

}