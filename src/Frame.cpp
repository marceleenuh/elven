#include "Frame.hpp"

void Frame::render(Camera& cam, const World& world) {
    printf("\033[1;33m[Render] Initiating Threads\n");
    // Prepare row indices (I've no idea how to do this correctly)
    std::vector<int> rows;
    rows.reserve(height);
    for (int _y = 0; _y < height; _y++)
        rows.push_back(_y);

    printf("\033[1;33m[Render] Rendering\n");
    ProgressTracker tracker("\033[1;33mRender", (uint64_t)height);
    Timer timer("\033[1;32mRender");

#if MT
    std::for_each(std::execution::par, rows.begin(), rows.end(),
    [this, &cam, &world, &tracker](int y) {
        for (int x = 0; x < width; x++) {
            glm::vec3 final_color(0);
            for (int i = 0; i < samples_per_pixel-1; i++) {
                auto u = glm::vec3(double(x + r_double()) / (width-1));
                auto v = glm::vec3(double(y + r_double()) / (height-1));

                Ray ray = cam.get_ray(u, v);
                glm::vec3 color_vec = Renderer::per_pixel(ray, world, rec_depth);
                final_color += color_vec;
            }
            uint32_t color = Renderer::as_pixel(final_color, 1.0 / samples_per_pixel, samples_per_pixel);
            data[x + y * width] = color;
        }
        #if TRACKING
        tracker.update();
        if (y % SAMPLE_RATE == 0)
            tracker.write();
        #endif
    });
    #if TRACKING 
    fprintf(stderr, "\r\t\t\t\n");
    fflush(stderr);
    #endif
#else
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 final_color(0);
            for (int i = 0; i < samples_per_pixel-1; i++) {
                auto u = glm::vec3(double(x + r_double()) / (width-1));
                auto v = glm::vec3(double(y + r_double()) / (height-1));

                Ray ray(cam.origin, cam.lower_left_corner + u * cam.horizontal + v * cam.vertical - cam.origin);
                glm::vec3 color_vec = Renderer::per_pixel(ray, world, rec_depth);
                final_color += color_vec;
            }
            uint32_t color = Renderer::as_pixel(final_color, 1.0 / samples_per_pixel, samples_per_pixel);
            data[x + y * width] = color;
        }
        #if TRACKING
        tracker.update();
        if (y % SAMPLE_RATE == 0)
            tracker.write();
        #endif
    }
    #if TRACKING 
    fprintf(stderr, "\r\t\t\t\n");
    fflush(stderr);
    #endif
#endif
}