//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    if(depth > this->maxDepth)
    {
        return Vector3f(0, 0, 0);
    }
    Intersection inter = this->intersect(ray);
    if(!inter.happened)
    {
        return Vector3f(0, 0, 0);
    }
    auto p = inter.coords;
    auto N = inter.normal;
    auto m = inter.m;
    if(m->hasEmission())
    {
        return m->getEmission();
    }
    float pdf_light;
    Intersection inter1;
    sampleLight(inter1, pdf_light);
    auto x = inter1.coords;
    auto ws = normalize(x-p);
    auto NN = inter1.normal;
    auto emit = inter1.emit;
    auto diff = intersect(Ray(x, ws)).coords - p;
    Vector3f L_dir, L_indir;
    if((diff.x*diff.x+diff.y*diff.y+diff.z*diff.z) < 0.01)
    {
        L_dir = emit*m->eval(ray.direction, ws, N)* dotProduct(ws, N)* dotProduct(ws, NN)
                / dotProduct((x-p),(x-p)) / pdf_light;
    }
    if(get_random_float() > RussianRoulette)
    {
        return L_dir;
    }
    auto wi = m->sample(ray.direction, N);
    L_indir = castRay(Ray(p, wi), depth+1) + m->eval(ray.direction, wi, N)* dotProduct(wi, N)
            / m->pdf(ray.direction, wi, N) / RussianRoulette;
    return L_dir + L_indir;
}