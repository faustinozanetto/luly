#include "lypch.h"
#include "debug_renderer.h"

#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_sphere_collision_shape_component.h"

namespace luly::renderer
{
    debug_renderer_data debug_renderer::s_debug_renderer_data = {};

    void debug_renderer::initialize()
    {
        s_debug_renderer_data.lines_thickness = 1.0f;
        s_debug_renderer_data.physics_box_collisions_color = {0.35f, 0.85f, 0.35f};
        s_debug_renderer_data.physics_sphere_collisions_color = {0.85f, 0.65f, 0.0f};
    }

    debug_renderer_data& debug_renderer::get_data()
    {
        return s_debug_renderer_data;
    }

    void debug_renderer::reset()
    {
        s_debug_renderer_data.debug_lines.clear();
    }

    void debug_renderer::submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color)
    {
        s_debug_renderer_data.debug_lines.emplace_back(point_a, point_b, color);
    }

    void debug_renderer::submit_bounding_box(const math::bounding_box& bounding_box, math::transform transform,
                                             const glm::vec3& color)
    {
        const math::bounding_box& transformed_bounding_box = bounding_box.get_transformed(transform);
        const glm::vec3& min = transformed_bounding_box.get_min_extents();
        const glm::vec3& max = transformed_bounding_box.get_max_extents();

        // Submit the lines that make up the bounding box
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
        submit_line(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), color);
        submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        submit_line(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
        submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        submit_line(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z), color);
        submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z), color);
        submit_line(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);
        submit_line(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
        submit_line(glm::vec3(min.x, max.y, max.z), glm::vec3(max.x, max.y, max.z), color);
        submit_line(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
    }

    void debug_renderer::submit_box(math::transform transform, const glm::vec3& color)
    {
        // Define the vertices of the box
        const glm::vec3 vertices[] = {
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {-0.5f, 0.5f, 0.5f}
        };

        // Define the indices of the box edges
        int edges[][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
            {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
            {0, 4}, {1, 5}, {2, 6}, {3, 7} // Connecting edges
        };

        // Apply transformation to the box vertices
        const glm::mat4 model = transform.get_transform();

        // Render the edges of the box
        for (const auto& edge : edges)
        {
            glm::vec3 point_a = glm::vec3(model * glm::vec4(vertices[edge[0]], 1.0f));
            glm::vec3 point_b = glm::vec3(model * glm::vec4(vertices[edge[1]], 1.0f));
            submit_line(point_a, point_b, color);
        }
    }

    void debug_renderer::submit_sphere(float radius, int segments, math::transform transform, const glm::vec3& color)
    {
        float pi = glm::pi<float>();
        float angle_increment = 2 * pi / segments;

        glm::vec3 center = transform.get_location();

        glm::vec3 prev_point(center.x + radius, center.y, center.z);
        glm::vec3 current_point;

        glm::mat4 rotation_matrix = mat4_cast(transform.get_rotation());

        // Loop through the sphere's circumference, submitting line segments
        for (int i = 1; i <= segments; ++i)
        {
            const float angle = i * angle_increment;
            current_point.x = center.x + radius * cos(angle);
            current_point.y = center.y;
            current_point.z = center.z + radius * sin(angle);

            glm::vec4 rotatedPoint = rotation_matrix * glm::vec4(current_point - center, 1.0f);
            current_point = glm::vec3(rotatedPoint) + center;

            submit_line(prev_point, current_point, color);
            prev_point = current_point;
        }

        // Loop through the sphere's slices, submitting line segments
        for (int i = 1; i <= segments; ++i)
        {
            const float slice_angle = pi * i / segments;
            const float slice_radius = radius * sin(slice_angle);
            const float slice_height = radius * cos(slice_angle);
            for (int j = 0; j < segments; ++j)
            {
                const float angle = j * angle_increment;
                const float next_angle = (j + 1) * angle_increment;
                current_point.x = center.x + slice_radius * cos(angle);
                current_point.y = center.y + slice_height;
                current_point.z = center.z + slice_radius * sin(angle);
                glm::vec3 next_point(center.x + slice_radius * cos(next_angle),
                                     center.y + slice_height,
                                     center.z + slice_radius * sin(next_angle));

                // Apply rotation to the current and next points
                glm::vec4 rotated_current_point = rotation_matrix * glm::vec4(current_point - center, 1.0f);
                glm::vec4 rotated_next_point = rotation_matrix * glm::vec4(next_point - center, 1.0f);
                current_point = glm::vec3(rotated_current_point) + center;
                next_point = glm::vec3(rotated_next_point) + center;

                submit_line(current_point, next_point, color);
            }
        }
    }

    void debug_renderer::submit_physics_box_collision(
        const std::shared_ptr<physics::physics_box_collision>& physics_box_collision, const math::transform& transform,
        const glm::vec3& color)
    {
        submit_box(transform, color);
    }

    void debug_renderer::submit_physics_sphere_collision(
        const std::shared_ptr<physics::physics_sphere_collision>& physics_sphere_collision,
        const math::transform& transform, const glm::vec3& color)
    {
        submit_sphere(physics_sphere_collision->get_radius(), transform, color);
    }

    void debug_renderer::collect_debugables()
    {
        collect_physics_box_collisions();
        collect_physics_sphere_collisions();
    }

    void debug_renderer::set_lines_thickness(float lines_thickness)
    {
        s_debug_renderer_data.lines_thickness = lines_thickness;
    }

    void debug_renderer::set_physics_sphere_collisions_enabled(bool enable_physics_sphere_collisions)
    {
        s_debug_renderer_data.enable_physics_sphere_collisions = enable_physics_sphere_collisions;
    }

    void debug_renderer::set_physics_sphere_collisions_color(const glm::vec3& physics_sphere_collisions_color)
    {
        s_debug_renderer_data.physics_sphere_collisions_color = physics_sphere_collisions_color;
    }

    void debug_renderer::set_physics_box_collisions_enabled(bool enable_physics_box_collisions)
    {
        s_debug_renderer_data.enable_physics_box_collisions = enable_physics_box_collisions;
    }

    void debug_renderer::set_physics_box_collisions_color(const glm::vec3& physics_box_collisions_color)
    {
        s_debug_renderer_data.physics_box_collisions_color = physics_box_collisions_color;
    }

    void debug_renderer::collect_physics_box_collisions()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene) return;

        if (!s_debug_renderer_data.enable_physics_box_collisions) return;

        const auto& view = current_scene->get_registry()->view<
            scene::transform_component, scene::physics_box_collision_shape_component>();
        for (const auto& [actor, transform_comp, physics_box_collision_shape_comp] : view.each())
        {
            const auto& box_collision = physics_box_collision_shape_comp.get_box_collision();
            submit_physics_box_collision(box_collision, s_debug_renderer_data.physics_box_collisions_color);
        }
    }

    void debug_renderer::collect_physics_sphere_collisions()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene) return;

        if (!s_debug_renderer_data.enable_physics_sphere_collisions) return;

        const auto& view = current_scene->get_registry()->view<
            scene::transform_component, scene::physics_sphere_collision_shape_component>();
        for (const auto& [actor, transform_comp, physics_sphere_collision_shape_component] : view.each())
        {
            const auto& sphere_collision = physics_sphere_collision_shape_component.get_sphere_collision();
            submit_physics_sphere_collision(sphere_collision, *transform_comp.get_transform(),
                                            s_debug_renderer_data.physics_sphere_collisions_color);
        }
    }
}
