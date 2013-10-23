#ifndef CORE_GAME
#define CORE_GAME

#include <tuple>
#include <string>
#include <fstream>
#include <sstream>
#include "defs.hpp"
#include "renderer.hpp"
#include "../effect/algebra.hpp"
#include "../game_objects/cube.hpp"
#include "../game_objects/star_nest.hpp"

namespace core {
    class Game {
    public:
        func run() -> int {
            var game_ctx = Game(init_ogl());
            float cur_time = glfwGetTime();
            for (;;) {
                float new_time = glfwGetTime();
                float delta = new_time - cur_time;
                cur_time = new_time;
                game_ctx.render();
                game_ctx.update(delta);
                glfwPollEvents();
            }
            return 0;
        }
    private:
        GLFWwindow* window;
        Pool effect_pool;
        Renderer render_ctx;
        game_objects::Cube<Renderer> cube;
        game_objects::StarNest<Renderer> star_nest;
        Game(GLFWwindow* window)
            : window(window)
            , cube(Vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, 0.f, 0.f, 1.f))
            , effect_pool(32)
            , render_ctx(
                glm::lookAt(Vec3(0.f, 0.f, -3.f), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f)),
                glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10.0f)
            )
        {
            val& def_s = render_ctx.default_shader;
            def_s.use();
            glUniform3f(def_s.light_position, 0, 2, -20);
            glUniform3f(def_s.light_intensity, 0.3, 0, 1.0);
            glUniform3f(def_s.k_diffuse, 1, 1, 1);
            glUniform3f(def_s.k_ambient, 0.8, 0.8, 0.8);
            glUniform3f(def_s.k_specular, 1, 1, 0);
            glUniform1f(def_s.shininess, 1);
            val& star_s = render_ctx.star_nest_shader;
            star_s.use();
            glUniform2f(star_s.resolution, 640, 480);
            val view_proj = render_ctx.proj * render_ctx.view;
            glUniformMatrix4fv(star_s.view_proj, 1, GL_FALSE, &view_proj[0][0]);
        }
        meth render() -> void {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cube.render(render_ctx);
            star_nest.render(render_ctx);
            glfwSwapBuffers(window);
        }
        meth update(float delta) -> void {
            render_ctx.global_time += delta;
            cube.update(delta, effect_pool);
        }
        func init_ogl() -> GLFWwindow* {
            glfwSetErrorCallback([](int error, const char* description) -> void {
                fputs(description, stderr);
            });
            if (!glfwInit()) exit(EXIT_FAILURE);
            val window = glfwCreateWindow(1920, 1080, "memspacer", glfwGetPrimaryMonitor(), NULL);
            // val window = glfwCreateWindow(640, 480, "memspacer", NULL, NULL);
            if (!window) die();
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) -> void {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                    glfwDestroyWindow(window);
                    die();
                }
            });
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            if (glewInit() != GLEW_OK) die();
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            return window;
        }
        func die() -> void {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    };
}

#endif CORE_GAME