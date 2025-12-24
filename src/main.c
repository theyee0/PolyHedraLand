#include "raylib.h"
#include "map.h"
#include <math.h>

int main() {
        const int width = 1024;
        const int height = 1024;

        const int virtual_width = 1024;
        const int virtual_height = 1024;

        const float virtual_ratio = (float)width / (float)virtual_width;
        InitWindow(width, height, "");

        Camera2D world_camera = {0};
        Camera2D screen_camera = {0};

        Vector2 origin = {0, 0};

        RenderTexture2D target = LoadRenderTexture(virtual_width, virtual_height);

        Rectangle source = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
        Rectangle dest = { -virtual_ratio, -virtual_ratio, width + (virtual_ratio*2), height + (virtual_ratio*2) };



        world_camera.zoom = 1;
        screen_camera.zoom = 1;

        int density;
        int suboctaves = 0;

        while (!WindowShouldClose()) {
                if (IsKeyDown(KEY_SPACE)) {
                        suboctaves++;
                        suboctaves %= 10;
                }

                BeginTextureMode(target); {
                        ClearBackground(RAYWHITE);

                        BeginMode2D(world_camera); {
                                for (int i = 0; i < virtual_width; i++) {
                                        for (int j = 0; j < virtual_height; j++) {
                                                density = (tanh(fractal_perlin2d(i, j, 0.023, suboctaves + 1)) + 1) * 120;
                                                DrawPixel(i, j, (Color){0, 0, 0, density});
                                        }
                                }
                        } EndMode2D();
                } EndTextureMode();

                BeginDrawing(); {
                        ClearBackground(RED);

                        BeginMode2D(screen_camera); {
                                DrawTexturePro(target.texture, source, dest, origin, 0, WHITE);
                        } EndMode2D();
                } EndDrawing();
        }

        UnloadRenderTexture(target);
        CloseWindow();

        return 0;
}
