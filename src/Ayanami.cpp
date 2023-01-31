// STL
#include "Ayanami.hpp"

#include "Scene.hpp"
#include "Serializer.hpp"

int main(int argc, char** argv) {
    Serializer::SceneSerializer serializer;
    serializer.load_scene("sample_scene.elv");
    Scene* scene = serializer.get_scene();
    scene->render();
    scene->save();

#if 0
    const char* filename = "image.png";
    // Initial frame parameters
    const auto aspect_ratio = 21.0 / 9.0;
    const int image_width = 1920;
    const int image_height = (int)(image_width / aspect_ratio);
    const int num_samples = 1 << 7; // 128
    const int max_depth = 50;
    Frame frame(image_width, image_height, num_samples, max_depth);

    // World
    World world;
    {
        shared_ptr<Material> big_sphere = make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
        world.append(make_shared<Sphere>(glm::vec3(0.5, 1.0, 3), 1.0, big_sphere));
    }
    {
        shared_ptr<Material> big_sphere = make_shared<Diffuse>(glm::vec3(1.0, 0.5, 0.5));
        world.append(make_shared<Sphere>(glm::vec3(4, 1.5, 0), 1.0, big_sphere));
    }

    // Camera
    /* RTIOW Copy */

    glm::vec3 lookfrom(13, 2, 3);
    glm::vec3 lookat(0, 0, 0);
    glm::vec3 vup(0, 1, 0);
    double focus_distance = 10.0;
    double aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 90.0, aspect_ratio, aperture, focus_distance);
    dump_render_info(frame, cam);
    frame.render(cam, world);

    printf("\033[1;33m[Save] Saving %s\n", filename);
    if (!Image::save(filename, image_width, image_height, frame.data_as_uint8())) {
        printf("\033[1;31m[Error] failed to save %s\n", filename);
        exit(-1);
    }
    printf("\033[1;32m[Save] Successfully saved %s\n", filename);
    printf("\033[0;37m");
#endif
}