#pragma once
#include "Ayanami.hpp"
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include "Ray.hpp"
#include "Utils.hpp"
#include "Object.hpp"
#include "Spec.hpp"

class Material {
public:
    virtual bool scatter(const Ray& ray, const Hit& hit, glm::vec3& color, Ray& ray_out) const = 0;
};

class Diffuse : public Material {
public:
    Diffuse(const glm::vec3& albedo) : albedo(albedo) {}
    Diffuse(MaterialSpec& spec) {
        albedo = spec.albedo;
    }

    virtual bool scatter(
        const Ray& ray, const Hit& hit, glm::vec3& color, Ray& ray_out
    ) const override {
        glm::vec3 scatter_direction = hit.normal + glm::normalize(r_vec3());

        if (near_zero(scatter_direction))
            scatter_direction = hit.normal;

        ray_out = Ray(hit.point, scatter_direction);
        color = albedo;
        return true;
    }

public:
    glm::vec3 albedo;
};

class Metal : public Material {
public:
    Metal(const glm::vec3& albedo, double roughness) : albedo(albedo), roughness(roughness < 1 ? roughness : 1) {}
    Metal(MaterialSpec& spec) : albedo(spec.albedo), roughness(spec.roughness) {}

    virtual bool scatter(
        const Ray& ray, const Hit& hit, glm::vec3& color, Ray& ray_out
    ) const override {
        glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction), hit.normal);
        ray_out = Ray(hit.point, reflected + glm::vec3(roughness) * r_sphere_vec3());
        color = albedo;
        return (glm::dot(ray_out.direction, hit.normal) > 0);
    }
public:
    glm::vec3 albedo;
    double roughness;
};

// Broken :O
// TODO: fix fr
// 31/1/23 20:54 - going crazy 
class Dielectric : public Material {
public:
    Dielectric(double refraction_index) : refraction_index(refraction_index) {}
    Dielectric(MaterialSpec& spec) : refraction_index(spec.refraction_index) {}

    virtual bool scatter(
        const Ray& ray, const Hit& hit, glm::vec3& color, Ray& ray_out
    ) const override {
        color = glm::vec3(1.0, 1.0, 1.0);
        double refraction_ratio = hit.front ? (1.0 / refraction_index) : refraction_index;

        glm::vec3 unit_direction = glm::normalize(ray.direction);
        double cos_theta = fmin(glm::dot(-unit_direction, hit.normal), 1.0);
        double sin_theta = glm::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        glm::vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > r_double()) {
            direction = glm::reflect(unit_direction, hit.normal);
        } else {
            direction = glm::refract(unit_direction, hit.normal, (float)refraction_ratio);
        }

        ray_out = Ray(hit.point, direction);
        return true;
    }
public:
    double refraction_index;

private:
    static double reflectance(double cosine, double refraction_index) {
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * glm::pow(1 - cosine, 5);
    }
};